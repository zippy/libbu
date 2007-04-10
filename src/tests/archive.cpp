#include "archive.h"
#include "file.h"

using namespace Bu;

int main()
{
	File f("test.dat", "wb");
	Archive ar( f, Archive::save );

	std::string s("Hello there");
	ar << s;

	return 0;
}

