#include "sfile.h"
#include "exceptions.h"

SFile::SFile( const char *sName, const char *sFlags )
{
	fh = fopen( sName, sFlags );
}

SFile::~SFile()
{
}

void SFile::close()
{
	if( fh )
	{
		fclose( fh );
		fh = NULL;
	}
}

size_t SFile::read( char *pBuf, size_t nBytes )
{
	if( !fh )
		throw FileException("File not open.");

	return fread( pBuf, 1, nBytes, fh );
}

size_t SFile::write( const char *pBuf, size_t nBytes )
{
	if( !fh )
		throw FileException("File not open.");

	return fwrite( pBuf, 1, nBytes, fh );
}

long SFile::tell()
{
	if( !fh )
		throw FileException("File not open.");

	return ftell( fh );
}

void SFile::seek( long offset )
{
	if( !fh )
		throw FileException("File not open.");

	fseek( fh, offset, SEEK_CUR );
}

void SFile::setPos( long pos )
{
	if( !fh )
		throw FileException("File not open.");

	fseek( fh, pos, SEEK_SET );
}

void SFile::setPosEnd( long pos )
{
	if( !fh )
		throw FileException("File not open.");
	
	fseek( fh, pos, SEEK_END );
}

bool SFile::isEOS()
{
	return feof( fh );
}

