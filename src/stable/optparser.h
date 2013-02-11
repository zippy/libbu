/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_OPT_PARSER_H
#define BU_OPT_PARSER_H

#include "bu/string.h"
#include "bu/list.h"
#include "bu/hash.h"
#include "bu/signals.h"
#include "bu/array.h"
#include "bu/membuf.h"
#include "bu/formatter.h"
#include "bu/variant.h"

namespace Bu
{
    typedef Bu::Array<Bu::String> StrArray;

    /**
     * POSIX/Gnu style command line parser.  Handles long and short options in
     * a variety of fun and useful ways, along with singal based callbacks and
     * automatic variable setting.  It's pretty easy to use, and very flexible.
     *
     * OptParser supports it's own builtin help mechanism which automatically
     * enumerates the available options and their help in a well formatted and
     * easy to read way, automatically formatting your help text per option and
     * allows for addition "help banners" which can be placed wherever you
     * would like.
     */
    class OptParser
    {
    private:
        class _ValueProxy
        {
        public:
            _ValueProxy();
            virtual ~_ValueProxy();

            virtual void setValueFromStr( const Bu::String & )=0;
            virtual void setValue( const Bu::Variant &vVar )=0;
            virtual _ValueProxy *clone()=0;
        };

        template<typename ptype>
        class ValueProxy : public _ValueProxy
        {
        public:
            ValueProxy( ptype &v ) :
                v( v )
            {
            }

            virtual ~ValueProxy()
            {
            }

            virtual void setValueFromStr( const Bu::String &sVal )
            {
                Bu::MemBuf mb( sVal );
                Bu::Formatter f( mb );
                f << Bu::Fmt().tokenize( false );
                f >> v;
            }

            virtual void setValue( const Bu::Variant &vVar )
            {
                if( vVar.getType() == typeid(ptype) )
                {
                    v = vVar.get<ptype>();
                }
                else if( vVar.getType() == typeid(Bu::String) )
                {
                    setValueFromStr( vVar.get<Bu::String>() );
                }
                else
                {
                    Bu::MemBuf mb;
                    Bu::Formatter f( mb );
//                  f << vVar;
                    setValueFromStr( mb.getString() );
                }
            }
            
            virtual _ValueProxy *clone()
            {
                return new ValueProxy<ptype>( v );
            }

        private:
            ptype &v;
        };

    public:
        typedef Signal1<int, StrArray> OptionSignal;
        class Option
        {
        public:
            Option();
            Option( const Option &rSrc );
            virtual ~Option();

            char cOpt;
            Bu::String sOpt;
            Bu::String sHelp;
            OptionSignal sUsed;
            _ValueProxy *pProxy;
            Bu::Variant sOverride;
            Bu::String sHelpDefault;
        };
    
    private:
        typedef Bu::List<Option> OptionList;
        typedef Bu::Hash<char, Option *> ShortOptionHash;
        typedef Bu::Hash<Bu::String, Option *> LongOptionHash;
        
        class Banner
        {
        public:
            Bu::String sText;
            bool bFormatted;
            OptionList::const_iterator iAfter;
        };

        typedef Bu::List<Banner> BannerList;

    public:
        OptParser();
        virtual ~OptParser();

        void parse( int argc, char **argv );
        void parse( const Bu::String &sLine );

        void addOption( const Option &opt );
        
        template<typename vtype>
        void addOption( vtype &var, char cOpt, const Bu::String &sOpt,
                const Bu::String &sHelp )
        {
            Option o;
            o.cOpt = cOpt;
            o.sOpt = sOpt;
            o.pProxy = new ValueProxy<vtype>( var );
            o.sHelp = sHelp;
            addOption( o );
        }
        
        template<typename vtype>
        void addOption( vtype &var, const Bu::String &sOpt,
                const Bu::String &sHelp )
        {
            addOption( var, '\0', sOpt, sHelp );
        }
        
        template<typename vtype>
        void addOption( vtype &var, char cOpt, const Bu::String &sHelp )
        {
            addOption( var, cOpt, "", sHelp );
        }

        void addOption( OptionSignal sUsed, char cOpt, const Bu::String &sOpt,
                const Bu::String &sHelp )
        {
            Option o;
            o.cOpt = cOpt;
            o.sOpt = sOpt;
            o.sUsed = sUsed;
            o.sHelp = sHelp;
            addOption( o );
        }
        
        void addOption( OptionSignal sUsed, const Bu::String &sOpt,
                const Bu::String &sHelp )
        {
            addOption( sUsed, '\0', sOpt, sHelp );
        }
        
        void addOption( OptionSignal sUsed, char cOpt,
                const Bu::String &sHelp )
        {
            addOption( sUsed, cOpt, "", sHelp );
        }

        void setOverride( char cOpt, const Bu::Variant &sOverride );
        void setOverride( const Bu::String &sOpt,
                const Bu::Variant &sOverride );

        void setHelpDefault( const Bu::String &sOpt, const Bu::String &sTxt );
        
        void addHelpOption( char c='h', const Bu::String &s="help",
                const Bu::String &sHelp="This help." );
        void addHelpBanner( const Bu::String &sText, bool bFormatted=true );

        int optHelp( StrArray aParams );

        /**
         * This function is called when an unrecognized option is found, the
         * default behaviour is to print an error to stdout and exit( 1 ), if
         * you want to do something different, just override this function.
         * This is also called by default when something is found that hasn't
         * been handled by an option, and isn't an option (starts with - or --).
         * To change this behaviour call 
         */
        virtual void optionError( const Bu::String &sOption );

        void setNonOption( OptionSignal sSignal );

    private:
        Bu::String format( const Bu::String &sIn, int iWidth, int iIndent );

        OptionList lOption;
        ShortOptionHash hsOption;
        LongOptionHash hlOption;
        BannerList lBanner;
        OptionSignal sNonOption;
    };
};

#endif
