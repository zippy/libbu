#ifndef PARAM_PROC_H
#define PARAM_PROC_H

#include <stdint.h>
#include <string>
#include <list>
#include "staticstring.h"

class ParamProc
{
public:
	class ParamPtr
	{
	public:
		ParamPtr();
		ParamPtr( std::string *str );
		ParamPtr( uint64_t    *uint64  );
		ParamPtr( uint32_t    *uint32  );
		ParamPtr( uint16_t    *uint16  );
		ParamPtr( uint8_t     *uint8   );
		ParamPtr( int64_t     *int64   );
		ParamPtr( int32_t     *int32   );
		ParamPtr( int16_t     *int16   );
		ParamPtr( int8_t      *int8    );
		ParamPtr( float       *float32 );
		ParamPtr( double      *float64 );
		ParamPtr( long double *float96 );
		ParamPtr( bool        *bln     );

		enum
		{
			vtunset,
			vtstr,
			vtuint64,
			vtuint32,
			vtuint16,
			vtuint8,
			vtint64,
			vtint32,
			vtint16,
			vtint8,
			vtfloat32,
			vtfloat64,
			vtfloat96,
			vtbln,
		};
		ParamPtr &operator=( ParamPtr &ptr );
		ParamPtr &operator=( const char *str );

		bool isSet();

	private:
		int type;
		union
		{
			std::string *str;
			uint64_t *uint64;
			uint32_t *uint32;
			uint16_t *uint16;
			uint8_t *uint8;
			int64_t *int64;
			int32_t *int32;
			int16_t *int16;
			int8_t *int8;
			float *float32;
			double *float64;
			long double *float96;
			bool *bln;
		} val;
	};

	typedef int (ParamProc::*Proc)( int, char *[] );

	typedef struct ArgSpec
	{
		uint8_t nFlags;
		StaticString sWord;
		char cChar;
		Proc proc;
		ParamProc::ParamPtr val;
		StaticString sExtra;
		StaticString sDesc;
		StaticString sValue;
	} ArgSpec;

public:
	ParamProc();
	virtual ~ParamProc();
	
	void addParam( const char *lpWord, char cChar, Proc proc, ParamPtr val,
		const char *lpDesc=NULL, const char *lpExtra=NULL,
		const char *lpValue=NULL
		);
	void addParam( const char *lpWord, char cChar, Proc proc,
		const char *lpDesc=NULL, const char *lpExtra=NULL,
		const char *lpValue=NULL
		);
	void addParam( const char *lpWord, char cChar, ParamPtr val,
		const char *lpDesc=NULL, const char *lpExtra=NULL,
		const char *lpValue=NULL
		);

	void addParam( const char *lpWord, Proc proc, ParamPtr val,
		const char *lpDesc=NULL, const char *lpExtra=NULL,
		const char *lpValue=NULL
		);
	void addParam( const char *lpWord, Proc proc,
		const char *lpDesc=NULL, const char *lpExtra=NULL,
		const char *lpValue=NULL
		);
	void addParam( const char *lpWord, ParamPtr val,
		const char *lpDesc=NULL, const char *lpExtra=NULL,
		const char *lpValue=NULL
		);
	
	void addParam( char cChar, Proc proc, ParamPtr val,
		const char *lpDesc=NULL, const char *lpExtra=NULL,
		const char *lpValue=NULL
		);
	void addParam( char cChar, Proc proc,
		const char *lpDesc=NULL, const char *lpExtra=NULL,
		const char *lpValue=NULL
		);
	void addParam( char cChar, ParamPtr val,
		const char *lpDesc=NULL, const char *lpExtra=NULL,
		const char *lpValue=NULL
		);

	void process( int argc, char *argv[] );
	ArgSpec *checkWord( const char *arg );
	ArgSpec *checkLetr( const char arg );

private:
	std::list<ArgSpec *> lArg;
};

#define mkproc( cls ) static_cast<int (ParamProc::*)( int, char *[])>(&cls)

#endif
