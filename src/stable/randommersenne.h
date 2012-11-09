#ifndef BU_RANDOM_MERSENNE_H
#define BU_RANDOM_MERSENNE_H

#include "bu/randombase.h"

namespace Bu
{
    /**
     * An implementation of Mersenne Twister (MT19937) algorithm.  This as an
     * algorithm with an excellent reputation for very good random number
     * generation and a very large period.  It is however, relatively slow and
     * complex compared to, say the Complementary Multiply With Carry
     * (Bu::RandomCmwc), and may not actually be a *better* random number
     * generator.
     */
    class RandomMersenne : public Bu::RandomBase
    {
    public:
        RandomMersenne( int32_t iSeed=0 );
        virtual ~RandomMersenne();

        virtual void seed( int32_t iSeed );
        virtual int32_t rand();

    private:
        int32_t iIndex;
        uint32_t iMt[624];
    };
};

#endif
