#include "file.h"
#include "exceptions.h"
#include <errno.h>

Bu::File::File( const char *sName, const char *sFlags )
{
	fh = fopen( sName, sFlags );
	if( fh == NULL )
	{
		throw Bu::FileException( errno, strerror(errno) );
	}
}

Bu::File::~File()
{
	close();
}

void Bu::File::close()
{
	if( fh )
	{
		fclose( fh );
		fh = NULL;
	}
}

size_t Bu::File::read( void *pBuf, size_t nBytes )
{
	if( !fh )
		throw FileException("File not open.");

	int nAmnt = fread( pBuf, 1, nBytes, fh );

	if( nAmnt == 0 )
		throw FileException("End of file.");

	return nAmnt;
}

size_t Bu::File::write( const void *pBuf, size_t nBytes )
{
	if( !fh )
		throw FileException("File not open.");

	return fwrite( pBuf, 1, nBytes, fh );
}

long Bu::File::tell()
{
	if( !fh )
		throw FileException("File not open.");

	return ftell( fh );
}

void Bu::File::seek( long offset )
{
	if( !fh )
		throw FileException("File not open.");

	fseek( fh, offset, SEEK_CUR );
}

void Bu::File::setPos( long pos )
{
	if( !fh )
		throw FileException("File not open.");

	fseek( fh, pos, SEEK_SET );
}

void Bu::File::setPosEnd( long pos )
{
	if( !fh )
		throw FileException("File not open.");
	
	fseek( fh, pos, SEEK_END );
}

bool Bu::File::isEOS()
{
	return feof( fh );
}

bool Bu::File::canRead()
{
	return true;
}

bool Bu::File::canWrite()
{
	return true;
}

bool Bu::File::canSeek()
{
	return true;
}

bool Bu::File::isBlocking()
{
	return true;
}

void Bu::File::setBlocking( bool bBlocking )
{
	return;
}

