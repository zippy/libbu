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
	void *(*createPlugin)();
	void (*destroyPlugin)( void * );
} PluginInfo;

typedef struct PluginReg
{
	bool bBuiltin;
	void *dlHandle;
	PluginInfo *pInfo;
} PluginReg;

#define PluginInterface( classname, baseclass, name, ver, rev )			\
extern "C" {															\
	baseclass *create ##classname()								\
	{																	\
		return new classname();											\
	}																	\
	void destroy ##classname( baseclass *pCls )					\
	{																	\
		delete pCls;													\
	}																	\
	PluginInfo classname = {						\
		#classname, name, ver, rev,										\
		create ##classname, destroy ##classname };						\
}

#define PluginInterface2( pluginname, classname, baseclass, name, ver, rev ) \
extern "C" {															\
	baseclass *create ##classname()										\
	{																	\
		return new classname();											\
	}																	\
	void destroy ##classname( baseclass *pCls )							\
	{																	\
		delete pCls;													\
	}																	\
	PluginInfo pluginname = {											\
		#pluginname, name, ver, rev,									\
		(void *(*)())(create ##classname),								\
		(void (*)( void * ))(destroy ##classname) };					\
}

#define PluginInterface3( structname, pluginname, classname, baseclass, name, ver, rev ) \
extern "C" {															\
	baseclass *create ##classname()										\
	{																	\
		return new classname();											\
	}																	\
	void destroy ##classname( baseclass *pCls )							\
	{																	\
		delete pCls;													\
	}																	\
	PluginInfo structname = {											\
		#pluginname, name, ver, rev,									\
		(void *(*)())(create ##classname),								\
		(void (*)( void * ))(destroy ##classname) };					\
}

template<class T>
class Plugger
{
public:

public:
	Plugger() :
		hPlugin( new HashFunctionString(), 11 ),
		hObj( new HashFunctionInt(), 11 )
	{
	}

	virtual ~Plugger()
	{
		void *pos = hObj.getFirstItemPos();
		while( (pos = hObj.getNextItemPos( pos )) )
		{
			T *pPlug = (T *)hObj.getItemID( pos );
			PluginReg *pReg = (PluginReg *)hObj.getItemData( pos );
			pReg->pInfo->destroyPlugin( pPlug );
		}

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

		T *p = (T *)pReg->pInfo->createPlugin();
		hObj.insert( p, pReg );
		//printf("pReg:  %08X, pPlug: %08X\n", pReg, p );
	
		return p;
	}

	bool hasPlugin( const char *lpName )
	{
		if( hPlugin[lpName] == NULL )
			return false;
		return true;
	}

	void destroy( T *pPlug )
	{
		PluginReg *pReg = (PluginReg *)hObj[pPlug];
		//printf("pReg:  %08X, pPlug: %08X\n", pReg, pPlug );
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
