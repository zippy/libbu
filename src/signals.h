#ifndef BU_OBJECT_SIGNALS_H
#define BU_OBJECT_SIGNALS_H

#include "bu/util.h"

namespace Bu
{
	//
	// 0 Parameters
	//

	template<typename ret>
	class _Slot0
	{
	public:
		_Slot0() { }
		virtual ~_Slot0() { }

		virtual ret operator()()=0;

		virtual _Slot0<ret> *clone() const=0;
	};

	template<typename cls, typename ret>
	class __Slot0 : public _Slot0<ret>
	{
	public:
		__Slot0( cls *pCls, ret (cls::*pFnc)() ) :
			pCls( pCls ), pFnc( pFnc ) { }
		virtual ~__Slot0() { }

		virtual ret operator()()
		{
			return (pCls->*pFnc)();
		}
		
		virtual _Slot0<ret> *clone() const
		{
			return new __Slot0<cls, ret>( pCls, pFnc );
		}
	
	private:
		cls *pCls;
		ret (cls::*pFnc)();
	};

	template<typename ret>
	class __Slot0F : public _Slot0<ret>
	{
	public:
		__Slot0F( ret (*pFnc)() ) :
			pFnc( pFnc ) { }
		virtual ~__Slot0F() { }

		virtual ret operator()()
		{
			return (*pFnc)();
		}
		
		virtual _Slot0<ret> *clone() const
		{
			return new __Slot0F<ret>( pFnc );
		}
	
	private:
		ret (*pFnc)();
	};
	
	template<typename ret>
	class Signal0
	{
	public:
		Signal0() : pCb( NULL ) { }
		Signal0( _Slot0<ret> *pCb ) : pCb( pCb ) { }
		Signal0( const Signal0<ret> &rSrc ) :
			pCb( rSrc.pCb->clone() ) { }

		virtual ~Signal0() { delete pCb; pCb = NULL; }

		ret operator()()
		{
			return (*pCb)();
		}

	private:
		_Slot0<ret> *pCb;
	};
	
	template<typename cls, typename ret>
	Signal0<ret> slot( cls *pCls, ret (cls::*pFnc)() )
	{
		return Signal0<ret>(
			new __Slot0<cls, ret>( pCls, pFnc )
			);
	}
	
	template<typename ret>
	Signal0<ret> slot( ret (*pFnc)() )
	{
		return Signal0<ret>(
			new __Slot0F<ret>( pFnc )
			);
	}

	//
	// 1 Parameter
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
		ret (*pFnc)( p1t p1 );
	};
	
	template<typename ret, typename p1t>
	class Signal1
	{
	public:
		Signal1() : pCb( NULL ) { }
		Signal1( _Slot1<ret, p1t> *pCb ) : pCb( pCb ) { }
		Signal1( const Signal1<ret, p1t> &rSrc ) :
			pCb( rSrc.pCb->clone() ) { }

		virtual ~Signal1() { delete pCb; pCb = NULL; }

		ret operator()( p1t p1 )
		{
			return (*pCb)( p1 );
		}

	private:
		_Slot1<ret, p1t> *pCb;
	};
	
	template<typename cls, typename ret, typename p1t>
	Signal1<ret, p1t> slot( cls *pCls, ret (cls::*pFnc)( p1t ) )
	{
		return Signal1<ret, p1t>(
			new __Slot1<cls, ret, p1t>( pCls, pFnc )
			);
	}
	
	template<typename ret, typename p1t>
	Signal1<ret, p1t> slot( ret (*pFnc)( p1t ) )
	{
		return Signal1<ret, p1t>(
			new __Slot1F<ret, p1t>( pFnc )
			);
	}

	//
	// 2 Parameters
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
			pCb( rSrc.pCb->clone() ) { }

		virtual ~Signal2() { delete pCb; pCb = NULL; }

		ret operator()( p1t p1, p2t p2 )
		{
			return (*pCb)( p1, p2 );
		}

	private:
		_Slot2<ret, p1t, p2t> *pCb;
	};
	
	template<typename cls, typename ret, typename p1t, typename p2t>
	Signal2<ret, p1t, p2t> slot( cls *pCls, ret (cls::*pFnc)( p1t, p2t ) )
	{
		return Signal2<ret, p1t, p2t>(
			new __Slot2<cls, ret, p1t, p2t>( pCls, pFnc )
			);
	}
	
	template<typename ret, typename p1t, typename p2t>
	Signal2<ret, p1t, p2t> slot( ret (*pFnc)( p1t, p2t ) )
	{
		return Signal2<ret, p1t, p2t>(
			new __Slot2F<ret, p1t, p2t>( pFnc )
			);
	}

	//
	// 3 Parameters
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
			pCb( rSrc.pCb->clone() ) { }

		virtual ~Signal3() { delete pCb; pCb = NULL; }

		ret operator()( p1t p1, p2t p2, p3t p3 )
		{
			return (*pCb)( p1, p2, p3 );
		}

	private:
		_Slot3<ret, p1t, p2t, p3t> *pCb;
	};
	
	template<typename cls, typename ret, typename p1t, typename p2t, typename p3t>
	Signal3<ret, p1t, p2t, p3t> slot( cls *pCls, ret (cls::*pFnc)( p1t, p2t, p3t ) )
	{
		return Signal3<ret, p1t, p2t, p3t>(
			new __Slot3<cls, ret, p1t, p2t, p3t>( pCls, pFnc )
			);
	}
	
	template<typename ret, typename p1t, typename p2t, typename p3t>
	Signal3<ret, p1t, p2t, p3t> slot( ret (*pFnc)( p1t, p2t, p3t ) )
	{
		return Signal3<ret, p1t, p2t, p3t>(
			new __Slot3F<ret, p1t, p2t, p3t>( pFnc )
			);
	}

	//
	// 4 Parameters
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
			pCb( rSrc.pCb->clone() ) { }

		virtual ~Signal4() { delete pCb; pCb = NULL; }

		ret operator()( p1t p1, p2t p2, p3t p3, p4t p4 )
		{
			return (*pCb)( p1, p2, p3, p4 );
		}

	private:
		_Slot4<ret, p1t, p2t, p3t, p4t> *pCb;
	};
	
	template<typename cls, typename ret, typename p1t, typename p2t, typename p3t, typename p4t>
	Signal4<ret, p1t, p2t, p3t, p4t> slot( cls *pCls, ret (cls::*pFnc)( p1t, p2t, p3t, p4t ) )
	{
		return Signal4<ret, p1t, p2t, p3t, p4t>(
			new __Slot4<cls, ret, p1t, p2t, p3t, p4t>( pCls, pFnc )
			);
	}
	
	template<typename ret, typename p1t, typename p2t, typename p3t, typename p4t>
	Signal4<ret, p1t, p2t, p3t, p4t> slot( ret (*pFnc)( p1t, p2t, p3t, p4t ) )
	{
		return Signal4<ret, p1t, p2t, p3t, p4t>(
			new __Slot4F<ret, p1t, p2t, p3t, p4t>( pFnc )
			);
	}

	//
	// 5 Parameters
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
			pCb( rSrc.pCb->clone() ) { }

		virtual ~Signal5() { delete pCb; pCb = NULL; }

		ret operator()( p1t p1, p2t p2, p3t p3, p4t p4, p5t p5 )
		{
			return (*pCb)( p1, p2, p3, p4, p5 );
		}

	private:
		_Slot5<ret, p1t, p2t, p3t, p4t, p5t> *pCb;
	};
	
	template<typename cls, typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t>
	Signal5<ret, p1t, p2t, p3t, p4t, p5t> slot( cls *pCls, ret (cls::*pFnc)( p1t, p2t, p3t, p4t, p5t ) )
	{
		return Signal5<ret, p1t, p2t, p3t, p4t, p5t>(
			new __Slot5<cls, ret, p1t, p2t, p3t, p4t, p5t>( pCls, pFnc )
			);
	}
	
	template<typename ret, typename p1t, typename p2t, typename p3t, typename p4t, typename p5t>
	Signal5<ret, p1t, p2t, p3t, p4t, p5t> slot( ret (*pFnc)( p1t, p2t, p3t, p4t, p5t ) )
	{
		return Signal5<ret, p1t, p2t, p3t, p4t, p5t>(
			new __Slot5F<ret, p1t, p2t, p3t, p4t, p5t>( pFnc )
			);
	}

};

#endif
