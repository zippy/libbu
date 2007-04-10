#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <stdint.h>
#include <string>
#include "archival.h"
#include "stream.h"
#include <list>

namespace Bu
{
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
