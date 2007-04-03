#include "sfile.h"
#include "exceptions.h"

Bu::SFile::SFile( const char *sName, const char *sFlags )
{
	fh = fopen( sName, sFlags );
}

Bu::SFile::~SFile()
{
	close();
}

void Bu::SFile::close()
{
	if( fh )
	{
		fclose( fh );
		fh = NULL;
	}
}

size_t Bu::SFile::read( char *pBuf, size_t nBytes )
{
	if( !fh )
		throw FileException("File not open.");

	return fread( pBuf, 1, nBytes, fh );
}

size_t Bu::SFile::write( const char *pBuf, size_t nBytes )
{
	if( !fh )
		throw FileException("File not open.");

	return fwrite( pBuf, 1, nBytes, fh );
}

long Bu::SFile::tell()
{
	if( !fh )
		throw FileException("File not open.");

	return ftell( fh );
}

void Bu::SFile::seek( long offset )
{
	if( !fh )
		throw FileException("File not open.");

	fseek( fh, offset, SEEK_CUR );
}

void Bu::SFile::setPos( long pos )
{
	if( !fh )
		throw FileException("File not open.");

	fseek( fh, pos, SEEK_SET );
}

void Bu::SFile::setPosEnd( long pos )
{
	if( !fh )
		throw FileException("File not open.");
	
	fseek( fh, pos, SEEK_END );
}

bool Bu::SFile::isEOS()
{
	return feof( fh );
}

bool Bu::SFile::canRead()
{
	return true;
}

bool Bu::SFile::canWrite()
{
	return true;
}

bool Bu::SFile::canSeek()
{
	return true;
}

