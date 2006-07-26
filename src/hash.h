#ifndef HASH_H
#define HASH_H
/*
#include <stddef.h>
#include <string.h>
#include "hashable.h"

#define bitsToBytes( n ) (n/8+(n%8>0 ? 1 : 0))

template<class key, class value>
class Hash
{
	class iterator
	{
		friend class Hash<key, value>;
	private:
		iterator( Hash<key, value> *hsh, int nIndex, key keyval, bool bInsert ) :
			hsh( hsh ),
			nIndex( nIndex ),
			keyval( keyval ),
			bInsert( bInsert )
		{
		}

	public:
		iterator() :
			hsh( NULL ),
			nIndex( -1 )
		{
		}

		iterator &operator=( iterator &src )
		{
			this->hsh = src.hsh;
			this->nIndex = src.nIndex;
		}

		iterator &operator=( value &src )
		{
			if( bInsert )
				printf("You wanted to insert %d\n", src );
			else
				printf("You wanted to insert %d\n", src );
		}

	private:
		Hash<key, value> *hsh;
		int nIndex;
		bool bInsert;
		key keyval;
	};

	template<class refval>
	class VRef
	{
	public:
		VRef( refval &data ) :
			data( data )
		{
		}
		refval &data;
	};

public:
	Hash() :
		nCapacity( 11 ),
		nFilled( 0 ),
		bFilled( NULL ),
		aKeys( NULL ),
		aValues( NULL ),
		aHashCodes( NULL )
	{
		int nKeysSize = bitsToBytes( nCapacity );
		bFilled = new unsigned char[ nKeysSize ];
		memset( bFilled, 0, nKeysSize );

		aKeys = new VRef<key>*[nCapacity];
		aValues = new value[nCapacity];
	}

	virtual ~Hash()
	{
	}

	iterator operator[]( key keyval )
	{
		//iterator i( this, 4, keyval, true );
		//return i;
		printf("%s\n", keyval.getString() );
	}

	int hasKey( key keyval )
	{
		printf("%s\n", keyval.getString() );
	}

private:
	int nCapacity;
	int nFilled;
	unsigned char *bFilled;
	VRef<key> **aKeys;
	unsigned long int *aHashCodes;
	value *aValues;
};
*/
#endif
