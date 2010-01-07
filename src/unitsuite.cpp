/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/unitsuite.h"

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

// Argument handling is coming soon, I promise.
int Bu::UnitSuite::run( int /*argc*/, char * /*argv */ [] )
{
	int iEPass = 0;
	int iEFail = 0;
	int iUPass = 0;
	int iUFail = 0;
	for( TestList::iterator i = lTests.begin(); i != lTests.end(); i++ )
	{
		printf("%s: ", i->sName.getStr() );
		fflush( stdout );
		try
		{
			(this->*(i->fTest))();
			switch( i->eExpect )
			{
				case expectPass: printf("expected pass.\n"); iEPass++; break;
				case expectFail: printf("unexpected pass.\n"); iUPass++; break;
			}
		}
		catch( Failed &e )
		{
			switch( i->eExpect )
			{
				case expectPass: printf("unexpected "); iUFail++; break;
				case expectFail: printf("expected "); iEFail++; break;
			}
			if( e.bFile )
			{
				printf("fail in unitTest(%s). (%s:%d)\n",
					e.str.getStr(),
					e.sFile.getStr(),
					e.nLine
					);
			}
			else
			{
				printf("fail in unitTest(%s).\n",
					e.str.getStr()
					);
			}

			if( (iOptions & optStopOnError) )
				return 0;
		}
		catch( std::exception &e )
		{
			switch( i->eExpect )
			{
				case expectPass: printf("unexpected "); iUFail++; break;
				case expectFail: printf("expected "); iEFail++; break;
			}
			printf("fail with unknown exception.  what: %s\n", e.what() );

			if( (iOptions & optStopOnError) )
				return 0;
		}
		catch( ... )
		{
			switch( i->eExpect )
			{
				case expectPass: printf("unexpected "); iUFail++; break;
				case expectFail: printf("expected "); iEFail++; break;
			}
			printf("fail with external exception.\n");

			if( (iOptions & optStopOnError) )
				return 0;
		}
	}

	printf("\nReport:\n"
		"\tTotal tests run:     %ld\n"
		"\tExpected passes:     %d\n"
		"\tExpected failures:   %d\n"
		"\tUnexpected passes:   %d\n"
		"\tUnexpected failures: %d\n\n",
		lTests.getSize(), iEPass, iEFail, iUPass, iUFail );
	if( iUPass == 0 && iUFail == 0 )
		printf("\tNothing unexpected.\n\n");

	return 0;
}

void Bu::UnitSuite::add( Test fTest, const Bu::FString &sName, Expect e )
{
	TestInfo ti;
	ti.sName = sName;
	ti.eExpect = e;
	long index = ti.sName.rfindIdx("::");
	if( index != -1 )
	{
		FString tmp = sSuiteName;
		tmp += ti.sName.getStr()+index;
		ti.sName = tmp;
	}
	ti.fTest = fTest;
	lTests.append( ti );
}

void Bu::UnitSuite::setName( const FString &sName )
{
	sSuiteName = sName;
}

