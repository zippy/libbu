/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/file.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include "bu/random.h"

#include "bu/config.h"

namespace Bu { subExceptionDef( FileException ) }

Bu::File::File( const Bu::String &sName, int iFlags ) :
    fd( -1 ),
    bEos( true )
{
#ifdef USE_64BIT_IO
    fd = ::open64( sName.getStr(), getPosixFlags( iFlags ), 0666 );
#else
    fd = ::open( sName.getStr(), getPosixFlags( iFlags ), 0666 );
#endif
    if( fd < 0 )
    {
        throw Bu::FileException( errno, "%s: %s",
            strerror(errno), sName.getStr() );
    }
    bEos = false;
}

Bu::File::File( int fd ) :
    fd( fd )
{
    bEos = false;
}

Bu::File::~File()
{
    close();
}

void Bu::File::close()
{
    if( fd >= 0 )
    {
        if( ::close( fd ) )
        {
            throw Bu::FileException( errno, "%s",
                strerror(errno) );
        }
        fd = -1;
        bEos = true;
    }
}

Bu::size Bu::File::read( void *pBuf, Bu::size nBytes )
{
    if( fd < 0 )
        throw FileException("File not open.");

    Bu::size iRead = ::read( fd, pBuf, nBytes );
    if( iRead == 0 )
        bEos = true;
    else if( iRead == -1 && errno == EAGAIN )
        return 0;
    else if( iRead < 0 )
        throw FileException( errno, "%s", strerror( errno ) );
    return iRead;
}

Bu::size Bu::File::write( const void *pBuf, Bu::size nBytes )
{
    if( fd < 0 )
        throw FileException("File not open.");

    Bu::size iWrote = ::write( fd, pBuf, nBytes );
    if( iWrote < 0 )
        throw FileException( errno, "%s", strerror( errno ) );
    return iWrote;
}

Bu::size Bu::File::tell()
{
    if( fd < 0 )
        throw FileException("File not open.");

#ifdef USE_64BIT_IO
    return lseek64( fd, 0, SEEK_CUR );
#else
    return lseek( fd, 0, SEEK_CUR );
#endif
}

void Bu::File::seek( Bu::size offset )
{
    if( fd < 0 )
        throw FileException("File not open.");

#ifdef USE_64BIT_IO
    lseek64( fd, offset, SEEK_CUR );
#else
    lseek( fd, offset, SEEK_CUR );
#endif
    bEos = false;
}

void Bu::File::setPos( Bu::size pos )
{
    if( fd < 0 )
        throw FileException("File not open.");

#ifdef USE_64BIT_IO
    lseek64( fd, pos, SEEK_SET );
#else
    lseek( fd, pos, SEEK_SET );
#endif
    bEos = false;
}

void Bu::File::setPosEnd( Bu::size pos )
{
    if( fd < 0 )
        throw FileException("File not open.");
    
    lseek64( fd, pos, SEEK_END );
    bEos = false;
}

bool Bu::File::isEos()
{
    return bEos;
}

bool Bu::File::canRead()
{
#ifdef WIN32
    return true;
#else
    int iMode = fcntl( fd, F_GETFL, 0 )&O_ACCMODE;
    if( iMode == O_RDONLY || iMode == O_RDWR )
        return true;
    return false;
#endif
}

bool Bu::File::canWrite()
{
#ifdef WIN32
    return true;
#else
    int iMode = fcntl( fd, F_GETFL, 0 )&O_ACCMODE;
    if( iMode == O_WRONLY || iMode == O_RDWR )
        return true;
    return false;
#endif
}

bool Bu::File::isReadable()
{
    return true;
}

bool Bu::File::isWritable()
{
    return true;
}

bool Bu::File::isSeekable()
{
    return true;
}

bool Bu::File::isBlocking()
{
    return true;
}

void Bu::File::setBlocking( bool bBlocking )
{
#ifdef WIN32
    fprintf(stderr, "STUB: Bu::File::setBlocking\n");
#else
    if( bBlocking )
        fcntl( 
            fd,
            F_SETFL, fcntl( fd, F_GETFL, 0 )&(~O_NONBLOCK)
            );
    else
        fcntl( 
            fd,
            F_SETFL, fcntl( fd, F_GETFL, 0 )|O_NONBLOCK
            );
#endif
}

Bu::File Bu::File::tempFile( Bu::String &sName )
{
    int iXes;
    for( iXes = sName.getSize()-1; iXes >= 0; iXes-- )
    {
        if( sName[iXes] != 'X' )
            break;
    }
    iXes++;
    if( iXes == sName.getSize() )
        throw Bu::ExceptionBase("Invalid temporary filename template.");
    for( int iter = 0; iter < 1000; iter++ )
    {
        for( int j = iXes; j < sName.getSize(); j++ )
        {
            uint32_t iX = Bu::Random::rand();
            sName[j] = ('A'+(iX%26)) | ((iX&0x1000)?(0x20):(0));
        }

        try
        {
            return Bu::File( sName, Bu::File::Read|Bu::File::Write
                    |Bu::File::Create|Bu::File::Exclusive );
        } catch(...) { }
    }
    throw Bu::FileException("Failed to create unique temporary file after 1000"
            " iterations.");
}

void Bu::File::setSize( Bu::size iSize )
{
#ifdef WIN32
    chsize( fd, iSize );
#else
    ftruncate( fd, iSize );
#endif
}

Bu::size Bu::File::getSize() const
{
    struct stat st;
    fstat( fd, &st );
    return st.st_size;
}

Bu::size Bu::File::getBlockSize() const
{
#ifdef WIN32
    return 4096;
#else
    struct stat st;
    fstat( fd, &st );
    return st.st_blksize;
#endif
}

Bu::String Bu::File::getLocation() const
{
    return "to be implemented";
}

#ifndef WIN32
void Bu::File::chmod( mode_t t )
{
    fchmod( fd, t );
}
#endif

void Bu::File::flush()
{
    // There is no flushing with direct I/O...
    //fflush( fh );
}

bool Bu::File::isOpen()
{
    return (fd > -1);
}

int Bu::File::getPosixFlags( int iFlags )
{
    int iRet = 0;
    switch( (iFlags&ReadWrite) )
    {
        // According to posix, O_RDWR is not necesarilly O_RDONLY|O_WRONLY, so
        // lets be proper and use the right value in the right place.
        case Read:      iRet = O_RDONLY; break;
        case Write:     iRet = O_WRONLY; break;
        case ReadWrite: iRet = O_RDWR; break;
        default:
            throw FileException(
                "You must specify Read, Write, or both when opening a file.");
    }

    if( (iFlags&Create) )
        iRet |= O_CREAT;
    if( (iFlags&Append) )
        iRet |= O_APPEND;
    if( (iFlags&Truncate) )
        iRet |= O_TRUNC;
#ifndef WIN32
    if( (iFlags&NonBlock) )
        iRet |= O_NONBLOCK;
#endif
    if( (iFlags&Exclusive) == Exclusive )
        iRet |= O_EXCL;

#ifdef O_BINARY
    iRet |= O_BINARY;
#endif

    return iRet;
}

