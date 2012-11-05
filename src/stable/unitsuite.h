/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_UNIT_SUITE_H
#define BU_UNIT_SUITE_H

#include <stdint.h>
#include "bu/list.h"
#include "bu/string.h"
#include "bu/file.h"
#include "bu/array.h"
#include "bu/hash.h"

namespace Bu
{
    /**
     * Provides a unit testing framework.  This is pretty easy to use, probably
     * the best way to get started is to use ch to generate a template, or use
     * the code below with appropriate tweaks:
     *@code
     * #include "unitsuite.h"
     * 
     * class Unit : public Bu::UnitSuite
     * {
     * public:
     *  Unit()
     *  {
     *      setName("Example");
     *      addTest( Unit::test );
     *  }
     * 
     *  virtual ~Unit() { }
     * 
     *  //
     *  // Tests go here
     *  //
     *  void test()
     *  {
     *      unitTest( 1 == 1 );
     *  }
     * };
     * 
     * int main( int argc, char *argv[] )
     * {
     *  return Unit().run( argc, argv );
     * }
     *
     @endcode
     * The main function can contain other things, but using this one exactly
     * makes all of the test suites work exactly the same.  Using the optional
     * setName at the top of the constructor replaces the class name with the
     * chosen name when printing out stats and info.
     */
    class UnitSuite
    {
    public:
        UnitSuite();
        UnitSuite( int iOptions );
        virtual ~UnitSuite();

        int run( int argc=0, char *argv[]=NULL );

        Bu::File tempFile( Bu::String &sFileName );

        typedef void (UnitSuite::*Test)();

        class Failed
        {
        public:
            Failed() : str(""), bFile( false ) { }
            Failed( const String &s ) : str( s ), bFile( false ) { }
            Failed( const String &s, const String &sFile, int nLine ) :
                str( s ), sFile( sFile ), nLine( nLine ), bFile( true ) { }

            String str;
            String sFile;
            int nLine;
            bool bFile;
        };

        enum
        {
            optStopOnError      = 0x000001
        };

        enum Expect
        {
            expectPass,
            expectFail
        };
        
    protected:
        void add( Test fTest, const Bu::String &sName, Expect e=expectPass );
        void setName( const String &sName );

        void dispProgress();
        void setStepCount( int iSteps );
        void incProgress( int iAmnt = 1 );
        void setProgress( int iAmnt );

    private:
        int onListCases( Bu::Array<Bu::String> aParam );
        int onAddTest( Bu::Array<Bu::String> aParam );

    private:
        typedef struct TestInfo
        {
            String sName;
            Test fTest;
            Expect eExpect;
        } TestInfo;

        typedef Bu::List<TestInfo> TestList;
        TestList lTests;
        String sSuiteName;

        int iOptions;

        typedef Bu::List<Bu::String> StrList;
        StrList lFileCleanup;
        int iNameWidth;
        int iStepCount;
        int iProgress;
        time_t tLastUpdate;

        Bu::Hash<Bu::String, bool> hSelTests;
    };

Bu::Formatter &operator<<( Bu::Formatter &f, const Bu::UnitSuite::Expect &e );
}

#define addTest( fn ) add( static_cast<Bu::UnitSuite::Test>(&fn), #fn )
#define unitTest( tst ) if( !(tst) )                            \
{                                                               \
    throw Bu::UnitSuite::Failed( #tst, __FILE__, __LINE__ );    \
} else (void)0
#define unitFailed( msg ) throw Bu::UnitSuite::Failed(msg, __FILE__, __LINE__)

#endif
