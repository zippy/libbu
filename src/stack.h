/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_STACK_H
#define BU_STACK_H

#include <memory>

namespace Bu
{
	template<typename value, typename valuealloc=std::allocator<value> >
	class Stack
	{
	private:
		typedef struct Chunk
		{
			value *pValue;
			Chunk *pPrev;
		} Chunk;
	public:
		Stack() :
			pTop( NULL )
		{
		}

		virtual ~Stack()
		{
		}

		void push( const value &v )
		{
			Chunk *pChnk = new Chunk;
			pChnk->pValue = va.allocate( 1 );
			va.construct( pChnk->pValue, v );
			pChnk->pPrev = pTop;
			pTop = pChnk;
		}

		value &peek()
		{
			return *pTop->pValue;
		}
		
		value &top()
		{
			return *pTop->pValue;
		}

		value pop()
		{
			value ret( *pTop->pValue );
			
			Chunk *pChnk = pTop;
			pTop = pTop->pPrev;

			va.destroy( pChnk->pValue );
			va.deallocate( pChnk->pValue, 1 );
			delete pChnk;

			return ret;
		}

		void clear()
		{
			while( !isEmpty() )
				pop();
		}

		bool isEmpty()
		{
			return pTop == NULL;
		}

	private:
		Chunk *pTop;
		valuealloc va;
	};
}

#endif
