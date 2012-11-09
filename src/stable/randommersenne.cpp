#include "bu/randommersenne.h"

Bu::RandomMersenne::RandomMersenne( int32_t iSeed ) :
    iIndex( 0 )
{
    seed( iSeed );
}

Bu::RandomMersenne::~RandomMersenne()
{
}

void Bu::RandomMersenne::seed( int32_t iSeed )
{
     iMt[0] = iSeed;
     for( int j = 1; j < 624; j++ )
     {
         iMt[j] = 0x6c078965 * (iMt[j-1]^(iMt[j-1]>>30)) + j;
     }
}

int32_t Bu::RandomMersenne::rand()
{
    if( iIndex == 0 )
    {
        for( int j = 0; j < 624; j++ )
        {
            uint32_t y = (iMt[j] & 0x80000000) + (iMt[(j+1)%624] & 0x7fffffff);
            iMt[j] = iMt[(j+397)%624] ^ (y>>1);
            if( (y%2) )
                iMt[j] = iMt[j]^0x9908b0df;
        }
    }

    uint32_t y = iMt[iIndex];
    y = y ^ (y>>11);
    y = y ^ ((y<<7)&0x9d2c5680);
    y = y ^ ((y<<15)&0xefc60000);
    y = y ^ (y>>18);

    iIndex = (iIndex + 1) % 624;
    return y;
}

