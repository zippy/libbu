#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cpptest.h>
#include <string.h>

#include "xmlstringreader.h"
#include "xmlexception.h"

class XmlCoreTestSuite : public Test::Suite
{
public:
	XmlCoreTestSuite()
	{
		TEST_ADD( XmlCoreTestSuite::badXml01 )
		TEST_ADD( XmlCoreTestSuite::badXml02 )
		TEST_ADD( XmlCoreTestSuite::badXml03 )
	}
	
private:
	void badXml01()
	{
		TEST_THROWS( XmlStringReader r("<hello></bye>"), XmlException & );
	}
	
	void badXml02()
	{
		TEST_THROWS( XmlStringReader r("<hello>"), XmlException & );
	}

	void badXml03()
	{
		TEST_THROWS( XmlStringReader r("<hello param=\"stuff?"), XmlException & );
	}
};

int main( int argc, char *argv[] )
{
	Test::TextOutput output( Test::TextOutput::Verbose );
	XmlCoreTestSuite ts;
	return ts.run( output ) ? EXIT_SUCCESS : EXIT_FAILURE;
}

