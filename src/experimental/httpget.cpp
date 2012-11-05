/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/httpget.h"

Bu::HttpGet::HttpGet( const Bu::Url &uSrc, const Bu::String &sMethod ) :
    uSrc( uSrc ),
    sMethod( sMethod ),
    sSrv( uSrc.getHost(), uSrc.getPort() )
{
    sSrv.write( sMethod + " " + uSrc.getFullPath() + " HTTP/1.1\r\n" );
}

Bu::HttpGet::~HttpGet()
{
}

void Bu::HttpGet::close()
{
}

void Bu::HttpGet::get()
{
    for( MimeHash::iterator i = hMimeOut.begin(); i; i++ )
    {
        sSrv.write( i.getKey() + ": " + i.getValue() + "\r\n" );
    }
    sSrv.write("\r\n", 2 );

//  sSrv.read(
}

Bu::size Bu::HttpGet::read( void * /*pBuf*/, Bu::size /*nBytes*/ )
{
    return 0;
}

Bu::size Bu::HttpGet::write( const void * /*pBuf*/, Bu::size /*nBytes*/ )
{
    return 0;
}

Bu::size Bu::HttpGet::tell()
{
    return 0;
}

void Bu::HttpGet::seek( Bu::size )
{
}

void Bu::HttpGet::setPos( Bu::size )
{
}

void Bu::HttpGet::setPosEnd( Bu::size )
{
}

bool Bu::HttpGet::isEos()
{
    return false;
}

bool Bu::HttpGet::isOpen()
{
    return true;
}

void Bu::HttpGet::flush()
{
}

bool Bu::HttpGet::canRead()
{
    return true;
}

bool Bu::HttpGet::canWrite()
{
    return false;
}

bool Bu::HttpGet::isReadable()
{
    return true;
}

bool Bu::HttpGet::isWritable()
{
    return false;
}

bool Bu::HttpGet::isSeekable()
{
    return false;
}

bool Bu::HttpGet::isBlocking()
{
    return true;
}

void Bu::HttpGet::setBlocking( bool /*bBlocking*/ )
{
}

Bu::size Bu::HttpGet::getSize() const
{
    return 0;
}

Bu::size Bu::HttpGet::getBlockSize() const
{
    return 0;
}

Bu::String Bu::HttpGet::getLocation() const
{
    return uSrc.getUrl();
}

