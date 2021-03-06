/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_UTIL_H
#define BU_UTIL_H

#ifndef NULL
# define NULL 0
#endif

/* I borrowed this from someone who borrowed it from glib who borrowed it
 * from...
 */
#if (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1))
# define DEPRECATED  __attribute__((__deprecated__))
#else
# define DEPRECATED
#endif /* __GNUC__ */

#include <string.h>

namespace Bu
{
    /**
     * Swap the value of two variables, uses references, so it's pretty safe.
     * Objects passed in must support a basic assignemnt operator (=);
     *@param a Variable to recieve the value of parameter b
     *@param b Variable to recieve the value of parameter a
     */
    template<typename item>
    void swap( item &a, item &b )
    {
        item tmp = a;
        a = b;
        b = tmp;
    }

    /**
     * Finds the lesser of the two objects, objects passed in must be
     * less-than-comparable.
     *@param a A value to test.
     *@param b Another value to test.
     *@returns A reference to the lesser of a or b.
     */
    template<typename item>
    const item &buMin( const item &a, const item &b )
    {
        return a<b?a:b;
    }
    
    /**
     * Finds the lesser of the two objects, objects passed in must be
     * less-than-comparable.
     *@param a A value to test.
     *@param b Another value to test.
     *@returns A reference to the lesser of a or b.
     */
    template<typename item>
    item &buMin( item &a, item &b )
    {
        return a<b?a:b;
    }
    
    /**
     * Finds the greater of the two objects, objects passed in must be
     * less-than-comparable.
     *@param a A value to test.
     *@param b Another value to test.
     *@returns A reference to the greater of a or b.
     */
    template<typename item>
    const item &buMax( const item &a, const item &b )
    {
        return b<a?a:b;
    }
    
    /**
     * Finds the greater of the two objects, objects passed in must be
     * less-than-comparable.
     *@param a A value to test.
     *@param b Another value to test.
     *@returns A reference to the greater of a or b.
     */
    template<typename item>
    item &buMax( item &a, item &b )
    {
        return b<a?a:b;
    }

    /**
     * Given three objects this finds the one between the other two.
     *@param a A value to test.
     *@param b Another value to test.
     *@param c Yet another value to test.
     *@returns A reference to the mid-value of a, b, and c.
     */
    template<typename item>
    const item &buMid( const item &a, const item &b, const item &c )
    {
        return buMin( buMax( a, b ), c );
    }
        
    /**
     * Given three objects this finds the one between the other two.
     *@param a A value to test.
     *@param b Another value to test.
     *@param c Yet another value to test.
     *@returns A reference to the mid-value of a, b, and c.
     */
    template<typename item>
    item &buMid( item &a, item &b, item &c )
    {
        return buMin( buMax( a, b ), c );
    }
    
    //
    //  Basic comparison functors
    //
    /**
     * Simple less-than comparison functor.  Objects being used should be
     * less-than-comparable.
     */
    template<typename item>
    struct __basicLTCmp
    {
        bool operator()( const item &a, const item &b )
        {
            return a < b;
        }
    };
    
    /**
     * Simple greater-than comparison functor.  Objects being used should be
     * greater-than-comparable.
     */
    template<typename item>
    struct __basicGTCmp
    {
        bool operator()( const item &a, const item &b )
        {
            return a > b;
        }
    };

    /**
     * As __basicLTCmp but dereferences the passed in pointers before comparing.
     */
    template<typename item>
    struct __basicPtrLTCmp
    {
        bool operator()( const item &a, const item &b )
        {
            return *a < *b;
        }
    };
    
    /**
     * As __basicGTCmp but dereferences the passed in pointers before comparing.
     */
    template<typename item>
    struct __basicPtrGTCmp
    {
        bool operator()( const item &a, const item &b )
        {
            return *a > *b;
        }
    };

    /**
     * Get the number of days in the month in the gregorian calendar, taking
     * leap years into account.
     */
    int getDaysInMonth( int iMonth, int iYear );

    void memcpy( void *pDest, const void *pSrc, size_t iBytes );
};

#endif
