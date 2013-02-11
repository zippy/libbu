/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/unitsuite.h"
#include "bu/file.h"
#include "bu/sio.h"
#include "bu/optparser.h"
#include <stdlib.h>
#include <time.h>

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

int Bu::UnitSuite::run( int argc, char *argv[] )
{
    bool bCleanup = true;
    OptParser p;
    p.addOption( Bu::slot( this, &Bu::UnitSuite::onListCases ), 'l', "list",
            "List available test cases." );
    p.addOption( bCleanup, "no-cleanup", "Don't erase temp files.");
    p.setOverride( "no-cleanup", false );
    p.setNonOption( Bu::slot( this, &Bu::UnitSuite::onAddTest ) );
    p.addHelpOption();
    p.parse( argc, argv );

    if( !hSelTests.isEmpty() )
    {
        TestList lSub;
        for( TestList::iterator i = lTests.begin(); i != lTests.end(); i++ )
        {
            if( hSelTests.has( (*i).sName ) )
                lSub.append( *i );
        }

        lTests = lSub;
    }

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
            iStepCount = -1;
            iProgress = 0;
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

    if( bCleanup )
    {
        for( StrList::iterator i = lFileCleanup.begin(); i; i++ )
        {
            unlink( (*i).getStr() );
        }
    }

    return 0;
}

Bu::File Bu::UnitSuite::tempFile( Bu::String &sFileName )
{
    Bu::File f = Bu::File::tempFile( sFileName );
    lFileCleanup.append( sFileName );
    return f;
}

void Bu::UnitSuite::add( Test fTest, const Bu::String &sName, Expect e )
{
    TestInfo ti;
    ti.sName = sName;
    ti.eExpect = e;
    long index = ti.sName.rfindIdx("::");
    if( index != -1 )
    {
        String tmp = sSuiteName;
        tmp += ti.sName.getStr()+index;
        ti.sName = tmp;
    }
    ti.fTest = fTest;
    lTests.append( ti );
    if( iNameWidth < ti.sName.getSize() )
        iNameWidth = ti.sName.getSize();
}

void Bu::UnitSuite::setName( const String &sName )
{
    sSuiteName = sName;
}

void Bu::UnitSuite::dispProgress()
{
    if( tLastUpdate == time( NULL ) )
        return;
    sio << Fmt(3) << (iProgress*100/iStepCount) << "%" << "\b\b\b\b"
        << sio.flush;
    tLastUpdate = time( NULL );
}

void Bu::UnitSuite::setStepCount( int iSteps )
{
    iStepCount = iSteps;
    if( iStepCount < 0 )
        return;
    tLastUpdate = 0;
    dispProgress();
}

void Bu::UnitSuite::incProgress( int iAmnt )
{
    iProgress += iAmnt;
    if( iProgress < 0 )
        iProgress = 0;
    if( iProgress > iStepCount )
        iProgress = iStepCount;
    dispProgress();
}

void Bu::UnitSuite::setProgress( int iAmnt )
{
    iProgress = iAmnt;
    if( iProgress < 0 )
        iProgress = 0;
    if( iProgress > iStepCount )
        iProgress = iStepCount;
    dispProgress();
}

int Bu::UnitSuite::onListCases( StrArray )
{
    sio << "Test cases:" << sio.nl;
    for( TestList::iterator i = lTests.begin(); i; i++ )
    {
        sio << "\t- " << Fmt( iNameWidth, 10, Fmt::Left ) << (*i).sName << "  "
            << (*i).eExpect << sio.nl;
    }
    sio << sio.nl;
    exit( 0 );
    return 0;
}

int Bu::UnitSuite::onAddTest( StrArray aParam )
{
    hSelTests.insert( aParam[0], true );
    return 0;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &f, const Bu::UnitSuite::Expect &e )
{
    switch( e )
    {
        case Bu::UnitSuite::expectPass:
            return f << "expect pass";

        case Bu::UnitSuite::expectFail:
            return f << "expect fail";
    }

    return f << "**error**";
}

