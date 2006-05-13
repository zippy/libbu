#ifndef SERIALIZER_BINARY_H
#define SERIALIZER_BINARY_H

#include "serializer.h"
#include <stdio.h>

class SerializerBinary : public Serializer
{
public:
	SerializerBinary(FILE *fhFile, bool bLoading);
	SerializerBinary(char *sFileName, bool bLoading);
	~SerializerBinary();

	virtual void close();

	virtual void write(const void *, int32_t);
	virtual void read(void *, int32_t);
	
	virtual Serializer &operator<<(bool);
	virtual Serializer &operator<<(int8_t);
	virtual Serializer &operator<<(int16_t);
	virtual Serializer &operator<<(int32_t);
	virtual Serializer &operator<<(int64_t);
	virtual Serializer &operator<<(uint8_t);
	virtual Serializer &operator<<(uint16_t);
	virtual Serializer &operator<<(uint32_t);
	virtual Serializer &operator<<(uint64_t);
	virtual Serializer &operator<<(float);
	virtual Serializer &operator<<(double);
	virtual Serializer &operator<<(long double);

	virtual Serializer &operator>>(bool &);
	virtual Serializer &operator>>(int8_t &);
	virtual Serializer &operator>>(int16_t &);
	virtual Serializer &operator>>(int32_t &);
	virtual Serializer &operator>>(int64_t &);
	virtual Serializer &operator>>(uint8_t &);
	virtual Serializer &operator>>(uint16_t &);
	virtual Serializer &operator>>(uint32_t &);
	virtual Serializer &operator>>(uint64_t &);
	virtual Serializer &operator>>(float &);
	virtual Serializer &operator>>(double &);
	virtual Serializer &operator>>(long double &);

private:
	FILE *fhFile;
	bool bCloseFile;
};

#endif
