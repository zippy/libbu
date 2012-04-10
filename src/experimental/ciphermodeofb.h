#ifndef BU_MODE_OFB_H
#define BU_MODE_OFB_H

#include "bu/filter.h"
#include "bu/string.h"

namespace Bu
{
	template<int iBlockSize, typename CipherType>
	class CipherModeOfb : public CipherType
	{
	public:
		CipherModeOfb(class Stream &rNext ) :
			CipherType( rNext ),
			bStart( true )
		{
			memset( aVector, 0, iBlockSize );
		}

		virtual ~CipherModeOfb()
		{
		}

		void setIv( const Bu::String &sIv )
		{
			memcpy( aVector, sIv.getStr(), iBlockSize );
		}

	protected:
		void decipher( void *pBuf )
		{
			CipherType::encipher( aVector );
			uint8_t aTmp[iBlockSize];
			memcpy( aTmp, aVector, iBlockSize );
			for( int j = 0; j < iBlockSize; j++ )
				((uint8_t *)pBuf)[j] ^= aVector[j];
			memcpy( aVector, aTmp, iBlockSize );
		}

		void encipher( void *pBuf )
		{
			CipherType::encipher( aVector );
			uint8_t aTmp[iBlockSize];
			memcpy( aTmp, aVector, iBlockSize );
			for( int j = 0; j < iBlockSize; j++ )
				((uint8_t *)pBuf)[j] ^= aVector[j];
			memcpy( aVector, aTmp, iBlockSize );
		}

	private:
		bool bStart;
		uint8_t aVector[iBlockSize];
	};
};

#endif
