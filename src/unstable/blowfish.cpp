/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/blowfish.h"

template class Bu::CipherModeEcb<8, Bu::Blowfish<1> >;
template class Bu::CipherModeCfb<8, Bu::Blowfish<1> >;
template class Bu::CipherModeCbc<8, Bu::Blowfish<1> >;
template class Bu::CipherModeOfb<8, Bu::Blowfish<1> >;
