#include "serializerbinary.h"
#include "serializable.h"

SerializerBinary::SerializerBinary(FILE *fhFile, bool bLoading):
	Serializer(bLoading),
	fhFile(fhFile),
	bCloseFile(false)
{	
}

SerializerBinary::SerializerBinary(char *sFileName, bool bLoading):
	Serializer(bLoading),
	bCloseFile(true)
{
	if (bLoading)
	{
		fhFile = fopen(sFileName, "rb");
	}
	else
	{
		fhFile = fopen(sFileName, "wb");
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
	fread(pData, nSize, 1, fhFile);
}

Serializer &SerializerBinary::operator<<(bool p)
{
	fwrite(&p, sizeof(p), 1, fhFile);
	return *this;
}
Serializer &SerializerBinary::operator<<(int8_t p)
{
	fwrite(&p, sizeof(p), 1, fhFile);
	return *this;
}
Serializer &SerializerBinary::operator<<(int16_t p)
{
	fwrite(&p, sizeof(p), 1, fhFile);
	return *this;
}
Serializer &SerializerBinary::operator<<(int32_t p)
{
	fwrite(&p, sizeof(p), 1, fhFile);
	return *this;
}
Serializer &SerializerBinary::operator<<(int64_t p)
{
	fwrite(&p, sizeof(p), 1, fhFile);
	return *this;
}
Serializer &SerializerBinary::operator<<(uint8_t p)
{
	fwrite(&p, sizeof(p), 1, fhFile);
	return *this;
}
Serializer &SerializerBinary::operator<<(uint16_t p)
{
	fwrite(&p, sizeof(p), 1, fhFile);
	return *this;
}
Serializer &SerializerBinary::operator<<(uint32_t p)
{
	fwrite(&p, sizeof(p), 1, fhFile);
	return *this;
}
Serializer &SerializerBinary::operator<<(uint64_t p)
{
	fwrite(&p, sizeof(p), 1, fhFile);
	return *this;
}
Serializer &SerializerBinary::operator<<(float p)
{
	fwrite(&p, sizeof(p), 1, fhFile);
	return *this;
}
Serializer &SerializerBinary::operator<<(double p)
{
	fwrite(&p, sizeof(p), 1, fhFile);
	return *this;
}
Serializer &SerializerBinary::operator<<(long double p)
{
	fwrite(&p, sizeof(p), 1, fhFile);
	return *this;
}

Serializer &SerializerBinary::operator>>(bool &p)
{
	fread(&p, sizeof(p), 1, fhFile);
	return *this;
}
Serializer &SerializerBinary::operator>>(int8_t &p)
{
	fread(&p, sizeof(p), 1, fhFile);
	return *this;
}
Serializer &SerializerBinary::operator>>(int16_t &p)
{
	fread(&p, sizeof(p), 1, fhFile);
	return *this;
}
Serializer &SerializerBinary::operator>>(int32_t &p)
{
	fread(&p, sizeof(p), 1, fhFile);
	return *this;
}
Serializer &SerializerBinary::operator>>(int64_t &p)
{
	fread(&p, sizeof(p), 1, fhFile);
	return *this;
}
Serializer &SerializerBinary::operator>>(uint8_t &p)
{
	fread(&p, sizeof(p), 1, fhFile);
	return *this;
}
Serializer &SerializerBinary::operator>>(uint16_t &p)
{
	fread(&p, sizeof(p), 1, fhFile);
	return *this;
}
Serializer &SerializerBinary::operator>>(uint32_t &p)
{
	fread(&p, sizeof(p), 1, fhFile);
	return *this;
}
Serializer &SerializerBinary::operator>>(uint64_t &p)
{
	fread(&p, sizeof(p), 1, fhFile);
	return *this;
}
Serializer &SerializerBinary::operator>>(float &p)
{
	fread(&p, sizeof(p), 1, fhFile);
	return *this;
}
Serializer &SerializerBinary::operator>>(double &p)
{
	fread(&p, sizeof(p), 1, fhFile);
	return *this;
}
Serializer &SerializerBinary::operator>>(long double &p)
{
	fread(&p, sizeof(p), 1, fhFile);
	return *this;
}
