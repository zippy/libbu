/*
 * Copyright (C) 2007 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_MINI_MACRO_H
#define BU_MINI_MACRO_H

namespace Bu
{
	/**
	 * A processor for Libbu++ brand Mini Macros.  These are really simple, but
	 * still fairly flexible.  It's mainly text replacement, but with a few
	 * extras like filter functions and conditional text segments.  So far we
	 * don't support loops or anything, I'm not sure we ever will.
	 *
	 * Anatomy of a mini macro:
	 * - Every macro begins with a two character code, the first character is
	 *   always '{', the second character determines the operation to perform.
	 * - If the '{' is followed by a character that is not valid it is not
	 *   considered for expansion and the characters are copied to the output.
	 * - Every macro ends with a closing '}'
	 * - Macro types:
	 *   - '=': variable replacement.  The '=' is immediatley followed by the
	 *     name of the variable to replace, then any number of optional filter
	 *     segments.
	 *   - '?': conditional text.  The '?' is immediately followed by the
	 *     variable to test.  This works two ways, the variable can be alone, in
	 *     which case it's existance is tested, or it can be followed by a "="
	 *     and a string to compare to.  This is then followed by a text segment
	 *     that will be used if the test is true, and an optional text segment
	 *     to be used if the test is false.
	 * - Segments:
	 *   - Each segment is seperated by a colon.
	 *   - Filter segments give the name of the filter, followed by
	 *     parenthesies.  Parameters may be provided within the parenthesies.
	 *   - Text segments should always be quoted, but may contain any characters
	 *     within the quotes, backslash is used as per C/ANSI/ISO standard.
	 *     You can also quote any text using [" "] instead of quotes, which
	 *     allows for nested strings.  The [" token is only recognised within
	 *     a macro.
	 *
	 *@verbatim
	 {=name:tolower()}
	 {=name:ccsplit("_"):toupper()}
	 {?name:"name exists and is {=name}"}
	 {?name:"{=name}":"no name!"}
	 {?name="bob":"You're named bob!":"Who are you?  I only know bob..."}
	 @endverbatim
	 */
	class MiniMacro
	{
	public:
		MiniMacro();
		virtual ~MiniMacro();

	private:
	};
};

#endif
