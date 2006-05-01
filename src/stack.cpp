#include "stack.h"

void Stack::push( void *data )
{
	lStackData.append( data );
}

void *Stack::top()
{
	return lStackData.getAt( lStackData.getSize()-1 );
}

void Stack::pop()
{
	lStackData.deleteAt( lStackData.getSize()-1 );
}

void *Stack::poptop()
{
	void *dat = top();
	pop();
	return dat;
}

bool Stack::isEmpty()
{
	return lStackData.isEmpty();
}

void Stack::empty()
{
	lStackData.empty();
}
