/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_QUEUE_H
#define BU_QUEUE_H

namespace Bu
{
	/**
	 * Queue abstract baseclass
	 */
	template<typename value>
	class Queue
	{
	public:
		Queue()
		{
		}

		virtual ~Queue()
		{
		}

		virtual void enqueue( const value &i )=0;
		virtual value dequeue()=0;
		virtual value &peek()=0;
		virtual const value &peek() const=0;
		virtual bool isEmpty() const=0;
		virtual int getSize() const=0;

	private:

	};
}

#endif
