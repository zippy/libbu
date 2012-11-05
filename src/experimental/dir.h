#ifndef BU_DIR_H
#define BU_DIR_H

#include "bu/filesystem.h"
#include "bu/string.h"

namespace Bu
{
    class Dir : public FileSystem
    {
    public:
        Dir();
        Dir( const Bu::String &sPath );
        virtual ~Dir();

    private:
    };
};

#endif
