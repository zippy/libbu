#ifndef HASH_H
#define HASH_H

#include <stddef.h>
#include <string.h>
#include <memory>
#include <iostream>
#include "exceptionbase.h"
#include "hashable.h"

#define bitsToBytes( n ) (n/32+(n%32>0 ? 1 : 0))

subExceptionDecl( HashException )

enum eHashException
{
	excodeNotFilled
};

template<typename T>
uint32_t __calcHashCode( T k );

template<typename T>
bool __cmpHashKeys( T a, T b );

struct __calcNextTSize_fast
{
  uint32_t operator()( uint32_t nCapacity, uint32_t nFill, uint32_t nDeleted ) const
  {
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
	operator _value()
	{
		if( bFilled == false )
			throw HashException(
					excodeNotFilled,
					"No data assosiated with that key."
					);
		return *pValue;
	}

	_value value()
	{
		if( bFilled == false )
			throw HashException(
					excodeNotFilled,
					"No data assosiated with that key."
					);
		return *pValue;
	}

	bool isFilled()
	{
		return bFilled;
	}

	void erase()
	{
		if( bFilled )
			hsh._erase( nPos );
	}

	_value operator=( _value nval )
	{
		if( bFilled )
		{
			hsh.va.destroy( pValue );
			hsh.va.construct( pValue, nval );
		}
		else
		{
			hsh.fill( nPos, *pKey, nval, hash ); 
		}

		return nval;
	}
};

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

	uint32_t getCapacity()
	{
		return nCapacity;
	}

	uint32_t getFill()
	{
		return nFilled;
	}

	uint32_t getDeleted()
	{
		return nDeleted;
	}

	HashProxy<key, value, sizecalc, keyalloc, valuealloc, challoc> operator[]( key k )
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

	void insert( key k, value v )
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

	void erase( key k )
	{
		uint32_t hash = __calcHashCode( k );
		bool bFill;
		uint32_t nPos = probe( hash, k, bFill );

		if( bFill )
		{
			_erase( nPos );
		}
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
		
		clearBits();
	}

	value get( key k )
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

	bool has( key k )
	{
		bool bFill;
		probe( __calcHashCode( k ), k, bFill );

		return bFill;
	}

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
		iterator operator++( int )
		{
			if( bFinished == false )
				nPos = hsh.getNextPos( nPos, bFinished );

			return *this;
		}
		
		iterator operator++()
		{
			if( bFinished == false )
				nPos = hsh.getNextPos( nPos, bFinished );

			return *this;
		}

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
		
		bool operator!=( const iterator &oth )
		{
			return !(*this == oth );
		}

		std::pair<key,value> operator *()
		{
			return hsh.getAtPos( nPos );
		}
	};

	iterator begin()
	{
		return iterator( *this );
	}
	
	iterator end()
	{
		return iterator( *this, true );
	}

private:
	void clearBits()
	{
		for( uint32_t j = 0; j < nKeysSize; j++ )
		{
			bFilled[j] = bDeleted[j] = 0;
		}
	}

	void fill( uint32_t loc, key &k, value &v, uint32_t hash )
	{
		bFilled[loc/32] |= (1<<(loc%32));
		va.construct( &aValues[loc], v );
		ka.construct( &aKeys[loc], k );
		aHashCodes[loc] = hash;
		nFilled++;
	}

	void _erase( uint32_t loc )
	{
		bDeleted[loc/32] |= (1<<(loc%32));
		va.destroy( &aValues[loc] );
		ka.destroy( &aKeys[loc] );
	}

	std::pair<key,value> getAtPos( uint32_t nPos )
	{
		return std::pair<key,value>(aKeys[nPos],aValues[nPos]);
	}

	uint32_t getFirstPos( bool &bFinished )
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

	uint32_t getNextPos( uint32_t nPos, bool &bFinished )
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

	void reHash( uint32_t nNewSize )
	{
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

		// Re-insert all of the old data (except deleted items)
		for( uint32_t j = 0; j < nOldCapacity; j++ )
		{
			if( (bOldFilled[j/32]&(1<<(j%32)))!=0 )
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

	bool isFilled( uint32_t loc )
	{
		return (bFilled[loc/32]&(1<<(loc%32)))!=0;
	}

	bool isDeleted( uint32_t loc )
	{
		return (bDeleted[loc/32]&(1<<(loc%32)))!=0;
	}

private:
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

template<> uint32_t __calcHashCode<const int>( const int k );
template<> bool __cmpHashKeys<const int>( const int a, const int b );

template<> uint32_t __calcHashCode<int>( int k );
template<> bool __cmpHashKeys<int>( int a, int b );

template<> uint32_t __calcHashCode<const char *>( const char *k );
template<> bool __cmpHashKeys<const char *>( const char *a, const char *b );

template<> uint32_t __calcHashCode<char *>( char *k );
template<> bool __cmpHashKeys<char *>( char *a, char *b );

template<> uint32_t __calcHashCode<const std::string>( const std::string k );
template<> bool __cmpHashKeys<const std::string>( const std::string a, const std::string b );

template<> uint32_t __calcHashCode<std::string>( std::string k );
template<> bool __cmpHashKeys<std::string>( std::string a, std::string b );

template<> uint32_t __calcHashCode<Hashable &>( Hashable &k );
template<> bool __cmpHashKeys<Hashable &>( Hashable &a, Hashable &b );

#endif
