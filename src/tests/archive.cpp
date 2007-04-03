#include "archive.h"
#include "sfile.h"

using namespace Bu;

int main()
{
	SFile f("test.dat", "wb");
	Archive ar( f, Archive::save );

	std::string s("Hello there");
	ar << s;

	return 0;
}

