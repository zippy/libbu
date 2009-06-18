#ifndef BU_BASE64_H
#define BU_BASE64_H

#include "bu/filter.h"

namespace Bu
{
	/**
	 *
	 *@ingroup Streams
	 */
	class Base64 : public Bu::Filter
	{
	public:
		enum Mode
		{
			Encode		= 0x01,
			Read		= 0x01,
			Decode		= 0x02,
			Write		= 0x02
		};

	public:
		Base64( Bu::Stream &rNext, Mode eMode );
		virtual ~Base64();

		virtual void start();
		virtual size_t stop();
		virtual size_t read( void *pBuf, size_t nBytes );
		virtual size_t write( const void *pBuf, size_t nBytes );

		virtual bool isOpen();

	private:
		int iBPos;
		int iBuf;
		size_t iTotalIn;
		size_t iTotalOut;
		static const char tblEnc[65];
		char tblDec[80];
		Mode eMode;
	};
};

#endif
