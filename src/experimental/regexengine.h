#ifndef BU_REG_EX_ENGINE_H
#define BU_REG_EX_ENGINE_H

#include "bu/sharedcore.h"
#include "bu/array.h"
#include "bu/sio.h"

namespace Bu
{
    template<typename chr> class RegExEngine;

    template<typename chr>
    class RegExEngineCore
    {
    friend class RegExEngine<chr>;
    friend class SharedCore<RegExEngine<chr>, RegExEngineCore<chr> >;
    private:
        RegExEngineCore()
        {
        }

        virtual ~RegExEngineCore()
        {
        }

        class Range
        {
        public:
            Range( chr cLower, chr cUpper, int iTrgState ) :
                cLower( cLower ), cUpper( cUpper ), iTrgState( iTrgState )
            {
            }
        
            chr cLower;
            chr cUpper;
            int iTrgState;
        };

        class State
        {
        public:
            Bu::Array<Range> aRange;
        };

        int addState()
        {
            aState.append( State() );
            return aState.getSize()-1;
        }

        void addCompletion( int iState, chr cLower, chr cUpper, int iTrgState )
        {
            aState[iState].aRange.append( Range( cLower, cUpper, iTrgState ) );
        }

        template<typename str>
        bool match( const str &sIn, int &iSize, int &iCompletion )
        {
            bool bMatch;
            int iState = 0;
            iSize = 0;
            for( typename str::const_iterator i = sIn.begin(); i; i++ )
            {
                Bu::sio << "Finding char " << *i << " in state " << iState
                    << ":" << Bu::sio.nl;
                bMatch = false;
                for( typename Bu::Array<Range>::iterator j =
                     aState[iState].aRange.begin(); j; j++ )
                {
                    Bu::sio << "    Testing range " << (*j).cLower << " - " << (*j).cUpper << Bu::sio.nl;
                    if( *i >= (*j).cLower && *i <= (*j).cUpper )
                    {
                        iState = (*j).iTrgState;
                        bMatch = true;
                        iSize++;
                        if( iState < 0 )
                        {
                            iCompletion = iState;
                            return true;
                        }
                    }
                }
                if( bMatch == false )
                {
                    return false;
                }
            }

            iCompletion = 0;
            return true;
        }

        typedef Bu::Array<State> StateArray;
        StateArray aState;
    };

    template<typename chr>
    class RegExEngine : public SharedCore<RegExEngine<chr>,
                        RegExEngineCore<chr> >
    {
    private:
        typedef class RegExEngine<chr> MyType;
        typedef class RegExEngineCore<chr> Core;
        typedef class Core::Range Range;
        typedef class Core::State State;
    
    protected:
        using SharedCore<MyType, Core>::core;
        using SharedCore<MyType, Core>::_hardCopy;
        using SharedCore<MyType, Core>::_resetCore;
        using SharedCore<MyType, Core>::_allocateCore;

    public:
        RegExEngine()
        {
        }

        virtual ~RegExEngine()
        {
        }

        int addState()
        {
            return core->addState();
        }

        void addCompletion( int iState, chr cLower, chr cUpper, int iTrgState )
        {
            core->addCompletion( iState, cLower, cUpper, iTrgState );
        }

        template<typename str>
        bool match( const str &sIn, int &iSize, int &iCompletion )
        {
            return core->match( sIn, iSize, iCompletion );
        }

    private:
    };
};

#endif
