/*
 * Copyright (C) 2007 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_LOGGER_H
#define BU_LOGGER_H

#include "bu/singleton.h"
#include "bu/fstring.h"

namespace Bu
{
	/**
	 * Simple logging facility.  All output goes straight to stdout, unlike the
	 * old multi-log system.  Generally we expect any program complex enough to
	 * want to use this will have other facilities for processing the logging
	 * output, but if we need it we can add other output methods.
	 *
	 * Currently implemented as a singleton to avoid clutter with globals, you
	 * generally never want to use the logging system directly, it's annoying.
	 * Instead use the handy macros lineLog, setLogMask, setLogFormat, and
	 * setLogLevel.  They do all the real work for you.
	 *
	 * In the log format, you can specify extra information that will be written
	 * to the log with every message, and extras in printf style.  Use %X flags
	 * where X is one of the following:
	 * - L - Logging level of the log message (not the current mask)
	 * - y - Full year
	 * - m - Month
	 * - d - Day of month
	 * - h - Hour (24-hour format)
	 * - M - Minutes
	 * - s - Seconds 
	 * - f - Source file
	 * - l - Line number
	 * - F - function name
	 * - t - Text of message (usually important)
	 *
	 * You can include anything extra that you would like, a newline will always
	 * be added automatically, so no need to worry about that.  You can also
	 * include any extra printf style formatting that you would like, for
	 * example: "%h:%02M:%02s" for the time 4:02:09 instead of 4:2:9.
	 *
	 * It's generally handy to create an enum of values you use as levels during
	 * program execution (such as error, warning, info, debug, etc).  These
	 * levels should be treated as bitflags, and the most desirable messages,
	 * i.e. serious errors and the like should be low order (0x01), and the much
	 * less desirable messages, like debugging info, should be higher order
	 * (0xF0).  During operation you can then set either an explicit mask,
	 * selecting just the levels that you would like to see printed, or set the
	 * mask using the setLevel helper function, which simulates verbosity
	 * levels, enabling every flag lower order than the highest order set bit
	 * passed.  I.E. if you had the following enumerated levels:
	 * 
	 *@code
	 enum {
	 	logError	= 0x01,
		logWarning	= 0x02,
		logInfo		= 0x04,
		logDebug	= 0x08
	 };
	 @endcode
	 * And you set the mask with setMask( logInfo ) the only messages you would
	 * see are the ones catagorized logInfo.  However, if you used 
	 * setLevel( logInfo ) then you would see logInfo, logWarning, and logError
	 * type messages, since they are lower order.
	 */
	class Logger : public Bu::Singleton<Bu::Logger>
	{
		friend class Bu::Singleton<Bu::Logger>;
	private:
		Logger();
		virtual ~Logger();

	public:
		void log( uint32_t nLevel, const char *sFile, const char *sFunction, int nLine, const char *sFormat, ...);

		void setFormat( const Bu::FString &str );
		void setMask( uint32_t n );
		void setLevel( uint32_t n );
		uint32_t getMask();

		void hexDump( uint32_t nLevel, const char *sFile, const char *sFunction, int nLine, const void *pData, long nDataLen, const char *lpName );

	private:
		Bu::FString sLogFormat;
		uint32_t nLevelMask;
	};
}

/**
 * Use Bu::Logger to log a message at the given level and with the given message
 * using printf style formatting, and include extra data such as the current
 * file, line number, and function.
 */
#define lineLog( nLevel, sFrmt, ...) \
	Bu::Logger::getInstance().log( nLevel, __FILE__, __PRETTY_FUNCTION__, __LINE__, sFrmt, ##__VA_ARGS__ ) 

#define logHexDump( nLevel, pData, iSize, sName ) \
	Bu::Logger::getInstance().hexDump( nLevel, __FILE__, __PRETTY_FUNCTION__, __LINE__, pData, iSize, sName ) 

/**
 * Set the Bu::Logger logging mask directly.  See Bu::Logger::setMask for
 * details.
 */
#define setLogMask( nLevel ) \
	Bu::Logger::getInstance().setMask( nLevel ) 

/**
 * Set the Bu::Logger format. See Bu::Logger::setFormat for details.
 */
#define setLogFormat( sFrmt ) \
	Bu::Logger::getInstance().setFormat( sFrmt ) 

/**
 * Set the Bu::Logger logging mask simulating levels.  See Bu::Logger::setLevel
 * for details.
 */
#define setLogLevel( nLevel ) \
	Bu::Logger::getInstance().setLevel( nLevel ) 

#endif
