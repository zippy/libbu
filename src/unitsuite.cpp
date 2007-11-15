/*
 * Copyright (C) 2007 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "unitsuite.h"

Bu::UnitSuite::UnitSuite() :
	iOptions( 0 )
{
}

Bu::UnitSuite::UnitSuite( int iOptions ) :
	iOptions( iOptions )
{
}

Bu::UnitSuite::~UnitSuite()
{
}

int Bu::UnitSuite::run( int argc, char *argv[] )
{
	for( TestList::iterator i = lTests.begin(); i != lTests.end(); i++ )
	{
		printf("%s: ", i->sName.getStr() );
		fflush( stdout );
		try
		{
			(this->*(i->fTest))();
			printf("passed.\n");
		}
		catch( Failed &e )
		{
			if( e.bFile )
			{
				printf("unitTest(%s) failed. (%s:%d)\n",
					e.str.getStr(),
					e.sFile.getStr(),
					e.nLine
					);
			}
			else
			{
				printf("unitTest(%s) failed.\n",
					e.str.getStr()
					);
			}

			if( (iOptions & optStopOnError) )
				return 0;
		}
		catch( std::exception &e )
		{
			printf("failed with unknown exception.  what: %s\n", e.what() );

			if( (iOptions & optStopOnError) )
				return 0;
		}
		catch( ... )
		{
			printf("failed with external exception.\n");

			if( (iOptions & optStopOnError) )
				return 0;
		}
	}

	return 0;
}

void Bu::UnitSuite::add( Test fTest, Bu::FString sName )
{
	TestInfo ti;
	ti.sName = sName;
	long index = ti.sName.rfind("::");
	if( index != -1 )
	{
		FString tmp = sSuiteName;
		tmp += ti.sName.getStr()+index;
		ti.sName = tmp;
	}
	ti.fTest = fTest;
	lTests.push_back( ti );
}

void Bu::UnitSuite::setName( const FString &sName )
{
	sSuiteName = sName;
}

