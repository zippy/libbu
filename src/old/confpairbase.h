#ifndef CONF_PAIR_BASE_H
#define CONF_PAIR_BASE_H

#include <stdint.h>
#include <string>
#include <ostream>
#include <istream>

class ConfPairBase
{
public:
	ConfPairBase();
	virtual ~ConfPairBase();

	virtual void setFromString( const std::string &sStr )=0;
	virtual std::string getAsString()=0;

	ConfPairBase &operator=( const std::string &s );

private:

};

#endif
