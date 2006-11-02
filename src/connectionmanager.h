/**
 *@file
 * Contains the ConnectionManager.
 *@author Mike Buland
 */

#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include "multilog.h"
#include "connection.h"
#include "connectionmonitor.h"
#include <sys/types.h>
#include <list>
#include <map>

/** Manges incoming network connections as a server.  Creates and works with
  * Connection objects.  All operations are performed on TCP/IP v4 right now,
  * and on a single port, although any number of connections can be handled.
  *@author Mike Buland
  */
class ConnectionManager
{
public:
	/**
	 * Sets up the basics, like storage for the pool, and so on.  This does not
	 * actually start a server, bind to a port, or create a connection pool.
	 * That's all handled by startServer().
	 */
	ConnectionManager( int nInitPool=40 );

	/**
	 * Cleans up everything, and even clears out all still-connected Connection
	 * objects.
	 */
	virtual ~ConnectionManager();

	/**
	 * Starts a server socket and binds to it, listening for new connections.
	 *@param nPort The port to listen on.
	 *@param nInitPool The size of the initial connection pool.  This will
	 * grow automatically if necesarry.
	 *@returns True if the socket was bound to the port and serving was
	 * started.  False if there was a problem connecting to the port.
	 */
	bool startServer( int nPort );

	/**
	 * This is identicle to the simpler startServer function except that it
	 * will automatically try to connect multiple times in case the first
	 * attempt or two doesn't work for some reason.  Initially this was
	 * written to compensate for server sockets staying locked after they were
	 * closed for a while.
	 *@param nPort The port to listen on.
	 *@param nInitPool The size of the initial connection pool.  This will
	 * grow automatically if necesarry.
	 *@param nNumTries The maximum number of times to try to connect.
	 *@param nTimeout The amount of time to wait in-between connection
	 * attempts.
	 *@returns True if the socket was bound to the port and serving was
	 * started.  False if there was a problem connecting to the port.
	 */
	bool startServer( int nPort, int nNumTries, int nTimeout );

	/**
	 * Scans all open connections, halting the calling processes until data
	 * is received or nTimeout ms have gone by.  While waiting for the timeout
	 * to complete the process is placed into an idle mode.
	 *@param nTimeout The number of millisecconds to wait if there is nothing
	 * to actually do.
	 *@param bForceTimeout If set to true, this will force the scanner to wait
	 * for the timout to complete before returning, even if there was pending
	 * data.
	 */
	bool scanConnections( int nTimeout, bool bForceTimeout );

	/** Shutdown the server and all assosiated sockets.
	  *@returns True if every socket was closed without problem.
	  */
	bool shutdownServer();

	/** Sends a message directly to every connected port.
	  *@param lpData A null-terminated string of data to send.
	  *@param nExcludeSocket An optional socket to exclude from the broadcast.
	  *@returns True if every socket that should have gotten the message did.
	  */
	bool broadcastMessage( const char *lpData, int nExcludeSocket=-1 );

	/** Sets a monitor for the manager.  The monitor is sent notifications
	  * whenever a socket is connected, disconnected, or whenever an error
	  * occurs.
	  *@param pNewMonitor A pointer to a preconstructed ConnectionMonitor
	  */
	void setConnectionMonitor( ConnectionMonitor *pNewMonitor );

	void connect( const char *lpAddress, int nPort, int nProtocolPort );

private:
	/**
	 * Take care of the work of actually accepting a connection. This will
	 * accept the connection, set the initial modes, and add it to the master
	 * list of active connections, as well as fire off any messages that need
	 * to be handled by anything else.
	 *@param nSocket The handle of the listening socket that had an incoming
	 * connection.
	 *@returns True if everything worked, False otherwise.
	 */
	bool addConnection( int nSocket );

	/**
	 * Seraches the internal lists of connections for one with a specific
	 * socket.
	 *@param nSocket The socket the connection is using for communication.
	 * This is the unique socket and not the one that the connection was
	 * initially to.
	 *@returns NULL if no connection was found, otherwise a pointer to a live
	 * Connection object.
	 */
	Connection *findActiveConnection( int nSocket );

	/**
	 * Searches the connection pool for an object that isn't in use yet, and
	 * returns it, ready to be filled in and used.
	 *@returns An unused connection object ready for use.
	 *@todo Check this code over to insure that the pool grows appropriately
	 * when enough extra connections are detected.
	 */
	Connection *getInactiveConnection();

	std::map<int,int> sMasterSocket;
	//int nMasterSocket; /**< The listening or server socket. */
	fd_set fdActive; /**< The active socket set. */
	fd_set fdRead; /**< The sockets ready for a read. */
	fd_set fdWrite; /**< The sockets ready for a write. */
	fd_set fdException; /**< The sockets that have gotten errors. */
	std::list<Connection *> lInactive;	/**< The pool of inactive Connections */
	std::list<Connection *> lActive;	/**< The pool of active Connections */
	MultiLog &xLog;	/**< A handle to the active multilog. */
	
	/** The ConnectionMonitor to notify of new connections. */
	ConnectionMonitor *pMonitor; 
};

#endif
