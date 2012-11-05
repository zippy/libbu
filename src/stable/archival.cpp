/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/archival.h"

Bu::Archival::Archival()
{
}

Bu::Archival::~Archival()
{
}

Bu::ArchiveBase &Bu::operator<<(Bu::ArchiveBase &s, const Bu::Archival &p)
{
    const_cast<Bu::Archival &>(p).archive( s );
    return s;
}

Bu::ArchiveBase &Bu::operator<<(Bu::ArchiveBase &s, Bu::Archival &p)
{
    p.archive( s );
    return s;
}

Bu::ArchiveBase &Bu::operator>>(Bu::ArchiveBase &s, Bu::Archival &p)
{
    p.archive( s );
    return s;
}

