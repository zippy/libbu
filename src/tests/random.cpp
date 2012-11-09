#include <bu/randombasic.h>
#include <bu/randomcmwc.h>
#include <bu/randomsystem.h>
#include <bu/sio.h>
#include <time.h>

using namespace Bu;

template<typename T>
void coverage()
{
    T rand;
    rand.seed( time( NULL ) );

    uint32_t uBucket[78];
    memset( uBucket, 0, sizeof(uint32_t)*78 );

    for( int j = 0; j < 1000; j++ )
    {
        uBucket[(int)(((uint32_t)rand.rand())/(double)(0xfffffffful)*78+0.5)]++;
    }

    uint32_t uMax = 0;
    for( int j = 0; j < 78; j++ )
    {
        if( uMax < uBucket[j] )
            uMax = uBucket[j];
    }

    for( int y = 20; y >= 1; y-- )
    {
        uint32_t iT = (uint32_t)((y/20.0)*uMax);
        for( int x = 0; x < 78; x++ )
        {
            sio << ((iT<=uBucket[x])?"#":" ");
        }
        sio << sio.nl;
    }
}

int main()
{
    coverage<RandomBasic>();
    coverage<RandomCmwc>();
    coverage<RandomSystem>();

    return 0;
}

