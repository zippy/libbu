#include "bu/xmlreader.h"
#include "bu/exceptions.h"
#include <string.h>

XmlReader::XmlReader( Bu::Stream &sIn, bool bStrip ) :
	sIn( sIn ),
	bStrip( bStrip )
{
	buildDoc();
}

XmlReader::~XmlReader()
{
}

char XmlReader::getChar( int nIndex )
{
	if( sBuf.getSize() <= nIndex )
	{
		int nInc = nIndex-sBuf.getSize()+1;
		char *buf = new char[nInc];
		sIn.read( buf, nInc );
		sBuf.append( buf, nInc );
		delete[] buf;
	}

	return sBuf[nIndex];
}

void XmlReader::usedChar( int nAmnt )
{
	if( nAmnt >= sBuf.getSize() )
	{
		sBuf.clear();
	}
	else
	{
		char *s = sBuf.getStr();
		memcpy( s, s+nAmnt, sBuf.getSize()-nAmnt );
		sBuf.resize( sBuf.getSize()-nAmnt );
	}
}

void XmlReader::addEntity( const Bu::FString &name, const Bu::FString &value )
{
	htEntity[name] = value;
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
	textDecl();
	entity();
	addEntity("gt", ">");
	addEntity("lt", "<");
	addEntity("amp", "&");
	addEntity("apos", "\'");
	addEntity("quot", "\"");
	gcall( node() );

	return true;
}

void XmlReader::textDecl()
{
	if( getChar() == '<' && getChar( 1 ) == '?' )
	{
		usedChar( 2 );
		for(;;)
		{
			if( getChar() == '?' )
			{
				if( getChar( 1 ) == '>' )
				{
					usedChar( 2 );
					return;
				}
			}
			usedChar();
		}
	}
}

void XmlReader::entity()
{
	for(;;)
	{
		ws();

		if( getChar() == '<' && getChar( 1 ) == '!' )
		{
			usedChar( 2 );
			ws();
			Bu::FString buf;
			for(;;)
			{
				char chr = getChar();
				usedChar();
				if( isws( chr ) ) break;
				buf += chr;
			}

			if( strcmp( buf.c_str(), "ENTITY") == 0 )
			{
				ws();
				Bu::FString name;
				for(;;)
				{
					char chr = getChar();
					usedChar();
					if( isws( chr ) ) break;
					name += chr;
				}
				ws();
				char quot = getChar();
				usedChar();
				if( quot != '\'' && quot != '\"' )
				{
					throw Bu::XmlException(
						"Only quoted entity values are supported."
						);
				}
				Bu::FString value;
				for(;;)
				{
					char chr = getChar();
					usedChar();
					if( chr == '&' )
					{
						Bu::FString tmp = getEscape();
						value += tmp;
					}
					else if( chr == quot )
					{
						break;
					}
					else
					{
						value += chr;
					}
				}
				ws();
				if( getChar() == '>' )
				{
					usedChar();

					addEntity( name.c_str(), value.c_str() );
				}
				else
				{
					throw Bu::XmlException(
						"Malformed ENTITY:  unexpected '%c' found.",
						getChar()
						);
				}
			}
			else
			{
				throw Bu::XmlException(
					"Unsupported header symbol: %s",
					buf.c_str()
					);
			}
		}
		else
		{
			return;
		}
	}
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
			throw Bu::XmlException("Close node in singleNode malformed!");
		}
	}
	else
	{
		throw Bu::XmlException("Close node expected, but not found.");
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
			Bu::FString sName;
			usedChar();
			gcall( ws() );

			while( true )
			{
				char chr = getChar();
				if( isws( chr ) || chr == '>' )
				{
					// Here we actually compare the name we got to the name
					// we already set, they have to match exactly.
					if( getCurrent()->getName() == sName )
					{
						closeNode();
						break;
					}
					else
					{
						throw Bu::XmlException("Got a mismatched node close tag.");
					}
				}
				else
				{
					sName += chr;
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
				throw Bu::XmlException("Got extra junk data instead of node close tag.");
			}
		}
		else
		{
			// We're good, format is consistant
			//addNode();

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
		throw Bu::XmlException("Expected to find node opening char, '<'.");
	}

	return true;
}

bool XmlReader::name()
{
	Bu::FString sName;
	
	while( true )
	{
		char chr = getChar();
		if( isws( chr ) || chr == '>' || chr == '/' )
		{
			addNode( sName );
			return true;
		}
		else
		{
			sName += chr;
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

Bu::FString XmlReader::getEscape()
{
	if( getChar( 1 ) == '#' )
	{
		// If the entity starts with a # it's a character escape code
		int base = 10;
		usedChar( 2 );
		if( getChar() == 'x' )
		{
			base = 16;
			usedChar();
		}
		char buf[4];
		int j = 0;
		for( j = 0; getChar() != ';'; j++ )
		{
			buf[j] = getChar();
			usedChar();
		}
		usedChar();
		buf[j] = '\0';
		buf[0] = (char)strtol( buf, (char **)NULL, base );
		buf[1] = '\0';

		return buf;
	}
	else
	{
		// ...otherwise replace with the appropriate string...
		Bu::FString buf;
		usedChar();
		for(;;)
		{
			char cbuf = getChar();
			usedChar();
			if( cbuf == ';' ) break;
			buf += cbuf;
		}

		return htEntity[buf];
	}
}

bool XmlReader::param()
{
	Bu::FString sName;
	Bu::FString sValue;

	while( true )
	{
		char chr = getChar();
		if( isws( chr ) || chr == '=' )
		{
			break;
		}
		else
		{
			sName.append( chr );
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
					addProperty( sName.getStr(), sValue.getStr() );
					return true;
				}
				else
				{
					if( chr == '&' )
					{
						sValue += getEscape();
					}
					else
					{
						sValue += chr;
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
					addProperty( sName.getStr(), sValue.getStr() );
					return true;
				}
				else
				{
					if( chr == '&' )
					{
						sValue += getEscape();
					}
					else
					{
						sValue += chr;
						usedChar();
					}
				}
			}
		}
	}
	else
	{
		throw Bu::XmlException("Expected an equals to seperate the params.");
		return false;
	}

	return true;
}

bool XmlReader::content()
{
	Bu::FString sContent;

	if( bStrip ) gcall( ws() );

	while( true )
	{
		char chr = getChar();
		if( chr == '<' )
		{
			if( getChar(1) == '/' )
			{
				if( sContent.getSize() > 0 )
				{
					if( bStrip )
					{
						int j;
						for( j = sContent.getSize()-1; isws(sContent[j]); j-- );
						sContent[j+1] = '\0';
					}
					setContent( sContent.getStr() );
				}
				usedChar( 2 );
				gcall( ws() );
				Bu::FString sName;
				while( true )
				{
					chr = getChar();
					if( isws( chr ) || chr == '>' )
					{
						if( !strcasecmp( getCurrent()->getName().getStr(), sName.getStr() ) )
						{
							closeNode();
							break;
						}
						else
						{
							throw Bu::XmlException("Mismatched close tag found: <%s> to <%s>.", getCurrent()->getName().getStr(), sName.getStr() );
						}
					}
					else
					{
						sName += chr;
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
					throw Bu::XmlException("Malformed close tag.");
				}
			}
			else if( getChar(1) == '!' )
			{
				// We know it's a comment, let's see if it's proper
				if( getChar(2) != '-' ||
					getChar(3) != '-' )
				{
					// Not a valid XML comment
					throw Bu::XmlException("Malformed comment start tag found.");
				}

				usedChar( 4 );
				
				// Now burn text until we find the close tag
				for(;;)
				{
					if( getChar() == '-' )
					{
						if( getChar( 1 ) == '-' )
						{
							// The next one has to be a '>' now
							if( getChar( 2 ) != '>' )
							{
								throw Bu::XmlException("Malformed comment close tag found.  You cannot have a '--' that isn't followed by a '>' in a comment.");
							}
							usedChar( 3 );
							break;
						}
						else
						{
							// Found a dash followed by a non dash, that's ok...
							usedChar( 2 );
						}
					}
					else
					{
						// Burn comment chars
						usedChar();
					}
				}
			}
			else
			{
				if( sContent.getSize() > 0 )
				{
					if( bStrip )
					{
						int j;
						for( j = sContent.getSize()-1; isws(sContent[j]); j-- );
						sContent[j+1] = '\0';
					}
					setContent( sContent.getStr() );
					sContent.clear();
				}
				gcall( node() );
			}

			if( bStrip ) gcall( ws() );
		}
		else if( chr == '&' )
		{
			sContent += getEscape();
		}
		else
		{
			sContent += chr;
			usedChar();
		}
	}
}

