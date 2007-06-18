#ifndef HASH_H
#define HASH_H

#include <stddef.h>
#include <string.h>
#include <memory>
#include <iostream>
#include <list>
#include <utility>
#include "exceptionbase.h"
///#include "archival.h"
///#include "archive.h"

#define bitsToBytes( n ) (n/32+(n%32>0 ? 1 : 0))

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

	struct __calcNextTSize_fast
	{
		uint32_t operator()( uint32_t nCapacity, uint32_t nFill, uint32_t nDeleted ) const
		{
			if( nDeleted >= nCapacity/2 )
				return nCapacity;
			return nCapacity*2+1;
		}
	};

	template<typename key, typename value, typename sizecalc = __calcNextTSize_fast, typename keyalloc = std::allocator<key>, typename valuealloc = std::allocator<value>, typename challoc = std::allocator<uint32_t> >
	class Hash;

	template< typename key, typename _value, typename sizecalc = __calcNextTSize_fast, typename keyalloc = std::allocator<key>, typename valuealloc = std::allocator<_value>, typename challoc = std::allocator<uint32_t> >
	struct HashProxy
	{
		friend class Hash<key, _value, sizecalc, keyalloc, valuealloc, challoc>;
	private:
		HashProxy( Hash<key, _value, sizecalc, keyalloc, valuealloc, challoc> &h, key *k, uint32_t nPos, uint32_t hash ) :
			hsh( h ),
			pKey( k ),
			nPos( nPos ),
			hash( hash ),
			bFilled( false )
		{
		}

		HashProxy( Hash<key, _value, sizecalc, keyalloc, valuealloc, challoc> &h, uint32_t nPos, _value *pValue ) :
			hsh( h ),
			nPos( nPos ),
			pValue( pValue ),
			bFilled( true )
		{
		}

		Hash<key, _value, sizecalc, keyalloc, valuealloc, challoc> &hsh;
		key *pKey;
		uint32_t nPos;
		_value *pValue;
		uint32_t hash;
		bool bFilled;

	public:
		/**
		 * Cast operator for HashProxy.
		 *@returns (value_type &) The value the HashProxy is pointing to.
		 */
		operator _value &()
		{
			if( bFilled == false )
				throw HashException(
						excodeNotFilled,
						"No data assosiated with that key."
						);
			return *pValue;
		}

		/**
		 * Direct function for retrieving a value out of the HashProxy.
		 *@returns (value_type &) The value pointed to by this HashProxy.
		 */
		_value &value()
		{
			if( bFilled == false )
				throw HashException(
						excodeNotFilled,
						"No data assosiated with that key."
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
				hsh._erase( nPos );
				hsh.onDelete();
			}
		}

		/**
		 * Assign data to this point in the hash table.
		 *@param nval (value_type) the data to assign.
		 */
		_value operator=( _value nval )
		{
			if( bFilled )
			{
				hsh.va.destroy( pValue );
				hsh.va.construct( pValue, nval );
				hsh.onUpdate();
			}
			else
			{
				hsh.fill( nPos, *pKey, nval, hash ); 
				hsh.onInsert();
			}

			return nval;
		}

		/**
		 * Pointer extraction operator. Access to members of data pointed to
		 * 		by HashProxy.
		 *@returns (value_type *)
		 */
		_value *operator->()
		{
			if( bFilled == false )
				throw HashException(
						excodeNotFilled,
						"No data assosiated with that key."
						);
			return pValue;
		}
	};

	/**
	 * Libbu Template Hash Table
	 *@param key (typename) The datatype of the hashtable keys
	 *@param value (typename) The datatype of the hashtable data
	 *@param sizecalc (typename) Functor to compute new table size on rehash
	 *@param keyalloc (typename) Memory allocator for hashtable keys
	 *@param valuealloc (typename) Memory allocator for hashtable values
	 *@param challoc (typename) Byte allocator for bitflags
	 */
	template<typename key, typename value, typename sizecalc, typename keyalloc, typename valuealloc, typename challoc >
	class Hash
	{
		friend struct HashProxy<key, value, sizecalc, keyalloc, valuealloc, challoc>;
	public:
		Hash() :
			nCapacity( 11 ),
			nFilled( 0 ),
			nDeleted( 0 ),
			bFilled( NULL ),
			bDeleted( NULL ),
			aKeys( NULL ),
			aValues( NULL ),
			aHashCodes( NULL )
		{
			nKeysSize = bitsToBytes( nCapacity );
			bFilled = ca.allocate( nKeysSize );
			bDeleted = ca.allocate( nKeysSize );
			clearBits();

			aHashCodes = ca.allocate( nCapacity );
			aKeys = ka.allocate( nCapacity );
			aValues = va.allocate( nCapacity );
		}

		Hash( const Hash &src ) :
			nCapacity( src.nCapacity ),
			nFilled( 0 ),
			nDeleted( 0 ),
			bFilled( NULL ),
			bDeleted( NULL ),
			aKeys( NULL ),
			aValues( NULL ),
			aHashCodes( NULL )
		{
			nKeysSize = bitsToBytes( nCapacity );
			bFilled = ca.allocate( nKeysSize );
			bDeleted = ca.allocate( nKeysSize );
			clearBits();

			aHashCodes = ca.allocate( nCapacity );
			aKeys = ka.allocate( nCapacity );
			aValues = va.allocate( nCapacity );

			for( uint32_t j = 0; j < src.nCapacity; j++ )
			{
				if( src.isFilled( j ) )
				{
					insert( src.aKeys[j], src.aValues[j] );
				}
			}
		}

		/**
		 * Hashtable assignment operator. Clears this hashtable and
		 * copies RH into it.
		 */
		Hash &operator=( const Hash &src )
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

			nFilled = 0;
			nDeleted = 0;
			nCapacity = src.nCapacity;
			nKeysSize = bitsToBytes( nCapacity );
			bFilled = ca.allocate( nKeysSize );
			bDeleted = ca.allocate( nKeysSize );
			clearBits();

			aHashCodes = ca.allocate( nCapacity );
			aKeys = ka.allocate( nCapacity );
			aValues = va.allocate( nCapacity );

			for( uint32_t j = 0; j < src.nCapacity; j++ )
			{
				if( src.isFilled( j ) )
				{
					insert( src.aKeys[j], src.aValues[j] );
				}
			}

			return *this;
		}

		virtual ~Hash()
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
		}

		/**
		 * Get the current hash table capacity. (Changes at re-hash)
		 *@returns (uint32_t) The current capacity.
		 */
		uint32_t getCapacity()
		{
			return nCapacity;
		}

		/**
		 * Get the number of hash locations spoken for. (Including 
		 * not-yet-cleaned-up deleted items.)
		 *@returns (uint32_t) The current fill state.
		 */
		uint32_t getFill()
		{
			return nFilled;
		}

		/**
		 * Get the number of items stored in the hash table.
		 *@returns (uint32_t) The number of items stored in the hash table.
		 */
		uint32_t size()
		{
			return nFilled-nDeleted;
		}

		/**
		 * Get the number of items which have been deleted, but not yet
		 * cleaned up.
		 *@returns (uint32_t) The number of deleted items.
		 */
		uint32_t getDeleted()
		{
			return nDeleted;
		}

		/**
		 * Hash table index operator
		 *@param k (key_type) Key of data to be retrieved.
		 *@returns (HashProxy) Proxy pointing to the data.
		 */
		virtual HashProxy<key, value, sizecalc, keyalloc, valuealloc, challoc> operator[]( key k )
		{
			uint32_t hash = __calcHashCode( k );
			bool bFill;
			uint32_t nPos = probe( hash, k, bFill );

			if( bFill )
			{
				return HashProxy<key, value, sizecalc, keyalloc, valuealloc, challoc>( *this, nPos, &aValues[nPos] );
			}
			else
			{
				return HashProxy<key, value, sizecalc, keyalloc, valuealloc, challoc>( *this, &k, nPos, hash );
			}
		}

		/**
		 * Insert a value (v) under key (k) into the hash table
		 *@param k (key_type) Key to list the value under.
		 *@param v (value_type) Value to store in the hash table.
		 */
		virtual void insert( key k, value v )
		{
			uint32_t hash = __calcHashCode( k );
			bool bFill;
			uint32_t nPos = probe( hash, k, bFill );

			if( bFill )
			{
				va.destroy( &aValues[nPos] );
				va.construct( &aValues[nPos], v );
				onUpdate();
			}
			else
			{
				fill( nPos, k, v, hash );
				onInsert();
			}
		}

		/**
		 * Remove a value from the hash table.
		 *@param k (key_type) The data under this key will be erased.
		 */
		virtual void erase( key k )
		{
			uint32_t hash = __calcHashCode( k );
			bool bFill;
			uint32_t nPos = probe( hash, k, bFill );

			if( bFill )
			{
				_erase( nPos );
				onDelete();
			}
		}

		struct iterator;

		/**
		 * Remove a value from the hash pointed to from an iterator.
		 *@param i (iterator &) The data to be erased.
		 */
		virtual void erase( struct iterator &i )
		{
			if( this != &i.hsh )
				throw HashException("This iterator didn't come from this Hash.");
			if( isFilled( i.nPos ) && !isDeleted( i.nPos ) )
			{
				_erase( i.nPos );
				onDelete();
			}
		}

		/**
		 * Remove all data from the hash table.
		 */
		virtual void clear()
		{
			for( uint32_t j = 0; j < nCapacity; j++ )
			{
				if( isFilled( j ) )
					if( !isDeleted( j ) )
					{
						va.destroy( &aValues[j] );
						ka.destroy( &aKeys[j] );
						onDelete();
					}
			}
			
			clearBits();
		}

		/**
		 * Get an item of data from the hash table.
		 *@param k (key_type) Key pointing to the data to be retrieved.
		 *@returns (value_type &) The data pointed to by (k).
		 */
		virtual value &get( key k )
		{
			uint32_t hash = __calcHashCode( k );
			bool bFill;
			uint32_t nPos = probe( hash, k, bFill );

			if( bFill )
			{
				return aValues[nPos];
			}
			else
			{
				throw HashException(
						excodeNotFilled,
						"No data assosiated with that key."
						);
			}
		}
		
		/**
		 * Get a const item of data from the hash table.
		 *@param k (key_type) Key pointing to the data to be retrieved.
		 *@returns (const value_type &) A const version of the data pointed
		 *		to by (k).
		 */
		virtual const value &get( key k ) const
		{
			uint32_t hash = __calcHashCode( k );
			bool bFill;
			uint32_t nPos = probe( hash, k, bFill );

			if( bFill )
			{
				return aValues[nPos];
			}
			else
			{
				throw HashException(
						excodeNotFilled,
						"No data assosiated with that key."
						);
			}
		}

		/**
		 * Does the hash table contain an item under key (k).
		 *@param k (key_type) The key to check.
		 *@returns (bool) Whether there was an item in the hash under key (k).
		 */
		virtual bool has( key k )
		{
			bool bFill;
			probe( __calcHashCode( k ), k, bFill, false );

			return bFill;
		}

		/**
		 * Iteration structure for iterating through the hash.
		 */
		typedef struct iterator
		{
			friend class Hash<key, value, sizecalc, keyalloc, valuealloc, challoc>;
		private:
			iterator( Hash<key, value, sizecalc, keyalloc, valuealloc, challoc> &hsh ) :
				hsh( hsh ),
				nPos( 0 ),
				bFinished( false )
			{
				nPos = hsh.getFirstPos( bFinished );
			}
			
			iterator( Hash<key, value, sizecalc, keyalloc, valuealloc, challoc> &hsh, bool bDone ) :
				hsh( hsh ),
				nPos( 0 ),
				bFinished( bDone )
			{
			}

			Hash<key, value, sizecalc, keyalloc, valuealloc, challoc> &hsh;
			uint32_t nPos;
			bool bFinished;

		public:
			/**
			 * Iterator incrementation operator. Move the iterator forward.
			 */
			iterator operator++( int )
			{
				if( bFinished == false )
					nPos = hsh.getNextPos( nPos, bFinished );

				return *this;
			}
			
			/**
			 * Iterator incrementation operator. Move the iterator forward.
			 */
			iterator operator++()
			{
				if( bFinished == false )
					nPos = hsh.getNextPos( nPos, bFinished );

				return *this;
			}

			/**
			 * Iterator equality comparison operator. Iterators the same?
			 */
			bool operator==( const iterator &oth )
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
			bool operator!=( const iterator &oth )
			{
				return !(*this == oth );
			}

			/**
			 * Iterator assignment operator.
			 */
			iterator operator=( const iterator &oth )
			{
				if( &hsh != &oth.hsh )
					throw HashException(
						"Cannot mix iterators from different hash objects.");
				nPos = oth.nPos;
				bFinished = oth.bFinished;
			}

			/**
			 * Iterator dereference operator... err.. get the value
			 *@returns (value_type &) The value behind this iterator.
			 */
			value &operator *()
			{
				return hsh.getValueAtPos( nPos );
			}
			
			/**
			 * Get the key behind this iterator.
			 *@returns (key_type &) The key behind this iterator.
			 */
			key &getKey()
			{
				return hsh.getKeyAtPos( nPos );
			}

			/**
			 * Get the value behind this iterator.
			 *@returs (value_type &) The value behind this iterator.
			 */
			value &getValue()
			{
				return hsh.getValueAtPos( nPos );
			}
		};

		/**
		 * Get an iterator pointing to the first item in the hash table.
		 *@returns (iterator) An iterator pointing to the first item in the
		 *		hash table.
		 */
		iterator begin()
		{
			return iterator( *this );
		}
		
		/**
		 * Get an iterator pointing to a point just past the last item in the
		 * 		hash table.
		 *@returns (iterator) An iterator pointing to a point just past the
		 *		last item in the hash table.
		 */
		iterator end()
		{
			return iterator( *this, true );
		}

		/**
		 * Get a list of all the keys in the hash table.
		 *@returns (std::list<key_type>) The list of keys in the hash table.
		 */
		std::list<key> getKeys()
		{
			std::list<key> lKeys;

			for( uint32_t j = 0; j < nCapacity; j++ )
			{
				if( isFilled( j ) )
				{
					if( !isDeleted( j ) )
					{
						lKeys.push_back( aKeys[j] );
					}
				}
			}

			return lKeys;
		}

	protected:
		virtual void onInsert() {}
		virtual void onUpdate() {}
		virtual void onDelete() {}
		virtual void onReHash() {}

		virtual void clearBits()
		{
			for( uint32_t j = 0; j < nKeysSize; j++ )
			{
				bFilled[j] = bDeleted[j] = 0;
			}
		}

		virtual void fill( uint32_t loc, key &k, value &v, uint32_t hash )
		{
			bFilled[loc/32] |= (1<<(loc%32));
			va.construct( &aValues[loc], v );
			ka.construct( &aKeys[loc], k );
			aHashCodes[loc] = hash;
			nFilled++;
			//printf("Filled: %d, Deleted: %d, Capacity: %d\n",
			//	nFilled, nDeleted, nCapacity );
		}

		virtual void _erase( uint32_t loc )
		{
			bDeleted[loc/32] |= (1<<(loc%32));
			va.destroy( &aValues[loc] );
			ka.destroy( &aKeys[loc] );
			nDeleted++;
			//printf("Filled: %d, Deleted: %d, Capacity: %d\n",
			//	nFilled, nDeleted, nCapacity );
		}

		virtual std::pair<key,value> getAtPos( uint32_t nPos )
		{
			return std::pair<key,value>(aKeys[nPos],aValues[nPos]);
		}
		
		virtual key &getKeyAtPos( uint32_t nPos )
		{
			return aKeys[nPos];
		}
		
		virtual value &getValueAtPos( uint32_t nPos )
		{
			return aValues[nPos];
		}

		virtual uint32_t getFirstPos( bool &bFinished )
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

		virtual uint32_t getNextPos( uint32_t nPos, bool &bFinished )
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

		uint32_t probe( uint32_t hash, key k, bool &bFill, bool rehash=true )
		{
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

			// This is our insurance, if the table is full, then go ahead and
			// rehash, then try again.
			if( isFilled( nCur ) && rehash == true )
			{
				reHash( szCalc(getCapacity(), getFill(), getDeleted()) );

				// This is potentially dangerous, and could cause an infinite loop.
				// Be careful writing probe, eh?
				return probe( hash, k, bFill );
			}

			bFill = false;
			return nCur;
		}
		
		uint32_t probe( uint32_t hash, key k, bool &bFill, bool rehash=true ) const
		{
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

		void reHash( uint32_t nNewSize )
		{
			//printf("---REHASH---");
			//printf("Filled: %d, Deleted: %d, Capacity: %d\n",
			//	nFilled, nDeleted, nCapacity );
			
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
			nKeysSize = bitsToBytes( nCapacity );
		
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
				if( (bOldFilled[j/32]&(1<<(j%32)))!=0 )
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

		virtual bool isFilled( uint32_t loc ) const
		{
			return (bFilled[loc/32]&(1<<(loc%32)))!=0;
		}

		virtual bool isDeleted( uint32_t loc ) const
		{
			return (bDeleted[loc/32]&(1<<(loc%32)))!=0;
		}

	protected:
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

	template<> uint32_t __calcHashCode<int>( const int &k );
	template<> bool __cmpHashKeys<int>( const int &a, const int &b );

	template<> uint32_t __calcHashCode<unsigned int>( const unsigned int &k );
	template<> bool __cmpHashKeys<unsigned int>( const unsigned int &a, const unsigned int &b );

	template<> uint32_t __calcHashCode<const char *>( const char * const &k );
	template<> bool __cmpHashKeys<const char *>( const char * const &a, const char * const &b );

	template<> uint32_t __calcHashCode<char *>( char * const &k );
	template<> bool __cmpHashKeys<char *>( char * const &a, char * const &b );

	template<> uint32_t __calcHashCode<std::string>( const std::string &k );
	template<> bool __cmpHashKeys<std::string>( const std::string &a, const std::string &b );

	/*
	template<typename key, typename value>
	Archive &operator<<( Archive &ar, Hash<key,value> &h )
	{
		ar << h.size();
		for( typename Hash<key,value>::iterator i = h.begin(); i != h.end(); i++ )
		{
			std::pair<key,value> p = *i;
			ar << p.first << p.second;
		}

		return ar;
	}

	template<typename key, typename value>
	Archive &operator>>( Archive &ar, Hash<key,value> &h )
	{
		h.clear();
		uint32_t nSize;
		ar >> nSize;

		for( uint32_t j = 0; j < nSize; j++ )
		{
			key k; value v;
			ar >> k >> v;
			h.insert( k, v );
		}

		return ar;
	}*/

	/*
	template<typename key, typename value>
	Serializer &operator&&( Serializer &ar, Hash<key,value> &h )
	{
		if( ar.isLoading() )
		{
			return ar >> h;
		}
		else
		{
			return ar << h;
		}
	}*/
}

#endif
