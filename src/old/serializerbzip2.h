#ifndef SERIALIZER_BINARY_H
#define SERIALIZER_BINARY_H

#include "serializer.h"
#include <stdio.h>

class SerializerBZip2 : public Serializer
{
public:
	SerializerBZip2(FILE *fhFile, bool bLoading);
	SerializerBZip2(char *sFileName, bool bLoading);
	virtual ~SerializerBZip2();

	virtual void close();

	virtual void write(const void *, int32_t);
	virtual void read(void *, int32_t);
	
private:
	void checkBZError();
	FILE *fhFile;
	void *bzFile;
	bool bCloseFile;
	int bzerror;
};

#endif
