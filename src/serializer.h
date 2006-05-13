#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <stdint.h>
//#include "serializable.h"

class Serializer
{
private:
	bool bLoading;
public:
	bool isLoading();
	
	Serializer(bool bLoading);
	virtual ~Serializer();
	virtual void close()=0;

	virtual Serializer &operator<<(bool)=0;
	virtual Serializer &operator<<(int8_t)=0;
	virtual Serializer &operator<<(int16_t)=0;
	virtual Serializer &operator<<(int32_t)=0;
	virtual Serializer &operator<<(int64_t)=0;
	virtual Serializer &operator<<(uint8_t)=0;
	virtual Serializer &operator<<(uint16_t)=0;
	virtual Serializer &operator<<(uint32_t)=0;
	virtual Serializer &operator<<(uint64_t)=0;
	virtual Serializer &operator<<(float)=0;
	virtual Serializer &operator<<(double)=0;
	virtual Serializer &operator<<(long double)=0;

	virtual Serializer &operator>>(bool &)=0;
	virtual Serializer &operator>>(int8_t &)=0;
	virtual Serializer &operator>>(int16_t &)=0;
	virtual Serializer &operator>>(int32_t &)=0;
	virtual Serializer &operator>>(int64_t &)=0;
	virtual Serializer &operator>>(uint8_t &)=0;
	virtual Serializer &operator>>(uint16_t &)=0;
	virtual Serializer &operator>>(uint32_t &)=0;
	virtual Serializer &operator>>(uint64_t &)=0;
	virtual Serializer &operator>>(float &)=0;
	virtual Serializer &operator>>(double &)=0;
	virtual Serializer &operator>>(long double &)=0;
	
	/*
	virtual Serializer &operator&(bool &);
	virtual Serializer &operator&(int8_t &);
	virtual Serializer &operator&(int16_t &);
	virtual Serializer &operator&(int32_t &);
	virtual Serializer &operator&(int64_t &);
	virtual Serializer &operator&(uint8_t &);
	virtual Serializer &operator&(uint16_t &);
	virtual Serializer &operator&(uint32_t &);
	virtual Serializer &operator&(uint64_t &);
	virtual Serializer &operator&(float &);
	virtual Serializer &operator&(double &);
	virtual Serializer &operator&(long double &);
	*/

	virtual Serializer &operator<<(class Serializable &);
	virtual Serializer &operator>>(class Serializable &);
	//virtual Serializer &operator&(Serializable &);
};

#endif
