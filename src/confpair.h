#ifndef CONF_PAIR_H
#define CONF_PAIR_H

#include <stdint.h>
#include <string>
#include <sstream>
#include "confpairbase.h"

/**
 *
 */
template<class T>
class ConfPair : public ConfPairBase
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

	virtual void setFromString( const std::string &sStr )
	{
		std::stringstream(sStr) >> tValue;
	}

	virtual std::string getAsString()
	{
		std::stringstream tmp;
		tmp << tValue;
		return tmp.str();
	}

private:
	std::string sName;
	T tValue;
};

template<>
void ConfPair<std::string>::setFromString( const std::string &sStr )
{
	tValue = sStr;
}

template<>
std::string ConfPair<std::string>::getAsString()
{
	return tValue;
}

template<>
void ConfPair<bool>::setFromString( const std::string &sStr )
{
	if( !strcasecmp( sStr.c_str(), "true" ) ||
		!strcasecmp( sStr.c_str(), "yes" ) ||
		!strcasecmp( sStr.c_str(), "on" ) )
		tValue = true;
	else
		tValue = false;
}

template<>
std::string ConfPair<bool>::getAsString()
{
	if( tValue == true )
		return "True";
	return "False";
}

#endif
