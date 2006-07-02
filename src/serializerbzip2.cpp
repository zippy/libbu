#include "serializerbzip2.h"
#include "serializable.h"

#include <bzlib.h>

SerializerBZip2::SerializerBZip2(FILE *fhFile, bool bLoading):
	Serializer(bLoading),
	fhFile(fhFile),
	bCloseFile(false)
{
	if( bLoading )
	{
		bzFile = BZ2_bzReadOpen( &bzerror, fhFile, 0, 0, NULL, 0 );
		checkBZError();
	}
	else
	{
		bzFile = BZ2_bzWriteOpen( &bzerror, fhFile, 9, 0, 0 );
		checkBZError();
	}
}

SerializerBZip2::SerializerBZip2(char *sFileName, bool bLoading):
	Serializer(bLoading),
	bCloseFile(true)
{
	if (bLoading)
	{
		fhFile = fopen(sFileName, "rb");
		bzFile = BZ2_bzReadOpen( &bzerror, fhFile, 0, 0, NULL, 0 );
		checkBZError();
	}
	else
	{
		fhFile = fopen(sFileName, "wb");
		bzFile = BZ2_bzWriteOpen( &bzerror, fhFile, 9, 0, 0 );
		checkBZError();
	}
}

SerializerBZip2::~SerializerBZip2()
{
	close();
}

void SerializerBZip2::checkBZError()
{
}

void SerializerBZip2::close()
{
	if( bzFile != NULL )
	{
		if( isLoading() )
		{
			void *unused;
			int nUnused;
			BZ2_bzReadGetUnused( &bzerror, bzFile, &unused, &nUnused );
			BZ2_bzReadClose( &bzerror, bzFile );
			if( nUnused )
				fseek( fhFile, -nUnused, SEEK_CUR );
		}
		else
		{
			BZ2_bzWriteClose( &bzerror, bzFile, 0, 0, 0 );
		}
		checkBZError();
		bzFile = NULL;
	}
	if( fhFile != NULL && bCloseFile )
	{
		fclose(fhFile);
		fhFile = NULL;
	}
}

void SerializerBZip2::write(const void * pData, int32_t nSize)
{
	BZ2_bzWrite( &bzerror, bzFile, (void *)pData, nSize );
	checkBZError();
}

void SerializerBZip2::read(void * pData, int32_t nSize)
{
	BZ2_bzRead( &bzerror, bzFile, pData, nSize );
	checkBZError();
}

Serializer &SerializerBZip2::operator<<(bool p)
{
	write( &p, sizeof(p) );
	return *this;
}
Serializer &SerializerBZip2::operator<<(int8_t p)
{
	write( &p, sizeof(p) );
	return *this;
}
Serializer &SerializerBZip2::operator<<(int16_t p)
{
	write (&p, sizeof(p) );
	return *this;
}
Serializer &SerializerBZip2::operator<<(int32_t p)
{
	write( &p, sizeof(p) );
	return *this;
}
Serializer &SerializerBZip2::operator<<(int64_t p)
{
	write( &p, sizeof(p) );
	return *this;
}
Serializer &SerializerBZip2::operator<<(uint8_t p)
{
	write( &p, sizeof(p) );
	return *this;
}
Serializer &SerializerBZip2::operator<<(uint16_t p)
{
	write( &p, sizeof(p) );
	return *this;
}
Serializer &SerializerBZip2::operator<<(uint32_t p)
{
	write( &p, sizeof(p) );
	return *this;
}
Serializer &SerializerBZip2::operator<<(uint64_t p)
{
	write( &p, sizeof(p) );
	return *this;
}
Serializer &SerializerBZip2::operator<<(float p)
{
	write( &p, sizeof(p) );
	return *this;
}
Serializer &SerializerBZip2::operator<<(double p)
{
	write( &p, sizeof(p) );
	return *this;
}
Serializer &SerializerBZip2::operator<<(long double p)
{
	write( &p, sizeof(p) );
	return *this;
}

Serializer &SerializerBZip2::operator>>(bool &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Serializer &SerializerBZip2::operator>>(int8_t &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Serializer &SerializerBZip2::operator>>(int16_t &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Serializer &SerializerBZip2::operator>>(int32_t &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Serializer &SerializerBZip2::operator>>(int64_t &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Serializer &SerializerBZip2::operator>>(uint8_t &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Serializer &SerializerBZip2::operator>>(uint16_t &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Serializer &SerializerBZip2::operator>>(uint32_t &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Serializer &SerializerBZip2::operator>>(uint64_t &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Serializer &SerializerBZip2::operator>>(float &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Serializer &SerializerBZip2::operator>>(double &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Serializer &SerializerBZip2::operator>>(long double &p)
{
	read( &p, sizeof(p) );
	return *this;
}

