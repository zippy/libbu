#include "bu/tafreader.h"
#include "bu/file.h"

int main()
{
	Bu::File f("test.taf", "rb");
	Bu::TafReader tr( f );
}

