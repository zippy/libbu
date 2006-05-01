#ifndef MULTILOGTEXT_H
#define MULTILOGTEXT_H

#include "multilogchannel.h"

/**
 * Simple MultiLogChannel that takes the logdata, formats it textually, and
 * writes it to a text device, either a file or the screen, yay!  This takes
 * the place of the old standard logging facility.
 * The entries in the format follow the standard printf % style, and are as
 * follows:
 * <ul>
 *  <li>%y - current year</li>
 *  <li>%m - current month</li>
 *  <li>%d - current day</li>
 *  <li>%h - current hour (24-hour format)</li>
 *  <li>%M - current minute</li>
 *  <li>%s - current seccond</li>
 *  <li>%l - Loglevel (numerical)</li>
 *  <li>%f - Filename</li>
 *  <li>%L - Line number</li>
 *  <li>%t - Full text of the log entry</li>
 * </ul>
 *@author Mike Buland
 */
class MultiLogText : public MultiLogChannel
{
public:
	/**
	 * Construct a MultiLogText object around a specific filename and format.
	 * The file named by sFileName will be opened for writting in text+append
	 * mode.  No existing data will be destroyed.
	 *@param sFileName The file to output log-data to.
	 *@param lpFormat The format using the above specifications to be used for
	 * every log entry.
	 */
	MultiLogText( const char *sFileName, const char *lpFormat );
	
	/**
	 * Construct a MultiLogText object around a specific file and format.
	 * The file descriptor passed in should describe an already opened and set-
	 * up file or device.  This could easily be a socket or stdout or stderr.
	 *@param nFileDesc The already opened descriptor to send data to.
	 *@param lpFormat The format using the above specifications to be used for
	 * every log entry.
	 */
	MultiLogText( int nFileDesc, const char *lpFormat );

	/**
	 * Destroy the object.
	 */
	~MultiLogText();
	
	bool openLog();
	bool append( MultiLog::LogEntry *pEntry );
	bool closeLog();

	/**
	 * Change the log format on the fly.
	 *@param lpFormat The new format to use for all future log entries.
	 *@returns True if everything was fine, false for catastrophic failure.
	 */
	bool setLogFormat( const char *lpFormat );

private:
	int nFD;	/**< The file descriptor we're writing to. */
	char *lpFormat;	/**< The format that we're using, converted for printf. */
};

#endif
