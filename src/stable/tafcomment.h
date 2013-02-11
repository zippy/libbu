/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_TAF_COMMENT_H
#define BU_TAF_COMMENT_H

#include <stdint.h>
#include "bu/tafnode.h"

namespace Bu
{
    /**
     *
     *@ingroup Taf
     */
    class TafComment : public TafNode
    {
    public:
        TafComment( const Bu::TafComment &rSrc );
        TafComment( const Bu::String &sText, bool bEOL=false );
        virtual ~TafComment();

        const Bu::String &getText() const;
        bool isEOLStyle() const;

    private:
        Bu::String sText;
        bool bEOL;
    };
}

#endif
