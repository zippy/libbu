/**\file http.h
 * Describe a Hyper Text Transfer Protocol processor.  This class will allow
 * any program to act as either an HTTP server, client, or both.  It contains
 * a number of additional helpers and subclasses.
 *@author Mike Buland
 */

#ifndef HTTP_H
#define HTTP_H

#include <iostream>
#include "connection.h"
#include "linkedlist.h"
#include "hashtable.h"

#define CR '\r' /**< The ASCII value of a Carrage Return */
#define LF '\n' /**< The ASCII value of a Line Feed */
#define CRLF CR LF /**< Combo of CR+LF for use in http */

/**
 * Macro to create combined http version codes.  This just makes processing a
 * little bit faster for the most part.
 *@param maj Major version number, between 0 and 15
 *@param min Minor version number, between 0 and 15
 *@returns A one byte combined version number suitable for use in switches.
 */
#define HTTPVER( maj, min )  ((maj<<4)|(min))

#define HTTP10  HTTPVER( 1, 0 ) /**< Combined version code for http 1.0 */
#define HTTP11  HTTPVER( 1, 1 ) /**< Combined version code for http 1.1 */

/**
 * This is the master HTTP processing class.  One instance handles one
 * transaction, in the future a different mechanism may be thought up, but for
 * now this means that you must create multiple objects to handle a single
 * connection that contains multiple requests.
 * In the constructor the Http class is given a connection object.  This object
 * should already be initialized and connected to whatever socket it wants to
 * be sending and receiving data to and from.  Once that's done you can call
 * parseRequest if you're acting as a server, or a variety of buildRequest
 * functions to create and send a request if you're a client.
 * Please note that this class does not provide any HTTP or extended format
 * processing systems, but will allow for mime types tables to be registered.
 *@author Mike Buland
 */
class Http
{
public:
	/**
	 * Create an Http object tied to an existing connection object.
	 *@param pConnection The live connection object to deal with.
	 */
	Http( Connection *pConnection );
	
	/**
	 * Standard Deconstructor.
	 */
	~Http();

	/**
	 * Perform all parsing needed to figure out what an HTTP client wants from
	 * us.  This will setup a number of properties in the Http object itself
	 * and has the possibility of setting one or more response states initially.
	 * These states should be checked for immediately after parsing to see if
	 * an appropriate error message should be generated.  These errors can
	 * include issues with protocol, data formats, or unknown versions of the
	 * protocol.
	 *@returns True means that all processing is finished, false means that
	 * the parseRequest function should be called again when more data is
	 * ready.  A return value of true does not indicate success, only that
	 * processing is finished, the getResponseStatus function should be called
	 * to see what status was set in the parse routine.  A 200 indicates that
	 * as far as the parser is concerned, everything when smoothly.  Otherwise
	 * it's your responsibility to build the appropriate error response body
	 * (like an html file) and send it as the response.
	 */
	bool parseRequest();

	/**
	 * Get a request type's internal Http object id based on the string
	 * representation.  These can be any HTTP/1.1 standard request type.
	 *@param sType The string that should be checked for type.  This is in all
	 * caps, just like if it came from the HTTP client, which is most often
	 * the case.
	 *@returns The numerical ID of the given request type.  Please note that
	 * HTTP/1.1 standard specifies that any string is valid here as long as
	 * the non-basic string is a request type understood by the serving
	 * software.  This means that anything that is non-standard will return
	 * a type reqExtension and not an error.  This is not a mistake.
	 */
	short getRequestType( const char *sType );

	/**
	 * Get the string representation of an Http object request type integer ID.
	 * This is used mainly for debugging to be sure the system has what we
	 * think it has.
	 *@param nType The integer ID of the request type to process.
	 *@returns The HTTP/1.1 string representation of that Http object ID code.
	 */
	const char *getRequestType( short nType );

	/**
	 * Returns the Http object request type ID code that is stored in the
	 * object by either the parseRequest function or use of the buildRequest
	 * functions.
	 *@returns The ID of the request type stored in the object.
	 */
	short getRequestType();

	/**
	 * Same as getRequestType, only returns the string representation.
	 *@returns The string representation of the request type ID stored in the
	 * object.
	 */
	const char *getRequestTypeStr();

	/**
	 * Sets the version of the request used by the system.  This will be used
	 * by parse request, but is also part of the buildRequest tool functions.
	 *@param nMajor The major version number.
	 *@param nMinor The minor version number.
	 */
	void setRequestVersion( unsigned char nMajor, unsigned char nMinor );

	/**
	 * Gets the major version number of the protocol used/to be used in this
	 * request.
	 *@returns The major version number of the request protocol.
	 */
	unsigned char getRequestMinorVer();

	/**
	 * Gets the minor version number of the protocol used/to be used in this
	 * request.
	 *@returns The minor version number of the request protocol.
	 */
	unsigned char getRequestMajorVer();

	/**
	 * Checks the stored request version against an internal table of supported
	 * protocol versions.
	 *@returns True if the protocol version is supported, false otherwise.
	 */
	bool checkRequestVer();
	
	/**
	 * Converts an arbitrary string to a new string object with space saving
	 * operations performed ala the HTTP/1.1 specs.  All leading and trailing
	 * whitespace is stripped, and all whitespace within the string is reduced
	 * to a single space char.
	 *@param sStr A pointer to the string data to process.
	 *@param nLen The length of the string to process.  Since this function is
	 * often called on stream data, there is no null terminator where we need
	 * one.  This is here for convinience so the data doesn't need to be hacked
	 * up or moved to an intermediate buffer.
	 *@returns A new string that may well be shorter than the original but that
	 * will have the same value as far as the HTTP/1.1 specs are concerned.
	 */
	std::string *convSpaceString( const char *sStr, int nLen );

	/**
	 * Gets a string pointer to the URI that was/is being requested.  This can
	 * be any RFC standard URI, with or without protocol and domain.
	 *@returns A pointer to the URI that was/is being requested.
	 */
	const char *getRequestURI();

	/**
	 * Set a new response status.  This status can be anything that the HTTP
	 * specs allow.  Other values are allowed as well, but beware, not all
	 * servers/clients will accept values that are not in the tables in this
	 * class.
	 *@param nStatus The status to set.
	 */
	void setResponseStatus( short nStatus );

	bool buildResponse( short nResponseCode=-1, const char *sResponse=NULL );
	void setResponseContent( const char *sMime, const char *sContent, int nLen );
	void setResponsePersistant( bool bPersistant );
	bool sendResponse();
	
	enum
	{
		reqOptions,
		reqGet,
		reqHead,
		reqPost,
		reqPut,
		reqDelete,
		reqTrace,
		reqConnect,
		reqExtension
	};

	enum
	{
		statusContinue = 100,
		statusSwitchProto = 101,

		statusOK = 200,
		statusCreated = 201,
		statusAccepted = 202,
		statusNonAuthInfo = 203,
		statusNoContent = 204,
		statusResetContent = 205,
		statusPartialContent = 206,
		
		statusMultiChoices = 300,
		statusMovedPermanently = 301,
		statusFound = 302,
		statusSeeOther = 303,
		statusNotModified = 304,
		statusUseProxy = 305,
		statusUnused = 306,
		statusTempRedirect = 307,

		statusBadRequest = 400,
		statusUnauthorized = 401,
		statusPaymentRequired = 402,
		statusForbidden = 403,
		statusNotFound = 404,
		statusMethodNotAllowed = 405,
		statusNotAcceptable = 406,
		statusProxyAuthRequired = 407,
		statusRequestTimeout = 408,
		statusConflict = 409,
		statusGone = 410,
		statusLengthRequired = 411,
		statusPreconditionFailed = 412,
		statusRequestEntityTooLarge = 413,
		statusRequestURITooLong = 414,
		statusUnsupportedMediaType = 415,
		statusRequestedRangeNotSatisfiable = 416,
		statusExpectationFailed = 417,

		statusInternalServerError = 500,
		statusNotImplemented = 501,
		statusBadGateway = 502,
		statusServiceUnavailable = 503,
		statusGatewayTimeout = 504,
		statusHTTPVersionNotSupported = 505
	};

private:
	Connection *pCon;
	unsigned char nParseState;

	short nReqType;
	std::string *pReqStr;
	std::string sReqURI;
	unsigned char cReqVersion;
	HashTable hReqHeader;
	LinkedList lStrings;
	
	std::string sServerStr;
	std::string sResMime;
	std::string sResContent;
	std::string sResStatusStr;
	bool bResPersistant;
	struct tm tResTime;
	short nResStatus;

	enum
	{
		parseInit,
		parseHeader,
		parseFinished
	};
};

#endif
