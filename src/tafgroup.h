/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_TAF_GROUP_H
#define BU_TAF_GROUP_H

#include <stdint.h>
#include "bu/tafnode.h"
#include "bu/fstring.h"
#include "bu/hash.h"
#include "bu/list.h"

namespace Bu
{
	typedef Bu::List<Bu::FString> StrList;
	class TafProperty;
	class TafComment;
	/**
	 *
	 *@ingroup Taf
	 */
	class TafGroup : public TafNode
	{
	public:
		typedef Bu::List<Bu::FString> PropList;
		typedef Bu::Hash<Bu::FString, PropList> PropHash;
		typedef Bu::List<class Bu::TafGroup *> GroupList;
		typedef Bu::Hash<Bu::FString, GroupList> GroupHash;
		typedef Bu::List<class Bu::TafNode *> NodeList;

		TafGroup( const Bu::FString &sName );
		virtual ~TafGroup();

		const Bu::FString &getName() const;
		void setName( const Bu::FString &sName );

		bool hasProperty( const Bu::FString &sName ) const;
		const Bu::FString &getProperty( const Bu::FString &sName ) const;
		const Bu::FString &getProperty( const Bu::FString &sName,
			const Bu::FString &sDef ) const;
		const PropList &getProperties( const Bu::FString &sName ) const;
		bool hasChild( const Bu::FString &sName ) const;
		const TafGroup *getChild( const Bu::FString &sName ) const;
		const GroupList &getChildren( const Bu::FString &sName ) const;
		TafNode *addChild( TafNode *pNode );
		TafGroup *addChild( TafGroup *pNode );
		TafProperty *addChild( TafProperty *pNode );
		TafComment *addChild( TafComment *pNode );
		TafGroup *addGroup( const Bu::FString &sName );
		TafProperty *addProperty(
			const Bu::FString &sName, const Bu::FString &sValue );
		const NodeList &getChildren() const;
		const TafGroup *getChildByPath( const Bu::FString &sPath ) const;
		const TafGroup *getChildByPath( StrList lPath ) const;
		const Bu::FString &getByPath( const Bu::FString &sPath ) const;
		const Bu::FString &getByPath( StrList lPath ) const;

	private:
		Bu::FString sName;
		PropHash hProp;
		GroupHash hChildren;
		NodeList lChildren;
	};
}

#endif
