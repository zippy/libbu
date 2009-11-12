#include <bu/uuid.h>
#include <bu/sio.h>

using namespace Bu;

int main()
{
	Uuid i = Uuid::gen();

	sio << i.toString() << sio.nl;

	return 0;
}

