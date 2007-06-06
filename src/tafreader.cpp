#include "bu/tafreader.h"
#include "bu/exceptions.h"
#include "bu/fstring.h"

using namespace Bu;

Bu::TafReader::TafReader( Bu::Stream &sIn ) :
	sIn( sIn )
{
	next();
	node();
}

Bu::TafReader::~TafReader()
{
}

Bu::TafNode *Bu::TafReader::readNode()
{
}

void Bu::TafReader::node()
{
	ws();
	if( c != '{' )
		throw TafException("Expected '{'");
	next();
	ws();
	FString sName = readStr();
	next();
	printf("Node[%s]:\n", sName.getStr() );

	nodeContent();

	if( c != '}' )
		throw TafException("Expected '}'");

	next();
}

void Bu::TafReader::nodeContent()
{
	for(;;)
	{
		ws();
		if( c == '{' )
			node();
		else if( c == '}' )
			return;
		else
			nodeProperty();
	}
}

void Bu::TafReader::nodeProperty()
{
	FString sName = readStr();
	ws();
	if( c != '=' )
	{
		printf("  %s (true)\n", sName.getStr() );
		return;
	}
	next();
	FString sValue = readStr();
	printf("  %s = %s\n", sName.getStr(), sValue.getStr() );
}

Bu::FString Bu::TafReader::readStr()
{
	ws();
	FString s;
	if( c == '"' )
	{
		next();
		for(;;)
		{
			if( c == '\\' )
			{
				next();
				if( c == 'x' )
				{
					char code[3]={'\0','\0','\0'};
					next();
					code[0] = c;
					next();
					code[1] = c;
					c = (unsigned char)strtol( code, NULL, 16 );
				}
				else if( c == '"' )
					c = '"';
				else
					throw TafException("Invalid escape sequence.");
			}
			else if( c == '"' )
				break;
			s += c;
			next();
		}
		next();
	}
	else
	{
		for(;;)
		{
			if( isws() || c == '}' || c == '{' || c == ':' || c == '=' )
				break;
			s += c;
			next();
		}
	}

	return s;
}

void Bu::TafReader::ws()
{
	for(;;)
	{
		if( !isws() )
			return;

		next();
	}
}

bool Bu::TafReader::isws()
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

void Bu::TafReader::next()
{
	sIn.read( &c, 1 );
}

