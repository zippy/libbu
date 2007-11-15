/*
 * Copyright (C) 2007 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_SPTR_H
#define BU_SPTR_H

#include <stdint.h>
#include <stdio.h>

namespace Bu
{
	template<typename T> class SPtr;
	template< typename Tb, typename Ta > SPtr<Tb> SPtrCast( SPtr<Ta> src );

	/**
	 *@ingroup Containers
	 */
	template<typename T>
	class SPtr
	{
		template<typename Tb, typename Ta>
		friend SPtr<Tb> SPtrCast( SPtr<Ta> pt );
	public:
		SPtr() :
			pRefCnt( NULL ),
			pData( NULL )
		{
		}

		~SPtr()
		{
			decCount();
		}

		SPtr( const SPtr<T> &src ) :
			pRefCnt( src.pRefCnt ),
			pData( src.pData )
		{
			if( pRefCnt )
				(*pRefCnt) += 1;
		}

		SPtr( T *pSrc ) :
			pRefCnt( NULL ),
			pData( pSrc )
		{
			if( pData )
			{
				pRefCnt = new int32_t;
				(*pRefCnt) = 1;
			}
		}

		/**
		 * Get the number of references to this pointer.
		 *@returns (int32_t) The number of references to this pointer.
		 */
		int32_t count() const
		{
			return *pRefCnt;
		}

		/**
		 * Pointer access operator.
		 *@returns (const T *)
		 */
		const T *operator->() const
		{
			return pData;
		}

		/**
		 * Dereference operator.
		 *@returns (const T &) The value at the end of the pointer.
		 */
		const T &operator*() const
		{
			return *pData;
		}
		
		/**
		 * Pointer access operator.
		 *@returns (T *)
		 */
		T *operator->()
		{
			return pData;
		}

		/**
		 * Dereference operator.
		 *@returns (T &) The value at the end of the pointer.
		 */
		T &operator*()
		{
			return *pData;
		}

		/**
		 * Assignment operator.
		 *@param src (const SPtr<T> &)
		 */
		SPtr<T> operator=( const SPtr<T> &src )
		{
			decCount();
			pRefCnt = src.pRefCnt;
			pData = src.pData;
			if( pRefCnt )
				(*pRefCnt) += 1;

			return *this;
		}

		/**
		 * Assignment operator.
		 *@param src (const SPtr<T> &)
		 */
		const SPtr<T> operator=( const SPtr<T> &src ) const
		{
			decCount();
			pRefCnt = src.pRefCnt;
			pData = src.pData;
			if( pRefCnt )
				(*pRefCnt) += 1;

			return *this;
		}

		/**
		 * Equals comparison operator.
		 *@param src (const SPtr<T> &) The SPtr to compare to.
		 *@returns (bool) Are the equal?
		 */
		bool operator==( const SPtr<T> &src ) const
		{
			return pData == src.pData;
		}

		/**
		 * Equals comparison operator.
		 *@param src (const T *) The pointer to compare to.
		 *@returns (bool) Are the equal?
		 */
		bool operator==( const T *src ) const
		{
			return pData == src;
		}
		
		/**
		 * Not equals comparison operator.
		 *@param src (const SPtr<T> &) The SPtr to compare to.
		 *@returns (bool) Are the equal?
		 */
		bool operator!=( const SPtr<T> &src ) const
		{
			return !(pData == src.pData);
		}

		/**
		 * Not equals comparison operator.
		 *@param src (const T *) The pointer to compare to.
		 *@returns (bool) Are the equal?
		 */
		bool operator!=( const T *src ) const
		{
			return !(pData == src);
		}

		/**
		 * Boolean cast operator. Do we have a pointer?
		 */
		operator bool() const
		{
			return pRefCnt != NULL;
		}

		/**
		 * Do we have a pointer?
		 *@returns (bool) Do we have a pointer?
		 */
		bool isSet() const
		{
			return pRefCnt != NULL;
		}

	private:
		void decCount() const
		{
			if( pRefCnt )
			{
				(*pRefCnt) -= 1;
				//printf("Decrementing ref-count to %d\n", *pRefCnt );
				if( (*pRefCnt) == 0 )
				{
					delete pRefCnt;
					delete pData;
					pRefCnt = NULL;
					pData = NULL;
				}
			}
		}

		mutable int32_t *pRefCnt;
		mutable T *pData;
	};

	template< typename Tb, typename Ta > SPtr<Tb> SPtrCast( SPtr<Ta> src )
	{
		SPtr<Tb> ret;
		ret.pRefCnt = src.pRefCnt;
		ret.pData = dynamic_cast<Tb *>(src.pData);
		if( ret.pRefCnt )
			(*(ret.pRefCnt)) += 1;
		return ret;
	}
}

#endif
