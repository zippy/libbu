/*
 * Copyright (C) 2007 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "exceptions.h"
#include <stdarg.h>

namespace Bu
{
	subExceptionDef( XmlException )
	subExceptionDef( TafException )
	subExceptionDef( FileException )
	subExceptionDef( SocketException )
	subExceptionDef( ConnectionException )
	subExceptionDef( PluginException )
	subExceptionDef( UnsupportedException )
}
