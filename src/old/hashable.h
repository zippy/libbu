#ifndef HASHABLE_H
#define HASHABLE_H

class Hashable
{
public:
	virtual ~Hashable() {};
	virtual unsigned long int getHashCode() = 0;
	virtual bool compareForHash( Hashable &other ) = 0;
};

#endif
