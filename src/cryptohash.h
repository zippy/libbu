#ifndef BU_CRYPTO_HASH_H
#define BU_CRYPTO_HASH_H

#include "bu/fstring.h"

namespace Bu
{
	class CryptoHash
	{
	public:
		CryptoHash();
		virtual ~CryptoHash();

		virtual void reset() = 0;
		virtual void setSalt( const Bu::FString &sSalt ) = 0;
		virtual void addData( const void *sData, int iSize ) = 0;
		virtual void addData( const Bu::FString &sData );
		virtual FString getResult() = 0;
	};
};

#endif
