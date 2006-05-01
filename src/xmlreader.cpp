#include "xmlreader.h"
#include <string.h>

XmlReader::XmlReader( bool bStrip )
{
	nError = 0;
	this->bStrip = bStrip;
}

XmlReader::~XmlReader()
{
}

#define gcall( x ) if( x == false ) return false;

bool XmlReader::isws( char chr )
{
	return ( chr == ' ' || chr == '\t' || chr == '\n' || chr == '\r' );
}

bool XmlReader::ws()
{
	while( true )
	{
		char chr = getChar();
		if( isws( chr ) )
		{
			usedChar();
		}
		else
		{
			return true;
		}
	}
	return true;
}

bool XmlReader::buildDoc()
{
	// take care of initial whitespace
	gcall( ws() );
	gcall( node() );

	return true;
}

bool XmlReader::node()
{
	gcall( startNode() )

	// At this point, we are closing the startNode
	char chr = getChar();
	if( chr == '>' )
	{
		usedChar();

		// Now we process the guts of the node.
		gcall( content() );
	}
	else if( chr == '/' )
	{
		// This is the tricky one, one more validation, then we close the node.
		usedChar();
		if( getChar() == '>' )
		{
			closeNode();
			usedChar();
		}
		else
		{
			reportError("Close node in singleNode malformed!");
			return false;
		}
	}
	else
	{
		reportError("Close node expected, but not found.");
		return false;
	}

	return true;
}

bool XmlReader::startNode()
{
	if( getChar() == '<' )
	{
		usedChar();

		if( getChar() == '/' )
		{
			// Heh, it's actually a close node, go figure
			FlexBuf fbName;
			usedChar();
			gcall( ws() );

			while( true )
			{
				char chr = getChar();
				if( isws( chr ) || chr == '>' )
				{
					// Here we actually compare the name we got to the name
					// we already set, they have to match exactly.
					if( !strcasecmp( getCurrent()->getName(), fbName.getData() ) )
					{
						closeNode();
						break;
					}
					else
					{
						reportError("Got a mismatched node close tag.");
						return false;
					}
				}
				else
				{
					fbName.appendData( chr );
					usedChar();
				}
			}

			gcall( ws() );
			if( getChar() == '>' )
			{
				// Everything is cool.
				usedChar();
			}
			else
			{
				reportError("Got extra junk data instead of node close tag.");
				return false;
			}
		}
		else
		{
			// We're good, format is consistant
			addNode();

			// Skip extra whitespace
			gcall( ws() );
			gcall( name() );
			gcall( ws() );
			gcall( paramlist() );
			gcall( ws() );
		}
	}
	else
	{
		reportError("Expected to find node opening char, '<'.\n");
		return false;
	}

	return true;
}

bool XmlReader::name()
{
	FlexBuf fbName;
	
	while( true )
	{
		char chr = getChar();
		if( isws( chr ) || chr == '>' || chr == '/' )
		{
			setName( fbName.getData() );
			return true;
		}
		else
		{
			fbName.appendData( chr );
			usedChar();
		}
	}

	return true;
}

bool XmlReader::paramlist()
{
	while( true )
	{
		char chr = getChar();
		if( chr == '/' || chr == '>' )
		{
			return true;
		}
		else
		{
			gcall( param() );
			gcall( ws() );
		}
	}
	
	return true;
}

char XmlReader::getEscape()
{
	// Right now, we just do # escapes...
	if( getChar( 1 ) == '#' )
	{
		usedChar();
		usedChar();
		char buf[4];
		int j = 0;
		for( j = 0; getChar() != ';'; j++ )
		{
			buf[j] = getChar();
			usedChar();
		}
		usedChar();
		buf[j] = '\0';
		return (char)atoi( buf );
	}
	else
	{
		return '\0';
	}
}

bool XmlReader::param()
{
	FlexBuf fbName;
	FlexBuf fbValue;

	while( true )
	{
		char chr = getChar();
		if( isws( chr ) || chr == '=' )
		{
			break;
		}
		else
		{
			fbName.appendData( chr );
			usedChar();
		}
	}

	gcall( ws() );

	if( getChar() == '=' )
	{
		usedChar();

		gcall( ws() );

		char chr = getChar();
		if( chr == '"' )
		{
			// Better quoted rhs
			usedChar();

			while( true )
			{
				chr = getChar();
				if( chr == '"' )
				{
					usedChar();
					addProperty( fbName.getData(), fbValue.getData() );
					return true;
				}
				else
				{
					if( chr == '&' )
					{
						chr = getEscape();
						if( chr == '\0' ) return false;
						fbValue.appendData( chr );
					}
					else
					{
						fbValue.appendData( chr );
						usedChar();
					}
				}
			}
		}
		else
		{
			// Simple one-word rhs
			while( true )
			{
				chr = getChar();
				if( isws( chr ) || chr == '/' || chr == '>' )
				{
					addProperty( fbName.getData(), fbValue.getData() );
					return true;
				}
				else
				{
					if( chr == '&' )
					{
						chr = getEscape();
						if( chr == '\0' ) return false;
						fbValue.appendData( chr );
					}
					else
					{
						fbValue.appendData( chr );
						usedChar();
					}
				}
			}
		}
	}
	else
	{
		reportError("Expected an equals to seperate the params.");
		return false;
	}

	return true;
}

bool XmlReader::content()
{
	FlexBuf fbContent;

	if( bStrip ) gcall( ws() );

	while( true )
	{
		char chr = getChar();
		if( chr == '<' )
		{
			if( getChar(1) == '/' )
			{
				if( fbContent.getLength() > 0 )
				{
					if( bStrip )
					{
						int j;
						for( j = fbContent.getLength()-1; isws(fbContent.getData()[j]); j-- );
						((char *)fbContent.getData())[j+1] = '\0';
					}
					setContent( fbContent.getData() );
				}
				usedChar();
				usedChar();
				gcall( ws() );
				FlexBuf fbName;
				while( true )
				{
					chr = getChar();
					if( isws( chr ) || chr == '>' )
					{
						if( !strcasecmp( getCurrent()->getName(), fbName.getData() ) )
						{
							closeNode();
							break;
						}
						else
						{
							reportError("Mismatched close tag found.");
							return false;
						}
					}
					else
					{
						fbName.appendData( chr );
						usedChar();
					}
				}
				gcall( ws() );
				if( getChar() == '>' )
				{
					usedChar();
					return true;
				}
				else
				{
					reportError("Malformed close tag.");
					return false;
				}
			}
			else
			{
				if( fbContent.getLength() > 0 )
				{
					if( bStrip )
					{
						int j;
						for( j = fbContent.getLength()-1; isws(fbContent.getData()[j]); j-- );
						((char *)fbContent.getData())[j+1] = '\0';
					}
					setContent( fbContent.getData() );
					fbContent.clearData();
				}
				gcall( node() );
			}

			if( bStrip ) gcall( ws() );
		}
		else
		{
			fbContent.appendData( chr );
			usedChar();
		}
	}
}

void XmlReader::reportError( const char *sError )
{
	printf("XmlReader error: %s\n", sError );
}

int XmlReader::getError()
{
	return nError;
}

