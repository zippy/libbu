#ifndef BU_TAF_WRITER_H
#define BU_TAF_WRITER_H

#include <stdint.h>
#include "bu/tafnode.h"
#include "bu/stream.h"
#include "bu/fstring.h"

namespace Bu
{
	/**
	 *
	 */
	class TafWriter
	{
	public:
		TafWriter( Bu::Stream &sOut );
		virtual ~TafWriter();

		void writeNode( Bu::TafNode *pRoot );

	private:
		void writeString( const Bu::FString &str );
		Bu::Stream &sOut;
	};
}

#endif
