#ifndef BU_NIDS_STREAM_H
#define BU_NIDS_STREAM_H

#include "bu/stream.h"
#include "bu/nids.h"

namespace Bu
{
	class NidsStream : public Bu::Stream
	{
		friend class Nids;
	private:
		/**
		 * These can only be created by the Nids class.
		 */
		NidsStream( Nids &rNids, uint32_t uStream );

	public:
//		NidsStream( const NidsStream &rSrc );
		virtual ~NidsStream();

		virtual void close();
		virtual size_t read( void *pBuf, size_t nBytes );
		virtual size_t write( const void *pBuf, size_t nBytes );
		using Stream::write;
		virtual long tell();
		virtual void seek( long offset );
		virtual void setPos( long pos );
		virtual void setPosEnd( long pos );
		virtual bool isEOS();
		virtual bool isOpen();
		virtual void flush();
		virtual bool canRead();
		virtual bool canWrite();
		virtual bool isReadable();
		virtual bool isWritable();
		virtual bool isSeekable();
		virtual bool isBlocking();
		virtual void setBlocking( bool bBlocking=true );

	private:
		Nids &rNids;
		uint32_t uStream;
		Nids::Block *pCurBlock;
		uint32_t uCurBlock;
		uint32_t uSize;
		uint32_t uBlockSize;
		uint32_t uPos; 
	};
};

#endif
