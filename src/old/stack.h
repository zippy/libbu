#ifndef STACK_H
#define STACK_H
#include "linkedlist.h"

/** An ultra-simple stack implementation that just uses a linked list.
  *@author Mike Buland
  */
class Stack
{
public:
	/** Pushes a new value onto the top of the stack.
	  *@param data A new value for the stack.
	  *@author Mike Buland
	  */
	void push( void *data );

	/** Returns the top value off of the stack, but doesn't remove it from the
	  * stack.
	  *@returns The value at the top of the stack.
	  *@author Mike Buland
	  */
	void *top();

	/** Pops the top item off of the stack.
	  *@author Mike Buland
	  */
	void pop();

	/** Gets the top item off of the stack, pops it off the stack, and returns
	  * it.
	  *@returns The value at the top of the stack.
	  *@author Mike Buland
	  */
	void *poptop();

	/** Checks if the stack is empty.
	  *@returns True if the stack is empty, and false if it has things in it.
	  *@author Mike Buland
	  */
	bool isEmpty();

	/** Empty the stack.
	  *@author Mike Buland
	  */
	void empty();

private:
	LinkedList lStackData;	/**< The actual stack data. */
};
#endif
