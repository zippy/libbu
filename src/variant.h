/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_VARIANT_H
#define BU_VARIANT_H

#include <bu/string.h>
#include <typeinfo>
#include <bu/formatter.h>

namespace Bu
{
	class Formatter;
	class Variant;
	/** @cond DEVEL */
	template<class t> class VariantType;

	class VariantTypeRoot
	{
	public:
		VariantTypeRoot();
		virtual ~VariantTypeRoot();

		virtual const std::type_info &getType() const=0;
		virtual VariantTypeRoot *clone() const=0;
		virtual void format( Bu::Formatter &f ) const=0;
	};

	template<class t>
	class VariantType : public VariantTypeRoot
	{
		friend class Variant;
	private:
		VariantType()
		{
		}

		VariantType( const t &d ) : 
			data( d )
		{
		}

		VariantType( const VariantType<t> &vt ) :
			data( vt.data )
		{
		}

		virtual ~VariantType()
		{
		}

	public:
		t &getData()
		{
			return data;
		}
		
		const t &getData() const
		{
			return data;
		}

		virtual void format( Formatter &f ) const
		{
			f << data;
		}

		virtual const std::type_info &getType() const
		{
			return typeid( data );
		}

		VariantType<t> operator=( const t &rhs )
		{
			data = rhs;

			return *this;
		}

		virtual VariantTypeRoot *clone() const
		{
			return new VariantType<t>( *this );
		}

	private:
		t data;
	};
	/** @endcond */

	/**
	 * Store any data type and access it safely.  Variant gives you a way to
	 * pass arbitrary data types around without having to worry about what
	 * type a variable is.  It allows code to be easily extended and to manage
	 * data without having to know what type it is ahead of time.
	 *
	 * Because of the generic method that this class was implemented it may seem
	 * to have some drawbacks compared to other Variant classes you may have
	 * seen, however it is fairly easy to get it to do just about anything you
	 * may need.  It is also very low overhead.  On most compilers the class
	 * itself has only 3 words of overhead + the size of the variable you store
	 * in it.  And, since many parts of it are templatized they can often be
	 * optimized quite a bit.
	 */
	class Variant
	{
	friend Bu::Formatter &operator<<( Bu::Formatter &f, const Variant &v );
	public:
		Variant();
		Variant( const Variant &v );
		Variant( const char *t );
		template<class t>
		Variant( const t &v ) :
			pCore( new VariantType<t>() )
		{
			(*dynamic_cast<VariantType<t> *>(pCore)) = v;
		}
		virtual ~Variant();

		Bu::String toString() const;
		bool isSet() const;
		const std::type_info &getType() const;

		Variant &operator=( const Variant &rhs );

		template<class t>
		Variant &operator=( const t &rhs )
		{
			if( pCore ) // && pCore->getType() != typeid(t) )
			{
				delete pCore;
				pCore = NULL;
			}
			pCore = new VariantType<t>();
			(*dynamic_cast<VariantType<t> *>(pCore)) = rhs;
			return *this;
		}

		template<class t>
		t &get()
		{
			if( !pCore )
			{
				throw Bu::ExceptionBase("No data!");
			}
			if( pCore->getType() != typeid(t) )
			{
				throw Bu::ExceptionBase("Invalid type conversion.");
			}
			return dynamic_cast<VariantType<t> *>(pCore)->getData();
		}
		
		template<class t>
		t &get() const
		{
			if( !pCore )
			{
				throw Bu::ExceptionBase("No data!");
			}
			if( pCore->getType() != typeid(t) )
			{
				throw Bu::ExceptionBase("Invalid type conversion.");
			}
			return dynamic_cast<VariantType<t> *>(pCore)->getData();
		}
		
		template<class t>
		void set( const t &val )
		{
			if( pCore && pCore->getType() != typeid(t) )
			{
				delete pCore;
				pCore = NULL;
			}
			pCore = new VariantType<t>();
			(*dynamic_cast<VariantType<t> *>(pCore)) = val;
		}

		template<class t>
		bool isType() const
		{
			return pCore->getType() == typeid(t);
		}

		template<class t>
		operator t()
		{
			if( !pCore )
			{
				throw Bu::ExceptionBase("No data!");
			}
			if( pCore->getType() != typeid(t) )
			{
				throw Bu::ExceptionBase("Invalid type conversion.");
			}
			return dynamic_cast<VariantType<t> *>(pCore)->getData();
		}
		
		template<class t>
		operator t() const
		{
			if( !pCore )
			{
				throw Bu::ExceptionBase("No data!");
			}
			if( pCore->getType() != typeid(t) )
			{
				throw Bu::ExceptionBase("Invalid type conversion.");
			}
			return dynamic_cast<VariantType<t> *>(pCore)->getData();
		}

	private:
		VariantTypeRoot *pCore;
	};
/*
	template<class t>
	Bu::Formatter &operator<<( Bu::Formatter &f, const VariantType<t> &vt )
	{
		return f << vt.getData;
	}*/

	Bu::Formatter &operator<<( Bu::Formatter &f, const Variant &v );
};

#endif
