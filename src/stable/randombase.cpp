/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */
#include "bu/randombase.h"

Bu::RandomBase::RandomBase()
{
}

Bu::RandomBase::~RandomBase()
{
}

int32_t Bu::RandomBase::rand( int32_t iMax )
{
	return rand( 0, iMax );
}

int32_t Bu::RandomBase::rand( int32_t iMin, int32_t iMax )
{
	return iMin+(randNorm()*(iMax-iMin));
}

double Bu::RandomBase::randNorm()
{
	return (((uint32_t)rand())&0xfffffffeul)/(double)(0xfffffffful);
}

