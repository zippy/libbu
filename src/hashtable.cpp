#include <string.h>
#include <stdio.h>
#include <math.h>

#include "hashtable.h"

HashTable::HashTable( HashFunction *hNewFunc, unsigned long int nInitSize, bool bAllowDupes )
{
	hFunc = hNewFunc;
	nTableSize = nextPrime( nInitSize );
	aTable = new HashNode[nTableSize];
	//for( int j = 0; j < nTableSize; j++ ) if( aTable[j].id || aTable[j].data || aTable[j].bDeleted ) printf("Unclean entry\n");
	nSize = 0;
	nFilled = 0;
	this->bAllowDupes = bAllowDupes;
}

HashTable::~HashTable()
{
	delete[] aTable;
	delete hFunc;
}

void HashTable::set( int j, const void *newID, const void *newData )
{
	if( newData == NULL )
	{
		printf("Inserting NULL data is indestinguishable from uninserted data!\n");
	}
	aTable[j].id = newID;
	aTable[j].data = newData;
}

void HashTable::clear()
{
	memset( aTable, 0, sizeof(HashNode) * nTableSize );
}

bool HashTable::isFilled( int j )
{
	return (aTable[j].id != NULL)||(aTable[j].bDeleted);
}

void HashTable::reHash( unsigned long int nNewSize )
{
	HashNode *aOldTable = aTable;
	unsigned long int oldSize = nTableSize;

	// If the table can still be used if we just get rid of deleted items, don't
	// change the size of the table, otherwise, go ahead and use the number
	// passed in.
	if( nSize > nTableSize>>1 )
	{
		nTableSize = nextPrime( nNewSize );
	}

	aTable = newTable( nTableSize );
	//for( int j = 0; j < nTableSize; j++ ) if( aTable[j].id || aTable[j].data || aTable[j].bDeleted ) printf("Unclean entry\n");

	nSize = 0;
	nFilled = 0;

	for( unsigned long int j = 0; j < oldSize; j++ )
	{
		if( aOldTable[j].id != NULL && aOldTable[j].bDeleted == false )
		{
			insert( aOldTable[j].id, aOldTable[j].data );
		}
	}

	delete[] aOldTable;
}

unsigned long int HashTable::probe( unsigned long int nStart, const void *id )
{
	int nHash = nStart;
	nStart = nStart%nTableSize;
	if( bAllowDupes == true )
	{
		for(
			unsigned long int j=0;
			isFilled( nStart ) && j < 32;
			nStart = (nStart+(1<<j))%nTableSize, j++
			);

		/**
		 * This is an ugly little hack.  If the hash table is too full in allow-
		 * dups mode we have to fall back on a linear search, otherwise you can
		 * only get up to 32 entries with the same name.
		 */
		if( isFilled( nStart ) )
		{
			unsigned long int nOldStart = nStart;
			for(
				nStart++;
				isFilled( nStart ) && nStart != nOldStart;
				nStart = (nStart+1)%nTableSize
			   );
		}
	}
	else
	{
		for(
			unsigned long int j=0;
			isFilled( nStart ) && j < 32;
			nStart = (nStart+(1<<j))%nTableSize, j++
			)
		{
			if( isFilled( nStart ) )
			{
				if( hFunc->cmpIDs( aTable[nStart].id, id ) == true &&
					aTable[nStart].bDeleted == false )
				{
					return nStart;
				}
			}
		}
	}
	// This is our insurance, if the table is full, then go ahead and rehash,
	// then try again.
	if( isFilled( nStart ) )
	{
		reHash( getCapacity()*2 );
		return probe( nHash, id );
	}
	return nStart;
}

HashTable::HashNode *HashTable::newTable( unsigned long int nNewSize )
{
	return new HashNode[nNewSize];
}

#ifdef HASH_DEBUG_VIS
void HashTable::printDebugLine( const char *exData )
{
	char *buf = new char[getCapacity()+3];
	int j;
	buf[0] = '[';
	for( j = 0; j < getCapacity(); j++ )
	{
		buf[j+1] = (aTable[j].bDeleted)?('X'):((isFilled( j ))?('#'):('-'));
	}
	buf[j+1] = ']';
	buf[j+2] = '\0';
	printf("%s %s\n", buf, exData );
	delete[] buf;
}
#endif

bool HashTable::insert( const void *id, const void *data )
{
	unsigned long int nPos = probe( hFunc->hash( id ), id )%nTableSize;

	if( bAllowDupes == true )
	{
		if( aTable[nPos].id == NULL && aTable[nPos].bDeleted == false )
		{
			set( nPos, id, data );
#ifdef HASH_DEBUG_VIS
			printDebugLine( (const char *)id );
#endif
			nSize++;
			nFilled++;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if( aTable[nPos].id == NULL && aTable[nPos].bDeleted == false )
		{
			set( nPos, id, data );
#ifdef HASH_DEBUG_VIS
			printDebugLine( (const char *)id );
#endif
			nSize++;
			nFilled++;
			return true;
		}
		else if( hFunc->cmpIDs( aTable[nPos].id, id ) == true )
		{
			set( nPos, id, data );
#ifdef HASH_DEBUG_VIS
			printDebugLine( (const char *)id );
#endif
			return true;
		}
		else
		{
			return false;
		}
	}
}

const void *HashTable::get( const void *id, unsigned long int nSkip )
{
	unsigned long int nPos = hFunc->hash( id )%nTableSize;

	for( unsigned long int j=0; j < 32; nPos = (nPos+(1<<j))%nTableSize, j++ )
	{
		if( !isFilled( nPos ) ) return NULL;
		if( aTable[nPos].bDeleted == false )
		{
			if( hFunc->cmpIDs( id, aTable[nPos].id ) )
			{
				if( nSkip == 0 )
				{
					return aTable[nPos].data;
				}
				else
				{
					nSkip--;
				}
			}
		}
	}

	if( bAllowDupes )
	{
		unsigned long int nOldPos = nPos;
		for( nPos++; nPos != nOldPos; nPos=(nPos+1)%nTableSize )
		{
			if( !isFilled( nPos ) ) return NULL;
			if( aTable[nPos].bDeleted == false )
			{
				if( hFunc->cmpIDs( id, aTable[nPos].id ) )
				{
					if( nSkip == 0 )
					{
						return aTable[nPos].data;
					}
					else
					{
						nSkip--;
					}
				}
			}
		}
	}

	return NULL;
}

const void *HashTable::getKey( const void *id, unsigned long int nSkip )
{
	unsigned long int nPos = hFunc->hash( id )%nTableSize;

	for( unsigned long int j=0; j < 32; nPos = (nPos+(1<<j))%nTableSize, j++ )
	{
		if( !isFilled( nPos ) ) return NULL;
		if( aTable[nPos].bDeleted == false )
		{
			if( hFunc->cmpIDs( id, aTable[nPos].id ) )
			{
				if( nSkip == 0 )
				{
					return aTable[nPos].id;
				}
				else
				{
					nSkip--;
				}
			}
		}
	}

	if( bAllowDupes )
	{
		unsigned long int nOldPos = nPos;
		for( nPos++; nPos != nOldPos; nPos=(nPos+1)%nTableSize )
		{
			if( !isFilled( nPos ) ) return NULL;
			if( aTable[nPos].bDeleted == false )
			{
				if( hFunc->cmpIDs( id, aTable[nPos].id ) )
				{
					if( nSkip == 0 )
					{
						return aTable[nPos].id;
					}
					else
					{
						nSkip--;
					}
				}
			}
		}
	}

	return NULL;
}

void *HashTable::getFirstItemPos()
{
	HashPos *pos = new HashPos;
	return pos;
}

const void *HashTable::getItemData( void *xPos )
{
	return aTable[((HashPos *)xPos)->nPos].data;
}

const void *HashTable::getItemID( void *xPos )
{
	return aTable[((HashPos *)xPos)->nPos].id;
}

void *HashTable::getNextItemPos( void *xPos )
{
	HashPos *pos = (HashPos *)xPos;
	if( pos->bStarted == false )
	{
		pos->bStarted = true;
		pos->nPos = 0;
	}
	else
	{
		pos->nPos++;
	}
	if( pos->nPos < nTableSize )
	{
		for( ; pos->nPos < nTableSize; pos->nPos++ )
		{
			if( isFilled( pos->nPos ) &&
				aTable[pos->nPos].bDeleted == false )
			{
				return xPos;
			}
		}
	}

	delete pos;

	return NULL;
}

// Big-O sqrt(n)
// Change this to be erethpothynies table with a storage
// lookup later on.
bool HashTable::isPrime (int num)
{
    if (num == 2)         // the only even prime
        return true;
    else if (num % 2 == 0)     // other even numbers are composite
        return false;
    else
    {
        //bool prime = true;
        int divisor = 3;
        int upperLimit = static_cast<int>(sqrt(num) + 1);
        while (divisor <= upperLimit)
        {
            if (num % divisor == 0)
				return false;
            //    prime = false;
            divisor +=2;
        }
        return true;
    }
}

// Big-O n^(3/2)
int HashTable::nextPrime( int base )
{
	int nPrime;
	for( nPrime = base; isPrime( nPrime ) == false; nPrime++ );
	return nPrime;
}

unsigned long int HashTable::getCapacity()
{
	return nTableSize;
}

unsigned long int HashTable::getSize()
{
	return nSize;
}

double HashTable::getLoad()
{
	return (double)(nFilled)/(double)(nTableSize);
}

const void *HashTable::operator[](const void *id)
{
	return get( id );
}

bool HashTable::del( const void *id, int nSkip )
{
	unsigned long int nPos = hFunc->hash( id )%nTableSize;

	for( unsigned long int j=0; j < 32; nPos = (nPos+(1<<j))%nTableSize, j++ )
	{
		if( !isFilled( nPos ) ) return false;
		//printf("0x%08X \"%s\" == 0x%08X \"%s\" (%d)\n", id, id, aTable[nPos].id, aTable[nPos].id, nPos );
		if( hFunc->cmpIDs( id, aTable[nPos].id ) &&
			aTable[nPos].bDeleted == false )
		{
			if( nSkip == 0 )
			{
				aTable[nPos].bDeleted = true;
				nSize--;
#ifdef HASH_DEBUG_VIS
				printDebugLine( (const char *)id );
#endif
				return true;
			}
			else
			{
				nSkip--;
			}
		}
	}

	return false;
}

