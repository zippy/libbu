#include <iostream>
#include "bu/fstring.h"

int main()
{
	Bu::FString s("Hey there, dude.\n");

	std::cout << s << 5;
}
