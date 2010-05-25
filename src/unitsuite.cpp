/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/unitsuite.h"
#include "bu/file.h"
#include "bu/sio.h"

using namespace Bu;

#include <unistd.h>

Bu::UnitSuite::UnitSuite() :
	iOptions( 0 ),
	iNameWidth( 0 )
{
}

Bu::UnitSuite::UnitSuite( int iOptions ) :
	iOptions( iOptions ),
	iNameWidth( 0 )
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
		sio << Fmt( iNameWidth+3, Fmt::Left ).fill('.') << i->sName
			<< sio.flush;
		try
		{
			(this->*(i->fTest))();
			switch( i->eExpect )
			{
				case expectPass:
					sio << "pass." << sio.nl;
					iEPass++;
					break;

				case expectFail:
					sio << "unexpected pass." << sio.nl;
					iUPass++;
					break;
			}
		}
		catch( Failed &e )
		{
			switch( i->eExpect )
			{
				case expectPass:
					sio << "unexpected ";
					iUFail++;
					break;

				case expectFail:
					sio << "expected ";
					iEFail++;
					break;
			}
			if( e.bFile )
			{
				sio << "fail in unitTest(" << e.str << "). (" << e.sFile
					<< ":" << e.nLine << ")." << sio.nl;
			}
			else
			{
				sio << "fail in unitTest(" << e.str << ")." << sio.nl;
			}

			if( (iOptions & optStopOnError) )
				return 0;
		}
		catch( std::exception &e )
		{
			switch( i->eExpect )
			{
				case expectPass:
					sio << "unexpected ";
					iUFail++;
					break;

				case expectFail:
					sio << "expected ";
					iEFail++;
					break;
			}
			sio << "fail with unknown exception.  what: " << e.what() << sio.nl;

			if( (iOptions & optStopOnError) )
				return 0;
		}
		catch( ... )
		{
			switch( i->eExpect )
			{
				case expectPass:
					sio << "unexpected ";
					iUFail++;
					break;

				case expectFail:
					sio << "expected ";
					iEFail++;
					break;
			}
			sio << "fail with external exception." << sio.nl;

			if( (iOptions & optStopOnError) )
				return 0;
		}
	}

	sio << sio.nl
		<< "Report:" << sio.nl
		<< "\tTotal tests run:     " << lTests.getSize() << sio.nl
		<< "\tExpected passes:     " << iEPass << sio.nl
		<< "\tExpected failures:   " << iEFail << sio.nl
		<< "\tUnexpected passes:   " << iUPass << sio.nl
		<< "\tUnexpected failures: " << iUFail << sio.nl << sio.nl;
	if( iUPass == 0 && iUFail == 0 )
		sio << "\tNothing unexpected." << sio.nl << sio.nl;

	for( StrList::iterator i = lFileCleanup.begin(); i; i++ )
	{
		unlink( (*i).getStr() );
	}

	return 0;
}

Bu::File Bu::UnitSuite::tempFile( Bu::FString &sFileName )
{
	Bu::File f = Bu::File::tempFile( sFileName );
	lFileCleanup.append( sFileName );
	return f;
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
	if( iNameWidth < ti.sName.getSize() )
		iNameWidth = ti.sName.getSize();
}

void Bu::UnitSuite::setName( const FString &sName )
{
	sSuiteName = sName;
}

