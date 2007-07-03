#include "unitsuite.h"

Bu::UnitSuite::UnitSuite()
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
		}
		catch( ... )
		{
			printf("failed with external exception.\n");
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

