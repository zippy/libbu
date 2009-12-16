#include "bu/optparser.h"

Bu::OptParser::OptParser()
{
}

Bu::OptParser::~OptParser()
{
}

void Bu::OptParser::addOption( const Option &opt )
{
	lOption.append( opt );
	if( opt.cOpt != '\0' )
		hsOption.insert( opt.cOpt, &lOption.last() );
	if( opt.sOpt )
		hlOption.insert( opt.sOpt, &lOption.last() );

}

Bu::FString Bu::OptParser::format( const Bu::FString &sIn, int iWidth,
		int iIndent )
{

}


//
// Code for Bu::OptParser::Option
//

Bu::OptParser::Option::Option()
{
}

Bu::OptParser::Option::~Option()
{
}

