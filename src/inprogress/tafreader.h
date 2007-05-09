#ifndef BU_TAF_READER_H
#define BU_TAF_READER_H

#include <stdint.h>
#include "bu/tafdocument.h"
#include "bu/stream.h"

namespace Bu
{
	/**
	 *
	 */
	class TafReader : public Bu::TafDocument
	{
	public:
		TafReader( Bu::Stream &sIn );
		virtual ~TafReader();

	private:
		Stream &sIn;

	};
}

#endif
