#include "tafnode.h"

Bu::TafNode::TafNode()
{
}

Bu::TafNode::~TafNode()
{
}

void Bu::TafNode::setProperty( Bu::FString sName, Bu::FString sValue )
{
	if( hProp.has( sName ) )
	{
		hProp.insert( sName, PropList() );
	}

	hProp.get( sName ).append( sValue );
}

const Bu::TafNode::PropList &Bu::TafNode::getProperty( const Bu::FString &sName )
{
	return hProp.get( sName );
}

