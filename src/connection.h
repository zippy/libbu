/**\file
 * Contains the Connection class.
 *@author Mike Buland
 */

#ifndef CONNECTION_H
#define CONNECTION_H

#include "multilog.h"
#include "flexbuf.h"
#include "protocol.h"

/** Represents a single connection on a network.  While these connections
  * may be treated more or less just like files, occasionally problems arise
  * when writing data at any time you feel like.  Therefore you run all your
  * data through a Connection, which buffers all data and makes sure no
  * buffers are exceeded and nothing inappropriate for the recipient of the
  * data is sent.
  *@author Mike Buland
  */
class Connection
{
public:
	/**
	 * Construct a blank and non-connected Connection.  The created object is
	 * not yet connected to anything, and most of the functions except open are
	 * unusable.
	 */
	Connection();
	
	/**
	 * Destroy the connection, clean up all pending data requests and close the
	 * contained socket.  This does not send out pending data, especially since
	 * such an operation could take considerable time, depending on the pending
	 * data and state of the receiving end.
	 */
	virtual ~Connection();

	/**
	 * Open a connection to a remote server.  This sets up this connection as
	 * a client instead of a server and does all of the work that needs to be
	 * done to actually open an INET_AF connection, which is a lot of work.
	 *@param sAddr The address to connect to.  This can be in any format
	 * normally understood by your system to be an address, ip, domain name,
	 * etc.
	 *@param nPort The port number to connect to on the remote server.
	 *@returns True if the connection was successful and everything is setup,
	 * false if there were any of a dozen errors and the connection is not set.
	 *@todo Make this function add log entries to a standard MultiLog if
	 * something goes wrong.
	 */
	bool open( const char *sAddr, int nPort );

	/** Append the given data to the output.  The data is presumed to be null
	  * terminated.  To put binary data into the stream, use the other
	  * appendOutput function.  This should be the only method used to
	  * communicate with the socket.
	  *@param lpOutput The data to add to the output queue.
	  *@param nSize How much data is in the lpOutput buffer.  If this value
	  * is -1 then the program treats lpOutput as a null-terminated string.
	  *@returns True if everything is ok, false otherwise.
	  */
	bool appendOutput( const char *lpOutput, int nSize=-1 );

	/**
	 * Append the character to the output.
	 *@param lOutput The character to add to the output queue.
	 *@returns True if everything is ok, false otherwise.
	 */
	bool appendOutput( const char lOutput );

	/**
	 * Append the short to the output.
	 *@param lOutput The short to add to the output queue.
	 *@returns True if everything is ok, false otherwise.
	 */
	bool appendOutput( const short lOutput );

	/**
	 * Append the int to the output.
	 *@param lOutput The int to add to the output queue.
	 *@returns True if everything is ok, false otherwise.
	 */
	bool appendOutput( const int lOutput );

	/**
	 * Append the long to the output.
	 *@param lOutput The long to add to the output queue.
	 *@returns True if everything is ok, false otherwise.
	 */
	bool appendOutput( const long lOutput );

	/**
	 * Append the float to the output.
	 *@param lOutput The float to add to the output queue.
	 *@returns True if everything is ok, false otherwise.
	 */
	bool appendOutput( const float lOutput );

	/**
	 * Append the double to the output.
	 *@param lOutput The double to add to the output queue.
	 *@returns True if everything is ok, false otherwise.
	 */
	bool appendOutput( const double lOutput );

	/**
	 * Append the unsigned char to the output.
	 *@param lOutput The unsigned char to add to the output queue.
	 *@returns True if everything is ok, false otherwise.
	 */
	bool appendOutput( const unsigned char lOutput );

	/**
	 * Append the unsigned short to the output.
	 *@param lOutput The unsigned short to add to the output queue.
	 *@returns True if everything is ok, false otherwise.
	 */
	bool appendOutput( const unsigned short lOutput );

	/**
	 * Append the unsigned int to the output.
	 *@param lOutput The unsigned int to add to the output queue.
	 *@returns True if everything is ok, false otherwise.
	 */
	bool appendOutput( const unsigned int lOutput );

	/**
	 * Append the unsigned long to the output.
	 *@param lOutput The unsigned long to add to the output queue.
	 *@returns True if everything is ok, false otherwise.
	 */
	bool appendOutput( const unsigned long lOutput );

	/**
	 * Writes all input data in the buffer in a dual-view ascii and hex display
	 * to a file.  There are a number of options that also help with debugging.
	 *@param lpPrefix Text to be added to the begining of every line written
	 * out.  The default is a blank string.
	 *@param fh The file to write the data to in text mode.  This is stdout by
	 * default, but could be any already open file handle.
	 *@param nBytesMax The maximum number of bytes to write to the output.  The
	 * amount of data can be overwhelming sometimes, so you can limit it.  The
	 * default value is -1, which is also unlimited.
	 */
	void printInputDebug( const char *lpPrefix="", FILE *fh=stdout, int nBytesMax=-1 );

	/**
	 * Writes all output data in the buffer in a dual-view ascii and hex display
	 * to a file.  There are a number of options that also help with debugging.
	 *@param lpPrefix Text to be added to the begining of every line written
	 * out.  The default is a blank string.
	 *@param fh The file to write the data to in text mode.  This is stdout by
	 * default, but could be any already open file handle.
	 *@param nBytesMax The maximum number of bytes to write to the output.  The
	 * amount of data can be overwhelming sometimes, so you can limit it.  The
	 * default value is -1, which is also unlimited.
	 */
	void printOutputDebug( const char *lpPrefix="", FILE *fh=stdout, int nBytesMax=-1 );

	/**
	 * This is the low-level generic function that is called by both
	 * printInputDebug and printOutputDebug.  It works effectively just like
	 * both of them, except that you can give it a raw pointer to the data to
	 * print out.  This probably doesn't belong in this class, but this was
	 * where I was when I needed it.
	 *@param pData A pointer to the data to write.  This is not treated as a
	 * null terminated string, so make sure that the nDataLen param is set
	 * properly.
	 *@param nDataLen The number of bytes that are in pData and that you want to
	 * see.
	 *@param lpName The name of the data, this is used in the header where it
	 * says "Displaying nnn bytes of <lpName>."  A good example would be input
	 * or output.
	 *@param lpPrefix Text to put before every line output.  This just makes it
	 * easier to tell large blocks apart in the output.
	 *@param fh The file handle to write all data to.
	 *@param nBytesMax The maximum number of bytes.  This parameter is stupid.
	 * If it is set to -1, then nDataLen is used, otherwise the smaller value is
	 * used as the number of bytes to output.
	 *@todo Put this function somewhere more deserving.
	 *@todo Remove the nBytesMax param, we need that in the other functions,
	 * not this one!
	 */
	void printDataDebug( const unsigned char *pData, long nDataLen, const char *lpName, const char *lpPrefix, FILE *fh, int nBytesMax );

	/** Append the given data to the input.  The data is presumed to be null
	  * terminated.  To put binary data into the stream, use the other
	  * appendInput function.  This is mainly used by internal routines.
	  *@param lpInput The data to add to the input queue.
	  *@param nSize How much data is in the lpInput buffer.  If this value
	  * is -1 then the program treats lpOutput as a null-terminated string.
	  *@returns True if everything is ok, false otherwise.
	  */
	bool appendInput( const char *lpInput, int nSize=-1 );

	/** Searches through the current pending input for a certain character.
	  * This is useful for finding out where exactly the end of a line is, for
	  * example, to see if a command has been entered yet.
	  *@param cTarget The character to search for.
	  *@returns The position of the target relative to the begining of the input
	  * or -1 if the target wasn't found.
	  */
	int scanInputFor( char cTarget );

	/** Gets a pointer to the output buffer.  This is mainly used by internal
	  * routines, and is cleared every click when data is sent out again.
	  *@returns A pointer to the buffer holding the pending output data.
	  */
	const char *getOutput();

	/** Gets a pointer to the start of the input buffer's active data
	  * section.  Use this to gain access to the input you need to do
	  * your job.
	  *@returns A pointer to the data in the input buffer.  Do not delete this.
	  */
	const char *getInput();

	/** Clears all pending output, this is mainly just used internally.
	  *@returns True if operation was a success, otherwise false.
	  */
	bool clearOutput();

	/** Clears all pending input, weather it's been used or not.  Please
	  * refrain from calling this during normal operation, use usedInput
	  * instead, it's much safer.
	  *@returns True if the operation was a success, false otherwise.
	  */
	bool clearInput();

	/** Sets the socket that should be used internally.
	  *@param nNewSocket The new socket to work with.
	  */
	void setSocket( int nNewSocket );

	/** Gets the handle (number) of the working socket.  This can be a
	  * dangerous function to call, please refrain from calling it directly
	  * if any alternative can be found.
	  *@returns The number of the working socket.
	  */
	int getSocket();

	/** Determines if the connection is still active.
	  *@returns True if the connection is active, false otherwise.
	  */
	bool isActive();

	/** Clears all buffers and sets up the connection to be reused.
	  * Does not actually close the socket, that's handled by the
	  * ConnectionManager
	  */
	void close();

	/** Opens a socket.  Really just sets up the connection for use since
	  * the socket itself was created and opened by the ConnectionManager.
	  * This also calls setSocket so you don't have to.
	  *@param nNewSocket The socket to assosiate with.
	  */
	bool open( int nNewSocket );

	/**
	 * Reads all pending input from the connection.  If this is called outside
	 * of the ConnectionManager it will usually block indefinately waiting for
	 * new data.  The only way to change this behaviour is to modify the socket
	 * low-level when you connect it manually, or, preferably use the other
	 * readInput function to control blocking time.
	 *@returns True socket is still connected, otherwise false.
	 */
	int readInput();

	/** 
	 * Reads all pending input from the connection, blocking up to nSec
	 * seconds and nUSec micro-seconds for the data.  This uses select to
	 * simulate blocking, but has the same effect as standard io blocking.
	 * If you don't want to block, just set both values to zero.  The back
	 * parameters are optional, set to null to not use them.  The variables
	 * you pass in through the back parameters will contain the remaining
	 * time if data arrived before the max timeout was reached.
	 *@param nSec Max seconds to wait.
	 *@param nUSec Max micro-seconds to wait.
	 *@param pnSecBack The number of seconds remaining.
	 *@param pnUSecBack The number of micro-seconds remaining.
	 */
	bool readInput( int nSec, int nUSec, int *pnSecBack=NULL, int *pnUSecBack=NULL );

	/**
	 * Waits until at least nBytesIn are read into the input buffer and ready
	 * to be used.  Wait at most nSec seconds plus nUSec micro seconds.
	 * If the timeout is exceeded, this function throws an exception.  If this
	 * function returns normally, you are guranteed to have at least nBytesIn
	 * bytes in your input buffer.
	 *@param nBytesIn Number of bytes to read.
	 *@param nSec The max seconds to wait.
	 *@param sUSec The max microseconds to wait.
	 */
	void waitForInput( int nBytesIn, int nSec, int nUSec );

	/** Writes all data that is pending to the socket.
	  *@returns True if all data was written succesfully, false otherwise.
	  */
	bool writeOutput();

	/** Determines if the connection has output waiting to go out.
	  *@returns true if there is pending output, otherwise false.
	  */
	bool hasOutput();

	/** Sets internal flags so that this connection will be deleted next
	  * time through the ConnectionManager.
	  */
	void disconnect();

	/** Determines if this connection is ready to be disconnected or not.
	  *@returns True if it is time to disconnect, false if it isn't.
	  */
	bool needDisconnect();

	/** Tells the caller if there is pending input waiting to be processed.
	  *@returns True if there is pending input that has not been used, returns
	  * false if there isn't.
	  */
	bool hasInput();

	/** Removes bytes from the begining of the input queue.  Use this after
	  * getting the input and processing as much as you need to.
	  *@param nAmount The number of bytes used.
	  *@returns true if the update was successful, otherwise false.
	  */
	bool usedInput( int nAmount );

	/** Sets the protocol to be used by this connection.  All data in and out
	  * passes through the protocol object, which may process that data to
	  * filter out and process any special messages that may have been
	  * included.  Everything that isn't processed can be accessed in the
	  * standard method.
	  *@param pNewProtocol A pointer to a protocol object that you want to
	  * use.
	  */
	void setProtocol( class Protocol *pNewProtocol );

	/** Gets the number of bytes that are waiting in the input queue, the data
	  * that has yet to be processed.
	  *@returns The number of bytes in the input queue.
	  */
	int getInputAmnt();

	/** Gets the number of bytes that are waiting in the output queue, the data
	  * that has yet to be sent to the connected socket.
	  *@returns The number of bytes in the input queue.
	  */
	int getOutputAmnt();
	
	/** Gets a pointer to the protocol that is attatched to this connection
	  * object.  This is useful to set modes, and send special commands in
	  * addition to the standard raw data reads and writes that are normally
	  * permitted.  In fact, in everything besides a raw telnet protocol all
	  * data should be sent through the protocol and not the connection object.
	  *@returns A pointer to the Protocol assosiated with this connection.
	  */
	class Protocol *getProtocol();

private:
	/**
	 * A buffer to keep data read from the socket in.  This is filled in by
	 * the function readInput, which is automatically called by the
	 * ConnectionManager whenever new data is ready.
	 */
	FlexBuf xInputBuf;

	/**
	 * A buffer to keep data that should be sent to the socket.  This is filled
	 * in by using the AppendOutput functions and is sent to the socket using
	 * the writeOutput function, which is automatically called every cycle by
	 * the ConnectionManager when there is pending data.
	 */
	FlexBuf xOutputBuf;

	/**
	 * The socket that the user is connected to.  This is not the same as the
	 * socket number of the listening socket, this is the unique socket on the
	 * system that the data is coming to.
	 */
	int nSocket;

	/**
	 * True=active connection, False=connection lost
	 */
	bool bActive;

	/**
	 * True=disconnect next cycle (after data is transmitted), Flse=keep going.
	 */
	bool bDisconnectMe;

	/**
	 * A pointer to a protocol handler that can automatically process the data
	 * in the buffers.  This is optional if you use the connections on your own
	 * but reccomended if you use this with the rest of the ConnectionManager
	 * system.
	 */
	class Protocol *pProtocol;
};

#endif
