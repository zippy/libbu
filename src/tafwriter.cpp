#include "tafwriter.h"

Bu::TafWriter::TafWriter( Bu::Stream &sOut ) :
	sOut( sOut )
{
}

Bu::TafWriter::~TafWriter()
{
}

void Bu::TafWriter::writeGroup( const Bu::TafGroup *pRoot )
{
	sOut.write("{", 1 );
	writeString( pRoot->getName() );
	sOut.write(": ", 2 );
	const Bu::TafGroup::NodeList &nl = pRoot->getChildren();
	for( Bu::TafGroup::NodeList::const_iterator i = nl.begin(); i != nl.end(); i++ )
	{
		switch( (*i)->getType() )
		{
			case Bu::TafNode::typeGroup:
				writeGroup( (Bu::TafGroup *)(*i) );
				break;

			case Bu::TafNode::typeProperty:
				writeProperty( (Bu::TafProperty *)(*i) );
				break;

			case Bu::TafNode::typeComment:
				writeComment( (Bu::TafComment *)(*i) );
				break;
		}
	}
	sOut.write("}", 1 );
}

void Bu::TafWriter::writeProperty( const Bu::TafProperty *pProp )
{
	writeString( pProp->getName() );
	if( pProp->getValue().getStr() != NULL )
	{
		sOut.write("=", 1 );
		writeString( pProp->getValue() );
	}
	sOut.write(" ", 1 );
}

void Bu::TafWriter::writeComment( const Bu::TafComment *pComment )
{
	sOut.write("/*", 2 );
	sOut.write( pComment->getText().getStr(), pComment->getText().getSize() );
	sOut.write("*/ ", 3 );
}

void Bu::TafWriter::writeString( const Bu::FString &str )
{
	if( str.getStr() == NULL )
		return;
	sOut.write("\"", 1 );
	for( const char *s = str.getStr(); *s; s++ )
	{
		if( *s == '\"' )
			sOut.write("\\\"", 2 );
		else
			sOut.write( s, 1 );
	}
	sOut.write("\"", 1 );
}

