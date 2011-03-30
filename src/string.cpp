/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#define BU_TRACE
#include "bu/trace.h"

#include "bu/string.h"
#include "bu/hash.h"
#include "bu/membuf.h"
#include "bu/formatter.h"
#include <stdlib.h>

#define nMinSize	(256)

Bu::StringCore::StringCore() :
	nLength( 0 ),
	pFirst( NULL ),
	pLast( NULL )
{
}

Bu::StringCore::StringCore( const StringCore &rSrc ) :
	nLength( rSrc.nLength ),
	pFirst( NULL ),
	pLast( NULL )
{
	if( rSrc.pFirst == NULL || rSrc.nLength == 0 )
	{
		pFirst = pLast = NULL;
	}
	else
	{
		pFirst = pLast = newChunk( nLength );
		Chunk *pLink = rSrc.pFirst;
		int iPos = 0;
		while( pLink != NULL )
		{
			memcpy( pFirst->pData+iPos, pLink->pData, pLink->nLength );
			iPos += pLink->nLength;
			pLink = pLink->pNext;
		}
	}
}

Bu::StringCore::~StringCore()
{
	clear();
}

void Bu::StringCore::clear() const
{
	if( pFirst == NULL )
		return;

	Chunk *i = pFirst;
	for(;;)
	{
		Chunk *n = i->pNext;
		delete[] i->pData;
		delete i;
		if( n == NULL )
			break;
		i = n;
	}
	pFirst = pLast = NULL;
	nLength = 0;
}	

Bu::StringCore::Chunk *Bu::StringCore::newChunk() const
{
	Chunk *pNew = new Chunk;
	pNew->pNext = NULL;
	return pNew;
}

Bu::StringCore::Chunk *Bu::StringCore::newChunk( long nLen ) const
{
	Chunk *pNew = new Chunk;
	pNew->pNext = NULL;
	pNew->nLength = nLen;
	pNew->pData = new char[(nLen<nMinSize)?(nMinSize):(nLen)+1];
	pNew->pData[nLen] = (char)0;
	return pNew;
}

Bu::StringCore::Chunk *Bu::StringCore::copyChunk(
		Bu::StringCore::Chunk *pSrc ) const
{
	Chunk *pNew = new Chunk;
	pNew->pNext = pSrc->pNext;
	pNew->nLength = pSrc->nLength;
	pNew->pData = new char[
		(pNew->nLength<nMinSize)?(nMinSize):(pNew->nLength)+1
		];
	memcpy( pNew->pData, pSrc->pData, pSrc->nLength );
	pNew->pData[pNew->nLength] = (char)0;
	return pNew;
}

void Bu::StringCore::appendChunk( Bu::StringCore::Chunk *pNewChunk )
{
	if( pFirst == NULL )
		pLast = pFirst = pNewChunk;
	else
	{
		pLast->pNext = pNewChunk;
		pLast = pNewChunk;
	}

	nLength += pNewChunk->nLength;
}

void Bu::StringCore::prependChunk( Bu::StringCore::Chunk *pNewChunk )
{
	if( pFirst == NULL )
		pLast = pFirst = pNewChunk;
	else
	{
		pNewChunk->pNext = pFirst;
		pFirst = pNewChunk;
	}

	nLength += pNewChunk->nLength;
}

Bu::String::String()
{
}

Bu::String::String( const char *pData )
{
	append( pData );
}

Bu::String::String( const char *pData, long nLength )
{
	append( pData, nLength );
}

Bu::String::String( const Bu::String::String &rSrc ) :
	Bu::SharedCore<Bu::String, Bu::StringCore>( rSrc )
{
}

Bu::String::String( const Bu::String &rSrc, long nLength )
{
	append( rSrc, nLength );
}

Bu::String::String( const Bu::String &rSrc, long nStart, long nLength )
{
	append( rSrc, nStart, nLength );
}

Bu::String::String( long nSize )
{
	core->pFirst = core->pLast = core->newChunk( nSize );
	core->nLength = nSize;
}

Bu::String::String( const Bu::String::const_iterator &s )
{
	append( s );
}

Bu::String::String( const Bu::String::const_iterator &s,
		const Bu::String::const_iterator &e )
{
	append( s, e );
}

Bu::String::~String()
{
}

void Bu::String::append( const char *pData )
{
	if( !pData ) return;
	long nLen;
	for( nLen = 0; pData[nLen] != (char)0; nLen++ ) { }

	append( pData, 0, nLen );
}

void Bu::String::append( const char *pData, long nLen )
{
	append( pData, 0, nLen );
}

void Bu::String::append( const char *pData, long nStart, long nLen )
{
	if( !pData ) return;
	if( nLen <= 0 )
		return;

	pData += nStart;

	_hardCopy();

	if( core->pLast && core->pLast->nLength < nMinSize )
	{
		int nAmnt = nMinSize - core->pLast->nLength;
		if( nAmnt > nLen )
			nAmnt = nLen;
		memcpy(
			core->pLast->pData+core->pLast->nLength,
			pData,
			nAmnt
		   );
		pData += nAmnt;
		core->pLast->nLength += nAmnt;
		nLen -= nAmnt;
		core->nLength += nAmnt;
	}

	if( nLen > 0 )
	{
		Chunk *pNew = core->newChunk( nLen );
		memcpy( pNew->pData, pData, nLen );
		core->appendChunk( pNew );
//				core->nLength += nLen;
	}
}

void Bu::String::append( const char &cData )
{
	if( core->pLast && core->pLast->nLength < nMinSize )
	{
		_hardCopy();
		core->pLast->pData[core->pLast->nLength] = cData;
		++core->pLast->nLength; ++core->nLength;
//		pLast->pData[pLast->nLength] = (char)0;
	}
	else
	{
		append( &cData, 1 );
	}
}

void Bu::String::append( const String & sData )
{
	append( sData.getStr(), 0, sData.getSize() );
}

void Bu::String::append( const String & sData, long nLen )
{
	append( sData.getStr(), 0, nLen );
}

void Bu::String::append( const String & sData, long nStart, long nLen )
{
	if( nLen < 0 )
		nLen = sData.getSize() - nStart;
	append( sData.getStr(), nStart, nLen );
}

void Bu::String::append( const const_iterator &s )
{
	if( !s.isValid() )
		return;
	Chunk *pSrc = s.pChunk;

	Chunk *pNew = core->newChunk( pSrc->nLength-s.iPos );
	memcpy( pNew->pData, pSrc->pData+s.iPos, pSrc->nLength-s.iPos );

	_hardCopy();
	core->appendChunk( pNew );

	while( (pSrc = pSrc->pNext) )
	{
		core->appendChunk( core->copyChunk( pSrc ) );
	}
}

void Bu::String::append( const iterator &s )
{
	append( const_iterator( s ) );
}

void Bu::String::append( const const_iterator &s, const const_iterator &e )
{
	if( !s.isValid() )
		return;
	if( !e.isValid() )
	{
		append( s );
		return;
	}
	_hardCopy();
	if( s.pChunk == e.pChunk )
	{
		// Simple case, they're the same chunk
		Chunk *pNew = core->newChunk( e.iPos-s.iPos );
		memcpy( pNew->pData, s.pChunk->pData+s.iPos, e.iPos-s.iPos );
		core->appendChunk( pNew );
	}
	else
	{
		// A little trickier, scan the blocks...
		Chunk *pSrc = s.pChunk;
		Chunk *pNew = core->newChunk( pSrc->nLength-s.iPos );
		memcpy( pNew->pData, pSrc->pData+s.iPos, pSrc->nLength-s.iPos );
		core->appendChunk( pNew );

		while( (pSrc = pSrc->pNext) != e.pChunk )
		{
			core->appendChunk( core->copyChunk( pSrc ) );
		}

		pNew = core->newChunk( e.iPos );
		memcpy( pNew->pData, pSrc->pData, e.iPos );
		core->appendChunk( pNew );
	}
}

void Bu::String::prepend( const String & sData )
{
	prepend( sData.getStr(), sData.getSize() );
}

void Bu::String::prepend( const char *pData )
{
	if( pData == NULL )
		return;

	_hardCopy();
	long nLen;
	for( nLen = 0; pData[nLen] != (char)0; nLen++ ) { }
	
	Chunk *pNew = core->newChunk( nLen );
	memcpy( pNew->pData, pData, nLen );

	core->prependChunk( pNew );
}

void Bu::String::prepend( const char *pData, long nLen )
{
	Chunk *pNew = core->newChunk( nLen );
	
	memcpy( pNew->pData, pData, nLen );

	_hardCopy();
	core->prependChunk( pNew );
}

void Bu::String::prepend( const char c )
{
	prepend( &c, 1 );
}

void Bu::String::insert( long nPos, const char *pData, long nLen )
{
	if( nLen <= 0 )
		return;
	if( nPos <= 0 )
	{
		prepend( pData, nLen );
	}
	else if( nPos >= core->nLength )
	{
		append( pData, nLen );
	}
	else
	{
		// If we're going to flatten anyway, might as well for everyone
		flatten();
		_hardCopy();
		Chunk *p1 = core->newChunk( nPos );
		Chunk *p2 = core->newChunk( nLen );
		Chunk *p3 = core->newChunk( core->nLength-nPos );
		memcpy( p1->pData, core->pFirst->pData, nPos );
		memcpy( p2->pData, pData, nLen );
		memcpy( p3->pData, core->pFirst->pData+nPos, core->nLength-nPos );
		core->clear();
		core->appendChunk( p1 );
		core->appendChunk( p2 );
		core->appendChunk( p3 );
	}
}

void Bu::String::insert( long nPos, const String &str )
{
	if( nPos <= 0 )
	{
		prepend( str );
	}
	else if( nPos >= core->nLength )
	{
		append( str );
	}
	else
	{
		flatten();
		_hardCopy();
		Chunk *p1 = core->newChunk( nPos );
		Chunk *p3 = core->newChunk( core->nLength-nPos );
		memcpy( p1->pData, core->pFirst->pData, nPos );
		memcpy( p3->pData, core->pFirst->pData+nPos, core->nLength-nPos );
		core->clear();
		core->appendChunk( p1 );
		for( Chunk *pChnk = str.core->pFirst; pChnk;
			 pChnk = pChnk->pNext )
		{
			core->appendChunk( core->copyChunk( pChnk ) );
		}

		core->appendChunk( p3 );
	}
}

void Bu::String::insert( long nPos, const char *pData )
{
	insert( nPos, pData, strlen( pData ) );
}

void Bu::String::remove( long nPos, long nLen )
{
	if( nLen <= 0 || nPos < 0 || nPos >= core->nLength )
		return;
	if( nLen > core->nLength-nPos )
		nLen = core->nLength-nPos;
	flatten();
	_hardCopy();
	memmove( core->pFirst->pData+nPos, core->pFirst->pData+nPos+nLen, core->nLength-nPos-nLen+1 );
	core->nLength -= nLen;
	core->pFirst->nLength -= nLen;
}

void Bu::String::clear()
{
	_hardCopy();
	core->clear();
}

Bu::String Bu::String::replace( const Bu::String &fnd,
		const Bu::String &rep ) const
{
	String out;
	const_iterator o = begin();
	while( true )
	{
		const_iterator i = o.find( fnd, fnd.getSize() );
		if( !i )
		{
			out.append( o );
			return out;
		}
		else
		{
			out.append( o, i );
			out.append( rep );
			o = i;
			o += fnd.getSize();
		}
	}
}

void Bu::String::resize( long nNewSize )
{
	if( core->nLength == nNewSize )
		return;
	if( nNewSize < 0 )
		nNewSize = 0;

	flatten();
	_hardCopy();

	// TODO:  This is bad

	Chunk *pNew = core->newChunk( nNewSize );
	long nNewLen = (nNewSize<core->nLength)?(nNewSize):(core->nLength);
	if( core->nLength > 0 )
	{
		memcpy( pNew->pData, core->pFirst->pData, nNewLen );
		delete[] core->pFirst->pData;
		delete core->pFirst;
	}
	pNew->pData[nNewLen] = (char)0;
	core->pFirst = core->pLast = pNew;
	core->nLength = nNewSize;
}

long Bu::String::getSize() const
{
	return core->nLength;
}

char *Bu::String::getStr()
{
	if( core->pFirst == NULL || core->nLength == 0 )
		return (char *)"";

	flatten();
	_hardCopy();
	core->pFirst->pData[core->nLength] = (char)0;
	return core->pFirst->pData;
}

const char *Bu::String::getStr() const
{
	if( core->pFirst == NULL || core->nLength == 0 )
		return (char *)"";

	flatten();
	core->pFirst->pData[core->nLength] = (char)0;
	return core->pFirst->pData;
}

const char *Bu::String::getConstStr() const
{
	return getStr();
}

Bu::String Bu::String::getSubStrIdx( long iStart, long iSize ) const
{
	if( iStart < 0 )
		iStart = 0;
	if( iStart >= core->nLength )
		return (const char[]){(char)0};
	if( iSize < 0 )
		iSize = core->nLength;
	if( iStart+iSize > core->nLength )
		iSize = core->nLength-iStart;
	if( iSize == 0 )
		return (const char[]){(char)0};

	flatten();
	String ret( core->pFirst->pData+iStart, iSize );
	return ret;
}

Bu::String Bu::String::getSubStr( const_iterator iBegin,
		const_iterator iEnd ) const
{
	if( !iBegin.isValid() )
		return String();
	if( iBegin.pChunk == iEnd.pChunk )
	{
		return String( iBegin.pChunk->pData+iBegin.iPos,
			iEnd.iPos-iBegin.iPos );
	}
	else if( !iEnd.isValid() )
	{
		String ret;
		ret.append(
			iBegin.pChunk->pData+iBegin.iPos,
			iBegin.pChunk->nLength-iBegin.iPos
			);
		for( Chunk *pCur = iBegin.pChunk->pNext;
			 pCur; pCur = pCur->pNext )
		{
			ret.append( pCur->pData, pCur->nLength );
		}
		return ret;
	}
	else
	{
		String ret;
		ret.append(
			iBegin.pChunk->pData+iBegin.iPos,
			iBegin.pChunk->nLength-iBegin.iPos
			);
		for( Chunk *pCur = iBegin.pChunk->pNext;
			 pCur != iEnd.pChunk; pCur = pCur->pNext )
		{
			ret.append( pCur->pData, pCur->nLength );
		}
		ret.append(
			iEnd.pChunk->pData,
			iEnd.iPos
			);
		return ret;
	}
}

Bu::StringList Bu::String::split( const char c ) const
{
	Bu::StringList ret;
	const_iterator l, r;
	l = begin();
	for(r=l; l;)
	{
		for( r = l; r && r != c; r++ ) { }
		ret.append( String( l, r ) );
		l = r;
		l++;
	}
	return ret;
}

Bu::String &Bu::String::operator+=( const char *pData )
{
	append( pData );

	return (*this);
}

Bu::String &Bu::String::operator+=( const Bu::String &rSrc )
{
	append( rSrc );

	return (*this);
}

Bu::String &Bu::String::operator+=( const Bu::String::const_iterator &i )
{
	append( i, i+1 );

	return (*this);
}

Bu::String &Bu::String::operator+=( const char cData )
{
	if( core->pLast && core->pLast->nLength < nMinSize )
	{
		_hardCopy();
		core->pLast->pData[core->pLast->nLength] = cData;
		++core->pLast->nLength; ++core->nLength;
//		pLast->pData[pLast->nLength] = (char)0;
	}
	else
	{
		append( &cData, 1 );
	}
	//append( pData );

	return (*this);
}

Bu::String &Bu::String::operator=( const char *pData )
{
	set( pData );

	return (*this);
}

Bu::String Bu::String::operator+( const Bu::String &rRight ) const
{
	String ret( *this );
	ret.append( rRight );
	return ret;
}

Bu::String Bu::String::operator+( const char *pRight ) const
{
	String ret( *this );
	ret.append( pRight );
	return ret;
}

Bu::String Bu::String::operator+( char *pRight ) const
{
	String ret( *this );
	ret.append( pRight );
	return ret;
}

void Bu::String::set( const char *pData )
{
	clear();
	append( pData );
}

void Bu::String::set( const char *pData, long nSize )
{
	clear();
	append( pData, nSize );
}

void Bu::String::set( const char *pData, long nStart, long nSize )
{
	clear();
	append( pData, nStart, nSize );
}

void Bu::String::set( const String &rData )
{
	clear();
	append( rData );
}

void Bu::String::set( const String &rData, long nSize )
{
	clear();
	append( rData, nSize );
}

void Bu::String::set( const String &rData, long nStart, long nSize )
{
	clear();
	append( rData, nStart, nSize );
}

void Bu::String::set( const_iterator s )
{
	clear();
	append( s );
}

void Bu::String::set( const_iterator s, const_iterator e )
{
	clear();
	append( s, e );
}

void Bu::String::setSize( long iSize )
{
	_hardCopy();
	core->clear();
	core->appendChunk( core->newChunk( iSize ) );
}

bool Bu::String::operator==( const char *pData ) const
{
	if( core->pFirst == NULL || core->nLength == 0 ) {
		if( pData == NULL )
			return true;
		if( pData[0] == (char)0 )
			return true;
		return false;
	}

	flatten();
	core->pFirst->pData[core->nLength] = (char)0;
	const char *a = pData;
	char *b = core->pFirst->pData;
	for( long j = 0; *a!=(char)0 || *b!=(char)0; j++, a++, b++ )
	{
		if( *a != *b )
			return false;
		if( *a == (char)0 && j < core->nLength )
			return false;
	}

	return true;
}

bool Bu::String::operator==( const String &pData ) const
{
	if( core == pData.core )
		return true;
	if( core->pFirst == pData.core->pFirst )
		return true;
	if( (core->nLength == 0 && pData.core->nLength == 0) ) 
		return true;
	if( core->nLength != pData.core->nLength )
		return false;
	if( pData.core->pFirst == NULL || core->pFirst == NULL )
		return false;

	flatten();
	pData.flatten();
	const char *a = pData.core->pFirst->pData;
	char *b = core->pFirst->pData;
	for( long j = 0; j < core->nLength; j++, a++, b++ )
	{
		if( *a != *b )
			return false;
	}

	return true;
}

bool Bu::String::operator!=(const char *pData ) const
{
	return !(*this == pData);
}

bool Bu::String::operator!=(const String &pData ) const
{
	return !(*this == pData);
}

bool Bu::String::operator<(const String &pData ) const
{
	flatten();
	pData.flatten();

	const char *a = core->pFirst->pData;
	char *b = pData.core->pFirst->pData;
	for( long j = 0; j < core->nLength; j++, a++, b++ )
	{
		if( *a != *b )
			return *a < *b;
	}

	return false;
}

bool Bu::String::operator<=(const String &pData ) const
{
	flatten();
	pData.flatten();

	const char *a = core->pFirst->pData;
	char *b = pData.core->pFirst->pData;
	for( long j = 0; j < core->nLength; j++, a++, b++ )
	{
		if( *a != *b )
			return *a < *b;
	}

	return true;
}

bool Bu::String::operator>(const String &pData ) const
{
	flatten();
	pData.flatten();

	const char *a = core->pFirst->pData;
	char *b = pData.core->pFirst->pData;
	for( long j = 0; j < core->nLength; j++, a++, b++ )
	{
		if( *a != *b )
			return *a > *b;
	}

	return false;
}

bool Bu::String::operator>=(const String &pData ) const
{
	flatten();
	pData.flatten();

	const char *a = core->pFirst->pData;
	char *b = pData.core->pFirst->pData;
	for( long j = 0; j < core->nLength; j++, a++, b++ )
	{
		if( *a != *b )
			return *a > *b;
	}

	return true;
}

char &Bu::String::operator[]( long nIndex )
{
	if( nIndex < 0 || nIndex >= core->nLength )
		throw Bu::ExceptionBase("Index out of range.");
	flatten();
	_hardCopy();

	return core->pFirst->pData[nIndex];
}

const char &Bu::String::operator[]( long nIndex ) const
{
	if( nIndex < 0 || nIndex >= core->nLength )
		throw Bu::ExceptionBase("Index out of range.");
	flatten();

	return core->pFirst->pData[nIndex];
}

bool Bu::String::isSet() const
{
	return (core->pFirst != NULL);
}

bool Bu::String::compareSub( const char *pData, long nIndex, long nLen ) const
{
	if( core->pFirst == NULL || core->nLength == 0 ) {
		if( pData == NULL )
			return true;
		if( nLen == 0 )
			return true;
		if( pData[0] == (char)0 )
			return true;
		return false;
	}
	if( nIndex+nLen > core->nLength )
		return false;

	flatten();
	core->pFirst->pData[core->nLength] = (char)0;
	const char *a = pData;
	char *b = core->pFirst->pData+nIndex;
	for( long j = 0; j < nLen; j++, a++, b++ )
	{
		if( *a != *b )
			return false;
		if( *a == (char)0 && j < core->nLength )
			return false;
	}

	return true;
}

bool Bu::String::compareSub( const String &rData, long nIndex, long nLen ) const
{
	if( core->pFirst == NULL || core->nLength == 0 || rData.core->pFirst == NULL || rData.core->nLength == 0 ) 
		return false;
	if( nLen < 0 )
		nLen = rData.core->nLength;
	if( nIndex+nLen > core->nLength )
		return false;

	flatten();
	rData.flatten();
	const char *a = rData.core->pFirst->pData;
	char *b = core->pFirst->pData + nIndex;
	for( long j = 0; j < nLen; j++, a++, b++ )
	{
		if( *a != *b )
			return false;
	}

	return true;
}

bool Bu::String::isWS( long nIndex ) const
{
	flatten();

	return core->pFirst->pData[nIndex]==' ' || core->pFirst->pData[nIndex]=='\t'
		|| core->pFirst->pData[nIndex]=='\r' || core->pFirst->pData[nIndex]=='\n';
}

bool Bu::String::isAlpha( long nIndex ) const
{
	flatten();

	return (core->pFirst->pData[nIndex] >= 'a' && core->pFirst->pData[nIndex] <= 'z')
		|| (core->pFirst->pData[nIndex] >= 'A' && core->pFirst->pData[nIndex] <= 'Z');
}

Bu::String Bu::String::toLower() const
{
	Bu::String sRet = *this;

	sRet.flatten();
	sRet._hardCopy();

	for( long j = 0; j < sRet.core->nLength; j++ )
	{
		if( sRet.core->pFirst->pData[j] >= 'A' &&
			sRet.core->pFirst->pData[j] <= 'Z' )
			sRet.core->pFirst->pData[j] -= 'A'-'a';
	}

	return sRet;
}

Bu::String Bu::String::toUpper() const
{
	Bu::String sRet = *this;

	sRet.flatten();
	sRet._hardCopy();

	for( long j = 0; j < sRet.core->nLength; j++ )
	{
		if( sRet.core->pFirst->pData[j] >= 'a' &&
			sRet.core->pFirst->pData[j] <= 'z' )
			sRet.core->pFirst->pData[j] += 'A'-'a';
	}
	
	return sRet;
}

Bu::String::const_iterator Bu::String::find( const char cChar,
		Bu::String::const_iterator iStart ) const
{
	if( !iStart ) iStart = begin();
	for( ; iStart; iStart++ )
	{
		if( cChar == *iStart )
			return iStart;
	}
	return end();
}

Bu::String::const_iterator Bu::String::find( const char *sText, int nLen,
		Bu::String::const_iterator iStart ) const
{
	if( !iStart ) iStart = begin();
	for( ; iStart; iStart++ )
	{
		if( iStart.compare( sText, nLen ) )
			return iStart;
	}
	return end();
}

Bu::String::const_iterator Bu::String::find( const String &rStr,
		Bu::String::const_iterator iStart ) const
{
	if( !iStart ) iStart = begin();
	for( ; iStart; iStart++ )
	{
		if( iStart.compare( rStr ) )
			return iStart;
	}
	return end();
}

Bu::String::const_iterator Bu::String::find( const String &rStr, int nLen,
		Bu::String::const_iterator iStart ) const
{
	if( !iStart ) iStart = begin();
	for( ; iStart; iStart++ )
	{
		if( iStart.compare( rStr, nLen ) )
			return iStart;
	}
	return end();
}

Bu::String::iterator Bu::String::find( const char cChar,
		Bu::String::const_iterator iStart )
{
	if( !iStart ) iStart = begin();
	for( ; iStart; iStart++ )
	{
		if( cChar == *iStart )
			return iterator( iStart.pChunk, iStart.iPos );
	}
	return end();
}

Bu::String::iterator Bu::String::find( const char *sText, int nLen,
		Bu::String::const_iterator iStart )
{
	if( !iStart ) iStart = begin();
	for( ; iStart; iStart++ )
	{
		if( iStart.compare( sText, nLen ) )
			return iterator( iStart.pChunk, iStart.iPos );
	}
	return end();
}

Bu::String::iterator Bu::String::find( const String &rStr,
		Bu::String::const_iterator iStart )
{
	if( !iStart ) iStart = begin();
	for( ; iStart; iStart++ )
	{
		if( iStart.compare( rStr ) )
			return iterator( iStart.pChunk, iStart.iPos );
	}
	return end();
}

Bu::String::iterator Bu::String::find( const String &rStr, int nLen,
		Bu::String::const_iterator iStart )
{
	if( !iStart ) iStart = begin();
	for( ; iStart; iStart++ )
	{
		if( iStart.compare( rStr, nLen ) )
			return iterator( iStart.pChunk, iStart.iPos );
	}
	return end();
}

long Bu::String::findIdx( const char cChar, long iStart ) const
{
	flatten();
	for( long j = iStart; j < core->pFirst->nLength; j++ )
	{
		if( core->pFirst->pData[j] == cChar )
			return j;
	}
	return -1;
}

long Bu::String::findIdx( const char *sText, long iStart ) const
{
	long nTLen = strlen( sText );
	flatten();
	for( long j = iStart; j < core->pFirst->nLength-nTLen; j++ )
	{
		if( !strncmp( sText, core->pFirst->pData+j, nTLen ) )
			return j;
	}
	return -1;
}

long Bu::String::rfindIdx( const char *sText ) const
{
	long nTLen = strlen( sText );
	flatten();
	for( long j = core->pFirst->nLength-nTLen-1; j >= 0; j-- )
	{
		if( !strncmp( sText, core->pFirst->pData+j, nTLen ) )
			return j;
	}
	return -1;
}

void Bu::String::trimFront( long nAmnt )
{
	long nNewLen = core->nLength - nAmnt;
	flatten();
	Chunk *pNew = core->newChunk( nNewLen );
	memcpy( pNew->pData, core->pFirst->pData+nAmnt, nNewLen );
	_hardCopy();
	core->clear();
	core->appendChunk( pNew );
}
/*
void Bu::String::trimBack( char c )
{
	if( core->pFirst == NULL || core->nLength == 0 )
		return;
	flatten();
	for( ; core->pFirst->nLength > 0 &&
		core->pFirst->pData[core->pFirst->nLength-1] == c;
		core->pFirst->nLength--, core->nLength-- ) { }
}
*/
void Bu::String::trimBack( long iAmnt )
{
	if( iAmnt < 0 )
		return;
	if( core->nLength - iAmnt < 0 )
	{
		clear();
		return;
	}
	if( core->pFirst == NULL || core->nLength == 0 )
		return;

	flatten();
	core->pFirst->nLength -= iAmnt;
	core->nLength -= iAmnt;
}

Bu::String Bu::String::trimWhitespace() const
{
	if( core->nLength == 0 )
		return "";
	const_iterator i = begin();
	for( ; i && (*i == ' ' || *i == '\t' || *i == '\n' || *i == '\r'); i++ ) { }
	if( !i )
		return "";

	const_iterator e = i;
	for( ; e; e++ )
	{
		if( *e == ' ' || *e == '\t' || *e == '\n' || *e == '\r' )
		{
			const_iterator t = e;
			for( ; t && (*t == ' ' || *t == '\t' || *t == '\n' || *t == '\r'); t++ ) { }
			if( t )
			{
				e = t;
			}
			else
			{
				break;
			}
		}
	}

	return Bu::String( i, e );
}

Bu::String::iterator Bu::String::begin()
{
	if( core->nLength == 0 )
		return iterator( NULL, 0 );
	return iterator( core->pFirst, 0 );
}

Bu::String::const_iterator Bu::String::begin() const
{
	if( core->nLength == 0 )
		return const_iterator( NULL, 0 );
	return iterator( core->pFirst, 0 );
}

Bu::String::iterator Bu::String::end()
{
	return iterator( NULL, 0 );
}

Bu::String::const_iterator Bu::String::end() const
{
	return const_iterator( NULL, 0 );
}

bool Bu::String::isEmpty() const
{
	if( core->nLength == 0 )
		return true;
	return false;
}

void Bu::String::flatten() const
{
	if( isFlat() )
		return;

	if( core->pFirst == NULL || core->nLength == 0 )
		return;

	Chunk *pNew = core->newChunk( core->nLength );
	char *pos = pNew->pData;
	Chunk *i = core->pFirst;
	for(;;)
	{
		memcpy( pos, i->pData, i->nLength );
		pos += i->nLength;
		i = i->pNext;
		if( i == NULL )
			break;
	}
	core->clear();

	core->pLast = core->pFirst = pNew;
	core->nLength = pNew->nLength;
}

bool Bu::String::isFlat() const
{
	return (core->pFirst == core->pLast);
}

//
// Sub-class Bu::String::FormatProxy
//

Bu::String::FormatProxy::FormatProxy( const String &rFmt ) :
	rFmt( rFmt )
{
}

Bu::String::FormatProxy::~FormatProxy()
{
}

Bu::String::FormatProxy::operator Bu::String() const
{
	int iCount = lArgs.getSize();
	ArgList::const_iterator *aArg =
		new ArgList::const_iterator[iCount];
	{
		int j = 0;
		for( ArgList::const_iterator i = lArgs.begin();
			 i; i++, j++ )
		{
			aArg[j] = i;
		}
	}
	Bu::MemBuf mbOut;
	Bu::Formatter f( mbOut );
	for( String::const_iterator s = rFmt.begin(); s; s++ )
	{
		if( *s == '%' )
		{
			s++;
			if( *s == '%' )
				f << *s;
			else
			{
				String sNum;
				while( s && *s >= '0' && *s <= '9' )
				{
					sNum += *s;
					s++;
				}
				int iIndex = strtol( sNum.getStr(), 0, 10 )-1;
				if( iIndex < 0 || iIndex >= iCount )
				{
					delete[] aArg;
					throw Bu::ExceptionBase(
						"Argument index %d is outside of "
						"valid range (1-%d).", iIndex+1, iCount
						);
				}

				f << (*aArg[iIndex]).format << (*aArg[iIndex]).value;
				if( s )
					f << *s;
			}
		}
		else
		{
			f << *s;
		}
	}

	delete[] aArg;
	return mbOut.getString();
}















































template<> uint32_t Bu::__calcHashCode<Bu::String>( const Bu::String &k )
{
	long j, sz = k.getSize();
	const char *s = k.getStr();

	long nPos = 0;
	for( j = 0; j < sz; j++, s++ )
	{
		nPos = *s + (nPos << 6) + (nPos << 16) - nPos;
	}

	return nPos;
}

template<> bool Bu::__cmpHashKeys<Bu::String>(
		const Bu::String &a, const Bu::String &b )
{
	return a == b;
}

template<> void Bu::__tracer_format<Bu::String>( const Bu::String &v )
{
	printf("(%ld)\"%s\"", v.getSize(), v.getStr() );
}

bool &Bu::operator<<( bool &dst, const Bu::String &sIn )
{
	if( sIn == "true" || sIn == "yes" || sIn == "t" )
		dst = true;
	else
		dst = false;

	return dst;
}

uint8_t &Bu::operator<<( uint8_t &dst, const Bu::String &sIn )
{
	sscanf( sIn.getStr(), "%hhu", &dst );
	return dst;
}

int8_t &Bu::operator<<( int8_t &dst, const Bu::String &sIn )
{
	sscanf( sIn.getStr(), "%hhd", &dst );
	return dst;
}

char &Bu::operator<<( char &dst, const Bu::String &sIn )
{
	sscanf( sIn.getStr(), "%hhd", &dst );
	return dst;
}

uint16_t &Bu::operator<<( uint16_t &dst, const Bu::String &sIn )
{
	sscanf( sIn.getStr(), "%hu", &dst );
	return dst;
}

int16_t &Bu::operator<<( int16_t &dst, const Bu::String &sIn )
{
	sscanf( sIn.getStr(), "%hd", &dst );
	return dst;
}

uint32_t &Bu::operator<<( uint32_t &dst, const Bu::String &sIn )
{
	sscanf( sIn.getStr(), "%u", &dst );
	return dst;
}

int32_t &Bu::operator<<( int32_t &dst, const Bu::String &sIn )
{
	sscanf( sIn.getStr(), "%d", &dst );
	return dst;
}

uint64_t &Bu::operator<<( uint64_t &dst, const Bu::String &sIn )
{
	sscanf( sIn.getStr(), "%llu", &dst );
	return dst;
}

int64_t &Bu::operator<<( int64_t &dst, const Bu::String &sIn )
{
	sscanf( sIn.getStr(), "%lld", &dst );
	return dst;
}

float &Bu::operator<<( float &dst, const Bu::String &sIn )
{
	sscanf( sIn.getStr(), "%f", &dst );
	return dst;
}

double &Bu::operator<<( double &dst, const Bu::String &sIn )
{
	sscanf( sIn.getStr(), "%lf", &dst );
	return dst;
}

long double &Bu::operator<<( long double &dst, const Bu::String &sIn )
{
	sscanf( sIn.getStr(), "%Lf", &dst );
	return dst;
}

Bu::String &Bu::operator<<( Bu::String &dst, const Bu::String &sIn )
{
	dst = sIn;
	return dst;
}

Bu::ArchiveBase &Bu::operator<<( Bu::ArchiveBase &ar, const Bu::String &s )
{
	long n = s.getSize();
	ar << n;
	ar.write( s.getConstStr(), n );
	return ar;
}

Bu::ArchiveBase &Bu::operator>>( Bu::ArchiveBase &ar, Bu::String &s )
{
	long n;
	ar >> n;
	s.setSize( n );
	ar.read( s.getStr(), n );
	return ar;
}

