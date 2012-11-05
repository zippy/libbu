/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/xmlreader.h"
#include "bu/stream.h"

namespace Bu { subExceptionDef( XmlException ) }

Bu::XmlReader::XmlReader( Stream &rInput ) :
    rInput( rInput ),
    iCurToken( 0 ),
    iNextToken( 0 ),
    bIgnoreWS( true )
{
    nextToken();
    stDocument();
}

Bu::XmlReader::~XmlReader()
{
}

void Bu::XmlReader::fillBuffer()
{
    if( rInput.isEos() )
        return;
    char buf[1024];
    int iSize = rInput.read( buf, 1024 );
    sBuf.append( buf, iSize );
}

void Bu::XmlReader::cleanupBuffer( int iUsed )
{
    for( int j = 0; j < iUsed; j++ )
    {
        if( sBuf[j] == '\n' )
        {
            spNextToken.iLine++;
            spNextToken.iChar = 1;
        }
        else
        {
            spNextToken.iChar++;
        }
    }

    printf("--Deleting %d bytes from front of buffer.\n", iUsed );
    sBuf.trimFront( iUsed );
}

int Bu::XmlReader::nextToken()
{
    fillBuffer();

    int iUsed = 1;

    iCurToken = iNextToken;
    spCurToken = spNextToken;

    switch( sBuf[0] )
    {
        case '<':
            if( !strncmp( sBuf.getStr(), "<?xml", 5 ) )
            {
                iNextToken = tokXmlDeclHead;
                iUsed = 5;
            }
            else
            {
                iNextToken = '<';
            }
            break;

        case '?':
            if( sBuf[1] == '>' )
            {
                iNextToken = tokXmlDeclEnd;
                iUsed = 2;
            }
            else
            {
                iNextToken = '?';
            }
            break;

        case ' ':
        case '\t':
        case '\n':
        case '\r':
            for( int j = 1;; j++ )
            {
                if( j == sBuf.getSize() )
                {
                    if( rInput.isEos() )
                        error("Reached end of input while waiting for whitespace to end.");

                    fillBuffer();
                }
                if( sBuf[j] == ' ' || sBuf[j] == '\t' ||
                    sBuf[j] == '\n' || sBuf[j] == '\r' )
                    iUsed++;
                else
                    break;
            }
            sStr.clear();
            sStr.append( sBuf, iUsed );
            iNextToken = tokWS;
            break;

        case '=':
            iNextToken = sBuf[0];
            break;

        default:
            if( (sBuf[0] >= 'a' && sBuf[0] <= 'z') ||
                (sBuf[0] >= 'A' && sBuf[0] <= 'Z') )
            {
                for( int j = 1;; j++ )
                {
                    if( j == sBuf.getSize() )
                    {
                        if( rInput.isEos() )
                            error("Reached end of input while waiting for a string to end.");

                        fillBuffer();
                    }
                    if( (sBuf[j] >= 'a' && sBuf[j] <= 'z') ||
                        (sBuf[j] >= 'A' && sBuf[j] <= 'Z') )
                        iUsed++;
                    else
                        break;
                }
                sStr.clear();
                sStr.append( sBuf, iUsed );
                iNextToken = tokIdent;
            }
    }

    cleanupBuffer( iUsed );

    return iCurToken;
}

void Bu::XmlReader::error( const char *sMessage )
{
    throw Bu::XmlException("%d:%d:  %s",
        spCurToken.iLine, spCurToken.iChar, sMessage );
}

void Bu::XmlReader::stDocument()
{
    stProlog();
}

void Bu::XmlReader::stProlog()
{
    stXmlDecl();
}

void Bu::XmlReader::stXmlDecl()
{
    if( nextToken() != tokXmlDeclHead )
        error("You must begin your xml file with a declaration: <?xml ... ?>");
    if( nextToken() != tokIdent )
        error("A version comes first!");
    if( sStr != "version" )
        error("No, a version!");
}

