#include "serializerbinary.h"
#include "serializable.h"
#include "exceptions.h"

SerializerBinary::SerializerBinary(FILE *fhFile, bool bLoading):
	Serializer(bLoading),
	fhFile(fhFile),
	bCloseFile(false)
{	
}

SerializerBinary::SerializerBinary(const char *sFileName, bool bLoading):
	Serializer(bLoading),
	bCloseFile(true)
{
	if (bLoading)
	{
		fhFile = fopen(sFileName, "rb");
		if( fhFile == NULL )
			throw FileException("Unable to open file: %s", sFileName );
	}
	else
	{
		fhFile = fopen(sFileName, "wb");
		if( fhFile == NULL )
			throw FileException("Unable to open file: %s", sFileName );
	}
}

SerializerBinary::~SerializerBinary()
{
	close();
}

void SerializerBinary::close()
{
	if (fhFile != NULL && bCloseFile )
	{
		fclose(fhFile);
		fhFile = NULL;
	}
}

void SerializerBinary::write(const void * pData, int32_t nSize)
{
	fwrite(pData, nSize, 1, fhFile);
}

void SerializerBinary::read(void * pData, int32_t nSize)
{
	if( nSize == 0 )
		return;

	uint32_t nRead = fread(pData, nSize, 1, fhFile);
	if( nRead < 1 )
	{
		throw FileException( excodeEOF, "End of file read");
	}
}

