#include "confpairbase.h"

ConfPairBase::ConfPairBase()
{
}

ConfPairBase::~ConfPairBase()
{
}

ConfPairBase &ConfPairBase::operator=( const std::string &s )
{
	setFromString( s );

	return *this;
}

