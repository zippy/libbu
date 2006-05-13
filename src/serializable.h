#ifndef SERIALIZER_H
#define SERIALIZER_H

//#include "serializer.h"

class Serializable
{
public:
	Serializable();
	virtual ~Serializable();
	virtual void serialize(class Serializer &)=0;
};

#endif
