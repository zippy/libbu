/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_CLIENT_LINK_H
#define BU_CLIENT_LINK_H

#include "bu/string.h"

namespace Bu
{
    class ClientLink
    {
    public:
        ClientLink();
        virtual ~ClientLink();

        virtual void sendMessage( const Bu::String &sMsg )=0;
    };
};

#endif
