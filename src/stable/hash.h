/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_HASH_H
#define BU_HASH_H

#include <memory>
#include "bu/exceptionbase.h"
#include "bu/list.h"
#include "bu/util.h"
#include "bu/archivebase.h"
#include "bu/sharedcore.h"

namespace Bu
{
    subExceptionDecl( HashException )

    enum eHashException
    {
        excodeNotFilled
    };

    template<typename T>
    uint32_t __calcHashCode( const T &k );

    template<typename T>
    bool __cmpHashKeys( const T &a, const T &b );

    /**
     * Default functor used to compute the size of hash tables.  This version
     * effectively doubles the size of the table when space is low, ensuring
     * that you always wind up with an odd number for the table size.  A
     * better but slower option is to always find the next prime number that's
     * above double your current table size, but that has the potential to be
     * slower.
     */
    struct __calcNextTSize_fast
    {
        uint32_t operator()( uint32_t nCapacity, uint32_t nFilled,
                uint32_t nDeleted ) const
        {
            // This frist case will allow hashtables that are mostly deleted
            // items to reset to small allocations
            if( nFilled-nDeleted <= nCapacity/4 )
            {
                nCapacity = 11;
                while( nCapacity < nFilled*5/4 )
                    nCapacity = nCapacity*2+1;
                return nCapacity;
            }
            // This will hopefully prevent hash tables from growing needlessly
            if( nFilled-nDeleted <= nCapacity/2 )
            {
                if( nDeleted == 0 )
                    return nCapacity/4*5+1;  // Grow just a little
                else
                    return nCapacity;   // We're going to delete things
            }
            // Otherwise, just increase the capacity
            return nCapacity*2+1;
        }
    };

    template<typename totype>
    int bitsTo( int iCount )
    {
        return ( (iCount/(sizeof(totype)*8))
                + (iCount%(sizeof(totype)*8)>0 ? 1 : 0));
    }

    template<typename key, typename value, typename sizecalc, typename keyalloc,
        typename valuealloc, typename challoc>
    class Hash;

    /** @cond DEVEL */
    template<typename key, typename value, typename sizecalc, typename keyalloc,
        typename valuealloc, typename challoc >
    class HashCore
    {
    friend class Hash<key, value, sizecalc, keyalloc, valuealloc, challoc>;
    friend class SharedCore<
        Hash<key, value, sizecalc, keyalloc, valuealloc, challoc>,
        HashCore<key, value, sizecalc, keyalloc, valuealloc, challoc>
        >;
    private:
        HashCore() :
            nCapacity( 0 ),
            nFilled( 0 ),
            nDeleted( 0 ),
            bFilled( NULL ),
            bDeleted( NULL ),
            aKeys( NULL ),
            aValues( NULL ),
            aHashCodes( NULL )
        {
        }

        virtual ~HashCore()
        {
            clear();
        }

        void init()
        {
            if( nCapacity > 0 )
                return;

            nCapacity = 11;
            nKeysSize = bitsTo<uint32_t>( nCapacity );
            bFilled = ca.allocate( nKeysSize );
            bDeleted = ca.allocate( nKeysSize );
            clearBits();

            aHashCodes = ca.allocate( nCapacity );
            aKeys = ka.allocate( nCapacity );
            aValues = va.allocate( nCapacity );
        }

        void clearBits()
        {
            if( nCapacity == 0 )
                return;

            for( uint32_t j = 0; j < nKeysSize; j++ )
            {
                bFilled[j] = bDeleted[j] = 0;
            }
        }

        void fill( uint32_t loc, const key &k, const value &v, uint32_t hash )
        {
            init();

            bFilled[loc/32] |= (1<<(loc%32));
            va.construct( &aValues[loc], v );
            ka.construct( &aKeys[loc], k );
            aHashCodes[loc] = hash;
            nFilled++;
            //printf("Filled: %d, Deleted: %d, Capacity: %d\n",
            //  nFilled, nDeleted, nCapacity );
        }

        void _erase( uint32_t loc )
        {
            if( nCapacity == 0 )
                return;

            bDeleted[loc/32] |= (1<<(loc%32));
            va.destroy( &aValues[loc] );
            ka.destroy( &aKeys[loc] );
            nDeleted++;
            //printf("Filled: %d, Deleted: %d, Capacity: %d\n",
            //  nFilled, nDeleted, nCapacity );
        }

        key &getKeyAtPos( uint32_t nPos )
        {
            if( nPos >= nCapacity )
                throw HashException("Referenced position invalid.");
            return aKeys[nPos];
        }
        
        const key &getKeyAtPos( uint32_t nPos ) const
        {
            if( nPos >= nCapacity )
                throw HashException("Referenced position invalid.");
            return aKeys[nPos];
        }
        
        value &getValueAtPos( uint32_t nPos )
        {
            if( nPos >= nCapacity )
                throw HashException("Referenced position invalid.");
            return aValues[nPos];
        }
        
        const value &getValueAtPos( uint32_t nPos ) const
        {
            if( nPos >= nCapacity )
                throw HashException("Referenced position invalid.");
            return aValues[nPos];
        }

        uint32_t getFirstPos( bool &bFinished ) const
        {
            for( uint32_t j = 0; j < nCapacity; j++ )
            {
                if( isFilled( j ) )
                    if( !isDeleted( j ) )
                        return j;
            }

            bFinished = true;
            return 0;
        }

        uint32_t getNextPos( uint32_t nPos, bool &bFinished ) const
        {
            for( uint32_t j = nPos+1; j < nCapacity; j++ )
            {
                if( isFilled( j ) )
                    if( !isDeleted( j ) )
                        return j;
            }

            bFinished = true;
            return 0;
        }

        uint32_t probe( uint32_t hash, const key &k, bool &bFill, bool rehash=true )
        {
            init();

            uint32_t nCur = hash%nCapacity;

            // First we scan to see if the key is already there, abort if we
            // run out of probing room, or we find a non-filled entry
            int8_t j;
            for( j = 0;
                isFilled( nCur ) && j < 32;
                nCur = (nCur + (1<<j))%nCapacity, j++
                )
            {
                // Is this the same hash code we were looking for?
                if( hash == aHashCodes[nCur] )
                {
                    // Skip over deleted entries.  Deleted entries are also filled,
                    // so we only have to do this check here.
                    if( isDeleted( nCur ) )
                        continue;

                    // Is it really the same key? (for safety)
                    if( __cmpHashKeys( aKeys[nCur], k ) == true )
                    {
                        bFill = true;
                        return nCur;
                    }
                }
            }

            // This is our insurance, if the table is full, then go ahead and
            // rehash, then try again.
            if( (isFilled( nCur ) || j == 32) && rehash == true )
            {
                reHash( szCalc( nCapacity, nFilled, nDeleted ) );

                // This is potentially dangerous, and could cause an infinite loop.
                // Be careful writing probe, eh?
                return probe( hash, k, bFill );
            }

            bFill = false;
            return nCur;
        }
        
        uint32_t probe( uint32_t hash, key k, bool &bFill ) const
        {
            if( nCapacity == 0 )
                throw Bu::ExceptionBase("Probe in empty hash table.");

            uint32_t nCur = hash%nCapacity;

            // First we scan to see if the key is already there, abort if we
            // run out of probing room, or we find a non-filled entry
            for( int8_t j = 0;
                isFilled( nCur ) && j < 32;
                nCur = (nCur + (1<<j))%nCapacity, j++
                )
            {
                // Is this the same hash code we were looking for?
                if( hash == aHashCodes[nCur] )
                {
                    // Skip over deleted entries.  Deleted entries are also filled,
                    // so we only have to do this check here.
                    if( isDeleted( nCur ) )
                        continue;

                    // Is it really the same key? (for safety)
                    if( __cmpHashKeys( aKeys[nCur], k ) == true )
                    {
                        bFill = true;
                        return nCur;
                    }
                }
            }

            bFill = false;
            return nCur;
        }
        
        void insert( const key &k, const value &v )
        {
            uint32_t hash = __calcHashCode( k );
            bool bFill;
            uint32_t nPos = probe( hash, k, bFill );

            if( bFill )
            {
                va.destroy( &aValues[nPos] );
                va.construct( &aValues[nPos], v );
            }
            else
            {
                fill( nPos, k, v, hash );
            }
        }

        void reHash( uint32_t nNewSize )
        {
            //printf("--rehash: %d --> %d (%d, %d)\n", nCapacity, nNewSize, nFilled, nDeleted );
            //printf("---REHASH---");
            //printf("Filled: %d, Deleted: %d, Capacity: %d\n",
            //  nFilled, nDeleted, nCapacity );
            
            // Save all the old data
            uint32_t nOldCapacity = nCapacity;
            uint32_t *bOldFilled = bFilled;
            uint32_t *aOldHashCodes = aHashCodes;
            uint32_t nOldKeysSize = nKeysSize;
            uint32_t *bOldDeleted = bDeleted;
            value *aOldValues = aValues;
            key *aOldKeys = aKeys;
            
            // Calculate new sizes
            nCapacity = nNewSize;
            nKeysSize = bitsTo<uint32_t>( nCapacity );
        
            // Allocate new memory + prep
            bFilled = ca.allocate( nKeysSize );
            bDeleted = ca.allocate( nKeysSize );
            clearBits();

            aHashCodes = ca.allocate( nCapacity );
            aKeys = ka.allocate( nCapacity );
            aValues = va.allocate( nCapacity );

            nDeleted = nFilled = 0;

            // Re-insert all of the old data (except deleted items)
            for( uint32_t j = 0; j < nOldCapacity; j++ )
            {
                if( (bOldFilled[j/32]&(1<<(j%32)))!=0 &&
                    (bOldDeleted[j/32]&(1<<(j%32)))==0 )
                {
                    insert( aOldKeys[j], aOldValues[j] );
                }
            }

            // Delete all of the old data
            for( uint32_t j = 0; j < nOldCapacity; j++ )
            {
                if( (bOldFilled[j/32]&(1<<(j%32)))!=0 &&
                    (bOldDeleted[j/32]&(1<<(j%32)))==0 )
                {
                    va.destroy( &aOldValues[j] );
                    ka.destroy( &aOldKeys[j] );
                }
            }
            va.deallocate( aOldValues, nOldCapacity );
            ka.deallocate( aOldKeys, nOldCapacity );
            ca.deallocate( bOldFilled, nOldKeysSize );
            ca.deallocate( bOldDeleted, nOldKeysSize );
            ca.deallocate( aOldHashCodes, nOldCapacity );
        }

        bool isFilled( uint32_t loc ) const
        {
            if( loc >= nCapacity )
                throw HashException("Referenced position invalid.");
            return (bFilled[loc/32]&(1<<(loc%32)))!=0;
        }

        bool isDeleted( uint32_t loc ) const
        {
            if( loc >= nCapacity )
                throw HashException("Referenced position invalid.");
            return (bDeleted[loc/32]&(1<<(loc%32)))!=0;
        }

        void clear()
        {
            for( uint32_t j = 0; j < nCapacity; j++ )
            {
                if( isFilled( j ) )
                    if( !isDeleted( j ) )
                    {
                        va.destroy( &aValues[j] );
                        ka.destroy( &aKeys[j] );
                    }
            }
            va.deallocate( aValues, nCapacity );
            ka.deallocate( aKeys, nCapacity );
            ca.deallocate( bFilled, nKeysSize );
            ca.deallocate( bDeleted, nKeysSize );
            ca.deallocate( aHashCodes, nCapacity );
            
            bFilled = NULL;
            bDeleted = NULL;
            aKeys = NULL;
            aValues = NULL;
            aHashCodes = NULL;
            
            nCapacity = 0;
            nFilled = 0;
            nDeleted = 0;
        }

        uint32_t nCapacity;
        uint32_t nFilled;
        uint32_t nDeleted;
        uint32_t *bFilled;
        uint32_t *bDeleted;
        uint32_t nKeysSize;
        key *aKeys;
        value *aValues;
        uint32_t *aHashCodes;
        valuealloc va;
        keyalloc ka;
        challoc ca;
        sizecalc szCalc;
    };
    /** @endcond */

    /**
     * Libbu++ Template Hash Table.  This is your average hash table, that uses
     * template functions in order to do fast, efficient, generalized hashing.
     * It's pretty easy to use, and works well with all other libbu++ types so
     * far.
     *
     * In order to use it, I recommend the following for all basic usage:
     *@code
     // Define a Hash typedef with strings as keys and ints as values.
     typedef Bu::Hash<Bu::String, int> StrIntHash;

     // Create one
     StrIntHash hInts;

     // Insert some integers
     hInts["one"] = 1;
     hInts["forty-two"] = 42;
     hInts.insert("forty two", 42 );

     // Get values out of the hash, the last two options are the most explicit,
     // and must be used if the hash's value type does not match what you're
     // comparing to exactly.
     if( hInts["one"] == 1 ) doSomething();
     if( hInts["forty-two"].value() == 42 ) doSomething();
     if( hInts.get("forty two") == 42 ) doSomething();

     // Iterate through the Hash
     for( StrIntHash::iterator i = hInts.begin(); i != hInts.end(); i++ )
     {
        // i.getValue() works too
        print("'%s' = %d\n", i.getKey().getStr(), (*i) );
     }
                                                    
     @endcode
     *@param key (typename) The datatype of the hashtable keys
     *@param value (typename) The datatype of the hashtable data
     *@param sizecalc (typename) Functor to compute new table size on rehash
     *@param keyalloc (typename) Memory allocator for hashtable keys
     *@param valuealloc (typename) Memory allocator for hashtable values
     *@param challoc (typename) Byte allocator for bitflags
     *@ingroup Containers
     */
    template<typename key, typename value,
        typename sizecalc = __calcNextTSize_fast,
        typename keyalloc = std::allocator<key>,
        typename valuealloc = std::allocator<value>,
        typename challoc = std::allocator<uint32_t>
        >
    class Hash : public SharedCore<
                 Hash<key, value, sizecalc, keyalloc, valuealloc, challoc>,
                 HashCore<key, value, sizecalc, keyalloc, valuealloc, challoc>
                 >
    {
    private:
        typedef class HashCore<key, value, sizecalc, keyalloc, valuealloc, challoc> Core;
        typedef class Hash<key, value, sizecalc, keyalloc, valuealloc, challoc> MyType;
    protected:
        using SharedCore<MyType, Core>::core;
        using SharedCore<MyType, Core>::_hardCopy;
        using SharedCore<MyType, Core>::_resetCore;
        using SharedCore<MyType, Core>::_allocateCore;
    
    public:
        Hash()
        {
        }

        Hash( const MyType &src ) :
            SharedCore<MyType, Core >( src )
        {
        }

        virtual ~Hash()
        {
        }

        /**
         * Get the current hash table capacity. (Changes at re-hash)
         *@returns (uint32_t) The current capacity.
         */
        uint32_t getCapacity() const
        {
            return core->nCapacity;
        }

        /**
         * Get the number of hash locations spoken for. (Including 
         * not-yet-cleaned-up deleted items.)
         *@returns (uint32_t) The current fill state.
         */
        uint32_t getFill() const
        {
            return core->nFilled;
        }

        /**
         * Get the number of items stored in the hash table.
         *@returns (uint32_t) The number of items stored in the hash table.
         */
        uint32_t getSize() const
        {
            return core->nFilled-core->nDeleted;
        }

        bool isEmpty() const
        {
            return (core->nFilled-core->nDeleted) == 0;
        }

        /**
         * Get the number of items which have been deleted, but not yet
         * cleaned up.
         *@returns (uint32_t) The number of deleted items.
         */
        uint32_t getDeleted() const
        {
            return core->nDeleted;
        }

        struct HashProxy
        {
        friend class Hash<key, value, sizecalc, keyalloc, valuealloc, challoc>;
        private:
            HashProxy( MyType &h, const key *k, uint32_t nPos, uint32_t hash ) :
                hsh( h ),
                pKey( k ),
                nPos( nPos ),
                hash( hash ),
                bFilled( false )
            {
            }

            HashProxy( MyType &h, uint32_t nPos, value *pValue ) :
                hsh( h ),
                nPos( nPos ),
                pValue( pValue ),
                bFilled( true )
            {
            }

            MyType &hsh;
            const key *pKey;
            uint32_t nPos;
            value *pValue;
            uint32_t hash;
            bool bFilled;

        public:
            /**
             * Cast operator for HashProxy.
             *@returns (value_type &) The value the HashProxy is pointing to.
             */
            operator value &()
            {
                if( bFilled == false )
                    throw HashException(
                            excodeNotFilled,
                            "No data associated with that key."
                            );
                return *pValue;
            }

            /**
             * Direct function for retrieving a value out of the HashProxy.
             *@returns (value_type &) The value pointed to by this HashProxy.
             */
            value &getValue()
            {
                if( bFilled == false )
                    throw HashException(
                            excodeNotFilled,
                            "No data associated with that key."
                            );
                return *pValue;
            }

            /**
             * Whether this HashProxy points to something real or not.
             */
            bool isFilled()
            {
                return bFilled;
            }

            /**
             * Erase the data pointed to by this HashProxy.
             */
            void erase()
            {
                if( bFilled )
                {
                    hsh.core->_erase( nPos );
                }
            }

            /**
             * Assign data to this point in the hash table.
             *@param nval (value_type) the data to assign.
             */
            value operator=( value nval )
            {
                if( bFilled )
                {
                    hsh.core->va.destroy( &hsh.core->aValues[nPos] );
                    hsh.core->va.construct( &hsh.core->aValues[nPos], nval );
                }
                else
                {
                    hsh.core->fill( nPos, *pKey, nval, hash ); 
                }

                return nval;
            }

            /**
             * Pointer extraction operator. Access to members of data pointed to
             *      by HashProxy.
             *@returns (value_type *)
             */
            value *operator->()
            {
                if( bFilled == false )
                    throw HashException(
                            excodeNotFilled,
                            "No data associated with that key."
                            );
                return pValue;
            }
        };

        /**
         * Hash table index operator
         *@param k (key_type) Key of data to be retrieved.
         *@returns (HashProxy) Proxy pointing to the data.
         */
        HashProxy operator[]( const key &k )
        {
            _hardCopy();

            uint32_t hash = __calcHashCode( k );
            bool bFill;
            uint32_t nPos = core->probe( hash, k, bFill );

            if( bFill )
            {
                return HashProxy( *this, nPos, &core->aValues[nPos] );
            }
            else
            {
                return HashProxy( *this, &k, nPos, hash );
            }
        }

        /**
         * Insert a value (v) under key (k) into the hash table
         *@param k (key_type) Key to list the value under.
         *@param v (value_type) Value to store in the hash table.
         */
        void insert( const key &k, const value &v )
        {
            _hardCopy();

            core->insert( k, v );
        }

        /**
         * Remove a value from the hash table.
         *@param k (key_type) The data under this key will be erased.
         */
        void erase( const key &k )
        {
            _hardCopy();

            uint32_t hash = __calcHashCode( k );
            bool bFill;
            uint32_t nPos = core->probe( hash, k, bFill );

            if( bFill )
            {
                core->_erase( nPos );
            }
        }

        struct iterator;

        /**
         * Remove a value from the hash pointed to from an iterator.
         *@param i (iterator &) The data to be erased.
         */
        void erase( struct iterator &i )
        {
            if( this != i.hsh )
                throw HashException("This iterator didn't come from this Hash.");
            
            _hardCopy();

            if( core->isFilled( i.nPos ) && !core->isDeleted( i.nPos ) )
            {
                core->_erase( i.nPos );
            }
        }

        /**
         * Remove all data from the hash table.
         */
        virtual void clear()
        {
            _resetCore();
        }

        /**
         * Get an item of data from the hash table.
         *@param k (key_type) Key pointing to the data to be retrieved.
         *@returns (value_type &) The data pointed to by (k).
         */
        value &get( const key &k )
        {
            _hardCopy();

            uint32_t hash = __calcHashCode( k );
            bool bFill;
            uint32_t nPos = core->probe( hash, k, bFill, false );

            if( bFill )
            {
                return core->aValues[nPos];
            }
            else
            {
                throw HashException(
                        excodeNotFilled,
                        "No data associated with that key."
                        );
            }
        }
        
        /**
         * Get a const item of data from the hash table.
         *@param k (key_type) Key pointing to the data to be retrieved.
         *@returns (const value_type &) A const version of the data pointed
         *      to by (k).
         */
        const value &get( const key &k ) const
        {
            uint32_t hash = __calcHashCode( k );
            bool bFill;
            uint32_t nPos = core->probe( hash, k, bFill );

            if( bFill )
            {
                return core->aValues[nPos];
            }
            else
            {
                throw HashException(
                        excodeNotFilled,
                        "No data associated with that key."
                        );
            }
        }

        /**
         * Does the hash table contain an item under key (k).
         *@param k (key_type) The key to check.
         *@returns (bool) Whether there was an item in the hash under key (k).
         */
        bool has( const key &k ) const
        {
            bool bFill;
            core->probe( __calcHashCode( k ), k, bFill );

            return bFill;
        }

        /**
         * Iteration structure for iterating through the hash.
         */
        typedef struct iterator
        {
            friend class Hash<key, value, sizecalc, keyalloc, valuealloc, challoc>;
        private:
            iterator( MyType *hsh ) :
                hsh( hsh ),
                nPos( 0 ),
                bFinished( false )
            {
                nPos = hsh->core->getFirstPos( bFinished );
            }
            
            iterator( MyType *hsh, bool bDone ) :
                hsh( hsh ),
                nPos( 0 ),
                bFinished( bDone )
            {
            }

            MyType *hsh;
            uint32_t nPos;
            bool bFinished;

        public:
            iterator( const iterator &i ) :
                hsh( i.hsh ),
                nPos( i.nPos ),
                bFinished( i.bFinished )
            {
            }

            iterator() :
                hsh( NULL ),
                nPos( NULL ),
                bFinished( true )
            {
            }

            bool isValid() const
            {
                return !bFinished;
            }

            operator bool() const
            {
                return !bFinished;
            }

            /**
             * Iterator incrementation operator. Move the iterator forward.
             */
            iterator operator++( int )
            {
                if( bFinished == false )
                    nPos = hsh->core->getNextPos( nPos, bFinished );

                return *this;
            }
            
            /**
             * Iterator incrementation operator. Move the iterator forward.
             */
            iterator operator++()
            {
                if( bFinished == false )
                    nPos = hsh->core->getNextPos( nPos, bFinished );

                return *this;
            }

            /**
             * Iterator equality comparison operator. Iterators the same?
             */
            bool operator==( const iterator &oth ) const
            {
                if( bFinished != oth.bFinished )
                    return false;
                if( bFinished == true )
                {
                    return true;
                }
                else
                {
                    if( oth.nPos == nPos )
                        return true;
                    return false;
                }
            }
            
            /**
             * Iterator not equality comparison operator. Not the same?
             */
            bool operator!=( const iterator &oth ) const
            {
                return !(*this == oth );
            }

            /**
             * Iterator assignment operator.
             */
            iterator operator=( const iterator &oth )
            {
                hsh = oth.hsh;
                nPos = oth.nPos;
                bFinished = oth.bFinished;
                return *this;
            }

            /**
             * Iterator dereference operator... err.. get the value
             *@returns (value_type &) The value behind this iterator.
             */
            value &operator *()
            {
                hsh->_hardCopy();
                return hsh->core->getValueAtPos( nPos );
            }

            const value &operator *() const
            {
                return hsh->core->getValueAtPos( nPos );
            }
            
            /**
             * Get the key behind this iterator.
             *@returns (key_type &) The key behind this iterator.
             */
            const key &getKey() const
            {
                return hsh->core->getKeyAtPos( nPos );
            }

            /**
             * Get the value behind this iterator.
             *@returns (value_type &) The value behind this iterator.
             */
            value &getValue()
            {
                hsh->_hardCopy();
                return hsh->core->getValueAtPos( nPos );
            }

            /**
             * Get the value behind this iterator.
             *@returns (value_type &) The value behind this iterator.
             */
            const value &getValue() const
            {
                return hsh->core->getValueAtPos( nPos );
            }
        } iterator;
        
        /**
         * Iteration structure for iterating through the hash (const).
         */
        typedef struct const_iterator
        {
            friend class Hash<key, value, sizecalc, keyalloc, valuealloc, challoc>;
        private:
            const_iterator( const MyType *hsh ) :
                hsh( hsh ),
                nPos( 0 ),
                bFinished( false )
            {
                nPos = hsh->core->getFirstPos( bFinished );
            }
            
            const_iterator( const MyType *hsh, bool bDone ) :
                hsh( hsh ),
                nPos( 0 ),
                bFinished( bDone )
            {
            }

            const MyType *hsh;
            uint32_t nPos;
            bool bFinished;

        public:
            const_iterator() :
                hsh( NULL ),
                nPos( 0 ),
                bFinished( true )
            {
            }

            const_iterator( const const_iterator &src ) :
                hsh( src.hsh ),
                nPos( src.nPos ),
                bFinished( src.bFinished )
            {
            }

            const_iterator( const iterator &src ) :
                hsh( src.hsh ),
                nPos( src.nPos ),
                bFinished( src.bFinished )
            {
            }

            bool isValid() const
            {
                return !bFinished;
            }

            operator bool() const
            {
                return !bFinished;
            }

            /**
             * Iterator incrementation operator. Move the iterator forward.
             */
            const_iterator operator++( int )
            {
                if( bFinished == false )
                    nPos = hsh->core->getNextPos( nPos, bFinished );

                return *this;
            }
            
            /**
             * Iterator incrementation operator. Move the iterator forward.
             */
            const_iterator operator++()
            {
                if( bFinished == false )
                    nPos = hsh->core->getNextPos( nPos, bFinished );

                return *this;
            }

            /**
             * Iterator equality comparison operator. Iterators the same?
             */
            bool operator==( const const_iterator &oth ) const
            {
                if( bFinished != oth.bFinished )
                    return false;
                if( bFinished == true )
                {
                    return true;
                }
                else
                {
                    if( oth.nPos == nPos )
                        return true;
                    return false;
                }
            }
            
            /**
             * Iterator not equality comparison operator. Not the same?
             */
            bool operator!=( const const_iterator &oth ) const
            {
                return !(*this == oth );
            }

            /**
             * Iterator assignment operator.
             */
            const_iterator operator=( const const_iterator &oth )
            {
                hsh = oth.hsh;
                nPos = oth.nPos;
                bFinished = oth.bFinished;
                return *this;
            }

            /**
             * Iterator dereference operator... err.. get the value
             *@returns (value_type &) The value behind this iterator.
             */
            const value &operator *() const
            {
                return hsh->core->getValueAtPos( nPos );
            }
            
            /**
             * Get the key behind this iterator.
             *@returns (key_type &) The key behind this iterator.
             */
            const key &getKey() const
            {
                return hsh->core->getKeyAtPos( nPos );
            }

            /**
             * Get the value behind this iterator.
             *@returns (value_type &) The value behind this iterator.
             */
            const value &getValue() const
            {
                return hsh->core->getValueAtPos( nPos );
            }
        } const_iterator;

        /**
         * Get an iterator pointing to the first item in the hash table.
         *@returns (iterator) An iterator pointing to the first item in the
         *      hash table.
         */
        iterator begin()
        {
            return iterator( this );
        }
        
        const_iterator begin() const
        {
            return const_iterator( this );
        }
        
        /**
         * Get an iterator pointing to a point just past the last item in the
         *      hash table.
         *@returns (iterator) An iterator pointing to a point just past the
         *      last item in the hash table.
         */
        iterator end()
        {
            return iterator( this, true );
        }
        
        const_iterator end() const
        {
            return const_iterator( this, true );
        }

        /**
         * Get a list of all the keys in the hash table.
         *@returns (std::list<key_type>) The list of keys in the hash table.
         */
        Bu::List<key> getKeys() const
        {
            Bu::List<key> lKeys;

            for( uint32_t j = 0; j < core->nCapacity; j++ )
            {
                if( core->isFilled( j ) )
                {
                    if( !core->isDeleted( j ) )
                    {
                        lKeys.append( core->aKeys[j] );
                    }
                }
            }

            return lKeys;
        }
        
        Bu::List<value> getValues() const
        {
            Bu::List<value> lValues;

            for( uint32_t j = 0; j < core->nCapacity; j++ )
            {
                if( core->isFilled( j ) )
                {
                    if( !core->isDeleted( j ) )
                    {
                        lValues.append( core->aValues[j] );
                    }
                }
            }

            return lValues;
        }

        bool operator==( const MyType &rhs ) const
        {
            if( this == &rhs )
                return true;
            if( core == rhs.core )
                return true;
            if( core == NULL || rhs.core == NULL )
                return false;
            if( getSize() != rhs.getSize() )
                return false;

            for( uint32_t j = 0; j < core->nCapacity; j++ )
            {
                if( core->isFilled( j ) )
                {
                    if( !core->isDeleted( j ) )
                    {
                        // Check to see if this key is in the other hash
                        if( rhs.has( core->aKeys[j] ) )
                        {
                            if( !(core->aValues[j] == rhs.get( core->aKeys[j]) ) )
                            {
                                return false;
                            }
                        }
                        else
                        {
                            return false;
                        }
                    }
                }
            }

            return true;
        }

        bool operator!=( const MyType &rhs ) const
        {
            return !(*this == rhs);
        }

        MyType &operator+=( const MyType &rhs )
        {
            if( this == &rhs )
                return *this;
            if( core == rhs.core )
                return *this;
            if( core == NULL || rhs.core == NULL )
                return *this;

            for( const_iterator i = rhs.begin(); i; i++ )
                insert( i.getKey(), i.getValue() );

            return *this;
        }

    protected:
        virtual Core *_copyCore( Core *src )
        {
            Core *pRet = _allocateCore();

            pRet->nFilled = 0;
            pRet->nDeleted = 0;
            pRet->nCapacity = src->nCapacity;
            pRet->nKeysSize = bitsTo<uint32_t>( pRet->nCapacity );
            pRet->bFilled = pRet->ca.allocate( pRet->nKeysSize );
            pRet->bDeleted = pRet->ca.allocate( pRet->nKeysSize );
            pRet->clearBits();

            pRet->aHashCodes = pRet->ca.allocate( pRet->nCapacity );
            pRet->aKeys = pRet->ka.allocate( pRet->nCapacity );
            pRet->aValues = pRet->va.allocate( pRet->nCapacity );

            for( uint32_t j = 0; j < src->nCapacity; j++ )
            {
                if( src->isFilled( j ) && !src->isDeleted( j ) )
                {
                    pRet->insert( src->aKeys[j], src->aValues[j] );
                }
            }

            return pRet;
        }
    };
    
    template<typename T> uint32_t __calcHashCode( const T &k )
    {
        return static_cast<uint32_t>( k );
    }

    template<typename T> bool __cmpHashKeys( const T &a, const T &b )
    {
        return (a == b);
    }

    template<> uint32_t __calcHashCode<const char *>( const char * const &k );
    template<> bool __cmpHashKeys<const char *>( const char * const &a, const char * const &b );

    template<> uint32_t __calcHashCode<char *>( char * const &k );
    template<> bool __cmpHashKeys<char *>( char * const &a, char * const &b );

    class Formatter;
    Formatter &operator<<( Formatter &rOut, char *sStr );
    Formatter &operator<<( Formatter &rOut, signed char c );
    template<typename key, typename value>
    Formatter &operator<<( Formatter &f, const Bu::Hash<key, value> &l )
    {
        f << '{';
        for( typename Bu::Hash<key,value>::const_iterator i = l.begin(); i; i++ )
        {
            if( i != l.begin() )
                f << ", ";
            f << i.getKey() << ": " << i.getValue();
        }
        f << '}';

        return f;
    }

    template<typename key, typename value, typename a, typename b,
        typename c, typename d>
    ArchiveBase &operator<<( ArchiveBase &ar, const Hash<key,value,a,b,c,d> &h )
    {
        long iSize = h.getSize();
        ar << iSize;
        for( typename Hash<key,value,a,b,c,d>::const_iterator i = h.begin(); i != h.end(); i++ )
        {
            ar << (i.getKey());
            ar << (i.getValue());
        }

        return ar;
    }

    template<typename key, typename value, typename a, typename b,
        typename c, typename d>
    ArchiveBase &operator>>( ArchiveBase &ar, Hash<key,value,a,b,c,d> &h )
    {
        h.clear();
        long nSize;
        ar >> nSize;

        for( long j = 0; j < nSize; j++ )
        {
            key k; value v;
            ar >> k >> v;
            h.insert( k, v );
        }

        return ar;
    }
}

#endif
