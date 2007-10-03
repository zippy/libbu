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
	 *@ingroup Taf
	 */
	class TafWriter
	{
	public:
		TafWriter( Bu::Stream &sOut );
		virtual ~TafWriter();

		void writeGroup( const Bu::TafGroup *pRoot );

	private:
		void writeProperty( const Bu::TafProperty *pProp );
		void writeComment( const Bu::TafComment *pComment );
		void writeString( const Bu::FString &str );
		void ident();
		Bu::Stream &sOut;
		int iDepth;
	};
}

#endif
