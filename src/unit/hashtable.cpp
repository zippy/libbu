#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cpptest.h>
#include <string.h>
#include <set>
#include <map>

#include "hashfunctionstring.h"
#include "hashfunctioncasestring.h"
#include "hashfunctionint.h"

class HashFunctionSuite : public Test::Suite
{
public:
	HashFunctionSuite()
	{
		TEST_ADD( HashFunctionSuite::functionString )
		TEST_ADD( HashFunctionSuite::functionCaseString )
		TEST_ADD( HashFunctionSuite::functionInt )
	}

private:
	void functionStringWorker( HashFunction &hf, std::set<unsigned long> &sCodes, char *str, int nLevel, int nMax )
	{
		for( char let = 'A'; let <= 'z'; let += 3 )
		{
			str[nLevel+1] = '\0';
			str[nLevel] = let;
			unsigned long x = hf.hash( str );
			TEST_ASSERT( sCodes.find( x ) == sCodes.end() );
			TEST_ASSERT( hf.cmpIDs( str, str ) );
			sCodes.insert( x );

			if( nLevel < nMax )
				functionStringWorker( hf, sCodes, str, nLevel+1, nMax );
		}
	}
	
	void functionString()
	{
		HashFunctionString hf;
		char str[10];

		std::set<unsigned long> sCodes;

		functionStringWorker( hf, sCodes, str, 0, 3 );
	}
	
	void functionCaseStringWorker( HashFunction &hf, std::map<unsigned long, char *> &sCodes, char *str, int nLevel, int nMax )
	{
		for( char let = 'A'; let <= 'z'; let += 3 )
		{
			str[nLevel+1] = '\0';
			str[nLevel] = let;
			unsigned long x = hf.hash( str );
			std::map<unsigned long, char *>::iterator i = sCodes.find( x );
			if( i == sCodes.end() )
			{
				sCodes[x] = strdup( str );
			}
			else
			{
				TEST_ASSERT( strcasecmp( (*i).second, str ) == 0 );
				TEST_ASSERT( hf.cmpIDs( (*i).second, str ) == true );
			}

			if( nLevel < nMax )
				functionCaseStringWorker( hf, sCodes, str, nLevel+1, nMax );
		}
	}
	
	void functionCaseString()
	{
		HashFunctionCaseString hf;
		char str[10];

		std::map<unsigned long, char *> sCodes;

		functionCaseStringWorker( hf, sCodes, str, 0, 3 );

		std::map<unsigned long, char *>::iterator i;
		for( i = sCodes.begin(); i != sCodes.end(); i++ )
		{
			free( (*i).second );
		}
	}

	void functionInt()
	{
		HashFunctionInt hf;

		for( long i = -100000; i <= 100000; i += 100 )
		{
			TEST_ASSERT( ((long)hf.hash( (void *)i )) == i );
			TEST_ASSERT( ((long)hf.cmpIDs( (void *)i, (void *)i )) );
		}
	}
};

int main( int argc, char *argv[] )
{
	Test::TextOutput output( Test::TextOutput::Verbose );
	HashFunctionSuite ts;
	return ts.run( output ) ? EXIT_SUCCESS : EXIT_FAILURE;
}

