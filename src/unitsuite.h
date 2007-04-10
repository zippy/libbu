#ifndef UNIT_SUITE_H
#define UNIT_SUITE_H

#include <stdint.h>
#include <list>
#include "fstring.h"

namespace Bu
{
	/**
	 *
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

#endif
