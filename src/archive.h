#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <stdint.h>
#include <string>
#include "archable.h"

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
		
		Archive( bool bLoading );
		virtual ~Archive();
		virtual void close()=0;

		virtual void write(const void *, int32_t)=0;
		virtual void read(void *, int32_t)=0;
		
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
	};

	Archive &operator<<(Archive &, class Bu::Archable &);
	Archive &operator>>(Archive &, class Bu::Archable &);
	//Archive &operator&&(Archive &s, class Bu::Archable &p);

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
}

#endif
