#include "xmlreader.h"

Bu::XmlReader::XmlReader( Bu::Stream &sIn ) :
	sIn( sIn )
{
}

Bu::XmlReader::~XmlReader()
{
}

const char *Bu::XmlReader::lookahead( int nAmnt )
{
	if( sBuf.getSize() >= nAmnt )
		return sBuf.getStr();

	int nNew = nAmnt - sBuf.getSize();
	char *buf = new char[nNew];
	sIn.read( buf, nNew );
	sBuf.append( buf );

	return sBuf.getStr();
}

void Bu::XmlReader::burn( int nAmnt )
{
	if( sBuf.getSize() < nAmnt )
	{
		lookahead( nAmnt );
	}

	sBuf.remove( nAmnt );
}

void Bu::XmlNode::checkString( const char *str, int nLen )
{
	if( !strncmp( str, lookahead( nLen ), nLen ) )
	{
		burn( nLen );
		return;
	}

	throw Bu::ExceptionBase("Expected string '%s'", str );
}

Bu::XmlNode *Bu::XmlReader::read()
{
	prolog();
}

void Bu::XmlReader::prolog()
{
	XMLDecl();
	Misc();
}

void Bu::XmlReader::XMLDecl()
{
	checkString("<?xml", 5 );
	VersionInfo();
	EncodingDecl();
	SDDecl();
	S();
}

void Bu::XmlReader::Misc()
{
}

void Bu::XmlReader::S()
{
	for( int j = 0;; j++ )
	{
		char c = *lookahead( 1 );
		if( c == 0x20 || c == 0x9 || c == 0xD || c == 0xA )
			continue;
		if( j == 0 )
			printf("Error, expected whitespace!\n");
		return;
	}
}

void Bu::XmlReader::S()
{
	for(;;)
	{
		char c = *lookahead( 1 );
		if( c == 0x20 || c == 0x9 || c == 0xD || c == 0xA )
			continue;
		return;
	}
}

void Bu::XmlReader::VersionInfo()
{
	S();
	checkString("version", 7 );
	
}

void Bu::XmlReader::Eq()
{
	Sq();
	checkString("=", 1 );
	Sq();
}


