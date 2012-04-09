#ifndef BU_BLOWFISH_H
#define BU_BLOWFISH_H

#include "bu/cipher.h"

#define NUM_SUBKEYS  18
#define NUM_S_BOXES  4
#define NUM_ENTRIES  256

#define MAX_STRING   256
#define MAX_PASSWD   56  // 448bits

namespace Bu
{
	class Blowfish : public Bu::Cipher<8>
	{
	public:
		Blowfish( Bu::Stream &rNext );
		virtual ~Blowfish();

		void setPassword( const Bu::String &sPass );

	private:
		uint32_t PA[NUM_SUBKEYS];
		uint32_t SB[NUM_S_BOXES][NUM_ENTRIES];

#if __BYTE_ORDER == __BIG_ENDIAN
		struct WordByte
		{
			uint32_t zero:8;
			uint32_t one:8;
			uint32_t two:8;
			uint32_t three:8;
		};
#elif __BYTE_ORDER == __LITTLE_ENDIAN
		struct WordByte
		{
			uint32_t three:8;
			uint32_t two:8;
			uint32_t one:8;
			uint32_t zero:8;
		};
#else
#error No endianness defined
#endif

		union Word
		{
			uint32_t word;
			WordByte byte;
		};

		struct DWord
		{
			Word word0;
			Word word1;
		};

		void reset();
		virtual void encipher( void *pData );
		virtual void decipher( void *pData );
		inline void keyEncipher( Word &w1, Word &w2 );
	};
};

#endif
