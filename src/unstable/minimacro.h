/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_MINI_MACRO_H
#define BU_MINI_MACRO_H

#include "bu/hash.h"
#include "bu/string.h"

namespace Bu
{
    typedef Bu::Hash<Bu::String, Bu::String> StrHash;
    /**
     * A processor for Libbu++ brand Mini Macros.  These are really simple, but
     * still fairly flexible.  It's mainly text replacement, but with a few
     * extras like filter functions and conditional text segments.  So far we
     * don't support loops or anything, I'm not sure we ever will.
     *
     * Anatomy of a mini macro:
     * - Every macro begins with a two character code, the first character is
     *   always '{', the second character determines the operation to perform.
     * - If the '{' is followed by a character that is not valid it is not
     *   considered for expansion and the characters are copied to the output.
     * - Every macro ends with a closing '}'
     * - Macro types:
     *   - '=': variable replacement.  The '=' is immediatley followed by the
     *     name of the variable to replace, then any number of optional filter
     *     segments.
     *   - '?': conditional text.  The '?' is immediately followed by the
     *     variable to test.  This works two ways, the variable can be alone, in
     *     which case it's existance is tested, or it can be followed by a "="
     *     and a string to compare to.  This is then followed by a text segment
     *     that will be used if the test is true, and an optional text segment
     *     to be used if the test is false.
     *   - ':': command.  The ':' is immediately followed by a command string,
     *     of which there's only one right now, but that's ok.  These are not
     *     put into the output stream, but instead mark something for the
     *     parser.  Currently supported:
     *     - {:end}: end of parsing, stop here, also make note of how many input
     *       characters were used.
     * - Segments:
     *   - Each segment is seperated by a colon.
     *   - Filter segments give the name of the filter, followed by
     *     parenthesies.  Parameters may be provided within the parenthesies.
     *   - Text segments should always be quoted, but may contain any characters
     *     within the quotes, backslash is used as per C/ANSI/ISO standard.
     *     You can also quote any text using [' '] instead of quotes, which
     *     allows for nested strings.  The [' token is only recognised within
     *     a macro.
     *
     *@verbatim
     {=name:tolower()}
     {=name:ccsplit("_"):toupper()}
     {?name:"name exists and is {=name}"}
     {?name:"{=name}":"no name!"}
     {?name="bob":"You're named bob!":"Who are you?  I only know bob..."}
     @endverbatim
     */
    class MiniMacro
    {
    public:
        MiniMacro();
        MiniMacro( const StrHash &sVarSrc );
        virtual ~MiniMacro();

        Bu::String parse( const Bu::String &sIn );
        void addVar( const Bu::String &sName, const Bu::String &sValue );
        bool hasVar( const Bu::String &sName );
        const Bu::String &getVar( const Bu::String &sName );
        const StrHash &getVars();
        int getPosition();

    private:
        const char *sCur;
        Bu::String parseRepl();
        Bu::String parseCond();
        Bu::String parseCmd();
        Bu::String callFunc(
            const Bu::String &sIn, const Bu::String &sFunc );

        StrHash hVars;
        bool bContinue;
        int iLastPos;

    public:
        typedef Bu::List<Bu::String> StrList;
        class Func
        {
        public:
            Func(){}
            virtual ~Func(){}
            virtual Bu::String call(
                const Bu::String &sIn, StrList &lsParam )=0;
        };

        class FuncToUpper : public Func
        {
        public:
            FuncToUpper(){}
            virtual ~FuncToUpper(){}
            virtual Bu::String call(
                const Bu::String &sIn, StrList & )
            {
                return sIn.toUpper();
            }
        };

        class FuncToLower : public Func
        {
        public:
            FuncToLower(){}
            virtual ~FuncToLower(){}
            virtual Bu::String call(
                const Bu::String &sIn, StrList & )
            {
                return sIn.toLower();
            }
        };
        
    private:
        typedef Bu::Hash<Bu::String,class Func *> FuncHash;
        FuncHash hFuncs;
    };
};

#endif
