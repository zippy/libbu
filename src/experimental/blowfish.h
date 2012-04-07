#ifndef BU_BLOWFISH_H
#define BU_BLOWFISH_H

#include "bu/filter.h"

#define NUM_SUBKEYS  18
#define NUM_S_BOXES  4
#define NUM_ENTRIES  256

#define MAX_STRING   256
#define MAX_PASSWD   56  // 448bits

namespace Bu
{
	class Blowfish : public Bu::Filter
	{
	public:
		Blowfish( Bu::Stream &rNext );
		virtual ~Blowfish();

		void setPassword( const Bu::String &sPass );

		virtual void start();
		virtual Bu::size stop();

		virtual Bu::size read( void *pBuf, Bu::size iBytes );
		virtual Bu::size write( const void *pBuf, Bu::size iBytes );
		using Bu::Stream::read;
		using Bu::Stream::write;

	private:
		unsigned int PA[NUM_SUBKEYS];
		unsigned int SB[NUM_S_BOXES][NUM_ENTRIES];

#if __BYTE_ORDER == __BIG_ENDIAN
		struct WordByte
		{
			unsigned int zero:8;
			unsigned int one:8;
			unsigned int two:8;
			unsigned int three:8;
		};
#elif __BYTE_ORDER == __LITTLE_ENDIAN
		struct WordByte
		{
			unsigned int three:8;
			unsigned int two:8;
			unsigned int one:8;
			unsigned int zero:8;
		};
#else
#error No endianness defined
#endif

		union Word
		{
			unsigned int word;
			WordByte byte;
		};

		struct DWord
		{
			Word word0;
			Word word1;
		};

		void reset();
		inline void BF_En( Word *, Word * );
		inline void BF_De( Word *, Word * );
	};
};

#endif
