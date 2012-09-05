#include <bu/sio.h>

int main()
{
	Bu::print("hello there %1!\n").arg("Bob");

	Bu::println("This is %1 crazy, like %2 times over!").
		arg("totally").arg( 47.2 );
	Bu::println("This is unsubstituted?");

	return 0;
}

