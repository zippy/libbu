#ifndef XML_READER_H
#define XML_READER_H

#include <stdint.h>
#include "bu/stream.h"
#include "bu/fstring.h"
#include "bu/xmlnode.h"

namespace Bu
{
	/**
	 *
	 */
	class XmlReader
	{
	public:
		XmlReader( Bu::Stream &sIn );
		virtual ~XmlReader();

		XmlNode *read();

	private:
		Bu::Stream &sIn;
		Bu::FString sBuf;

	private: // Helpers
		const char *lookahead( int nAmnt );
		void burn( int nAmnt );
		void checkString( const char *str, int nLen );

	private: // States
		/**
		 * The headers, etc.
		 */
		void prolog();

		/**
		 * The xml decleration (version, encoding, etc).
		 */
		void XMLDecl();

		/**
		 * Misc things...?
		 */
		void Misc();

		/**
		 * Whitespace eater.
		 */
		void S();

		/**
		 * Optional whitespace eater.
		 */
		void Sq();

		/**
		 * XML Version spec
		 */
		void VersionInfo();

		/**
		 * Your basic equals sign with surrounding whitespace.
		 */
		void Eq();

	};
}

#endif
