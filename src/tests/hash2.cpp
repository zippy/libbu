/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include <bu/string.h>
#include <bu/hash.h>

int main()
{
	Bu::Hash<Bu::String, int> hCmd;

	hCmd.insert("help", 5 );
	hCmd.insert("exit", 5 );
	hCmd.insert("getkey", 5 );
	hCmd.insert("setcrypt", 5 );
	hCmd.insert("user", 5 );
	hCmd.insert("pass", 5 );
	hCmd.erase("getkey");
	hCmd.erase("setcrypt");
	hCmd.insert("user", 5 );
	hCmd.insert("pass", 5 );
	hCmd.erase("pass");
	hCmd.erase("user");
	hCmd.insert("acl", 5 );
	hCmd.insert("owner", 5 );
	hCmd.insert("operator", 5 );
	hCmd.insert("admin", 5 );
	hCmd.insert("monitor", 5 );
	hCmd.insert("shutdown", 5 );

	return 0;
}
