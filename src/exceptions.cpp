#include "exceptions.h"
#include <stdarg.h>

namespace Bu
{
	subExceptionDef( XmlException )
	subExceptionDef( FileException )
	subExceptionDef( SocketException )
	subExceptionDef( ConnectionException )
	subExceptionDef( PluginException )
	subExceptionDef( UnsupportedException )
}
