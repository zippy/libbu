#include "xmlfilewriter.h"
#include "xmlstringwriter.h"
#include "xmlstringreader.h"

void fillItIn( XmlWriter &w )
{
	w.addNode("thinglist");
	
	w.addNode("thing");
	w.addProperty("type", " ±ÓÓ∏∑¥<M-F6><M-F6>≥∏Æ∞ÍÚÏØ");
	
	w.addNode("id", "Klophin≤≥±π∏∑µ¥‰ÍÌÎ„ Staff", true );
	w.addNode("name", "Klophin Staff", true );
	w.addNode("durability", "0.01", true );
	w.addNode("size", "0.1", true );

	w.addNode("config");
	w.addNode("damage", "3d6+4", true );
	w.addNode("class", "melee", true );
	w.addNode("type", "bludgeon", true );
	w.addNode("damagedesc", "club/clubs", true );
	w.closeNode();

	w.closeNode();

	w.closeNode();
}

int main()
{
	printf("Testing XmlWriter...\n");

	//XmlStringReader *xsr = new XmlStringReader("<stuff/>");

	//printf("%08X\n%08X\n%08X\n", xsr, (XmlReader *)xsr, (XmlDocument *)xsr );

	//delete (XmlDocument *)xsr;
	XmlFileWriter wf("test.xml", "\t");

	fillItIn( wf );

	XmlStringWriter ws("\t");
	fillItIn( ws );

	printf("Now the string version:\n\n%s\n", ws.getString().c_str() );

	return 0;
}
