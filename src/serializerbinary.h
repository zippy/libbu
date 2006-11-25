#ifndef SERIALIZER_BINARY_H
#define SERIALIZER_BINARY_H

#include "serializer.h"
#include <stdio.h>

class SerializerBinary : public Serializer
{
public:
	SerializerBinary(FILE *fhFile, bool bLoading);
	SerializerBinary(const char *sFileName, bool bLoading);
	virtual ~SerializerBinary();

	virtual void close();

	virtual void write(const void *, int32_t);
	virtual void read(void *, int32_t);
	
private:
	FILE *fhFile;
	bool bCloseFile;
};

#endif
