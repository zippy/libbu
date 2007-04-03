#include "arraylist.h"
#include <stdlib.h>
#include <string.h>

ArrayList::ArrayList( int initSize, int growByFactor )
{
	apData = new void *[initSize];
	nSize = 0;
	nCapacity = initSize;
	nGrowByFactor = growByFactor;
}

ArrayList::~ArrayList(  )
{
	delete[] apData;
}

void *ArrayList::getAt( int index )
{
	if( index < 0 || index > nSize )
		return NULL;

	return apData[index];
}

void ArrayList::append( void *data )
{
	insertBefore( data, nSize );
}

void ArrayList::insertBefore( void *data, int pos )
{
	if( pos < 0 || pos > nSize )
		return;

	checkResize();
	memmove( &apData[pos+1], &apData[pos], (nSize-pos)*sizeof(void*) );
	apData[pos] = data;
	nSize++;
}

int ArrayList::getSize(  )
{
	return nSize;
}

bool ArrayList::isEmpty(  )
{
	return nSize==0;
}

void ArrayList::deleteAt( int index )
{
	if( index < 0 || index >= nSize )
		return;

	memmove( &apData[index], &apData[index+1], (nSize-index-1)*sizeof(void *) );
	nSize--;
}

void ArrayList::empty()
{
	// Probably the easiest as far as things go.
	nSize = 0;
}

void ArrayList::resizeTo( int newSize )
{
	void **apNew = new void *[newSize];
	memmove( apNew, apData, nSize*sizeof(void *) );
	nCapacity = newSize;
	delete[] apData;
	apData = apNew;
}

void ArrayList::checkResize()
{
	if( nSize >= nCapacity )
	{
		resizeTo( nCapacity + nGrowByFactor );
	}
}

void ArrayList::setSize( int newSize )
{
	if( newSize < 0 )
		return;

	nSize = newSize;
	checkResize();
}

void ArrayList::setAt( int index, void *data )
{
	if( index < 0 || index >= nSize )
		return;

	apData[index] = data;
}

