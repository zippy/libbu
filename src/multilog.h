#ifndef MULTILOG_H
#define MULTILOG_H

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "ringlist.h"
#include "linkedlist.h"
#include "singleton.h"

/**
 * Calls the DetailLog function but includes pre-processor macros to fill in
 * most of the fields for you.  This makes your life a lot easier, and makes the
 * log useful for system debugging as well as just letting people know what's
 * going on.
 *@param LEVEL The log level, comes from an enum in the MultiLog class.
 *@param FORMAT The text to store in the log, using printf style formatting.
 *@param ... Parameters to help format the text in the FROMAT param.
 */
#define LineLog( LEVEL, FORMAT, ...) DetailLog( LEVEL, __FILE__, __LINE__, __PRETTY_FUNCTION__, FORMAT, ##__VA_ARGS__ )

#define MultiLineLog( LEVEL, FORMAT, ...) MultiLog::getInstance().DetailLog( LEVEL, __FILE__, __LINE__, __PRETTY_FUNCTION__, FORMAT, ##__VA_ARGS__ )

/** MultiLog keeps track of logfile info in a myriad of varieties, and is
  * easily configurable between them all.  It allows output to the standard
  * output, error output, files, networks, and streams, which includes memory
  * buffers.
  * MultiLog uses the singleton pattern to keep only a single instance of
  * the log.  Instead of instantiating a new copy, call the getLog method.
  *@author Mike Buland
  */
class MultiLog : public Singleton<MultiLog>
{
	friend class Singleton<MultiLog>;
public:
	/**
	 * Keeps track of a single log entry, in a standard format, that can be
	 * processed by any MultiLogChannel derrived class.
	 *@author Mike Buland
	 */
	typedef struct LogEntry
	{
		/** Safely delete a log entry. */
		~LogEntry();
		time_t xTime;	/**< The time the log entry was made. */
		int nLevel;		/**< The log-level of the entry. */
		char *lpFile;	/**< The name of the file this entry came from. */
		int nLine;		/**< The line number that this log came from. */
		char *lpText;	/**< The text content of this log entry. */
	} LogEntry;
	
protected:
	/**
	 * Private constructor, this ensures that this is a singleton.
	 */
	MultiLog();

	/**
	 * Destroy the multilog.
	 */
	~MultiLog();

	/**
	 * Append a new logentry to the log list, possibly pushing an old entry off.
	 *@param pEntry The new entry to append.
	 */
	void append( LogEntry *pEntry );

	/**
	 * The actual log entry storage mechanism.
	 */
	RingList *rEntry;

	/**
	 * The number of entries that have rolled off the end of the RingList.
	 */
	unsigned long nEntriesLost;

	/**
	 * A list of all channels that are registered with the MultiLog.
	 */
	LinkedList *lChannel;
	
public:

	/** Sends info to the logfile.
	  *@param nLevel The type of data being logged (error, info, etc.)
	  *@param lpFormat The data to send to the log.
	  *@author Mike Buland
	  */
	//void Log( int nLevel, const char *lpFormat, ...);

	/** Sends info to the logfile with extra information, including the files
	  * that it was called from and the line in the code.  Besides that, it's
	  * exactly the same as Log.  Please use the LineLog macro to make DetailLog
	  * really easy to use.  It operates exacly like Log, but inserts the
	  * builtin macros as the lpFile and nLine parameters.
	  *@param nLevel The type of data being logged (error, info, etc.)
	  *@param lpFile The name of the file that called the log function.
	  *@param nLine The line in the file that this log function was called from.
	  *@param lpFunction The name of the function that called the log function.
	  *@param lpFormat The data to send to the log.
	  *@author Mike Buland
	  */
	void DetailLog( int nLevel, const char *lpFile, int nLine, const char *lpFunction, const char *lpFormat, ...);

	/**
	 * Adds a logging channel to the MultiLog channel chain.  Every added
	 * channel will automatically receive a complete log of everything that
	 * happened before the channel was added as well as all future messages.
	 *@param pChannel A pointer to the pre-contructed channel object to add.
	 */
	void addChannel( class MultiLogChannel *pChannel );

	/** The various pre-defined levels available to use when logging.
	  * The person logging can make up their own, just make sure to remember
	  * which value is which (all levels are integers).
	  *@author Mike Buland
	  */
	enum Levels
	{
		LError,
		LWarning,
		LInfo,
		LStatus
	};
};

#endif
