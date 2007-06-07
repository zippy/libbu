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
	 */
	class TafNode
	{
	public:
		typedef Bu::List<Bu::FString> PropList;
		typedef Bu::Hash<Bu::FString, PropList> PropHash;
		typedef Bu::List<TafNode *> NodeList;
		typedef Bu::Hash<Bu::FString, NodeList> NodeHash;

	public:
		TafNode();
		virtual ~TafNode();

		void setName( const Bu::FString &sName );
		const Bu::FString &getName() const;

		void setProperty( Bu::FString sName, Bu::FString sValue );
		const Bu::FString &getProperty( const Bu::FString &sName ) const;
		const TafNode *getNode( const Bu::FString &sName ) const;
		const PropList &getProperties( const Bu::FString &sName ) const;
		const NodeList &getNodes( const Bu::FString &sName ) const;
		void addChild( TafNode *pNode );

	private:
		Bu::FString sName;
		PropHash hProp;
		NodeHash hChildren;
	};
}

#endif
