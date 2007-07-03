#ifndef MULTILOGCHANNEL_H
#define MULTILOGCHANNEL_H

#include "multilog.h"

/**
 * The baseclass for any MultiLog output channel.  Any class that implements
 * all of these functions can be put in the log chain and will be sent
 * messages from active MultiLoggers.
 *@author Mike Buland
 */
class MultiLogChannel
{
public:
	/**
	 * Deconstruct a MultiLogChannel.
	 */
	virtual ~MultiLogChannel() {};

	/**
	 * Should perform any operations that need to take place in order to start
	 * the output of data into this channel.  This will be called once by the
	 * MultiLog when the MultiLogChannel is registered.
	 *@returns True means that everything can go as planned.  False means that
	 * the MultiLog should remove this channel from the list and delete it.
	 */
	virtual bool openLog() = 0;

	/**
	 * Should append a log entry to the long, by whatever means are necesarry.
	 *@param pEntry The LogEntry to append.
	 *@returns True means that everything can go as planned.  False means that
	 * the MultiLog should remove this channel from the list and delete it.
	 */
	virtual bool append( MultiLog::LogEntry *pEntry ) = 0;

	/**
	 * Should perform any operations that need to take place in order to safely
	 * close and cleanup the log.
	 *@returns True means that everything can go as planned.  False means that
	 * the MultiLog should remove this channel from the list and delete it.
	 */
	virtual bool closeLog() = 0;
};

#endif
