#include "stdio.h"
#include "plugin.h"
#include "plugger.h"

class Guy : public Plugin
{
public:
	Guy()
	{
		printf("I'm guy!\n");
	}

	virtual ~Guy()
	{
		printf("Guy is dead...\n");
	}

private:
};

PluginInterface( Guy, Plugin, "Mike", 0, 1 )

