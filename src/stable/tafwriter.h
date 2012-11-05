/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_TAF_WRITER_H
#define BU_TAF_WRITER_H

#include <stdint.h>

#include "bu/string.h"

namespace Bu
{
    class Stream;
    class TafNode;
    class TafGroup;
    class TafProperty;
    class TafComment;

    /**
     *
     *@ingroup Taf
     */
    class TafWriter
    {
    public:
        TafWriter( Bu::Stream &sOut );
        virtual ~TafWriter();

        void writeGroup( const Bu::TafGroup *pRoot );

    private:
        void writeProperty( const Bu::TafProperty *pProp );
        void writeComment( const Bu::TafComment *pComment );
        void writeString( const Bu::String &str );
        void ident();
        Bu::Stream &sOut;
        int iDepth;
    };
}

#endif
