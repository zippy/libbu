/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/regex.h"

#include <regex.h> // Please, please include the system regex.h file

#define re ((regex_t *)pRegEx)
#define aSubStr ((regmatch_t *)paSubStr)

Bu::RegEx::RegEx() :
    pRegEx( NULL ),
    bCompiled( false ),
    paSubStr( NULL )
{
}

Bu::RegEx::RegEx( const Bu::String &sSrc ) :
    pRegEx( NULL ),
    bCompiled( false ),
    paSubStr( NULL )
{
    compile( sSrc );
}

Bu::RegEx::~RegEx()
{
    if( bCompiled )
    {
        regfree( re );
        delete re;
        delete[] aSubStr;
    }
}

void Bu::RegEx::compile( const Bu::String &sSrc )
{
    if( bCompiled )
    {
        regfree( re );
        delete re;
        delete[] aSubStr;
        bCompiled = false;
    }
    pRegEx = (void *)(new regex_t);

    int nErr = regcomp( re, sSrc.getStr(), REG_EXTENDED|REG_NEWLINE );
    if( nErr )
    {
        size_t length = regerror( nErr, re, NULL, 0 );
        char *buffer = new char[length];
        (void) regerror( nErr, re, buffer, length );
        Bu::String s( buffer );
        delete[] buffer;
        throw "???"; // BuildException( s.getStr() );
    }
    bCompiled = true;
    this->sSrc = sSrc;

    nSubStr = re->re_nsub+1;
    paSubStr = (void *)(new regmatch_t[nSubStr]);
}

int Bu::RegEx::getNumSubStrings()
{
    return nSubStr;
}

bool Bu::RegEx::execute( const Bu::String &sSrc )
{
    sTest = sSrc;
    if( regexec( re, sSrc.getStr(), nSubStr, aSubStr, 0 ) )
        return false;
    return true;
}

void Bu::RegEx::getSubStringRange( int nIndex, int &iStart, int &iEnd )
{
    iStart = aSubStr[nIndex].rm_so;
    iEnd = aSubStr[nIndex].rm_eo;
}

Bu::String Bu::RegEx::getSubString( int nIndex )
{
//  regmatch_t *Subs = aSubStr;
    return Bu::String(
        sTest.getStr()+aSubStr[nIndex].rm_so,
        aSubStr[nIndex].rm_eo - aSubStr[nIndex].rm_so
        );
}

