/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_VARIANT_H
#define BU_VARIANT_H

#include <bu/fstring.h>
#include <typeinfo>
#include <bu/membuf.h>
#include <bu/formatter.h>

namespace Bu
{
	class Formatter;
	class Variant;
	template<class t> class VariantType;

	class VariantTypeRoot
	{
	public:
		VariantTypeRoot();
		virtual ~VariantTypeRoot();

		virtual Bu::FString toString() const=0;
		virtual const std::type_info &getType() const=0;
		virtual VariantTypeRoot *clone() const=0;
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

		virtual Bu::FString toString() const
		{
			MemBuf mb;
			Formatter f( mb );
			f << data;
			return mb.getString();
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

	class Variant
	{
	public:
		Variant();
		Variant( const Variant &v );
		virtual ~Variant();

		bool isSet();
		Bu::FString toString() const;
		const std::type_info &getType() const;

		Variant &operator=( const Variant &rhs );

		template<class t>
		Variant &operator=( const t &rhs )
		{
			if( pCore && pCore->getType() != typeid(t) )
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

	template<class t>
	Bu::Formatter &operator<<( Bu::Formatter &f, const VariantType<t> &vt )
	{
		return f << vt.toString();
	}

	Bu::Formatter &operator<<( Bu::Formatter &f, const Variant &v );

	template<> Bu::FString VariantType<int>::toString() const;
	template<> Bu::FString VariantType<bool>::toString() const;
};

#endif
