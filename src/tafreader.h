#ifndef BU_TAF_READER_H
#define BU_TAF_READER_H

#include <stdint.h>
#include "bu/tafnode.h"
#include "bu/stream.h"
#include "bu/fstring.h"

namespace Bu
{
	/**
	 *
	 */
	class TafReader
	{
	public:
		TafReader( Bu::Stream &sIn );
		virtual ~TafReader();

		Bu::TafNode *readNode();

	private:
		void node();
		void nodeContent();
		void nodeProperty();
		void ws();
		bool isws();
		void next();
		Bu::FString readStr();
		char c;
		Bu::Stream &sIn;
	};
}

#endif
