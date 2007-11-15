/*
 * Copyright (C) 2007 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/formula.h"

int main( int argc, char *argv[] )
{
	Bu::Formula<uint32_t> uForm;
	Bu::Formula<double> dForm;

	printf("u: %s = %u\n", argv[1], uForm.run( argv[1] ) );
	printf("d: %s = %f\n", argv[1], dForm.run( argv[1] ) );
}

