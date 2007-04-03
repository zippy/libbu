#include "xmlreader.h"
#include "exceptions.h"
#include <string.h>
#include "hashfunctionstring.h"

XmlReader::XmlReader( bool bStrip ) :
	bStrip( bStrip ),
	htEntity( new HashFunctionString(), 11 )
{
}

XmlReader::~XmlReader()
{
	void *i = htEntity.getFirstItemPos();
	while( (i = htEntity.getNextItemPos( i ) ) )
	{
		free( (char *)(htEntity.getItemID( i )) );
		delete (StaticString *)htEntity.getItemData( i );
	}
}

void XmlReader::addEntity( const char *name, const char *value )
{
	if( htEntity[name] ) return;

	char *sName = strdup( name );
	StaticString *sValue = new StaticString( value );

	htEntity.insert( sName, sValue );
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
			std::string buf;
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
				std::string name;
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
					throw XmlException(
						"Only quoted entity values are supported."
						);
				}
				std::string value;
				for(;;)
				{
					char chr = getChar();
					usedChar();
					if( chr == '&' )
					{
						StaticString *tmp = getEscape();
						if( tmp == NULL ) throw XmlException("Entity thing");
						value += tmp->getString();
						delete tmp;
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
					throw XmlException(
						"Malformed ENTITY:  unexpected '%c' found.",
						getChar()
						);
				}
			}
			else
			{
				throw XmlException(
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
			throw XmlException("Close node in singleNode malformed!");
		}
	}
	else
	{
		throw XmlException("Close node expected, but not found.");
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
						throw XmlException("Got a mismatched node close tag.");
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
				throw XmlException("Got extra junk data instead of node close tag.");
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
		throw XmlException("Expected to find node opening char, '<'.");
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

StaticString *XmlReader::getEscape()
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

		return new StaticString( buf );
	}
	else
	{
		// ...otherwise replace with the appropriate string...
		std::string buf;
		usedChar();
		for(;;)
		{
			char cbuf = getChar();
			usedChar();
			if( cbuf == ';' ) break;
			buf += cbuf;
		}

		StaticString *tmp = (StaticString *)htEntity[buf.c_str()];
		if( tmp == NULL ) return NULL;

		StaticString *ret = new StaticString( *tmp );
		return ret;
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
						StaticString *tmp = getEscape();
						if( tmp == NULL ) return false;
						fbValue.appendData( tmp->getString() );
						delete tmp;
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
						StaticString *tmp = getEscape();
						if( tmp == NULL ) return false;
						fbValue.appendData( tmp->getString() );
						delete tmp;
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
		throw XmlException("Expected an equals to seperate the params.");
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
				usedChar( 2 );
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
							throw XmlException("Mismatched close tag found: <%s> to <%s>.", getCurrent()->getName(), fbName.getData() );
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
					throw XmlException("Malformed close tag.");
				}
			}
			else if( getChar(1) == '!' )
			{
				// We know it's a comment, let's see if it's proper
				if( getChar(2) != '-' ||
					getChar(3) != '-' )
				{
					// Not a valid XML comment
					throw XmlException("Malformed comment start tag found.");
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
								throw XmlException("Malformed comment close tag found.  You cannot have a '--' that isn't followed by a '>' in a comment.");
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
		else if( chr == '&' )
		{
			StaticString *tmp = getEscape();
			if( tmp == NULL ) return false;
			fbContent.appendData( tmp->getString() );
			delete tmp;
		}
		else
		{
			fbContent.appendData( chr );
			usedChar();
		}
	}
}

