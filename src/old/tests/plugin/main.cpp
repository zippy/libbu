#include "plugger.h"
#include "plugin.h"

int main()
{
	Plugger<Plugin> p;

	p.registerExternalPlugin( "./guy.so", "Guy" );

	Plugin *t = p.instantiate( "Guy" );

	p.destroy( t );
}

