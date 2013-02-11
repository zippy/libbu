/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/util.h"

int Bu::getDaysInMonth( int iMonth, int iYear )
{
    if( iMonth > 11 )
    {
        iYear += iMonth/12;
        iMonth = iMonth%12;
    }
    switch( iMonth )
    {
    case 0:
    case 2:
    case 4:
    case 6:
    case 7:
    case 9:
    case 11:
        return 31;
        break;

    case 3:
    case 5:
    case 8:
    case 10:
        return 30;
        break;

    case 1:
        if( iYear%400 == 0 )
            return 29;
        if( iYear%100 == 0 )
            return 28;
        if( iYear%4 == 0 )
            return 29;
        return 28;
        break;

    default:
        return -1;
    }
}
void Bu::memcpy( void *pDest, const void *pSrc, size_t iBytes )
{
#ifdef VALTEST
    const char *src = (const char *)pSrc;
    char *dest = (char *)pDest;
    for( int j = 0; j < count; j++ )
    {
        *dest = *src;
        dest++;
        src++;
    }
#else
    ::memcpy( pDest, pSrc, iBytes );
#endif
}

