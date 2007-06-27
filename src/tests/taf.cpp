#include "bu/tafreader.h"
#include "bu/tafwriter.h"
#include "bu/file.h"

int main()
{
	Bu::File f("test.taf", "rb");
	Bu::TafReader tr( f );

	Bu::TafGroup *pGroup = tr.readGroup();

	const Bu::TafGroup *pStats = pGroup->getChild("stats");
	printf("%s\n", pStats->getName().getStr() );
	printf("  str = %s\n", pStats->getProperty("str").getStr() );

	{
		Bu::File fo("out.taf", "wb");
		Bu::TafWriter tw( fo );
		tw.writeGroup( pGroup );
	}

	delete pGroup;
}

