#ifndef FILE_H
#define FILE_H

#include <stdint.h>

#include "bu/stream.h"
#include "bu/fstring.h"

namespace Bu
{
	class File : public Bu::Stream
	{
	public:
		File( const char *sName, const char *sFlags );
		File( const Bu::FString &sName, const char *sFlags );
		File( int fd, const char *sFlags );
		virtual ~File();

		virtual void close();
		virtual size_t read( void *pBuf, size_t nBytes );
		virtual size_t write( const void *pBuf, size_t nBytes );

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

		inline static Bu::File tempFile( Bu::FString &sName, const char *sFlags )
		{
			int afh_d = mkstemp( sName.getStr() );

			return Bu::File( afh_d, sFlags );
		}

		void truncate( long nSize );
		void chmod( mode_t t );

	private:
		FILE *fh;

	};
}

#endif
