#ifndef PLUGGER_H
#define PLUGGER_H

#include "hashtable.h"
#include "list"
#include "hashfunctionstring.h"
#include "hashfunctionint.h"
#include "dlfcn.h"

typedef struct PluginInfo
{
	const char *sID;
	const char *sAuthor;
	unsigned short nVersion;
	unsigned short nRevision;
	class Plugin *(*createPlugin)();
	void (*destroyPlugin)( class Plugin * );
} PluginInfo;

#define PluginInterface( classname, name, ver, rev )					\
extern "C" {															\
	class Plugin *create ##classname()									\
	{																	\
		return new classname();											\
	}																	\
	void destroy ##classname( class Plugin *pCls )						\
	{																	\
		delete pCls;													\
	}																	\
	PluginInfo classname = { #classname, name, ver, rev, 				\
		create ##classname, destroy ##classname };						\
}

typedef struct PluginReg
{
	bool bBuiltin;
	void *dlHandle;
	PluginInfo *pInfo;
} PluginReg;

template<class T>
class Plugger
{
public:
	Plugger() :
		hPlugin( new HashFunctionString(), 11 ),
		hObj( new HashFunctionInt(), 11 )
	{
	}

	virtual ~Plugger()
	{
		std::list<PluginReg *>::iterator i;
		for( i = lPlugin.begin(); i != lPlugin.end(); i++ )
		{
			if( (*i)->bBuiltin == false )
			{
				dlclose( (*i)->dlHandle );
			}
			delete (*i);
		}
	}

	void registerBuiltinPlugin( PluginInfo *pInfo )
	{
		PluginReg *pReg = new PluginReg;
		pReg->bBuiltin = true;
		pReg->pInfo = pInfo;
		lPlugin.insert( lPlugin.end(), pReg );
		hPlugin.insert( pInfo->sID, pReg );
	}

	void registerExternalPlugin( const char *sFName, const char *sPluginName )
	{
		PluginReg *pReg = new PluginReg;
		pReg->bBuiltin = false;
		pReg->dlHandle = dlopen( sFName, RTLD_NOW );
		if( pReg->dlHandle == NULL )
		{
			printf("***ERROR:  %s\n\n", dlerror() );
			exit( 105 );
		}
		pReg->pInfo = (PluginInfo *)dlsym( pReg->dlHandle, sPluginName );
		if( pReg->pInfo == NULL )
		{
			printf("***ERROR:  %s\n\n", dlerror() );
			exit( 106 );
		}
		hPlugin.insert( pReg->pInfo->sID, pReg );
		lPlugin.insert( lPlugin.end(), pReg );
	}

	T *instantiate( const char *lpName )
	{
		PluginReg *pReg = (PluginReg *)hPlugin[lpName];
		if( pReg == NULL )
			return NULL;

		T *p = pReg->pInfo->createPlugin();
		hObj.insert( p, pReg );
		printf("pReg:  %08X, pPlug: %08X\n", pReg, p );
	
		return p;
	}

	void destroy( T *pPlug )
	{
		PluginReg *pReg = (PluginReg *)hObj[pPlug];
		printf("pReg:  %08X, pPlug: %08X\n", pReg, pPlug );
		if( pReg == NULL )
			return;

		pReg->pInfo->destroyPlugin( pPlug );

		hObj.del( pPlug );
	}

private:
	std::list<PluginReg *> lPlugin;
	HashTable hPlugin;
	HashTable hObj;
};

#endif
