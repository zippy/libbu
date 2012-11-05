/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_CSV_WRITER_H
#define BU_CSV_WRITER_H

#include "bu/string.h"
#include "bu/array.h"
#include "bu/signals.h"

namespace Bu
{
    class Stream;
    typedef Bu::Array<Bu::String> StrArray;

    class CsvWriter
    {
    public:
        typedef Bu::Signal1<Bu::String, const Bu::String &> EncodeSignal;
        enum Style
        {
            styleExcel, ///< Excel style quotes around things that need em
            styleC      ///< Escape things that need it C-style
        };

        CsvWriter( Stream &sOut, Style eStyle=styleExcel );
        CsvWriter( Stream &sOut, EncodeSignal sEncode );
        virtual ~CsvWriter();

        void writeLine( const StrArray &aStrs );

    private:
        Stream &sOut;
        EncodeSignal sEncode;

        static Bu::String encodeExcel( const Bu::String &sIn );
        static Bu::String encodeC( const Bu::String &sIn );
    };
};

#endif
