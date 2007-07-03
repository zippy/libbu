#include "xmlwriter.h"

int main()
{
	printf("Testing Xml Replacement...\n");
	XmlDocument w;

	w.addNode("text");
	w.setContent("this text is before the node.  ");
	w.addNode("keepme", "This one we keep...", true );
	w.setContent("this text is after.");
	w.addNode("deleteme", "This one we don't...", true );
	w.setContent("this is last..." );
	w.closeNode();

	//XmlWriter::writeNode( stdout, w.getRoot(), 0, NULL );

	printf("\n\n");

	//XmlNode *xNode = w.getRoot()->detatchNode( 1 );

	//XmlWriter::writeNode( stdout, w.getRoot(), 0, NULL );

	printf("\n\n");

	//XmlWriter::writeNode( stdout, xNode, 0, NULL );

	printf("\n\n");

	return 0;
}
