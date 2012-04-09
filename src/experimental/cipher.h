#ifndef BU_CIPHER_H
#define BU_CIPHER_H

#include "bu/filter.h"

namespace Bu
{
	class Cipher : Bu::Filter
	{
	public:
		Cipher( Bu::Stream &rNext );
		virtual ~Cipher();

		virtual void start();
		virtual Bu::size stop();

		virtual Bu::size read( void *pBuf, Bu::size iBytes );
		virtual Bu::size write( const void *pBuf, Bu::size iBytes );

		using Bu::Stream::read;
		using Bu::Stream::write;

	protected:
		virtual void encipher( void *pData )=0;
		virtual void decipher( void *pData )=0;
	};
};

#endif
