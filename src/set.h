#ifndef BU_SET_H
#define BU_SET_H

#include <stddef.h>
#include <string.h>
#include <memory>
#include <iostream>
#include <list>
#include <utility>
#include "bu/exceptionbase.h"
#include "bu/list.h"
///#include "archival.h"
///#include "archive.h"

#define bitsToBytes( n ) (n/32+(n%32>0 ? 1 : 0))

namespace Bu
{
	subExceptionDecl( SetException )

	template<typename T>
	uint32_t __calcHashCode( const T &k );

	template<typename T>
	bool __cmpHashKeys( const T &a, const T &b );

	template<typename key, typename sizecalc = struct __calcNextTSize_fast, typename keyalloc = std::allocator<key>, typename challoc = std::allocator<uint32_t> >
	class Set;

	/**
	 * Libbu Template Set
	 *@param key (typename) The datatype of the hashtable keys
	 *@param sizecalc (typename) Functor to compute new table size on rehash
	 *@param keyalloc (typename) Memory allocator for hashtable keys
	 *@param challoc (typename) Byte allocator for bitflags
	 */
	template<typename key, typename sizecalc, typename keyalloc, typename challoc >
	class Set
	{
	public:
		Set() :
			nCapacity( 11 ),
			nFilled( 0 ),
			nDeleted( 0 ),
			bFilled( NULL ),
			bDeleted( NULL ),
			aKeys( NULL ),
			aHashCodes( NULL )
		{
			nKeysSize = bitsToBytes( nCapacity );
			bFilled = ca.allocate( nKeysSize );
			bDeleted = ca.allocate( nKeysSize );
			clearBits();

			aHashCodes = ca.allocate( nCapacity );
			aKeys = ka.allocate( nCapacity );
		}

		Set( const Set &src ) :
			nCapacity( src.nCapacity ),
			nFilled( 0 ),
			nDeleted( 0 ),
			bFilled( NULL ),
			bDeleted( NULL ),
			aKeys( NULL ),
			aHashCodes( NULL )
		{
			nKeysSize = bitsToBytes( nCapacity );
			bFilled = ca.allocate( nKeysSize );
			bDeleted = ca.allocate( nKeysSize );
			clearBits();

			aHashCodes = ca.allocate( nCapacity );
			aKeys = ka.allocate( nCapacity );

			for( uint32_t j = 0; j < src.nCapacity; j++ )
			{
				if( src.isFilled( j ) )
				{
					insert( src.aKeys[j] );
				}
			}
		}

		/**
		 * Set assignment operator. Clears this hashtable and
		 * copies RH into it.
		 */
		Set &operator=( const Set &src )
		{
			for( uint32_t j = 0; j < nCapacity; j++ )
			{
				if( isFilled( j ) )
					if( !isDeleted( j ) )
					{
						ka.destroy( &aKeys[j] );
					}
			}
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

			for( uint32_t j = 0; j < src.nCapacity; j++ )
			{
				if( src.isFilled( j ) )
				{
					insert( src.aKeys[j] );
				}
			}

			return *this;
		}

		virtual ~Set()
		{
			for( uint32_t j = 0; j < nCapacity; j++ )
			{
				if( isFilled( j ) )
					if( !isDeleted( j ) )
					{
						ka.destroy( &aKeys[j] );
					}
			}
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
		 * Insert a value (v) under key (k) into the hash table
		 *@param k (key_type) Key to list the value under.
		 *@param v (value_type) Value to store in the hash table.
		 */
		virtual void insert( key k )
		{
			uint32_t hash = __calcHashCode( k );
			bool bFill;
			uint32_t nPos = probe( hash, k, bFill );

			if( bFill )
			{
				onUpdate();
			}
			else
			{
				fill( nPos, k, hash );
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
				throw SetException("This iterator didn't come from this Hash.");
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
						onDelete();
					}
			}
			
			clearBits();
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
			friend class Set<key, sizecalc, keyalloc, challoc>;
		private:
			iterator( Set<key, sizecalc, keyalloc, challoc> &hsh ) :
				hsh( hsh ),
				nPos( 0 ),
				bFinished( false )
			{
				nPos = hsh.getFirstPos( bFinished );
			}
			
			iterator( Set<key, sizecalc, keyalloc, challoc> &hsh, bool bDone ) :
				hsh( hsh ),
				nPos( 0 ),
				bFinished( bDone )
			{
			}

			Set<key, sizecalc, keyalloc, challoc> &hsh;
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
					throw SetException(
						"Cannot mix iterators from different hash objects.");
				nPos = oth.nPos;
				bFinished = oth.bFinished;
			}

			/**
			 * Iterator dereference operator... err.. get the value
			 *@returns (value_type &) The value behind this iterator.
			 */
			key &operator *()
			{
				return hsh.getKeyAtPos( nPos );
			}
		};
		
		/**
		 * Iteration structure for iterating through the hash (const).
		 */
		typedef struct const_iterator
		{
			friend class Set<key, sizecalc, keyalloc, challoc>;
		private:
			const_iterator( const Set<key, sizecalc, keyalloc, challoc> &hsh ) :
				hsh( hsh ),
				nPos( 0 ),
				bFinished( false )
			{
				nPos = hsh.getFirstPos( bFinished );
			}
			
			const_iterator( const Set<key, sizecalc, keyalloc, challoc> &hsh, bool bDone ) :
				hsh( hsh ),
				nPos( 0 ),
				bFinished( bDone )
			{
			}

			const Set<key, sizecalc, keyalloc, challoc> &hsh;
			uint32_t nPos;
			bool bFinished;

		public:
			/**
			 * Iterator incrementation operator. Move the iterator forward.
			 */
			const_iterator operator++( int )
			{
				if( bFinished == false )
					nPos = hsh.getNextPos( nPos, bFinished );

				return *this;
			}
			
			/**
			 * Iterator incrementation operator. Move the iterator forward.
			 */
			const_iterator operator++()
			{
				if( bFinished == false )
					nPos = hsh.getNextPos( nPos, bFinished );

				return *this;
			}

			/**
			 * Iterator equality comparison operator. Iterators the same?
			 */
			bool operator==( const const_iterator &oth )
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
			bool operator!=( const const_iterator &oth )
			{
				return !(*this == oth );
			}

			/**
			 * Iterator assignment operator.
			 */
			const_iterator operator=( const const_iterator &oth )
			{
				if( &hsh != &oth.hsh )
					throw SetException(
						"Cannot mix iterators from different hash objects.");
				nPos = oth.nPos;
				bFinished = oth.bFinished;
			}

			/**
			 * Iterator dereference operator... err.. get the value
			 *@returns (value_type &) The value behind this iterator.
			 */
			const key &operator *() const
			{
				return hsh.getKeyAtPos( nPos );
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
		
		const_iterator begin() const
		{
			return const_iterator( *this );
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
		
		const_iterator end() const
		{
			return const_iterator( *this, true );
		}

		/**
		 * Get a list of all the keys in the hash table.
		 *@returns (std::list<key_type>) The list of keys in the hash table.
		 */
		Bu::List<key> getKeys() const
		{
			Bu::List<key> lKeys;

			for( uint32_t j = 0; j < nCapacity; j++ )
			{
				if( isFilled( j ) )
				{
					if( !isDeleted( j ) )
					{
						lKeys.append( aKeys[j] );
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

		virtual void fill( uint32_t loc, key &k, uint32_t hash )
		{
			bFilled[loc/32] |= (1<<(loc%32));
			ka.construct( &aKeys[loc], k );
			aHashCodes[loc] = hash;
			nFilled++;
			//printf("Filled: %d, Deleted: %d, Capacity: %d\n",
			//	nFilled, nDeleted, nCapacity );
		}

		virtual void _erase( uint32_t loc )
		{
			bDeleted[loc/32] |= (1<<(loc%32));
			ka.destroy( &aKeys[loc] );
			nDeleted++;
			//printf("Filled: %d, Deleted: %d, Capacity: %d\n",
			//	nFilled, nDeleted, nCapacity );
		}

		virtual key &getKeyAtPos( uint32_t nPos )
		{
			return aKeys[nPos];
		}
		
		virtual const key &getKeyAtPos( uint32_t nPos ) const
		{
			return aKeys[nPos];
		}
		
		virtual uint32_t getFirstPos( bool &bFinished ) const
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

		virtual uint32_t getNextPos( uint32_t nPos, bool &bFinished ) const
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

			nDeleted = nFilled = 0;

			// Re-insert all of the old data (except deleted items)
			for( uint32_t j = 0; j < nOldCapacity; j++ )
			{
				if( (bOldFilled[j/32]&(1<<(j%32)))!=0 &&
					(bOldDeleted[j/32]&(1<<(j%32)))==0 )
				{
					insert( aOldKeys[j] );
				}
			}

			// Delete all of the old data
			for( uint32_t j = 0; j < nOldCapacity; j++ )
			{
				if( (bOldFilled[j/32]&(1<<(j%32)))!=0 )
				{
					ka.destroy( &aOldKeys[j] );
				}
			}
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
		uint32_t *aHashCodes;
		keyalloc ka;
		challoc ca;
		sizecalc szCalc;
	};
}

#endif
