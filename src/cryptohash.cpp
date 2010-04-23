/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/cryptohash.h"

Bu::CryptoHash::CryptoHash()
{
}

Bu::CryptoHash::~CryptoHash()
{
}

void Bu::CryptoHash::addData( const Bu::FString &sData )
{
	addData( sData.getStr(), sData.getSize() );
}

Bu::FString Bu::CryptoHash::getHexResult()
{
	Bu::FString sResult = getResult();
	Bu::FString sRet( 2*sResult.getSize() );
	static const char hex_tab[] = {"0123456789abcdef"};

	int k = 0;
	for( int i = 0; i < sResult.getSize(); i++ )
	{
		sRet[k++] = hex_tab[(((unsigned char)sResult[i])>>4) & 0xF];
		sRet[k++] = hex_tab[((unsigned char)sResult[i]) & 0xF];
	}

	return sRet;	
}

