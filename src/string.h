/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_STRING_H
#define BU_STRING_H

#include <stdint.h>
#include <memory>

#ifndef WIN32
#include <wordexp.h>
#endif

#include "bu/util.h"
#include "bu/sharedcore.h"
#include "bu/exceptionbase.h"
#include "bu/archivebase.h"
#include "bu/list.h"
#include "bu/fmt.h"
#include "bu/variant.h"

#include <string.h>

#define nMinSize	(256)

namespace Bu
{
	class String;

	/** @cond DEVEL */
	class StringCore
	{
	friend class String;
	friend class SharedCore<String, StringCore>;
	private:
		struct Chunk
		{
			long nLength;
			char *pData;
			Chunk *pNext;
		};

		StringCore();
		StringCore( const StringCore &rSrc );
		virtual ~StringCore();

		mutable long nLength;
		mutable Chunk *pFirst;
		mutable Chunk *pLast;

		void clear() const;
		Chunk *newChunk() const;
		Chunk *newChunk( long nLen ) const;
		Chunk *copyChunk( Chunk *pSrc ) const;
		void appendChunk( Chunk *pNewChunk );
		void prependChunk( Chunk *pNewChunk );
	};
	/** @endcond */

	/**
	 */
	class String : public SharedCore<String, StringCore>
	{
	protected:
		using SharedCore<String, StringCore >::core;
		using SharedCore<String, StringCore >::_hardCopy;

	private:
		typedef StringCore::Chunk Chunk;

	public: // Iterators
		struct iterator;
		typedef struct const_iterator
		{
			friend class String;
			friend struct iterator;
		private:
			const_iterator( Chunk *pChunk, int iPos ) :
				pChunk( pChunk ),
				iPos( iPos )
			{
			}

			Chunk *pChunk;
			int iPos;

		public:
			const_iterator( const const_iterator &i ) :
				pChunk( i.pChunk ),
				iPos( i.iPos )
			{
			}
			
			const_iterator( const struct iterator &i ) :
				pChunk( i.pChunk ),
				iPos( i.iPos )
			{
			}

			const_iterator() :
				pChunk( NULL ),
				iPos( 0 )
			{
			}

			bool operator==( const const_iterator &i ) const
			{
				return pChunk == i.pChunk && iPos == i.iPos;
			}

			bool operator!=( const const_iterator &i ) const
			{
				return !(*this == i);
			}

			const_iterator &operator=( const const_iterator &i )
			{
				pChunk = i.pChunk;
				iPos = i.iPos;
				return *this;
			}

			const_iterator &operator++()
			{
				if( !pChunk ) return *this;
				iPos++;
				if( iPos >= pChunk->nLength )
				{
					iPos = 0;
					pChunk = pChunk->pNext;
				}
				return *this;
			}

			const_iterator &operator++( int )
			{
				if( !pChunk ) return *this;
				iPos++;
				if( iPos >= pChunk->nLength )
				{
					iPos = 0;
					pChunk = pChunk->pNext;
				}
				return *this;
			}

			const_iterator &operator+=( int iAmnt )
			{
				if( !pChunk ) return *this;
				iPos += iAmnt;
				while( iPos >= pChunk->nLength )
				{
					iPos -= pChunk->nLength;
					pChunk = pChunk->pNext;
					if( pChunk == NULL )
						break;
				}
				return *this;
			}

			const_iterator operator+( int iAmnt ) const
			{
				if( !pChunk ) return *this;
				const_iterator ret( *this );
				ret += iAmnt;
				return ret;
			}

			const char &operator *() const
			{
				if( !pChunk ) throw Bu::ExceptionBase("Not a valid const_iterator.");
				return pChunk->pData[iPos];
			}

			bool operator==( const char &c ) const
			{
				if( !pChunk ) return false;
				return pChunk->pData[iPos] == c;
			}

			bool operator!=( const char &c ) const
			{
				if( !pChunk ) return false;
				return pChunk->pData[iPos] != c;
			}
			
			operator bool() const
			{
				return pChunk != NULL;
			}

			bool isValid() const
			{
				return pChunk != NULL;
			}

			bool compare( const const_iterator &c ) const
			{
				const_iterator a = *this;
				const_iterator b = c;
				if( a == b )
					return true;
				for(; a && b; a++, b++ )
				{
					if( *a != *b )
						return false;
				}
				return true;
			}

			bool compare( const const_iterator &c, int nLen ) const
			{
				const_iterator a = *this;
				const_iterator b = c;
				if( a == b )
					return true;
				for(int j = 0; a && b && j < nLen; a++, b++, j++ )
				{
					if( *a != *b )
						return false;
				}
				return true;
			}

			bool compare( const char *c ) const
			{
				if( !pChunk ) return false;
				const_iterator a = *this;
				for(; a && *c; a++, c++ )
				{
					if( *a != *c )
						return false;
				}
				if( a.isValid() != (*c!=(char)0) )
					return false;
				return true;
			}

			bool compare( const char *c, int nLen ) const
			{
				if( !pChunk ) return false;
				const_iterator a = *this;
				int j = 0;
				for(; a && j < nLen; a++, c++, j++ )
				{
					if( *a != *c )
						return false;
				}
				if( j < nLen )
					return false;
				return true;
			}

			bool compare( const String &s ) const
			{
				if( !pChunk ) return false;
				return compare( s.begin() );
			}

			bool compare( const String &s, int nLen ) const
			{
				if( !pChunk ) return false;
				return compare( s.begin(), nLen );
			}

			const_iterator find( const char c ) const
			{
				for( const_iterator i = *this; i; i++ )
				{
					if( *i == c )
						return i;
				}
				return const_iterator( NULL, 0 );
			}

			const_iterator find( const char *pStr, int nLen ) const
			{
				for( const_iterator i = *this; i; i++ )
				{
					if( i.compare( pStr, nLen ) )
						return i;
				}
				return const_iterator( NULL, 0 );
			}

			const_iterator find( const String &s ) const
			{
				for( const_iterator i = *this; i; i++ )
				{
					if( i.compare( s ) )
						return i;
				}
				return const_iterator( NULL, 0 );
			}

			const_iterator find( const String &s, int nLen ) const
			{
				for( const_iterator i = *this; i; i++ )
				{
					if( i.compare( s, nLen ) )
						return i;
				}
				return const_iterator( NULL, 0 );
			}
		} const_iterator;
		
		typedef struct iterator
		{
			friend class String;
			friend struct const_iterator;
		private:
			iterator( Chunk *pChunk, int iPos ) :
				pChunk( pChunk ),
				iPos( iPos )
			{
			}

			Chunk *pChunk;
			int iPos;

		public:
			iterator( const iterator &i ) :
				pChunk( i.pChunk ),
				iPos( i.iPos )
			{
			}

			iterator() :
				pChunk( NULL ),
				iPos( 0 )
			{
			}

			operator const_iterator() const
			{
				return const_iterator( pChunk, iPos );
			}

			bool operator==( const iterator &i ) const
			{
				return pChunk == i.pChunk && iPos == i.iPos;
			}

			bool operator!=( const iterator &i ) const
			{
				return !(*this == i);
			}

			iterator &operator=( const iterator &i )
			{
				pChunk = i.pChunk;
				iPos = i.iPos;
				return *this;
			}

			iterator &operator++()
			{
				if( !pChunk ) return *this;
				iPos++;
				if( iPos >= pChunk->nLength )
				{
					iPos = 0;
					pChunk = pChunk->pNext;
				}
				return *this;
			}

			iterator &operator++( int )
			{
				if( !pChunk ) return *this;
				iPos++;
				if( iPos >= pChunk->nLength )
				{
					iPos = 0;
					pChunk = pChunk->pNext;
				}
				return *this;
			}

			iterator &operator+=( int iAmnt )
			{
				if( !pChunk ) return *this;
				iPos += iAmnt;
				while( iPos >= pChunk->nLength )
				{
					iPos -= pChunk->nLength;
					pChunk = pChunk->pNext;
					if( pChunk == NULL )
						break;
				}
				return *this;
			}

			iterator operator+( int iAmnt ) const
			{
				if( !pChunk ) return *this;
				iterator ret( *this );
				ret += iAmnt;
				return ret;
			}

			char &operator*()
			{
				if( !pChunk ) throw Bu::ExceptionBase("Not a valid iterator.");
				return pChunk->pData[iPos];
			}

			const char &operator*() const
			{
				if( !pChunk ) throw Bu::ExceptionBase("Not a valid iterator.");
				return pChunk->pData[iPos];
			}

			bool operator==( const char &c ) const
			{
				if( !pChunk ) return false;
				return pChunk->pData[iPos] == c;
			}

			bool operator!=( const char &c ) const
			{
				if( !pChunk ) return false;
				return pChunk->pData[iPos] != c;
			}

			iterator &operator=( const char &c )
			{
				if( !pChunk ) throw Bu::ExceptionBase("Not a valid iterator.");
				pChunk->pData[iPos] = c;
				return *this;
			}

			operator bool() const
			{
				return pChunk != NULL;
			}

			bool isValid() const
			{
				return pChunk != NULL;
			}

			bool compare( const iterator &c ) const
			{
				iterator a = *this;
				iterator b = c;
				if( a == b )
					return true;
				for(; a && b; a++, b++ )
				{
					if( *a != *b )
						return false;
				}
				return true;
			}

			bool compare( const iterator &c, int nLen ) const
			{
				iterator a = *this;
				iterator b = c;
				if( a == b )
					return true;
				for(int j = 0; a && b && j < nLen; a++, b++, j++ )
				{
					if( *a != *b )
						return false;
				}
				return true;
			}

			bool compare( const char *c ) const
			{
				if( !pChunk ) return false;
				iterator a = *this;
				for(; a && *c; a++, c++ )
				{
					if( *a != *c )
						return false;
				}
				if( a.isValid() != (*c!=(char)0) )
					return false;
				return true;
			}

			bool compare( const char *c, int nLen ) const
			{
				if( !pChunk ) return false;
				iterator a = *this;
				int j = 0;
				for(; a && j < nLen; a++, c++, j++ )
				{
					if( *a != *c )
						return false;
				}
				if( j < nLen )
					return false;
				return true;
			}

			bool compare( const String &s ) const
			{
				if( !pChunk ) return false;
				return compare( s.begin() );
			}

			bool compare( const String &s, int nLen ) const
			{
				if( !pChunk ) return false;
				return compare( s.begin(), nLen );
			}

			iterator find( const char c ) const
			{
				for( iterator i = *this; i; i++ )
				{
					if( *i == c )
						return i;
				}
				return iterator( NULL, 0 );
			}

			iterator find( const char *pStr, int nLen ) const
			{
				for( iterator i = *this; i; i++ )
				{
					if( i.compare( pStr, nLen ) )
						return i;
				}
				return iterator( NULL, 0 );
			}

			iterator find( const String &s ) const
			{
				for( iterator i = *this; i; i++ )
				{
					if( i.compare( s ) )
						return i;
				}
				return iterator( NULL, 0 );
			}

			iterator find( const String &s, int nLen ) const
			{
				for( iterator i = *this; i; i++ )
				{
					if( i.compare( s, nLen ) )
						return i;
				}
				return iterator( NULL, 0 );
			}
		} iterator;

	public:
		String();
		String( const char *pData );
		String( const char *pData, long nLength );
		String( const String &rSrc );
		String( const String &rSrc, long nLength );
		String( const String &rSrc, long nStart, long nLength );
		String( long nSize );
		String( const const_iterator &s );
		String( const const_iterator &s, const const_iterator &e );
		virtual ~String();

		/**
		 * Append data to your string.
		 *@param pData (const char *) The data to append.
		 */
		void append( const char *pData );

		/**
		 * Append data to your string.
		 *@param pData (const char *) The data to append.
		 *@param nLen (long) The length of the data to append.
		 */
		void append( const char *pData, long nLen );
		
		/**
		 * Append data to your string.
		 *@param pData (const char *) The data to append.
		 *@param nStart (long) The start position to copy from.
		 *@param nLen (long) The length of the data to append.
		 */
		void append( const char *pData, long nStart, long nLen );

		/**
		 * Append a single char to your string.
		 *@param cData (const char &) The character to append.
		 */
		void append( const char &cData );

		/**
		 * Append another String to this one.
		 *@param sData (String &) The String to append.
		 *@todo This function can be made much faster by not using getStr()
		 */
		void append( const String & sData );
		
		/**
		 * Append another String to this one.
		 *@param sData (String &) The String to append.
		 *@param nLen How much data to append.
		 *@todo This function can be made much faster by not using getStr()
		 */
		void append( const String & sData, long nLen );
		
		/**
		 * Append another String to this one.
		 *@param sData (String &) The String to append.
		 *@param nStart Start position in sData to start copying from.
		 *@param nLen How much data to append.
		 *@todo This function can be made much faster by not using getStr()
		 */
		void append( const String & sData, long nStart, long nLen );
		
		/**
		 * Append data to this String using the passed in iterator as a base.
		 * The iterator is const, it is not changed.
		 *@param s Iterator from any compatible String to copy data from.
		 */
		void append( const const_iterator &s );

		/**
		 * Append data to this String using the passed in iterator as a base.
		 * The iterator is const, it is not changed.
		 *@param s Iterator from any compatible String to copy data from.
		 */
		void append( const iterator &s );

		/**
		 * Append data to this String using the passed in iterator as a base,
		 * and copy data until the ending iterator is reached.  The character
		 * at the ending iterator is not copied.
		 * The iterators are const, they are not changed.
		 *@param s Iterator from any compatible String to copy data from.
		 *@param e Iterator to stop copying at.
		 */
		void append( const const_iterator &s, const const_iterator &e );
		
		/**
		 * Prepend another String to this one.
		 *@param sData (String &) The String to prepend.
		 *@todo This function can be made much faster by not using getStr()
		 */
		void prepend( const String & sData );

		/**
		 * Prepend data to your string.
		 *@param pData (const char *) The data to prepend.
		 */
		void prepend( const char *pData );

		/**
		 * Prepend data to your string.
		 *@param pData (const char *) The data to prepend.
		 *@param nLen (long) The length of the data to prepend.
		 */
		void prepend( const char *pData, long nLen );

		void prepend( const char c );

		/**
		 * Insert pData before byte nPos, that is, the first byte of pData will
		 * start at nPos.  This could probably be made faster by avoiding
		 * flattening.
		 */
		void insert( long nPos, const char *pData, long nLen );

		void insert( long nPos, const String &str );

		/**
		 *@todo This function shouldn't use strlen, we should add our own to
		 * this class, one that can be overridden in a specific implementation.
		 */
		void insert( long nPos, const char *pData );

		void remove( long nPos, long nLen );

		/**
		 * Clear all data from the string.
		 */
		void clear();

		String replace( const String &fnd, const String &rep ) const;

		/**
		 * Force the string to resize
		 *@param nNewSize (long) The new size of the string.
		 */
		void resize( long nNewSize );

		/**
		 * Get the current size of the string.
		 *@returns (long) The current size of the string.
		 */
		long getSize() const;
		
		/**
		 * Get a pointer to the string array.
		 *@returns (char *) The string data.
		 */
		char *getStr();
		
		/**
		 * Get a const pointer to the string array.
		 *@returns (const char *) The string data.
		 */
		const char *getStr() const;

		/**
		 * A convinience function, this one won't cause as much work as the
		 * non-const getStr, so if you're not changing the data, consider it.
		 */
		const char *getConstStr() const;

		String getSubStrIdx( long iStart, long iSize=-1 ) const;

		String getSubStr( const_iterator iBegin,
				const_iterator iEnd=String::const_iterator() ) const;

		Bu::List<String> split( const char c ) const;

		/**
		 * Plus equals operator for String.
		 *@param pData (const char *) The data to append to your String.
		 */
		String &operator+=( const char *pData );
		
		/**
		 * Plus equals operator for String.
		 *@param rSrc (const String &) The String to append to your String.
		 */
		String &operator+=( const String &rSrc );

		String &operator+=( const String::const_iterator &i );

		/**
		 * Plus equals operator for String.
		 *@param cData (const char) The character to append to your String.
		 */
		String &operator+=( const char cData );

		/**
		 * Assignment operator.
		 *@param pData (const char *) The character array to append to your
		 *		String.
		 */
		String &operator=( const char *pData );

		String operator+( const String &rRight ) const;

		String operator+( const char *pRight ) const;

		String operator+( char *pRight ) const;

		/**
		 * Reset your String to this character array.
		 *@param pData (const char *) The character array to set your String to.
		 */
		void set( const char *pData );

		/**
		 * Reset your String to this character array.
		 *@param pData (const char *) The character array to set your String to.
		 *@param nSize (long) The length of the inputted character array.
		 */
		void set( const char *pData, long nSize );

		void set( const char *pData, long nStart, long nSize );
		
		void set( const String &rData );

		void set( const String &rData, long nSize );

		void set( const String &rData, long nStart, long nSize );

		void set( const_iterator s );

		void set( const_iterator s, const_iterator e );

		/**
		 * Resize the string, possibly to make room for a copy.  At the moment
		 * this operation *is* destructive.  What was in the string will in no
		 * way be preserved.  This is, however, very fast.  If you want to
		 * keep your data check out resize.
		 *@param iSize the new size in bytes.  The string is guranteed to have
		 * at least this much contiguous space available when done.
		 */
		void setSize( long iSize );

		/**
		 * Equals comparison operator.
		 *@param pData (const char *) The character array to compare your String
		 *		to.
		 */
		bool operator==( const char *pData ) const;
		
		/**
		 * Equals comparison operator.
		 *@param pData (const String &) The String to compare your String to.
		 */
		bool operator==( const String &pData ) const;

		/**
		 * Not equals comparison operator.
		 *@param pData (const char *) The character array to compare your String
		 *		to.
		 */
		bool operator!=(const char *pData ) const;
		
		/**
		 * Not equals comparison operator.
		 *@param pData (const String &) The String to compare your String to.
		 */
		bool operator!=(const String &pData ) const;

		bool operator<(const String &pData ) const;
		
		bool operator<=(const String &pData ) const;

		bool operator>(const String &pData ) const;

		bool operator>=(const String &pData ) const;

		/**
		 * Indexing operator
		 *@param nIndex (long) The index of the character you want.
		 *@returns (char &) The character at position (nIndex).
		 */
		char &operator[]( long nIndex );
		
		/**
		 * Const indexing operator
		 *@param nIndex (long) The index of the character you want.
		 *@returns (const char &) The character at position (nIndex).
		 */
		const char &operator[]( long nIndex ) const;

		bool isSet() const;

		bool compareSub( const char *pData, long nIndex, long nLen ) const;

		bool compareSub( const String &rData, long nIndex, long nLen ) const;
		
		/**
		 * Is the character at index (nIndex) white space?
		 *@param nIndex (long) The index of the character you want to check.
		 *@returns (bool) Is it white space?
		 */
		bool isWS( long nIndex ) const;

		/**
		 * Is the character at index (nIndex) a letter?
		 *@param nIndex (long) The index of the character you want to check.
		 *@returns (bool) Is it a letter?
		 */
		bool isAlpha( long nIndex ) const;

		/**
		 * Convert your alpha characters to lower case.
		 */
		String toLower() const;

		/**
		 * Convert your alpha characters to upper case.
		 */
		String toUpper() const;

		const_iterator find( const char cChar,
				const_iterator iStart=const_iterator() ) const;

		const_iterator find( const char *sText, int nLen,
				const_iterator iStart=const_iterator() ) const;

		const_iterator find( const String &rStr,
				const_iterator iStart=const_iterator() ) const;

		const_iterator find( const String &rStr, int nLen,
				const_iterator iStart=const_iterator() ) const;

		iterator find( const char cChar,
				const_iterator iStart=const_iterator() );

		iterator find( const char *sText, int nLen,
				const_iterator iStart=const_iterator() );

		iterator find( const String &rStr,
				const_iterator iStart=const_iterator() );

		iterator find( const String &rStr, int nLen,
				const_iterator iStart=const_iterator() );

		/**
		 * Find the index of the first occurrance of cChar
		 *@param cChar The character to search for.
		 *@param iStart The position in the string to start searching from.
		 *@returns (long) The index of the first occurrance. -1 for not found.
		 */
		long findIdx( const char cChar, long iStart=0 ) const;
		
		/**
		 * Find the index of the first occurrance of sText
		 *@param sText The null-terminated string to search for.
		 *@param iStart The position in the string to start searching from.
		 *@returns The index of the first occurrance. -1 for not found.
		 */
		long findIdx( const char *sText, long iStart=0 ) const;

		/**
		 * Do a reverse search for (sText)
		 *@param sText (const char *) The string to search for.
		 *@returns (long) The index of the last occurrance. -1 for not found.
		 */
		long rfindIdx( const char *sText ) const;

		/**
		 * Remove nAmnt bytes from the front of the string.  This function
		 * operates in O(n) time and should be used sparingly.
		 */
		void trimFront( long nAmnt );

	//	void trimBack( char c );
		void trimBack( long iAmnt );
		
		iterator begin();

		const_iterator begin() const;

		iterator end();

		const_iterator end() const;

		bool isEmpty() const;

	private:
		void flatten() const;
		bool isFlat() const;

		class FormatProxy
		{
		public:
			FormatProxy( const String &rFmt );
			virtual ~FormatProxy();

			template<typename T>
			FormatProxy &arg( const T &x )
			{
				lArgs.append( Arg( x ) );

				return *this;
			}

			template<typename T>
			FormatProxy &arg( const T &x, const Bu::Fmt &f )
			{
				lArgs.append( Arg( x, f ) );

				return *this;
			}

			operator String() const;

		private:
			const String &rFmt;
			class Arg
			{
			public:
				template<typename T>
				Arg( const T &v ) :
					value( v )
				{
				}

				template<typename T>
				Arg( const T &v, const Bu::Fmt &f ) :
					value( v ),
					format( f )
				{
				}

				Bu::Variant value;
				Bu::Fmt format;
			};
			typedef Bu::List<Arg> ArgList;
			ArgList lArgs;
		};

	public:
		template<typename ArgType>
		FormatProxy arg( const ArgType &x )
		{
			return FormatProxy( *this ).arg( x );
		}

		template<typename ArgType>
		FormatProxy arg( const ArgType &x, const Bu::Fmt &f )
		{
			return FormatProxy( *this ).arg( x, f );
		}
	};
	
	template<class T> String operator+( const T *pLeft, const String &rRight )
	{
		Bu::String ret( pLeft );
		ret.append( rRight );
		return ret;
	}

	ArchiveBase &operator<<( ArchiveBase &ar, const String &s );
	ArchiveBase &operator>>( ArchiveBase &ar, String &s );

	template<typename T>
	uint32_t __calcHashCode( const T &k );

	template<typename T>
	bool __cmpHashKeys( const T &a, const T &b );

	template<> uint32_t __calcHashCode<String>( const String &k );
	template<> bool __cmpHashKeys<String>(
		const String &a, const String &b );
	
	template<typename t> void __tracer_format( const t &v );
	template<> void __tracer_format<String>( const String &v );

	bool &operator<<( bool &dst, const String &sIn );
	uint8_t &operator<<( uint8_t &dst, const String &sIn );
	int8_t &operator<<( int8_t &dst, const String &sIn );
	char &operator<<( char &dst, const String &sIn );
	uint16_t &operator<<( uint16_t &dst, const String &sIn );
	int16_t &operator<<( int16_t &dst, const String &sIn );
	uint32_t &operator<<( uint32_t &dst, const String &sIn );
	int32_t &operator<<( int32_t &dst, const String &sIn );
	uint64_t &operator<<( uint64_t &dst, const String &sIn );
	int64_t &operator<<( int64_t &dst, const String &sIn );
	float &operator<<( float &dst, const String &sIn );
	double &operator<<( double &dst, const String &sIn );
	long double &operator<<( long double &dst, const String &sIn );
	Bu::String &operator<<( Bu::String &dst, const String &sIn );

	typedef Bu::List<String> StringList;
};

#endif
