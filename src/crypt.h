#ifndef BU_CRYPT_H
#define BU_CRYPT_H

#include "bu/fstring.h"

namespace Bu
{
	FString cryptPass( const FString &sPass, const FString &sSalt );
	FString cryptPass( const FString &sPass );
};

#endif
