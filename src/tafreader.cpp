#include "bu/tafreader.h"
#include "bu/exceptions.h"
#include "bu/fstring.h"

Bu::TafReader::TafReader( Bu::Stream &sIn ) :
	sIn( sIn )
{
	next();
	node();
}

Bu::TafReader::~TafReader()
{
}

void Bu::TafReader::node()
{
	ws();
	if( c != '{' )
		throw Bu::TafException("Expected '{'");
	next();
	ws();
	Bu::FString sName;
	for(;;)
	{
		if( c == ':' )
			break;
		else
			sName += c;
		next();
	}
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
	Bu::FString sName;
	for(;;)
	{
		if( isws() || c == '=' )
			break;
		else
			sName += c;
		next();
	}
	ws();
	if( c != '=' )
	{
		printf("  %s (true)\n", sName.getStr() );
		return;
	}
	next();
	ws();
	Bu::FString sValue;
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
			sValue += c;
			next();
		}
		next();
	}
	else
	{
		for(;;)
		{
			if( isws() || c == '}' || c == '{' )
				break;
			sValue += c;
			next();
		}
	}
	printf("  %s = %s\n", sName.getStr(), sValue.getStr() );
}

FString Bu::TafReader::readStr()
{
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

