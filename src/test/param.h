#ifndef PARAM_H
#define PARAM_H

#include <stdint.h>

#include "paramproc.h"

class Param : public ParamProc
{
public:
	Param();
	virtual ~Param();

private:
	int printStuff( int argc, char *argv[] );

	std::string str;
	uint32_t uint32;
};

#endif
