#include "bu/settings.h"
#include "bu/sio.h"

using namespace Bu;

int main()
{
	Bu::Settings s("Xagasoft", "Settings");

	sio << s.get("Something", "BAD").get() << sio.nl;
	sio << s.get("general/path", "BAD").get() << sio.nl;
	sio << s.get("general/magic", "BAD").get() << sio.nl;

	s.set("Something", "bob");
	s.set("general/path", "E:\\Place");

}

