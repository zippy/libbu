#ifndef BU_PLUGGER_H
#define BU_PLUGGER_H


#include "bu/hash.h"
#include "bu/list.h"
#include <dlfcn.h>
#include "bu/exceptions.h"
#include "bu/fstring.h"

namespace Bu
{
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
		Bu::PluginInfo classname = {						\
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
		Bu::PluginInfo pluginname = {											\
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
		Bu::PluginInfo structname = {											\
			#pluginname, name, ver, rev,									\
			(void *(*)())(create ##classname),								\
			(void (*)( void * ))(destroy ##classname) };					\
	}

	template<class T>
	class Plugger
	{
	public:
		typedef Bu::Hash<Bu::FString, PluginReg *> PluginHash;
		typedef Bu::Hash<int, void *> InstHash;

	public:
		Plugger()
		{
		}

		virtual ~Plugger()
		{
			for( InstHash::iterator i = hObj.begin(); i != hObj.end(); i++ )
			{
				T *pPlug = (T *)i.getKey();
				PluginReg *pReg = (PluginReg *)*i;
				pReg->pInfo->destroyPlugin( pPlug );
			}

			for( PluginHash::iterator i = hPlugin.begin();
				 i != hPlugin.end(); i++ )
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
			hPlugin.insert( pInfo->sID, pReg );
		}

		void registerExternalPlugin( const Bu::FString &sFName,
				const Bu::FString &sPluginName )
		{
			PluginReg *pReg;
			try {
				pReg = (PluginReg *)hPlugin[sPluginName];
				hPlugin.erase( sPluginName );
				dlclose( pReg->dlHandle );
				delete pReg;
				pReg = NULL;
			} catch( Bu::HashException &e )
			{
			}

			pReg = new PluginReg;

			pReg->bBuiltin = false;
			pReg->dlHandle = dlopen( sFName.getStr(), RTLD_NOW );
			if( pReg->dlHandle == NULL )
			{
				throw PluginException( 1, "Error on %s: %s", sFName.getStr(),
						dlerror() );
			}
			pReg->pInfo = (PluginInfo *)dlsym( pReg->dlHandle,
					sPluginName.getStr() );
			if( pReg->pInfo == NULL )
			{
				throw PluginException( 2, "Error on %s: %s", sFName.getStr(),
						dlerror() );
			}
			hPlugin.insert( pReg->pInfo->sID, pReg );
		}

		T *instantiate( const Bu::FString &lpName )
		{
			PluginReg *pReg = (PluginReg *)hPlugin[lpName];
			if( pReg == NULL )
				return NULL;

			T *p = (T *)pReg->pInfo->createPlugin();
			hObj.insert( (int )p, pReg );
			//printf("pReg:  %08X, pPlug: %08X\n", pReg, p );
		
			return p;
		}

		bool hasPlugin( const Bu::FString &lpName )
		{
			return hPlugin.has( lpName );
		}

		void destroy( T *pPlug )
		{
			PluginReg *pReg = (PluginReg *)hObj.get((int)pPlug);
			//printf("pReg:  %08X, pPlug: %08X\n", pReg, pPlug );
			if( pReg == NULL )
				return;

			pReg->pInfo->destroyPlugin( pPlug );

			hObj.erase( (int)pPlug );
		}

		void unloadAll()
		{
			for( PluginHash::iterator i = hPlugin.begin();
				 i != hPlugin.end(); i++ )
			{
				if( (*i)->bBuiltin == false )
				{
					dlclose( (*i)->dlHandle );
				}
				delete (*i);
			}
			hPlugin.clear();
		}

	private:
		PluginHash hPlugin;
		InstHash hObj;
	};
}

#endif
