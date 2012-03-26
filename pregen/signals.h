/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_SIGNALS_H
#define BU_SIGNALS_H

#include "bu/util.h"
#include "bu/exceptionbase.h"

namespace Bu
{
	subExceptionDecl( SignalException );

#ifndef BU_SIGNAL_PARAM_COUNT_0
#define BU_SIGNAL_PARAM_COUNT_0
	//
	// 0 Parameter(s)
	//
	template<typename ret>
	class _Slot0
	{
	public:
		_Slot0() { }
		virtual ~_Slot0() { }
		virtual ret operator()(  )=0;
		virtual _Slot0<ret> *clone() const=0;
	};
	
	template<typename cls, typename ret>
	class __Slot0 : public _Slot0<ret>
	{
	public:
		__Slot0( cls *pCls, ret (cls::*pFnc)(  ) ) :
			pCls( pCls ), pFnc( pFnc ) { }
		virtual ~__Slot0() { }
	
		virtual ret operator()(  )
		{
			return (pCls->*pFnc)(  );
		}
	
		virtual _Slot0<ret> *clone() const
		{
			return new __Slot0<cls, ret>( pCls, pFnc );
		}
	
	private:
		cls *pCls;
		ret (cls::*pFnc)(  );
	};
	
	template<typename ret>
	class __Slot0F : public _Slot0<ret>
	{
	public:
		__Slot0F( ret (*pFnc)(  ) ) :
			pFnc( pFnc ) { }
		virtual ~__Slot0F() { }
	
		virtual ret operator()(  )
		{
			return (*pFnc)(  );
		}
	
		virtual _Slot0<ret> *clone() const
		{
			return new __Slot0F<ret>( pFnc );
		}
	
	private:
		ret (*pFnc)(  );
	};
	
	template<typename ret>
	class Signal0
	{
	public:
		Signal0() : pCb( NULL ) { }
		Signal0( _Slot0<ret> *pCb ) : pCb( pCb ) { }
		Signal0( const Signal0<ret> &rSrc ) :
			pCb( (rSrc.pCb)?(rSrc.pCb->clone()):(NULL) ) { }
		virtual ~Signal0() { delete pCb; pCb = NULL; }
	
		ret operator()(  )
		{
			if( !pCb ) throw SignalException("Uninitialized signal called.");
			return (*pCb)(  );
		}
	
		bool isSet() const { return pCb != NULL; }
		operator bool() const { return isSet(); }
	
		Signal0<ret> &operator=( const Signal0<ret> &rhs )
		{
			pCb = rhs.pCb->clone();
			return *this;
		}
	
	private:
		_Slot0<ret> *pCb;
	};
	
	template<typename cls, typename ret>
	Signal0<ret> slot( cls *pCls, ret (cls::*pFnc)(  ) )
	{
		if( !pCls || !pFnc ) throw SignalException("NULL pointer in slot().");
		return Signal0<ret>(
			new __Slot0<cls, ret>( pCls, pFnc )
			);
	}
	
	template<typename ret>
	Signal0<ret> slot( ret (*pFnc)(  ) )
	{
		if( !pFnc ) throw SignalException("NULL pointer in slot().");
		return Signal0<ret>(
			new __Slot0F<ret>( pFnc )
			);
	}
#endif  // BU_SIGNAL_PARAM_COUNT_0

#ifndef BU_SIGNAL_PARAM_COUNT_1
#define BU_SIGNAL_PARAM_COUNT_1
	//
	// 1 Parameter(s)
	//
	template<typename ret, typename p1t>
	class _Slot1
	{
	public:
		_Slot1() { }
		virtual ~_Slot1() { }
		virtual ret operator()( p1t p1 )=0;
		virtual _Slot1<ret, p1t> *clone() const=0;
	};
	
	template<typename cls, typename ret, typename p1t>
	class __Slot1 : public _Slot1<ret, p1t>
	{
	public:
		__Slot1( cls *pCls, ret (cls::*pFnc)( p1t ) ) :
			pCls( pCls ), pFnc( pFnc ) { }
		virtual ~__Slot1() { }
	
		virtual ret operator()( p1t p1 )
		{
			return (pCls->*pFnc)( p1 );
		}
	
		virtual _Slot1<ret, p1t> *clone() const
		{
			return new __Slot1<cls, ret, p1t>( pCls, pFnc );
		}
	
	private:
		cls *pCls;
		ret (cls::*pFnc)( p1t );
	};
	
	template<typename ret, typename p1t>
	class __Slot1F : public _Slot1<ret, p1t>
	{
	public:
		__Slot1F( ret (*pFnc)( p1t ) ) :
			pFnc( pFnc ) { }
		virtual ~__Slot1F() { }
	
		virtual ret operator()( p1t p1 )
		{
			return (*pFnc)( p1 );
		}
	
		virtual _Slot1<ret, p1t> *clone() const
		{
			return new __Slot1F<ret, p1t>( pFnc );
		}
	
	private:
		ret (*pFnc)( p1t );
	};
	
	template<typename ret, typename p1t>
	class Signal1
	{
	public:
		Signal1() : pCb( NULL ) { }
		Signal1( _Slot1<ret, p1t> *pCb ) : pCb( pCb ) { }
		Signal1( const Signal1<ret, p1t> &rSrc ) :
			pCb( (rSrc.pCb)?(rSrc.pCb->clone()):(NULL) ) { }
		virtual ~Signal1() { delete pCb; pCb = NULL; }
	
		ret operator()( p1t p1 )
		{
			if( !pCb ) throw SignalException("Uninitialized signal called.");
			return (*pCb)( p1 );
		}
	
		bool isSet() const { return pCb != NULL; }
		operator bool() const { return isSet(); }
	
		Signal1<ret, p1t> &operator=( const Signal1<ret, p1t> &rhs )
		{
			pCb = rhs.pCb->clone();
			return *this;
		}
	
	private:
		_Slot1<ret, p1t> *pCb;
	};
	
	template<typename cls, typename ret, typename p1t>
	Signal1<ret, p1t> slot( cls *pCls, ret (cls::*pFnc)( p1t ) )
	{
		if( !pCls || !pFnc ) throw SignalException("NULL pointer in slot().");
		return Signal1<ret, p1t>(
			new __Slot1<cls, ret, p1t>( pCls, pFnc )
			);
	}
	
	template<typename ret, typename p1t>
	Signal1<ret, p1t> slot( ret (*pFnc)( p1t ) )
	{
		if( !pFnc ) throw SignalException("NULL pointer in slot().");
		return Signal1<ret, p1t>(
			new __Slot1F<ret, p1t>( pFnc )
			);
	}
#endif  // BU_SIGNAL_PARAM_COUNT_1

#ifndef BU_SIGNAL_PARAM_COUNT_2
#define BU_SIGNAL_PARAM_COUNT_2
	//
	// 2 Parameter(s)
	//
	template<typename ret, typename p1t, typename p2t>
	class _Slot2
	{
	public:
		_Slot2() { }
		virtual ~_Slot2() { }
		virtual ret operator()( p1t p1, p2t p2 )=0;
		virtual _Slot2<ret, p1t, p2t> *clone() const=0;
	};
	
	template<typename cls, typename ret, typename p1t, typename p2t>
	class __Slot2 : public _Slot2<ret, p1t, p2t>
	{
	public:
		__Slot2( cls *pCls, ret (cls::*pFnc)( p1t, p2t ) ) :
			pCls( pCls ), pFnc( pFnc ) { }
		virtual ~__Slot2() { }
	
		virtual ret operator()( p1t p1, p2t p2 )
		{
			return (pCls->*pFnc)( p1, p2 );
		}
	
		virtual _Slot2<ret, p1t, p2t> *clone() const
		{
			return new __Slot2<cls, ret, p1t, p2t>( pCls, pFnc );
		}
	
	private:
		cls *pCls;
		ret (cls::*pFnc)( p1t, p2t );
	};
	
	template<typename ret, typename p1t, typename p2t>
	class __Slot2F : public _Slot2<ret, p1t, p2t>
	{
	public:
		__Slot2F( ret (*pFnc)( p1t, p2t ) ) :
			pFnc( pFnc ) { }
		virtual ~__Slot2F() { }
	
		virtual ret operator()( p1t p1, p2t p2 )
		{
			return (*pFnc)( p1, p2 );
		}
	
		virtual _Slot2<ret, p1t, p2t> *clone() const
		{
			return new __Slot2F<ret, p1t, p2t>( pFnc );
		}
	
	private:
		ret (*pFnc)( p1t, p2t );
	};
	
	template<typename ret, typename p1t, typename p2t>
	class Signal2
	{
	public:
		Signal2() : pCb( NULL ) { }
		Signal2( _Slot2<ret, p1t, p2t> *pCb ) : pCb( pCb ) { }
		Signal2( const Signal2<ret, p1t, p2t> &rSrc ) :
			pCb( (rSrc.pCb)?(rSrc.pCb->clone()):(NULL) ) { }
		virtual ~Signal2() { delete pCb; pCb = NULL; }
	
		ret operator()( p1t p1, p2t p2 )
		{
			if( !pCb ) throw SignalException("Uninitialized signal called.");
			return (*pCb)( p1, p2 );
		}
	
		bool isSet() const { return pCb != NULL; }
		operator bool() const { return isSet(); }
	
		Signal2<ret, p1t, p2t> &operator=( const Signal2<ret, p1t, p2t> &rhs )
		{
			pCb = rhs.pCb->clone();
			return *this;
		}
	
	private:
		_Slot2<ret, p1t, p2t> *pCb;
	};
	
	template<typename cls, typename ret, typename p1t, typename p2t>
	Signal2<ret, p1t, p2t> slot( cls *pCls, ret (cls::*pFnc)( p1t, p2t ) )
	{
		if( !pCls || !pFnc ) throw SignalException("NULL pointer in slot().");
		return Signal2<ret, p1t, p2t>(
			new __Slot2<cls, ret, p1t, p2t>( pCls, pFnc )
			);
	}
	
	template<typename ret, typename p1t, typename p2t>
	Signal2<ret, p1t, p2t> slot( ret (*pFnc)( p1t, p2t ) )
	{
		if( !pFnc ) throw SignalException("NULL pointer in slot().");
		return Signal2<ret, p1t, p2t>(
			new __Slot2F<ret, p1t, p2t>( pFnc )
			);
	}
#endif  // BU_SIGNAL_PARAM_COUNT_2

#ifndef BU_SIGNAL_PARAM_COUNT_3
#define BU_SIGNAL_PARAM_COUNT_3
	//
	// 3 Parameter(s)
	//
	template<typename ret, typename p1t, typename p2t, typename p3t>
	class _Slot3
	{
	public:
		_Slot3() { }
		virtual ~_Slot3() { }
		virtual ret operator()( p1t p1, p2t p2, p3t p3 )=0;
		virtual _Slot3<ret, p1t, p2t, p3t> *clone() const=0;
	};
	
	template<typename cls, typename ret, typename p1t, typename p2t, typename p3t>
	class __Slot3 : public _Slot3<ret, p1t, p2t, p3t>
	{
	public:
		__Slot3( cls *pCls, ret (cls::*pFnc)( p1t, p2t, p3t ) ) :
			pCls( pCls ), pFnc( pFnc ) { }
		virtual ~__Slot3() { }
	
		virtual ret operator()( p1t p1, p2t p2, p3t p3 )
		{
			return (pCls->*pFnc)( p1, p2, p3 );
		}
	
		virtual _Slot3<ret, p1t, p2t, p3t> *clone() const
		{
			return new __Slot3<cls, ret, p1t, p2t, p3t>( pCls, pFnc );
		}
	
	private:
		cls *pCls;
		ret (cls::*pFnc)( p1t, p2t, p3t );
	};
	
	template<typename ret, typename p1t, typename p2t, typename p3t>
	class __Slot3F : public _Slot3<ret, p1t, p2t, p3t>
	{
	public:
		__Slot3F( ret (*pFnc)( p1t, p2t, p3t ) ) :
			pFnc( pFnc ) { }
		virtual ~__Slot3F() { }
	
		virtual ret operator()( p1t p1, p2t p2, p3t p3 )
		{
			return (*pFnc)( p1, p2, p3 );
		}
	
		virtual _Slot3<ret, p1t, p2t, p3t> *clone() const
		{
			return new __Slot3F<ret, p1t, p2t, p3t>( pFnc );
		}
	
	private:
		ret (*pFnc)( p1t, p2t, p3t );
	};
	
	template<typename ret, typename p1t, typename p2t, typename p3t>
	class Signal3
	{
	public:
		Signal3() : pCb( NULL ) { }
		Signal3( _Slot3<ret, p1t, p2t, p3t> *pCb ) : pCb( pCb ) { }
		Signal3( const Signal3<ret, p1t, p2t, p3t> &rSrc ) :
			pCb( (rSrc.pCb)?(rSrc.pCb->clone()):(NULL) ) { }
		virtual ~Signal3() { delete pCb; pCb = NULL; }
	
		ret operator()( p1t p1, p2t p2, p3t p3 )
		{
			if( !pCb ) throw SignalException("Uninitialized signal called.");
			return (*pCb)( p1, p2, p3 );
		}
	
		bool isSet() const { return pCb != NULL; }
		operator bool() const { return isSet(); }
	
		Signal3<ret, p1t, p2t, p3t> &operator=( const Signal3<ret, p1t, p2t, p3t> &rhs )
		{
			pCb = rhs.pCb->clone();
			return *this;
		}
	
	private:
		_Slot3<ret, p1t, p2t, p3t> *pCb;
	};
	
	template<typename cls, typename ret, typename p1t, typename p2t, typename p3t>
	Signal3<ret, p1t, p2t, p3t> slot( cls *pCls, ret (cls::*pFnc)( p1t, p2t, p3t ) )
	{
		if( !pCls || !pFnc ) throw SignalException("NULL pointer in slot().");
		return Signal3<ret, p1t, p2t, p3t>(
			new __Slot3<cls, ret, p1t, p2t, p3t>( pCls, pFnc )
			);
	}
	
	template<typename ret, typename p1t, typename p2t, typename p3t>
	Signal3<ret, p1t, p2t, p3t> slot( ret (*pFnc)( p1t, p2t, p3t ) )
	{
		if( !pFnc ) throw SignalException("NULL pointer in slot().");
		return Signal3<ret, p1t, p2t, p3t>(
			new __Slot3F<ret, p1t, p2t, p3t>( pFnc )
			);
	}
#endif  // BU_SIGNAL_PARAM_COUNT_3

#ifndef BU_SIGNAL_PARAM_COUNT_4
#define BU_SIGNAL_PARAM_COUNT_4
	//
	// 4 Parameter(s)
	//
	template<typename ret, typename p1t, typename p2t, typename p3t, typename p4t>
	class _Slot4
	{
	public:
		_Slot4() { }
		virtual ~_Slot4() { }
		virtual ret operator()( p1t p1, p2t p2, p3t p3, p4t p4 )=0;
		virtual _Slot4<ret, p1t, p2t, p3t, p4t> *clone() const=0;
	};
	
	template<typename cls, typename ret, typename p1t, typename p2t, typename p3t, typename p4t>
	class __Slot4 : public _Slot4<ret, p1t, p2t, p3t, p4t>
	{
	public:
		__Slot4( cls *pCls, ret (cls::*pFnc)( p1t, p2t, p3t, p4t ) ) :
			pCls( pCls ), pFnc( pFnc ) { }
		virtual ~__Slot4() { }
	
		virtual ret operator()( p1t p1, p2t p2, p3t p3, p4t p4 )
		{
			return (pCls->*pFnc)( p1, p2, p3, p4 );
		}
	
		virtual _Slot4<ret, p1t, p2t, p3t, p4t> *clone() const
		{
			return new __Slot4<cls, ret, p1t, p2t, p3t, p4t>( pCls, pFnc );
		}
	
	private:
		cls *pCls;
		ret (cls::*pFnc)( p1t, p2t, p3t, p4t );
	};
	
	template<typename ret, typename p1t, typename p2t, typename p3t, typename p4t>
	class __Slot4F : public _Slot4<ret, p1t, p2t, p3t, p4t>
	{
	public:
		__Slot4F( ret (*pFnc)( p1t, p2t, p3t, p4t ) ) :
			pFnc( pFnc ) { }
		virtual ~__Slot4F() { }
	
		virtual ret operator()( p1t p1, p2t p2, p3t p3, p4t p4 )
		{
			return (*pFnc)( p1, p2, p3, p4 );
		}
	
		virtual _Slot4<ret, p1t, p2t, p3t, p4t> *clone() const
		{
			return new __Slot4F<ret, p1t, p2t, p3t, p4t>( pFnc );
		}
	
	private:
		ret (*pFnc)( p1t, p2t, p3t, p4t );
	};
	
	template<typename ret, typename p1t, typename p2t, typename p3t, typename p4t>
	class Signal4
	{
	public:
		Signal4() : pCb( NULL ) { }
		Signal4( _Slot4<ret, p1t, p2t, p3t, p4t> *pCb ) : pCb( pCb ) { }
		Signal4( const Signal4<ret, p1t, p2t, p3t, p4t> &rSrc ) :
			pCb( (rSrc.pCb)?(rSrc.pCb->clone()):(NULL) ) { }
		virtual ~Signal4() { delete pCb; pCb = NULL; }
	
		ret operator()( p1t p1, p2t p2, p3t p3, p4t p4 )
		{
			if( !pCb ) throw SignalException("Uninitialized signal called.");
			return (*pCb)( p1, p2, p3, p4 );
		}
	
		bool isSet() const { return pCb != NULL; }
		operator bool() const { return isSet(); }
	
		Signal4<ret, p1t, p2t, p3t, p4t> &operator=( const Signal4<ret, p1t, p2t, p3t, p4t> &rhs )
		{
			pCb = rhs.pCb->clone();
			return *this;
		}
	
	private:
		_Slot4<ret, p1t, p2t, p3t, p4t> *pCb;
	};
	
	template<typename cls, typename ret, typename p1t, typename p2t, typename p3t, typename p4t>
	Signal4<ret, p1t, p2t, p3t, p4t> slot( cls *pCls, ret (cls::*pFnc)( p1t, p2t, p3t, p4t ) )
	{
		if( !pCls || !pFnc ) throw SignalException("NULL pointer in slot().");
		return Signal4<ret, p1t, p2t, p3t, p4t>(
			new __Slot4<cls, ret, p1t, p2t, p3t, p4t>( pCls, pFnc )
			);
	}
	
	template<typename ret, typename p1t, typename p2t, typename p3t, typename p4t>
	Signal4<ret, p1t, p2t, p3t, p4t> slot( ret (*pFnc)( p1t, p2t, p3t, p4t ) )
	{
		if( !pFnc ) throw SignalException("NULL pointer in slot().");
		return Signal4<ret, p1t, p2t, p3t, p4t>(
			new __Slot4F<ret, p1t, p2t, p3t, p4t>( pFnc )
			);
	}
#endif  // BU_SIGNAL_PARAM_COUNT_4

#ifndef BU_SIGNAL_PARAM_COUNT_5
#define BU_SIGNAL_PARAM_COUNT_5
	//
	// 5 Parameter(s)
	//
	template<typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t>
	class _Slot5
	{
	public:
		_Slot5() { }
		virtual ~_Slot5() { }
		virtual ret operator()( p1t p1, p2t p2, p3t p3, p4t p4, p5t p5 )=0;
		virtual _Slot5<ret, p1t, p2t, p3t, p4t, p5t> *clone() const=0;
	};
	
	template<typename cls, typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t>
	class __Slot5 : public _Slot5<ret, p1t, p2t, p3t, p4t, p5t>
	{
	public:
		__Slot5( cls *pCls, ret (cls::*pFnc)( p1t, p2t, p3t, p4t, p5t ) ) :
			pCls( pCls ), pFnc( pFnc ) { }
		virtual ~__Slot5() { }
	
		virtual ret operator()( p1t p1, p2t p2, p3t p3, p4t p4, p5t p5 )
		{
			return (pCls->*pFnc)( p1, p2, p3, p4, p5 );
		}
	
		virtual _Slot5<ret, p1t, p2t, p3t, p4t, p5t> *clone() const
		{
			return new __Slot5<cls, ret, p1t, p2t, p3t, p4t, p5t>( pCls, pFnc );
		}
	
	private:
		cls *pCls;
		ret (cls::*pFnc)( p1t, p2t, p3t, p4t, p5t );
	};
	
	template<typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t>
	class __Slot5F : public _Slot5<ret, p1t, p2t, p3t, p4t, p5t>
	{
	public:
		__Slot5F( ret (*pFnc)( p1t, p2t, p3t, p4t, p5t ) ) :
			pFnc( pFnc ) { }
		virtual ~__Slot5F() { }
	
		virtual ret operator()( p1t p1, p2t p2, p3t p3, p4t p4, p5t p5 )
		{
			return (*pFnc)( p1, p2, p3, p4, p5 );
		}
	
		virtual _Slot5<ret, p1t, p2t, p3t, p4t, p5t> *clone() const
		{
			return new __Slot5F<ret, p1t, p2t, p3t, p4t, p5t>( pFnc );
		}
	
	private:
		ret (*pFnc)( p1t, p2t, p3t, p4t, p5t );
	};
	
	template<typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t>
	class Signal5
	{
	public:
		Signal5() : pCb( NULL ) { }
		Signal5( _Slot5<ret, p1t, p2t, p3t, p4t, p5t> *pCb ) : pCb( pCb ) { }
		Signal5( const Signal5<ret, p1t, p2t, p3t, p4t, p5t> &rSrc ) :
			pCb( (rSrc.pCb)?(rSrc.pCb->clone()):(NULL) ) { }
		virtual ~Signal5() { delete pCb; pCb = NULL; }
	
		ret operator()( p1t p1, p2t p2, p3t p3, p4t p4, p5t p5 )
		{
			if( !pCb ) throw SignalException("Uninitialized signal called.");
			return (*pCb)( p1, p2, p3, p4, p5 );
		}
	
		bool isSet() const { return pCb != NULL; }
		operator bool() const { return isSet(); }
	
		Signal5<ret, p1t, p2t, p3t, p4t, p5t> &operator=( const Signal5<ret, p1t, p2t, p3t, p4t, p5t> &rhs )
		{
			pCb = rhs.pCb->clone();
			return *this;
		}
	
	private:
		_Slot5<ret, p1t, p2t, p3t, p4t, p5t> *pCb;
	};
	
	template<typename cls, typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t>
	Signal5<ret, p1t, p2t, p3t, p4t, p5t> slot( cls *pCls, ret (cls::*pFnc)( p1t, p2t, p3t, p4t, p5t ) )
	{
		if( !pCls || !pFnc ) throw SignalException("NULL pointer in slot().");
		return Signal5<ret, p1t, p2t, p3t, p4t, p5t>(
			new __Slot5<cls, ret, p1t, p2t, p3t, p4t, p5t>( pCls, pFnc )
			);
	}
	
	template<typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t>
	Signal5<ret, p1t, p2t, p3t, p4t, p5t> slot( ret (*pFnc)( p1t, p2t, p3t, p4t, p5t ) )
	{
		if( !pFnc ) throw SignalException("NULL pointer in slot().");
		return Signal5<ret, p1t, p2t, p3t, p4t, p5t>(
			new __Slot5F<ret, p1t, p2t, p3t, p4t, p5t>( pFnc )
			);
	}
#endif  // BU_SIGNAL_PARAM_COUNT_5

#ifndef BU_SIGNAL_PARAM_COUNT_6
#define BU_SIGNAL_PARAM_COUNT_6
	//
	// 6 Parameter(s)
	//
	template<typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t, typename p6t>
	class _Slot6
	{
	public:
		_Slot6() { }
		virtual ~_Slot6() { }
		virtual ret operator()( p1t p1, p2t p2, p3t p3, p4t p4, p5t p5, p6t p6 )=0;
		virtual _Slot6<ret, p1t, p2t, p3t, p4t, p5t, p6t> *clone() const=0;
	};
	
	template<typename cls, typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t, typename p6t>
	class __Slot6 : public _Slot6<ret, p1t, p2t, p3t, p4t, p5t, p6t>
	{
	public:
		__Slot6( cls *pCls, ret (cls::*pFnc)( p1t, p2t, p3t, p4t, p5t, p6t ) ) :
			pCls( pCls ), pFnc( pFnc ) { }
		virtual ~__Slot6() { }
	
		virtual ret operator()( p1t p1, p2t p2, p3t p3, p4t p4, p5t p5, p6t p6 )
		{
			return (pCls->*pFnc)( p1, p2, p3, p4, p5, p6 );
		}
	
		virtual _Slot6<ret, p1t, p2t, p3t, p4t, p5t, p6t> *clone() const
		{
			return new __Slot6<cls, ret, p1t, p2t, p3t, p4t, p5t, p6t>( pCls, pFnc );
		}
	
	private:
		cls *pCls;
		ret (cls::*pFnc)( p1t, p2t, p3t, p4t, p5t, p6t );
	};
	
	template<typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t, typename p6t>
	class __Slot6F : public _Slot6<ret, p1t, p2t, p3t, p4t, p5t, p6t>
	{
	public:
		__Slot6F( ret (*pFnc)( p1t, p2t, p3t, p4t, p5t, p6t ) ) :
			pFnc( pFnc ) { }
		virtual ~__Slot6F() { }
	
		virtual ret operator()( p1t p1, p2t p2, p3t p3, p4t p4, p5t p5, p6t p6 )
		{
			return (*pFnc)( p1, p2, p3, p4, p5, p6 );
		}
	
		virtual _Slot6<ret, p1t, p2t, p3t, p4t, p5t, p6t> *clone() const
		{
			return new __Slot6F<ret, p1t, p2t, p3t, p4t, p5t, p6t>( pFnc );
		}
	
	private:
		ret (*pFnc)( p1t, p2t, p3t, p4t, p5t, p6t );
	};
	
	template<typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t, typename p6t>
	class Signal6
	{
	public:
		Signal6() : pCb( NULL ) { }
		Signal6( _Slot6<ret, p1t, p2t, p3t, p4t, p5t, p6t> *pCb ) : pCb( pCb ) { }
		Signal6( const Signal6<ret, p1t, p2t, p3t, p4t, p5t, p6t> &rSrc ) :
			pCb( (rSrc.pCb)?(rSrc.pCb->clone()):(NULL) ) { }
		virtual ~Signal6() { delete pCb; pCb = NULL; }
	
		ret operator()( p1t p1, p2t p2, p3t p3, p4t p4, p5t p5, p6t p6 )
		{
			if( !pCb ) throw SignalException("Uninitialized signal called.");
			return (*pCb)( p1, p2, p3, p4, p5, p6 );
		}
	
		bool isSet() const { return pCb != NULL; }
		operator bool() const { return isSet(); }
	
		Signal6<ret, p1t, p2t, p3t, p4t, p5t, p6t> &operator=( const Signal6<ret, p1t, p2t, p3t, p4t, p5t, p6t> &rhs )
		{
			pCb = rhs.pCb->clone();
			return *this;
		}
	
	private:
		_Slot6<ret, p1t, p2t, p3t, p4t, p5t, p6t> *pCb;
	};
	
	template<typename cls, typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t, typename p6t>
	Signal6<ret, p1t, p2t, p3t, p4t, p5t, p6t> slot( cls *pCls, ret (cls::*pFnc)( p1t, p2t, p3t, p4t, p5t, p6t ) )
	{
		if( !pCls || !pFnc ) throw SignalException("NULL pointer in slot().");
		return Signal6<ret, p1t, p2t, p3t, p4t, p5t, p6t>(
			new __Slot6<cls, ret, p1t, p2t, p3t, p4t, p5t, p6t>( pCls, pFnc )
			);
	}
	
	template<typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t, typename p6t>
	Signal6<ret, p1t, p2t, p3t, p4t, p5t, p6t> slot( ret (*pFnc)( p1t, p2t, p3t, p4t, p5t, p6t ) )
	{
		if( !pFnc ) throw SignalException("NULL pointer in slot().");
		return Signal6<ret, p1t, p2t, p3t, p4t, p5t, p6t>(
			new __Slot6F<ret, p1t, p2t, p3t, p4t, p5t, p6t>( pFnc )
			);
	}
#endif  // BU_SIGNAL_PARAM_COUNT_6

#ifndef BU_SIGNAL_PARAM_COUNT_7
#define BU_SIGNAL_PARAM_COUNT_7
	//
	// 7 Parameter(s)
	//
	template<typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t, typename p6t, typename p7t>
	class _Slot7
	{
	public:
		_Slot7() { }
		virtual ~_Slot7() { }
		virtual ret operator()( p1t p1, p2t p2, p3t p3, p4t p4, p5t p5, p6t p6, p7t p7 )=0;
		virtual _Slot7<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t> *clone() const=0;
	};
	
	template<typename cls, typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t, typename p6t, typename p7t>
	class __Slot7 : public _Slot7<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t>
	{
	public:
		__Slot7( cls *pCls, ret (cls::*pFnc)( p1t, p2t, p3t, p4t, p5t, p6t, p7t ) ) :
			pCls( pCls ), pFnc( pFnc ) { }
		virtual ~__Slot7() { }
	
		virtual ret operator()( p1t p1, p2t p2, p3t p3, p4t p4, p5t p5, p6t p6, p7t p7 )
		{
			return (pCls->*pFnc)( p1, p2, p3, p4, p5, p6, p7 );
		}
	
		virtual _Slot7<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t> *clone() const
		{
			return new __Slot7<cls, ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t>( pCls, pFnc );
		}
	
	private:
		cls *pCls;
		ret (cls::*pFnc)( p1t, p2t, p3t, p4t, p5t, p6t, p7t );
	};
	
	template<typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t, typename p6t, typename p7t>
	class __Slot7F : public _Slot7<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t>
	{
	public:
		__Slot7F( ret (*pFnc)( p1t, p2t, p3t, p4t, p5t, p6t, p7t ) ) :
			pFnc( pFnc ) { }
		virtual ~__Slot7F() { }
	
		virtual ret operator()( p1t p1, p2t p2, p3t p3, p4t p4, p5t p5, p6t p6, p7t p7 )
		{
			return (*pFnc)( p1, p2, p3, p4, p5, p6, p7 );
		}
	
		virtual _Slot7<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t> *clone() const
		{
			return new __Slot7F<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t>( pFnc );
		}
	
	private:
		ret (*pFnc)( p1t, p2t, p3t, p4t, p5t, p6t, p7t );
	};
	
	template<typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t, typename p6t, typename p7t>
	class Signal7
	{
	public:
		Signal7() : pCb( NULL ) { }
		Signal7( _Slot7<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t> *pCb ) : pCb( pCb ) { }
		Signal7( const Signal7<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t> &rSrc ) :
			pCb( (rSrc.pCb)?(rSrc.pCb->clone()):(NULL) ) { }
		virtual ~Signal7() { delete pCb; pCb = NULL; }
	
		ret operator()( p1t p1, p2t p2, p3t p3, p4t p4, p5t p5, p6t p6, p7t p7 )
		{
			if( !pCb ) throw SignalException("Uninitialized signal called.");
			return (*pCb)( p1, p2, p3, p4, p5, p6, p7 );
		}
	
		bool isSet() const { return pCb != NULL; }
		operator bool() const { return isSet(); }
	
		Signal7<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t> &operator=( const Signal7<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t> &rhs )
		{
			pCb = rhs.pCb->clone();
			return *this;
		}
	
	private:
		_Slot7<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t> *pCb;
	};
	
	template<typename cls, typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t, typename p6t, typename p7t>
	Signal7<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t> slot( cls *pCls, ret (cls::*pFnc)( p1t, p2t, p3t, p4t, p5t, p6t, p7t ) )
	{
		if( !pCls || !pFnc ) throw SignalException("NULL pointer in slot().");
		return Signal7<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t>(
			new __Slot7<cls, ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t>( pCls, pFnc )
			);
	}
	
	template<typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t, typename p6t, typename p7t>
	Signal7<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t> slot( ret (*pFnc)( p1t, p2t, p3t, p4t, p5t, p6t, p7t ) )
	{
		if( !pFnc ) throw SignalException("NULL pointer in slot().");
		return Signal7<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t>(
			new __Slot7F<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t>( pFnc )
			);
	}
#endif  // BU_SIGNAL_PARAM_COUNT_7

#ifndef BU_SIGNAL_PARAM_COUNT_8
#define BU_SIGNAL_PARAM_COUNT_8
	//
	// 8 Parameter(s)
	//
	template<typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t, typename p6t, typename p7t, typename p8t>
	class _Slot8
	{
	public:
		_Slot8() { }
		virtual ~_Slot8() { }
		virtual ret operator()( p1t p1, p2t p2, p3t p3, p4t p4, p5t p5, p6t p6, p7t p7, p8t p8 )=0;
		virtual _Slot8<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t> *clone() const=0;
	};
	
	template<typename cls, typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t, typename p6t, typename p7t, typename p8t>
	class __Slot8 : public _Slot8<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t>
	{
	public:
		__Slot8( cls *pCls, ret (cls::*pFnc)( p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t ) ) :
			pCls( pCls ), pFnc( pFnc ) { }
		virtual ~__Slot8() { }
	
		virtual ret operator()( p1t p1, p2t p2, p3t p3, p4t p4, p5t p5, p6t p6, p7t p7, p8t p8 )
		{
			return (pCls->*pFnc)( p1, p2, p3, p4, p5, p6, p7, p8 );
		}
	
		virtual _Slot8<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t> *clone() const
		{
			return new __Slot8<cls, ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t>( pCls, pFnc );
		}
	
	private:
		cls *pCls;
		ret (cls::*pFnc)( p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t );
	};
	
	template<typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t, typename p6t, typename p7t, typename p8t>
	class __Slot8F : public _Slot8<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t>
	{
	public:
		__Slot8F( ret (*pFnc)( p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t ) ) :
			pFnc( pFnc ) { }
		virtual ~__Slot8F() { }
	
		virtual ret operator()( p1t p1, p2t p2, p3t p3, p4t p4, p5t p5, p6t p6, p7t p7, p8t p8 )
		{
			return (*pFnc)( p1, p2, p3, p4, p5, p6, p7, p8 );
		}
	
		virtual _Slot8<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t> *clone() const
		{
			return new __Slot8F<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t>( pFnc );
		}
	
	private:
		ret (*pFnc)( p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t );
	};
	
	template<typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t, typename p6t, typename p7t, typename p8t>
	class Signal8
	{
	public:
		Signal8() : pCb( NULL ) { }
		Signal8( _Slot8<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t> *pCb ) : pCb( pCb ) { }
		Signal8( const Signal8<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t> &rSrc ) :
			pCb( (rSrc.pCb)?(rSrc.pCb->clone()):(NULL) ) { }
		virtual ~Signal8() { delete pCb; pCb = NULL; }
	
		ret operator()( p1t p1, p2t p2, p3t p3, p4t p4, p5t p5, p6t p6, p7t p7, p8t p8 )
		{
			if( !pCb ) throw SignalException("Uninitialized signal called.");
			return (*pCb)( p1, p2, p3, p4, p5, p6, p7, p8 );
		}
	
		bool isSet() const { return pCb != NULL; }
		operator bool() const { return isSet(); }
	
		Signal8<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t> &operator=( const Signal8<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t> &rhs )
		{
			pCb = rhs.pCb->clone();
			return *this;
		}
	
	private:
		_Slot8<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t> *pCb;
	};
	
	template<typename cls, typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t, typename p6t, typename p7t, typename p8t>
	Signal8<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t> slot( cls *pCls, ret (cls::*pFnc)( p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t ) )
	{
		if( !pCls || !pFnc ) throw SignalException("NULL pointer in slot().");
		return Signal8<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t>(
			new __Slot8<cls, ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t>( pCls, pFnc )
			);
	}
	
	template<typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t, typename p6t, typename p7t, typename p8t>
	Signal8<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t> slot( ret (*pFnc)( p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t ) )
	{
		if( !pFnc ) throw SignalException("NULL pointer in slot().");
		return Signal8<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t>(
			new __Slot8F<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t>( pFnc )
			);
	}
#endif  // BU_SIGNAL_PARAM_COUNT_8

#ifndef BU_SIGNAL_PARAM_COUNT_9
#define BU_SIGNAL_PARAM_COUNT_9
	//
	// 9 Parameter(s)
	//
	template<typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t, typename p6t, typename p7t, typename p8t, typename p9t>
	class _Slot9
	{
	public:
		_Slot9() { }
		virtual ~_Slot9() { }
		virtual ret operator()( p1t p1, p2t p2, p3t p3, p4t p4, p5t p5, p6t p6, p7t p7, p8t p8, p9t p9 )=0;
		virtual _Slot9<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t> *clone() const=0;
	};
	
	template<typename cls, typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t, typename p6t, typename p7t, typename p8t, typename p9t>
	class __Slot9 : public _Slot9<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t>
	{
	public:
		__Slot9( cls *pCls, ret (cls::*pFnc)( p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t ) ) :
			pCls( pCls ), pFnc( pFnc ) { }
		virtual ~__Slot9() { }
	
		virtual ret operator()( p1t p1, p2t p2, p3t p3, p4t p4, p5t p5, p6t p6, p7t p7, p8t p8, p9t p9 )
		{
			return (pCls->*pFnc)( p1, p2, p3, p4, p5, p6, p7, p8, p9 );
		}
	
		virtual _Slot9<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t> *clone() const
		{
			return new __Slot9<cls, ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t>( pCls, pFnc );
		}
	
	private:
		cls *pCls;
		ret (cls::*pFnc)( p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t );
	};
	
	template<typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t, typename p6t, typename p7t, typename p8t, typename p9t>
	class __Slot9F : public _Slot9<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t>
	{
	public:
		__Slot9F( ret (*pFnc)( p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t ) ) :
			pFnc( pFnc ) { }
		virtual ~__Slot9F() { }
	
		virtual ret operator()( p1t p1, p2t p2, p3t p3, p4t p4, p5t p5, p6t p6, p7t p7, p8t p8, p9t p9 )
		{
			return (*pFnc)( p1, p2, p3, p4, p5, p6, p7, p8, p9 );
		}
	
		virtual _Slot9<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t> *clone() const
		{
			return new __Slot9F<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t>( pFnc );
		}
	
	private:
		ret (*pFnc)( p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t );
	};
	
	template<typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t, typename p6t, typename p7t, typename p8t, typename p9t>
	class Signal9
	{
	public:
		Signal9() : pCb( NULL ) { }
		Signal9( _Slot9<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t> *pCb ) : pCb( pCb ) { }
		Signal9( const Signal9<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t> &rSrc ) :
			pCb( (rSrc.pCb)?(rSrc.pCb->clone()):(NULL) ) { }
		virtual ~Signal9() { delete pCb; pCb = NULL; }
	
		ret operator()( p1t p1, p2t p2, p3t p3, p4t p4, p5t p5, p6t p6, p7t p7, p8t p8, p9t p9 )
		{
			if( !pCb ) throw SignalException("Uninitialized signal called.");
			return (*pCb)( p1, p2, p3, p4, p5, p6, p7, p8, p9 );
		}
	
		bool isSet() const { return pCb != NULL; }
		operator bool() const { return isSet(); }
	
		Signal9<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t> &operator=( const Signal9<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t> &rhs )
		{
			pCb = rhs.pCb->clone();
			return *this;
		}
	
	private:
		_Slot9<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t> *pCb;
	};
	
	template<typename cls, typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t, typename p6t, typename p7t, typename p8t, typename p9t>
	Signal9<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t> slot( cls *pCls, ret (cls::*pFnc)( p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t ) )
	{
		if( !pCls || !pFnc ) throw SignalException("NULL pointer in slot().");
		return Signal9<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t>(
			new __Slot9<cls, ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t>( pCls, pFnc )
			);
	}
	
	template<typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t, typename p6t, typename p7t, typename p8t, typename p9t>
	Signal9<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t> slot( ret (*pFnc)( p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t ) )
	{
		if( !pFnc ) throw SignalException("NULL pointer in slot().");
		return Signal9<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t>(
			new __Slot9F<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t>( pFnc )
			);
	}
#endif  // BU_SIGNAL_PARAM_COUNT_9

#ifndef BU_SIGNAL_PARAM_COUNT_10
#define BU_SIGNAL_PARAM_COUNT_10
	//
	// 10 Parameter(s)
	//
	template<typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t, typename p6t, typename p7t, typename p8t, typename p9t, typename p10t>
	class _Slot10
	{
	public:
		_Slot10() { }
		virtual ~_Slot10() { }
		virtual ret operator()( p1t p1, p2t p2, p3t p3, p4t p4, p5t p5, p6t p6, p7t p7, p8t p8, p9t p9, p10t p10 )=0;
		virtual _Slot10<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t, p10t> *clone() const=0;
	};
	
	template<typename cls, typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t, typename p6t, typename p7t, typename p8t, typename p9t, typename p10t>
	class __Slot10 : public _Slot10<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t, p10t>
	{
	public:
		__Slot10( cls *pCls, ret (cls::*pFnc)( p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t, p10t ) ) :
			pCls( pCls ), pFnc( pFnc ) { }
		virtual ~__Slot10() { }
	
		virtual ret operator()( p1t p1, p2t p2, p3t p3, p4t p4, p5t p5, p6t p6, p7t p7, p8t p8, p9t p9, p10t p10 )
		{
			return (pCls->*pFnc)( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 );
		}
	
		virtual _Slot10<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t, p10t> *clone() const
		{
			return new __Slot10<cls, ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t, p10t>( pCls, pFnc );
		}
	
	private:
		cls *pCls;
		ret (cls::*pFnc)( p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t, p10t );
	};
	
	template<typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t, typename p6t, typename p7t, typename p8t, typename p9t, typename p10t>
	class __Slot10F : public _Slot10<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t, p10t>
	{
	public:
		__Slot10F( ret (*pFnc)( p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t, p10t ) ) :
			pFnc( pFnc ) { }
		virtual ~__Slot10F() { }
	
		virtual ret operator()( p1t p1, p2t p2, p3t p3, p4t p4, p5t p5, p6t p6, p7t p7, p8t p8, p9t p9, p10t p10 )
		{
			return (*pFnc)( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 );
		}
	
		virtual _Slot10<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t, p10t> *clone() const
		{
			return new __Slot10F<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t, p10t>( pFnc );
		}
	
	private:
		ret (*pFnc)( p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t, p10t );
	};
	
	template<typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t, typename p6t, typename p7t, typename p8t, typename p9t, typename p10t>
	class Signal10
	{
	public:
		Signal10() : pCb( NULL ) { }
		Signal10( _Slot10<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t, p10t> *pCb ) : pCb( pCb ) { }
		Signal10( const Signal10<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t, p10t> &rSrc ) :
			pCb( (rSrc.pCb)?(rSrc.pCb->clone()):(NULL) ) { }
		virtual ~Signal10() { delete pCb; pCb = NULL; }
	
		ret operator()( p1t p1, p2t p2, p3t p3, p4t p4, p5t p5, p6t p6, p7t p7, p8t p8, p9t p9, p10t p10 )
		{
			if( !pCb ) throw SignalException("Uninitialized signal called.");
			return (*pCb)( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 );
		}
	
		bool isSet() const { return pCb != NULL; }
		operator bool() const { return isSet(); }
	
		Signal10<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t, p10t> &operator=( const Signal10<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t, p10t> &rhs )
		{
			pCb = rhs.pCb->clone();
			return *this;
		}
	
	private:
		_Slot10<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t, p10t> *pCb;
	};
	
	template<typename cls, typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t, typename p6t, typename p7t, typename p8t, typename p9t, typename p10t>
	Signal10<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t, p10t> slot( cls *pCls, ret (cls::*pFnc)( p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t, p10t ) )
	{
		if( !pCls || !pFnc ) throw SignalException("NULL pointer in slot().");
		return Signal10<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t, p10t>(
			new __Slot10<cls, ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t, p10t>( pCls, pFnc )
			);
	}
	
	template<typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t, typename p6t, typename p7t, typename p8t, typename p9t, typename p10t>
	Signal10<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t, p10t> slot( ret (*pFnc)( p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t, p10t ) )
	{
		if( !pFnc ) throw SignalException("NULL pointer in slot().");
		return Signal10<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t, p10t>(
			new __Slot10F<ret, p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t, p9t, p10t>( pFnc )
			);
	}
#endif  // BU_SIGNAL_PARAM_COUNT_10

};

#endif
