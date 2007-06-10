#include "bu/tafreader.h"
#include "bu/file.h"

int main()
{
	Bu::File f("test.taf", "rb");
	Bu::TafReader tr( f );

	Bu::TafNode *pNode = tr.getNode();

	const Bu::TafNode *pStats = pNode->getChild("stats");
	printf("%s\n", pStats->getName().getStr() );
	printf("  str = %s\n", pStats->getProperty("str").getStr() );

	delete pNode;
}

