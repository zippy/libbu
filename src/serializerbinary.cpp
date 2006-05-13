#include "serializerbinary.h"

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
	if (fhFile != NULL)
	{
		fclose(fhFile);
		fhFile = NULL;
	}
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
	printf("int: %d, size: %d\n", p, sizeof(p));
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
	bool bTmp;
	fread(&bTmp, sizeof(p), 1, fhFile);
	p = bTmp;
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
	printf("reding before: %d", p);
	fread(&p, sizeof(p), 1, fhFile);
	printf(" after: %d\n", p);
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
