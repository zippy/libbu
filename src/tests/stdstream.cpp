#include "bu/sio.h"

int main()
{
	Bu::sio << "Hello there" << Bu::sio.nl;

	Bu::sio << "sizeof(Fmt) = " << sizeof(Bu::Fmt) << Bu::sio.nl;

	Bu::sio << -123 << ", " << 0 << ", " << 123 << Bu::sio.nl;

	Bu::sio << "+----------+" << Bu::sio.nl;
	Bu::sio << "|" << Bu::Fmt( 10, 10, Bu::Fmt::Center ) << "Test" << "|" << Bu::sio.nl;
	Bu::sio << "+----------+" << Bu::sio.nl;
	Bu::sio << "|" << Bu::Fmt( 10, 10, Bu::Fmt::Left ) << 123 << "|" << Bu::sio.nl;
	Bu::sio << "|" << Bu::Fmt( 10, 10, Bu::Fmt::Center ) << 123 << "|" << Bu::sio.nl;
	Bu::sio << "|" << Bu::Fmt( 10, 10, Bu::Fmt::Right ) << 123 << "|" << Bu::sio.nl;
	Bu::sio << "+----------+" << Bu::sio.nl;

	return 0;
}

