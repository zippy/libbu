#include "xmlfilewriter.h"
#include "xmlstringwriter.h"

void fillItIn( XmlWriter &w )
{
	w.addNode("thinglist");
	
	w.addNode("thing");
	w.addProperty("type", "Weapon");
	
	w.addNode("id", "Klophin Staff", true );
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
	//XmlFileWriter wf("test.xml", "\t");

	//fillItIn( wf );

	//XmlStringWriter ws("\t");
	//fillItIn( ws );

	//printf("Now the string version:\n\n%s\n", ws.getString().c_str() );

	return 0;
}
