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
		throw FileException("SFile not open.");

	return fread( pBuf, 1, nBytes, fh );
}

size_t SFile::write( char *pBuf, size_t nBytes )
{
	if( !fh )
		throw FileException("SFile not open.");

	return fwrite( pBuf, 1, nBytes, fh );
}

