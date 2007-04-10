#ifndef UNIT_SUITE_H
#define UNIT_SUITE_H

#include <stdint.h>
#include <list>
#include "fstring.h"

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
	 * 	Unit()
	 * 	{
	 * 		setName("Example");
	 * 		addTest( Unit::test );
	 * 	}
	 * 
	 * 	virtual ~Unit() { }
	 * 
	 * 	//
	 * 	// Tests go here
	 * 	//
	 * 	void test()
	 * 	{
	 * 		unitTest( 1 == 1 );
	 * 	}
	 * };
	 * 
	 * int main( int argc, char *argv[] )
	 * {
	 * 	return Unit().run( argc, argv );
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
		virtual ~UnitSuite();

		int run( int argc=0, char *argv[]=NULL );

		typedef void (UnitSuite::*Test)();

		class Failed
		{
		public:
			Failed() : str(""), bFile( false ) { }
			Failed( const FString &s ) : str( s ), bFile( false ) { }
			Failed( const FString &s, const FString &sFile, int nLine ) :
				str( s ), sFile( sFile ), nLine( nLine ), bFile( true ) { }

			FString str;
			FString sFile;
			int nLine;
			bool bFile;
		};
		
	protected:
		void add( Test fTest, Bu::FString sName );
		void setName( const FString &sName );

	private:
		typedef struct TestInfo
		{
			FString sName;
			Test fTest;
		} TestInfo;

		typedef std::list<TestInfo> TestList;
		TestList lTests;
		FString sSuiteName;
	};
}

#define addTest( fn ) add( static_cast<Bu::UnitSuite::Test>(&fn), #fn )
#define unitTest( tst ) if( !(tst) )							\
{																\
	throw Bu::UnitSuite::Failed( #tst, __FILE__, __LINE__ );	\
}
#define unitFailed( msg ) throw Bu::UnitSuite::Failed(msg, __FILE__, __LINE__);

#endif
