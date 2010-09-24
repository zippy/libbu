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
		virtual size_t read( void *pBuf, size_t nBytes );
		virtual size_t write( const void *pBuf, size_t nBytes );

		virtual size_t write( const Bu::FString &sBuf );
		virtual long tell();
		virtual void seek( long offset );
		virtual void setPos( long pos );
		virtual void setPosEnd( long pos );
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
		virtual void setSize( long iSize );

	private:
		void checkStack();

	private:
		FilterList lFilts;
	};
};

#endif
