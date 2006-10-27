#include "file.h"
#include "exceptions.h"

File::File( const char *sName, const char *sFlags )
{
	fh = fopen( sName, sFlags );
}

File::~File()
{
}

void File::close()
{
	if( fh )
	{
		fclose( fh );
		fh = NULL;
	}
}

size_t File::read( char *pBuf, size_t nBytes )
{
	if( !fh )
		throw FileException("File not open.");

	return fread( pBuf, 1, nBytes, fh );
}

size_t File::write( char *pBuf, size_t nBytes )
{
	if( !fh )
		throw FileException("File not open.");

	return fwrite( pBuf, 1, nBytes, fh );
}

