/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/taf.h"
#include "bu/stream.h"

Bu::TafWriter::TafWriter( Bu::Stream &sOut ) :
    sOut( sOut ),
    iDepth( 0 )
{
}

Bu::TafWriter::~TafWriter()
{
}

void Bu::TafWriter::ident()
{
    for( int j = 0; j < iDepth; j++ )
        sOut.write("    ", 4 );
}

void Bu::TafWriter::writeGroup( const Bu::TafGroup *pRoot )
{
    ident();
    sOut.write("{", 1 );
    if( pRoot->getName().isSet() )
        writeString( pRoot->getName() );
    sOut.write(":\n", 2 );
    iDepth++;
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
    iDepth--;
    ident();
    sOut.write("}\n", 2 );
}

void Bu::TafWriter::writeProperty( const Bu::TafProperty *pProp )
{
    ident();
    if( !pProp->getName().isEmpty() )
    {
        writeString( pProp->getName() );
        sOut.write("=", 1 );
        writeString( pProp->getValue() );
    }
    else
    {
        writeString( pProp->getValue() );
    }
    sOut.write("\n", 1 );
}

void Bu::TafWriter::writeComment( const Bu::TafComment *pComment )
{
    ident();
    if( pComment->isEOLStyle() )
    {
        sOut.write("//", 2 );
        sOut.write( pComment->getText().getStr(), pComment->getText().getSize() );
        sOut.write("\n", 1 );
    }
    else
    {
        sOut.write("/*", 2 );
        sOut.write( pComment->getText().getStr(), pComment->getText().getSize() );
        sOut.write("*/ ", 3 );
    }
}

void Bu::TafWriter::writeString( const Bu::String &str )
{
    sOut.write("\"", 1 );
	for( Bu::String::const_iterator s = str.begin(); s != str.end(); s++ )
	{
		if( *s == '\"' )
            sOut.write("\\\"", 2 );
		else if( *s == '\\' )
			sOut.write("\\\\", 2 );
		else if( *s < 32 || *s > 126 )
		{
			char buf[5];
			sprintf( buf, "\\x%02X", (unsigned char)*s );
			sOut.write(buf, 4 );
		}
		else
		{
			const char buf = *s;
			sOut.write( &buf, 1 );
		}
	}
	sOut.write("\"", 1 );
}

