/**\file linkmessage.h
 */

#ifndef LINKMESSAGE_H
#define LINKMESSAGE_H

/**
 * A message to be broadcast accross ProgramLinks in a ProgramChain.  Generally
 * one would make a subclass of this in order to transmit more useful
 * information, but sometimes it isn't necesarry.
 *@author Mike Buland
 */
class LinkMessage
{
public:
	/**
	 * Construct a blank LinkMessage.
	 */
	LinkMessage() {};

	/**
	 * Deconstruct a LinkMessage.
	 */
	virtual ~LinkMessage();
	
	/**
	 * Create a LinkMessage object with a specific message assosiated with it
	 * to start with.
	 *@param nNewMsg The message to use in the Message object.
	 */
	LinkMessage( int nNewMsg );

	/**
	 * The message contained in the Message object.
	 */
	int nMsg;
};

#endif
