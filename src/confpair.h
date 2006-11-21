#ifndef CONF_PAIR_H
#define CONF_PAIR_H

#include <stdint.h>
#include <string>
/**
 *
 */
template<class T>
class ConfPair
{
public:
	ConfPair( const std::string &sName ) :
		sName( sName )
	{	}
	
	virtual ~ConfPair()
	{	}

	T &value()
	{
		return tValue;
	}

	const std::string &name()
	{
		return sName;
	}

private:
	std::string sName;
	T tValue;
};

#endif
