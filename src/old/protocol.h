#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "connection.h"

/** This is the template for a class that handles specialized input and output
  * to connections of different types with different protocols.
  *@author Mike Buland
  */
class Protocol
{
public:
	/** Constructor */
    Protocol();
	/** Deconstructor */
    virtual ~Protocol();

	/**
	 * Function is called every time there is new data on the line.  This is
	 * called directly from the Connection class to process data.  This is not
	 * called whever there is pending data on the input, but every time new data
	 * is added to the input buffer.
	 *@returns True if processing went alright, false if something went wrong,
	 * I suppose.  In truth this value is thrown away right now.
	 *@todo Either make a return value of false mean something, or make these
	 * void.
	 */
	virtual bool onNewData()=0;

	/**
	 * Function is called when there is a new connection.  This should only
	 * happen once per Protocol object, but gives each protocol object a
	 * chance to perform connection handshaking and initialization at a point
	 * where they know that they have a handle to an active Connection.
	 *@returns See onNewData
	 */
	virtual bool onNewConnection()=0;

	virtual void onNewClientConnection(){};

	virtual void poll(){};

	/**
	 * Sets the Protocol's Connection object.  This is rather important, and
	 * handled usually by the ConnectionManager.
	 *@param pNewConnection The Connection object that this protocol will use to
	 * deal with the outside world.
	 */
	void setConnection( class Connection *pNewConnection );

	/**
	 * Get a pointer to this object's Connection object, or NULL if one was
	 * never set.  If used with the ConnectionManager that should never happen.
	 *@returns A pointer to the active Connection.
	 */
	Connection *getConnection();

private:
	class Connection *pConnection; /**< The pointer to the Connection. */
};

#endif
