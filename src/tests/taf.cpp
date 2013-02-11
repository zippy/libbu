/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/taf.h"
#include "bu/file.h"

int main( int argc, char *argv[] )
{
    if( argc == 1 )
    {
        Bu::File f("test.taf", Bu::File::Read );
        Bu::TafReader tr( f );

        Bu::TafGroup *pGroup = tr.readGroup();
        
        {
            Bu::File fo("out.taf", Bu::File::Write|Bu::File::Create );
            Bu::TafWriter tw( fo );
            tw.writeGroup( pGroup );
        }

        delete pGroup;
    }
    else if( argc == 3 )
    {
        Bu::File f( argv[1], Bu::File::Read );
        Bu::TafReader tr( f );

        Bu::TafGroup *pGroup = tr.readGroup();
        
        {
            Bu::File fo( argv[2], Bu::File::Write|Bu::File::Create );
            Bu::TafWriter tw( fo );
            tw.writeGroup( pGroup );
        }

        delete pGroup;
    }
}

