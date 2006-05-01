/**@file
 * Describes the ConnectionMonitor class.
 */
#ifndef CONNECTIONMONITOR_H
#define CONNECTIONMONITOR_H

#include "connection.h"

/** Connection Monitor defines the base class of the objects that will be
  * notified whenever a connection is created or destroyed.
  *@author Mike Buland
  */
class ConnectionMonitor
{
public:
	/**
	 * This is only here for completeness.  It does nothing.
	 */
	ConnectionMonitor();

	/**
	 * This is only here for completeness.  It does nothing.
	 */
	virtual ~ConnectionMonitor();

	/** Receives the notification that new connection was received.
	  *@param pCon The connection that was created.
	  *@returns Should return a true value if everything is OK, a false to
	  * force a shutdown.
	  */
	virtual bool onNewConnection( Connection *pCon ) = 0;

	/** Receives the notification that a connection was closed.
	  *@param pCon The connection that was closed.
	  *@returns Should return a true value if everything is OK, a false to
	  * force a shutdown.
	  */
	virtual bool onClosedConnection( Connection *pCon ) = 0;
};

#endif
