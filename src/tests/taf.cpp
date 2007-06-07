#include "bu/tafreader.h"
#include "bu/file.h"

int main()
{
	Bu::File f("test.taf", "rb");
	Bu::TafReader tr( f );

	Bu::TafNode *pNode = tr.getNode();

	const Bu::TafNode::NodeList &l = pNode->getNode("stats");
	for( Bu::TafNode::NodeList::const_iterator i = l.begin();
		 i != l.end(); i++ )
	{
		printf("%s\n", (*i)->getName().getStr() );
	}

	delete pNode;
}

