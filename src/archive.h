#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <stdint.h>
#include <string>
#include "archival.h"
#include "stream.h"
#include <list>

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

	private:
		Stream &rStream;
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
		typename std::list<T>::size_type num = l.size();
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
}

#endif
