#ifndef BU_CIPHER_H
#define BU_CIPHER_H

#include "bu/filter.h"

namespace Bu
{
	template<int iBlockSize>
	class Cipher : Bu::Filter
	{
	public:
		Cipher( Bu::Stream &rNext ) :
			Bu::Filter( rNext )
		{
		}

		virtual ~Cipher()
		{
		}

		virtual void start()
		{
		}

		virtual Bu::size stop()
		{
			return 0;
		}

		virtual Bu::size read( void *pBuf, Bu::size iBytes )
		{
			uint32_t i;

			if (iBytes%iBlockSize)
			{
				return 0;
			}

			iBytes /= iBlockSize;

			for (i=0;i<iBytes;i++)
			{
				void *pSeg = ((char *)pBuf)+(i*iBlockSize);
				int iRead = rNext.read( pSeg, iBlockSize );
				decipher( pSeg );
			}

			return iBytes*iBlockSize;
		}

		virtual Bu::size write( const void *pBuf, Bu::size iBytes )
		{
			uint32_t i;

			if (iBytes%iBlockSize)
			{
				return 0;
			}

			iBytes /= iBlockSize;

			char buf[iBlockSize];

			for (i=0;i<iBytes;i++)
			{
				memcpy( buf, ((const char *)pBuf)+(i*iBlockSize), 8 );
				encipher( buf );
				rNext.write( buf, iBlockSize );
			}

			memset( &buf, 0, iBlockSize );
			return iBytes*iBlockSize;
		}

		using Bu::Stream::read;
		using Bu::Stream::write;

	protected:
		virtual void encipher( void *pData )=0;
		virtual void decipher( void *pData )=0;
	};
};

#endif
