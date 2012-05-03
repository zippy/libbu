#include "bu/blowfish.h"

template class Bu::CipherModeEcb<8, Bu::Blowfish<1> >;
template class Bu::CipherModeCfb<8, Bu::Blowfish<1> >;
template class Bu::CipherModeCbc<8, Bu::Blowfish<1> >;
template class Bu::CipherModeOfb<8, Bu::Blowfish<1> >;
