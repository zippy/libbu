#include "bu/xmlreader.h"
#include "bu/xmlnode.h"
#include "bu/xmldocument.h"
#include "bu/file.h"

int main()
{
	Bu::File f("test.xml", "r");
	Bu::XmlReader xr( f );

	xr.read();

	return 0;
}

