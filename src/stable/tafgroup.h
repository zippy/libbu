/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_TAF_GROUP_H
#define BU_TAF_GROUP_H

#include <stdint.h>
#include "bu/tafnode.h"
#include "bu/string.h"
#include "bu/hash.h"
#include "bu/list.h"

namespace Bu
{
	typedef Bu::List<Bu::String> StrList;
	class TafProperty;
	class TafComment;
	/**
	 *
	 *@ingroup Taf
	 */
	class TafGroup : public TafNode
	{
	public:
		typedef Bu::List<Bu::String> PropList;
		typedef Bu::Hash<Bu::String, PropList> PropHash;
		typedef Bu::List<class Bu::TafGroup *> GroupList;
		typedef Bu::Hash<Bu::String, GroupList> GroupHash;
		typedef Bu::List<class Bu::TafNode *> NodeList;

		TafGroup( const TafGroup &rSrc );
		TafGroup( const Bu::String &sName );
		virtual ~TafGroup();

		const Bu::String &getName() const;
		void setName( const Bu::String &sName );

		bool hasProperty( const Bu::String &sName ) const;
		const Bu::String &getProperty( const Bu::String &sName ) const;
		const Bu::String &getProperty( const Bu::String &sName,
			const Bu::String &sDef ) const;
		const PropList &getProperties( const Bu::String &sName ) const;
		bool hasChild( const Bu::String &sName ) const;
		const TafGroup *getChild( const Bu::String &sName ) const;
		const GroupList &getChildren( const Bu::String &sName ) const;
		TafNode *addChild( TafNode *pNode );
		TafGroup *addChild( TafGroup *pNode );
		TafProperty *addChild( TafProperty *pNode );
		TafComment *addChild( TafComment *pNode );
		TafGroup *addGroup( const Bu::String &sName );
		TafProperty *addProperty(
			const Bu::String &sName, const Bu::String &sValue );
		const NodeList &getChildren() const;
		const TafGroup *getChildByPath( const Bu::String &sPath ) const;
		const TafGroup *getChildByPath( StrList lPath ) const;
		const Bu::String &getByPath( const Bu::String &sPath ) const;
		const Bu::String &getByPath( StrList lPath ) const;

	private:
		Bu::String sName;
		PropHash hProp;
		GroupHash hChildren;
		NodeList lChildren;
	};
}

#endif
