#ifndef HTTP_GET_H
#define HTTP_GET_H

#include <stdint.h>
#include <string>
#include <list>
#include <utility>

#include "sbuffer.h"

class HttpGet
{
public:
	HttpGet();
	HttpGet( const std::string &url );
	virtual ~HttpGet();

	void setURL( const std::string &url );
	void addParam( const std::string &key, const std::string &value );
	void setUserAgent( const std::string &sUserAgent )
	{
		this->sUserAgent = sUserAgent;
	}
	void setHost( const std::string &sHost )
	{
		this->sHost = sHost;
	}

	std::string escape( const std::string &src );
	SBuffer *get();

private:
	std::string sProto;
	std::string sHost;
	std::string sPath;
	int nPort;
	std::string sUserAgent;
	typedef std::pair<std::string,std::string> StringPair;
	std::list<StringPair> lParams;
	static char hexcode[];

};

#endif
