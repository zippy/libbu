/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/deflate.h"
#include "bu/file.h"

int main( int argc, char *argv[] )
{
    if( argc < 3 )
    {
        printf("usage:  %s <in> <out>\n", argv[0] );
        return -1;
    }

    char buf[1024];
    size_t nRead;

    /*
    Bu::File fin( argv[1], Bu::File::Read );
    fin.seek( 4 );
    Bu::Deflate def( fin );
    
    Bu::File f( argv[2], Bu::File::WriteNew );

    for(;;)
    {
        nRead = def.read( buf, 1024 );
        if( nRead > 0 )
            f.write( buf, nRead );
        if( def.isEos() )
            break;
    }
    */

    Bu::File fin( argv[1], Bu::File::Read );
    
    Bu::File f( argv[2], Bu::File::WriteNew );
    Bu::Deflate def( f, 9, Bu::Deflate::Gzip );

    for(;;)
    {
        nRead = fin.read( buf, 1024 );
        if( nRead > 0 )
            def.write( buf, nRead );
        if( fin.isEos() )
            break;
    }
}

