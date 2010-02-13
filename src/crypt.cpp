/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/crypt.h"
#include "bu/md5.h"
#include "bu/base64.h"
#include "bu/membuf.h"
#include "bu/file.h"

Bu::FString Bu::cryptPass( const Bu::FString &sPass, const Bu::FString &sSalt )
{
	Bu::Md5 md5;
	Bu::MemBuf mbOut;
	Bu::Base64 b64Out( mbOut );
	
	Bu::FString::const_iterator i = sSalt.find('$');
	Bu::FString sSaltSml = sSalt.getSubStr( sSalt.begin(), i );

	md5.addData( sPass );
	md5.addData( sSaltSml );
	md5.writeResult( b64Out );

	b64Out.stop();

	return sSaltSml + "$" + mbOut.getString();
}

Bu::FString Bu::cryptPass( const Bu::FString &sPass )
{
	Bu::MemBuf mbSalt;
	Bu::Base64 b64Salt( mbSalt );
	Bu::File fRand("/dev/urandom", Bu::File::Read );

#define STR 6
	char buf[STR];
	fRand.read( buf, STR );
	b64Salt.write( buf, STR );

	b64Salt.stop();

	return cryptPass( sPass, mbSalt.getString() );
}

