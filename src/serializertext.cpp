#include "serializertext.h"

SerializerText::SerializerText(FILE *fhFile, bool bLoading):
	Serializer(bLoading),
	fhFile(fhFile),
	bCloseFile(false)
{	
}

SerializerText::SerializerText(const char *sFileName, bool bLoading):
	Serializer(bLoading),
	bCloseFile(true)
{
	if (bLoading)
	{
		fhFile = fopen(sFileName, "rt");
	}
	else
	{
		fhFile = fopen(sFileName, "wt");
	}
}

SerializerText::~SerializerText()
{
	close();
}

void SerializerText::close()
{
	if (fhFile != NULL)
	{
		fclose(fhFile);
		fhFile = NULL;
	}
}

void SerializerText::write(const void * pData, int32_t nSize)
{
	fwrite(pData, nSize, 1, fhFile);
	fprintf(fhFile, "\n");
}

void SerializerText::read(void * pData, int32_t nSize)
{
	fread(pData, nSize, 1, fhFile);
	fgetc(fhFile);
}

Serializer &SerializerText::operator<<(bool p)
{
	fprintf(fhFile, "%hhd\n", p);
	return *this;
}
Serializer &SerializerText::operator<<(int8_t p)
{
	fprintf(fhFile, "%hhd\n", p);
	return *this;
}
Serializer &SerializerText::operator<<(int16_t p)
{
	fprintf(fhFile, "%hd\n", p);
	return *this;
}
Serializer &SerializerText::operator<<(int32_t p)
{
	fprintf(fhFile, "%d\n", p);
	return *this;
}
Serializer &SerializerText::operator<<(int64_t p)
{
	fprintf(fhFile, "%lld\n", p);
	return *this;
}
Serializer &SerializerText::operator<<(uint8_t p)
{
	fprintf(fhFile, "%hhu\n", p);
	return *this;
}
Serializer &SerializerText::operator<<(uint16_t p)
{
	fprintf(fhFile, "%hu\n", p);
	return *this;
}
Serializer &SerializerText::operator<<(uint32_t p)
{
	fprintf(fhFile, "%u\n", p);
	return *this;
}
Serializer &SerializerText::operator<<(uint64_t p)
{
	fprintf(fhFile, "%llu\n", p);
	return *this;
}
Serializer &SerializerText::operator<<(float p)
{
	fprintf(fhFile, "%f\n", p);
	return *this;
}
Serializer &SerializerText::operator<<(double p)
{
	fprintf(fhFile, "%f\n", p);
	return *this;
}
Serializer &SerializerText::operator<<(long double p)
{
	fprintf(fhFile, "%Lf\n", p);
	return *this;
}

Serializer &SerializerText::operator>>(bool &p)
{
	fscanf(fhFile, "%hhd\n", ((signed char *)&p));
	return *this;
}
Serializer &SerializerText::operator>>(int8_t &p)
{
	fscanf(fhFile, "%hhd\n", &p);
	return *this;
}
Serializer &SerializerText::operator>>(int16_t &p)
{
	fscanf(fhFile, "%hd\n", &p);
	return *this;
}
Serializer &SerializerText::operator>>(int32_t &p)
{
	fscanf(fhFile, "%d\n", &p);
	return *this;
}
Serializer &SerializerText::operator>>(int64_t &p)
{
	fscanf(fhFile, "%lld\n", &p);
	return *this;
}
Serializer &SerializerText::operator>>(uint8_t &p)
{
	fscanf(fhFile, "%hhu\n", &p);
	return *this;
}
Serializer &SerializerText::operator>>(uint16_t &p)
{
	fscanf(fhFile, "%hu\n", &p);
	return *this;
}
Serializer &SerializerText::operator>>(uint32_t &p)
{
	fscanf(fhFile, "%u\n", &p);
	return *this;
}
Serializer &SerializerText::operator>>(uint64_t &p)
{
	fscanf(fhFile, "%llu\n", &p);
	return *this;
}
Serializer &SerializerText::operator>>(float &p)
{
	fscanf(fhFile, "%f\n", &p);
	return *this;
}
Serializer &SerializerText::operator>>(double &p)
{
	fscanf(fhFile, "%lf\n", &p);
	return *this;
}
Serializer &SerializerText::operator>>(long double &p)
{
	fscanf(fhFile, "%Lf\n", &p);
	return *this;
}
