/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/filter.h"

Bu::Filter::Filter( Bu::Stream &rNext ) :
    rNext( rNext )
{
}

Bu::Filter::~Filter()
{
}

void Bu::Filter::close()
{
    stop();
    rNext.close();
}

Bu::size Bu::Filter::tell()
{
    return rNext.tell();
}

void Bu::Filter::seek( Bu::size offset )
{
    rNext.seek( offset );
}

void Bu::Filter::setPos( Bu::size pos )
{
    rNext.setPos( pos );
}

void Bu::Filter::setPosEnd( Bu::size pos )
{
    rNext.setPosEnd( pos );
}

bool Bu::Filter::isEos()
{
    return rNext.isEos();
}

bool Bu::Filter::isOpen()
{
    return rNext.isOpen();
}

bool Bu::Filter::canRead()
{
    return rNext.canRead();
}

bool Bu::Filter::canWrite()
{
    return rNext.canWrite();
}

bool Bu::Filter::isReadable()
{
    return rNext.isReadable();
}

bool Bu::Filter::isWritable()
{
    return rNext.isWritable();
}

bool Bu::Filter::isSeekable()
{
    return rNext.isSeekable();
}

bool Bu::Filter::isBlocking()
{
    return rNext.isBlocking();
}

void Bu::Filter::setBlocking( bool bBlocking )
{
    rNext.setBlocking( bBlocking );
}

void Bu::Filter::setSize( Bu::size )
{
}

void Bu::Filter::flush()
{
    rNext.flush();
}

Bu::size Bu::Filter::getSize() const
{
    return rNext.getSize();
}

Bu::size Bu::Filter::getBlockSize() const
{
    return rNext.getBlockSize();
}

Bu::String Bu::Filter::getLocation() const
{
    return rNext.getLocation();
}

