/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_PLUGGER_H
#define BU_PLUGGER_H

#include "bu/hash.h"
#include "bu/list.h"
#include "bu/exceptionbase.h"
#include "bu/string.h"
#include <stddef.h>

#include "bu/config.h"

#ifdef WIN32
# include <windows.h>
#else
# include <dlfcn.h>
#endif

namespace Bu
{
	subExceptionDecl( PluginException );

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
#ifdef WIN32
		HMODULE dlHandle;
#else
		void *dlHandle;
#endif
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

//
// This is probably the main interface to use, I'll describe it some here...
//   structname - The name of the structure, this is what you have to pass to
//                register.  Depending on how you build your dll/so files this
//                will need to be unique (generally not)
//   pluginname - This is what will be used by the plugin system to refer to
//                your plugin once it's loaded.  This should be unique, but not
//                a string
//   classname  - The name of the class that is the plugin
//   baseclass  - The name of the base class that is the parent of the plugin
//   name       - The name of the author of this class (or company)
//   ver        - an integer version number for the plugin
//   rev        - an integer revision number for the plugin
//
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

	/**
	 * A complete dynamic plugin manager system.  This will allow you to design
	 * and use plugins that are compiled into your program and dynamically
	 * linked to your program interchangably.  It works on windows and on *nix
	 * and bsd type systems (anything that supports dlopen).  Basically you
	 * create a base class that will be the basic interface of your plugins.
	 * Then you create some classes that inherit from it, and use the
	 * PluginInterface3 macro to create the required data structures for it.
	 *
	 * Once you have plugins you can create a Plugger, by passing in the base
	 * class as it's template parameter.  Once it's created, you can register
	 * plugins.  To register a plugin that is builtin, you just need to pass
	 * a pointer to it's interface structure to the registerBuiltinPlugin
	 * function.  To register a plugin that is in a shared object or dll file
	 * you just pass the filename (with path, probably), and the name of the
	 * structure to load and you're all set.
	 *
	 * To instantiate an object from a plugin simply call instantiate with the
	 * name of the plugin as specified in the interface macro.  To destroy an
	 * object crated with the plugger do not delete it, instead pass it into
	 * Plugger's destroy function.
	 *
	 * Any objects not destroyed when the plugger is deleted will be destroyed
	 * automatically.
	 *
	 * It is important to note that some systems (linux at least) partition off
	 * the memory allocated by objects linked in at run time into a seperate
	 * segment that, while it can be accessed by the main program, cannot be
	 * safely or reliably freed by the main program.  With that in mind it is
	 * a good idea to free all memory allocated by a plugin object in the plugin
	 * object and not allow the calling program to delete it.
	 */
	template<class T>
	class Plugger
	{
	public:
		typedef Bu::Hash<Bu::String, PluginReg *> PluginHash;
		typedef Bu::Hash<ptrdiff_t, void *> InstHash;

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
#ifdef WIN32
					FreeLibrary( (*i)->dlHandle );
#else
					dlclose( (*i)->dlHandle );
#endif
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

		void registerExternalPlugin( const Bu::String &sFName,
				const Bu::String &sPluginName )
		{
			PluginReg *pReg;
			if( hPlugin.has( sPluginName ) )
				throw Bu::ExceptionBase("A plugin with name '%s' is already "
					"loaded.", sPluginName.getStr() );

			pReg = new PluginReg;

			pReg->bBuiltin = false;
#ifdef WIN32
			pReg->dlHandle = LoadLibrary( sFName.getStr() );
			if( pReg->dlHandle == NULL )
			{
				throw PluginException( 1, "Error opening %s: %s",
					sFName.getStr(), Bu::getLastWinError().getStr() );
			}
			pReg->pInfo = (PluginInfo *)GetProcAddress( pReg->dlHandle,
					sPluginName.getStr() );
			if( pReg->pInfo == NULL )
			{
				throw PluginException( 2, "Error mapping %s: %s",
					sFName.getStr(), Bu::getLastWinError().getStr() );
			}
#else
			pReg->dlHandle = dlopen( sFName.getStr(), RTLD_NOW );
			if( pReg->dlHandle == NULL )
			{
				throw PluginException( 1, "Error opening %s: %s",
					sFName.getStr(), dlerror() );
			}
			pReg->pInfo = (PluginInfo *)dlsym( pReg->dlHandle,
					sPluginName.getStr() );
			if( pReg->pInfo == NULL )
			{
				throw PluginException( 2, "Error mapping %s: %s",
					sFName.getStr(), dlerror() );
			}
#endif
			hPlugin.insert( pReg->pInfo->sID, pReg );
		}

		T *instantiate( const Bu::String &lpName )
		{
			PluginReg *pReg = (PluginReg *)hPlugin[lpName];
			if( pReg == NULL )
				return NULL;

			T *p = (T *)pReg->pInfo->createPlugin();
			hObj.insert( (ptrdiff_t)p, pReg );
			//printf("pReg:  %08X, pPlug: %08X\n", pReg, p );
		
			return p;
		}

		bool hasPlugin( const Bu::String &lpName )
		{
			return hPlugin.has( lpName );
		}

		void destroy( T *pPlug )
		{
			PluginReg *pReg = (PluginReg *)hObj.get((ptrdiff_t)pPlug);
			//printf("pReg:  %08X, pPlug: %08X\n", pReg, pPlug );
			if( pReg == NULL )
				return;

			pReg->pInfo->destroyPlugin( pPlug );

			hObj.erase( (ptrdiff_t)pPlug );
		}

		void unloadAll()
		{
			for( PluginHash::iterator i = hPlugin.begin();
				 i != hPlugin.end(); i++ )
			{
				if( (*i)->bBuiltin == false )
				{
#ifdef WIN32
					FreeLibrary( (*i)->dlHandle );
#else
					dlclose( (*i)->dlHandle );
#endif
				}
				delete (*i);
			}
			hPlugin.clear();
		}

		Bu::List<Bu::String> getPluginList()
		{
			return hPlugin.getKeys();
		}

	private:
		PluginHash hPlugin;
		InstHash hObj;
	};
}

#endif
