/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_STREAM_STACK_H
#define BU_STREAM_STACK_H

#include "bu/stream.h"

#include <typeinfo>

namespace Bu
{
	class StreamStack : public Bu::Stream
	{
	private:
		typedef Bu::List<Bu::Stream *> FilterList;

	public:
		StreamStack();
		StreamStack( Bu::Stream *pStream );
		virtual ~StreamStack();

		bool isEmpty();
		bool hasStream();
		void setStream( Bu::Stream *pStream );

		void clear();
		void popFilter();
		Bu::Stream *getTop();

		Bu::Stream *getStream();

		template<typename filter>
		Bu::Stream *findFilter()
		{
			for( FilterList::iterator i = lFilts.begin(); i; i++ )
			{
				if( typeid(**i) == typeid( filter ) )
				{
					return *i;
				}
			}

			throw Bu::ExceptionBase("Filter not found.");
		}

		template<typename filter>
		void pushFilter()
		{
			checkStack();

			filter *pFlt = new filter( *lFilts.first() );
			lFilts.prepend( pFlt );
		}

		template<typename filter, typename p1t>
		void pushFilter( p1t p1 )
		{
			checkStack();

			filter *pFlt = new filter( *lFilts.first(), p1 );
			lFilts.prepend( pFlt );
		}

		template<typename filter, typename p1t, typename p2t>
		void pushFilter( p1t p1, p2t p2 )
		{
			checkStack();

			filter *pFlt = new filter( *lFilts.first(), p1, p2 );
			lFilts.prepend( pFlt );
		}

		template<typename filter, typename p1t, typename p2t, typename p3t>
		void pushFilter( p1t p1, p2t p2, p3t p3 )
		{
			checkStack();

			filter *pFlt = new filter( *lFilts.first(), p1, p2, p3 );
			lFilts.prepend( pFlt );
		}

		template<typename filter, typename p1t, typename p2t, typename p3t,
			typename p4t>
		void pushFilter( p1t p1, p2t p2, p3t p3, p4t p4 )
		{
			checkStack();

			filter *pFlt = new filter( *lFilts.first(), p1, p2, p3, p4 );
			lFilts.prepend( pFlt );
		}

		template<typename filter, typename p1t, typename p2t, typename p3t,
			typename p4t, typename p5t>
		void pushFilter( p1t p1, p2t p2, p3t p3, p4t p4, p5t p5 )
		{
			checkStack();

			filter *pFlt = new filter( *lFilts.first(), p1, p2, p3, p4, p5 );
			lFilts.prepend( pFlt );
		}

		//
		// Everything below here merely passes on the call to the top of the
		// stream stack.
		//

		virtual void close();
		virtual Bu::size read( void *pBuf, Bu::size nBytes );
		virtual Bu::size write( const void *pBuf, Bu::size nBytes );

		virtual Bu::size write( const Bu::String &sBuf );
		virtual Bu::size tell();
		virtual void seek( Bu::size offset );
		virtual void setPos( Bu::size pos );
		virtual void setPosEnd( Bu::size pos );
		virtual bool isEos();
		virtual bool isOpen();
		virtual void flush();
		virtual bool canRead();
		virtual bool canWrite();
		virtual bool isReadable();
		virtual bool isWritable();
		virtual bool isSeekable();
		virtual bool isBlocking();
		virtual void setBlocking( bool bBlocking=true );
		virtual void setSize( Bu::size iSize );
		virtual size getSize() const;
		virtual size getBlockSize() const;
		virtual Bu::String getLocation() const;

	private:
		void checkStack() const;

	private:
		FilterList lFilts;
	};
};

#endif
