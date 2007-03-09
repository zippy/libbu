#ifndef F_STRING_H
#define F_STRING_H

#include <stdint.h>
#include <memory>

/**
 *
 */
template< typename chr, typename chralloc = std::allocator<chr> >
class FString
{
public:
	FString() :
		pData( NULL ),
		pnRefs( NULL )
	{
	}

	virtual ~FString()
	{
	}

private:
	long nLength;
	chr *pData;
	uint32_t *pnRefs;
	chralloc aChars;
};

#endif
