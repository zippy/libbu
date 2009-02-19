#include "bu/cryptohash.h"

Bu::CryptoHash::CryptoHash()
{
}

Bu::CryptoHash::~CryptoHash()
{
}

void Bu::CryptoHash::addData( const Bu::FString &sData )
{
	addData( sData.getStr(), sData.getSize() );
}

