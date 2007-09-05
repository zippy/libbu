#ifndef BU_ARCHIVE_H
#define BU_ARCHIVE_H

#include <stdint.h>
#include <string>
#include "bu/archival.h"
#include "bu/stream.h"
#include <list>
#include "bu/hash.h"
#include "bu/list.h"

namespace Bu
{
	/**
	 * Provides a framework for serialization of objects and primitives.  The
	 * archive will handle any basic primitive, a few special types, like char *
	 * strings, as well as STL classes and anything that inherits from the
	 * Archival class.  Each Archive operates on a Stream, so you can send the
	 * data using an Archive almost anywhere.
	 *
	 * In order to use an Archive to store something to a file, try something
	 * like:
	 *@code
     * File sOut("output", "wb"); // This is a stream subclass
	 * Archive ar( sOut, Archive::save );
	 * ar << myClass;
	 @endcode
	 * In this example myClass is any class that inherits from Archival.  When
	 * the storage operator is called, the Archival::archive() function in the
	 * myClass object is called with a reference to the Archive.  This can be
	 * handled in one of two ways:
	 *@code
	 * void MyClass::archive( Archive &ar )
	 * {
	 * 	ar && sName && nAge && sJob;
	 * }
	 @endcode
	 * Here we don't worry about weather we're loading or saving by using the
	 * smart && operator.  This allows us to write very consistent, very simple
	 * archive functions that really do a lot of work.  If we wanted to do
	 * something different in the case of loading or saving we would do:
	 *@code
	 * void MyClass::archive( Archive &ar )
	 * {
	 * 	if( ar.isLoading() )
	 * 	{
	 * 		ar >> sName >> nAge >> sJob;
	 * 	} else
	 * 	{
	 * 		ar << sName << nAge << sJob;
	 * 	}
	 * }
	 @endcode
	 * Archive currently does not provide facility to make fully portable
	 * archives.  For example, it will not convert between endianness for you,
	 * nor will it take into account differences between primitive sizes on
	 * different platforms.  This, at the moment, is up to the user to ensure.
	 * One way of dealing with the latter problem is to make sure and use
	 * explicit primitive types from the stdint.h header, i.e. int32_t.
	 */
	class Archive
	{
	private:
		bool bLoading;
	public:
		bool isLoading();

		enum
		{
			load	=	true,
			save	=	false
		};
		
		Archive( Stream &rStream, bool bLoading );
		virtual ~Archive();
		virtual void close();

		virtual void write(const void *, int32_t);
		virtual void read(void *, int32_t);
		
		virtual Archive &operator<<(bool);
		virtual Archive &operator<<(int8_t);
		virtual Archive &operator<<(int16_t);
		virtual Archive &operator<<(int32_t);
		virtual Archive &operator<<(int64_t);
		virtual Archive &operator<<(uint8_t);
		virtual Archive &operator<<(uint16_t);
		virtual Archive &operator<<(uint32_t);
		virtual Archive &operator<<(uint64_t);
		virtual Archive &operator<<(long);
		virtual Archive &operator<<(float);
		virtual Archive &operator<<(double);
		virtual Archive &operator<<(long double);

		virtual Archive &operator>>(bool &);
		virtual Archive &operator>>(int8_t &);
		virtual Archive &operator>>(int16_t &);
		virtual Archive &operator>>(int32_t &);
		virtual Archive &operator>>(int64_t &);
		virtual Archive &operator>>(uint8_t &);
		virtual Archive &operator>>(uint16_t &);
		virtual Archive &operator>>(uint32_t &);
		virtual Archive &operator>>(uint64_t &);
		virtual Archive &operator>>(long &);
		virtual Archive &operator>>(float &);
		virtual Archive &operator>>(double &);
		virtual Archive &operator>>(long double &);
		
		virtual Archive &operator&&(bool &);
		virtual Archive &operator&&(int8_t &);
		virtual Archive &operator&&(int16_t &);
		virtual Archive &operator&&(int32_t &);
		virtual Archive &operator&&(int64_t &);
		virtual Archive &operator&&(uint8_t &);
		virtual Archive &operator&&(uint16_t &);
		virtual Archive &operator&&(uint32_t &);
		virtual Archive &operator&&(uint64_t &);
		virtual Archive &operator&&(float &);
		virtual Archive &operator&&(double &);
		virtual Archive &operator&&(long double &);

		uint32_t getID( const void *ptr );
		void assocPtrID( void **ptr, uint32_t id );
		void readID( const void *ptr, uint32_t id );

	private:
		Stream &rStream;
		uint32_t nNextID;
		Hash<uint32_t,uint32_t> hPtrID;
		Hash<uint32_t,List<void **> > hPtrDest;
	};

	Archive &operator<<(Archive &, class Bu::Archival &);
	Archive &operator>>(Archive &, class Bu::Archival &);
	//Archive &operator&&(Archive &s, class Bu::Archival &p);

	Archive &operator<<(Archive &, std::string &);
	Archive &operator>>(Archive &, std::string &);
	//Archive &operator&&(Archive &, std::string &);
	
	template<typename T> Archive &operator&&( Archive &ar, T &dat )
	{
		if( ar.isLoading() )
		{
			return ar >> dat;
		}
		else
		{
			return ar << dat;
		}
	}

	template<typename T> Archive &operator<<( Archive &ar, std::list<T> &l )
	{
		typename std::list<T>::size_type num = l.getSize();
		ar << num;
		for( typename std::list<T>::const_iterator i = l.begin(); i != l.end();
			 i++ )
		{
			ar << *i;
		}

		return ar;
	}

	template<typename T> Archive &operator>>( Archive &ar, std::list<T> &l )
	{
		typename std::list<T>::size_type num;
		ar >> num;

		l.resize( num );
		for( typename std::list<T>::const_iterator i = l.begin();
			 i != l.end(); i++ )
		{
			ar >> *i;
		}

		return ar;
	}

	template<typename key, typename value>
	Archive &operator<<( Archive &ar, Hash<key,value> &h )
	{
		ar << h.getSize();
		for( typename Hash<key,value>::iterator i = h.begin(); i != h.end(); i++ )
		{
			//std::pair<key,value> p = *i;
			ar << (i.getKey()) << (i.getValue());
		}

		return ar;
	}

	template<typename key, typename value>
	Archive &operator>>( Archive &ar, Hash<key,value> &h )
	{
		h.clear();
		uint32_t nSize;
		ar >> nSize;

		for( uint32_t j = 0; j < nSize; j++ )
		{
			key k; value v;
			ar >> k >> v;
			h.insert( k, v );
		}

		return ar;
	}

	template<typename value>
	Archive &operator<<( Archive &ar, List<value> &h )
	{
		ar << h.getSize();
		for( typename List<value>::iterator i = h.begin(); i != h.end(); i++ )
		{
			ar << (*i);
		}

		return ar;
	}

	template<typename value>
	Archive &operator>>( Archive &ar, List<value> &h )
	{
		h.clear();
		uint32_t nSize;
		ar >> nSize;

		for( uint32_t j = 0; j < nSize; j++ )
		{
			value v;
			ar >> v;
			h.append( v );
		}

		return ar;
	}	
}

#endif
