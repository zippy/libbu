/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_CLIENT_LINK_FACTORY_H
#define BU_CLIENT_LINK_FACTORY_H

namespace Bu
{
    class Client;
    class ClientLink;

    class ClientLinkFactory
    {
    public:
        ClientLinkFactory();
        virtual ~ClientLinkFactory();

        virtual Bu::ClientLink *createLink( Bu::Client *pClient )=0;
    };
};

#endif
