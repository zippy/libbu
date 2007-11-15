/*
 * Copyright (C) 2007 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_TAF_NODE_H
#define BU_TAF_NODE_H

#include <stdint.h>
#include "bu/fstring.h"
#include "bu/hash.h"
#include "bu/list.h"

namespace Bu
{
	/**
	 *
	 *@ingroup Taf
	 */
	class TafNode
	{
	public:
		enum NodeType
		{
			typeGroup,
			typeProperty,
			typeComment
		};

	public:
		TafNode( NodeType eType );
		virtual ~TafNode();

		const NodeType getType() const;

	private:
		NodeType eType;
	};

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

		const Bu::FString &getProperty( const Bu::FString &sName ) const;
		const PropList &getProperties( const Bu::FString &sName ) const;
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

	private:
		Bu::FString sName;
		PropHash hProp;
		GroupHash hChildren;
		NodeList lChildren;
	};

	/**
	 *
	 *@ingroup Taf
	 */
	class TafProperty : public TafNode
	{
	public:
		TafProperty( const Bu::FString &sName, const Bu::FString &sValue );
		virtual ~TafProperty();

		const Bu::FString &getName() const;
		const Bu::FString &getValue() const;

	private:
		Bu::FString sName;
		Bu::FString sValue;
	};

	/**
	 *
	 *@ingroup Taf
	 */
	class TafComment : public TafNode
	{
	public:
		TafComment( const Bu::FString &sText, bool bEOL=false );
		virtual ~TafComment();

		const Bu::FString &getText() const;
		bool isEOLStyle() const;

	private:
		Bu::FString sText;
		bool bEOL;
	};
}

#endif
