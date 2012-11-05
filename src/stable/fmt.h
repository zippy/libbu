#ifndef BU_FMT_H
#define BU_FMT_H

namespace Bu
{
    typedef struct Fmt
    {
        enum Alignment
        {
            Left    = 0,
            Center  = 1,
            Right   = 2
        };
        Fmt() :
            uMinWidth( 0 ),
            cFillChar(' '),
            uRadix( 10 ),
            uAlign( Right ),
            bPlus( false ),
            bCaps( false ),
            bTokenize( true )
        {
        }

        Fmt( unsigned int uMinWidth, unsigned int uRadix=10,
                Alignment a=Right, bool bPlus=false, bool bCaps=true,
                char cFill=' ') :
            uMinWidth( uMinWidth ),
            cFillChar(cFill),
            uRadix( uRadix ),
            uAlign( a ),
            bPlus( bPlus ),
            bCaps( bCaps ),
            bTokenize( true )
        {
        }
        Fmt( unsigned int uMinWidth, Alignment a,
                unsigned int uRadix=10, bool bPlus=false, bool bCaps=true,
                char cFill=' ') :
            uMinWidth( uMinWidth ),
            cFillChar(cFill),
            uRadix( uRadix ),
            uAlign( a ),
            bPlus( bPlus ),
            bCaps( bCaps ),
            bTokenize( true )
        {
        }

        static Fmt hex( unsigned int uWidth=0, bool bCaps=false )
        {
            return Fmt( uWidth, 16, Right, false, bCaps, '0' );
        }
        
        static Fmt oct( unsigned int uWidth=0 )
        {
            return Fmt( uWidth, 8, Right, false, false, '0' );
        }

        static Fmt bin( unsigned int uWidth=0 )
        {
            return Fmt( uWidth, 1, Right, false, false, '0' );
        }

        static Fmt ptr( bool bCaps=true )
        {
            return Fmt( sizeof(ptrdiff_t)*2, 16, Right, false, bCaps, '0' );
        }

        Fmt &width( unsigned int uWidth );
        Fmt &fill( char cFill='0' );
        Fmt &radix( unsigned int uRadix );
        Fmt &align( Alignment eAlign );
        Fmt &plus( bool bPlus=true );
        Fmt &caps( bool bCaps=true );
        Fmt &upper();
        Fmt &lower();
        Fmt &tokenize( bool bTokenize=true );

        Fmt &left();
        Fmt &right();
        Fmt &center();

        unsigned char uMinWidth;
        char cFillChar;
        unsigned short uRadix : 6;
        unsigned short uAlign : 2;
        unsigned short bPlus : 1;
        unsigned short bCaps : 1;
        unsigned short bTokenize : 1;
    } Fmt;
};

#endif
