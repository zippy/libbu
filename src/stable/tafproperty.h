/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_TAF_PROPERTY_H
#define BU_TAF_PROPERTY_H

#include <stdint.h>
#include "bu/tafnode.h"

namespace Bu
{
    /**
     *
     *@ingroup Taf
     */
    class TafProperty : public TafNode
    {
    public:
        TafProperty( const Bu::TafProperty &rSrc );
        TafProperty( const Bu::String &sName, const Bu::String &sValue );
        virtual ~TafProperty();

        const Bu::String &getName() const;
        const Bu::String &getValue() const;

    private:
        Bu::String sName;
        Bu::String sValue;
    };
}

#endif
