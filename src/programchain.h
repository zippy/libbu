/*
 * Copyright (C) 2007 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_PROGRAMCHAIN_H
#define BU_PROGRAMCHAIN_H

#include "bu/list.h"
#include "bu/linkmessage.h"

namespace Bu
{
	class ProgramLink;
	/**
	 * The Program Chain links together program "chunks" to more easily facilitate
	 * a generalized program loop with modular extensions.
	 *@author Mike Buland
	 */
	class ProgramChain
	{
	public:
		/**
		 * Construct an empty chain.
		 */
		ProgramChain();

		/**
		 * Destroy your chain.
		 */
		virtual ~ProgramChain();

		/**
		 * Adds a link to the end of the chain.
		 *@param pLink A pointer to the link to add to the chain.
		 *@returns True if adding the link was successful, otherwise false
		 *@author Mike Buland
		 */
		bool addLink( Bu::ProgramLink *pLink );

		/**
		 * Gets a link by name.
		 *@param lpName The name of the link you're looking for.  Every link has a
		 * name, apparently.
		 *@returns A pointer to the specified ProgramLink, or NULL if none were
		 * found matching your criteria.
		 *@author Mike Buland
		 */
		class ProgramLink *getLink( const char *lpName );

		/**
		 * Gets the very first link in the chain.
		 *@returns A pointer to the first link in the chain.
		 *@author Mike Buland
		 */
		class ProgramLink *getBaseLink();

		/**
		 * Runs through the chain once.  Useful if you want to have more control
		 * over the operation of the chain.
		 *@returns true if every link returned true.  If at least one link returns
		 * false, then returns false.
		 *@author Mike Buland
		 */
		bool execChainOnce();

		/**
		 * Enters the master chain loop, looping over the entire chain and
		 * executing every link's TimeSlice routine in order, over and over, until
		 * a link returns a false value.
		 *@returns False, always.  It returns true unless a link returned false,
		 * but loops until a link does return false.
		 *@author Mike Buland
		 **/
		bool enterChainLoop();

		/**
		 * Broadcasts an Immediate Response Message to all active links, save the
		 * sender.  Whatever link first responds with a non-null response message
		 * will have it's messages sent back to the broadcasting link as the returns
		 * of this function call.  Therefore it is very important that all message
		 * processing code is handled in a fairly timely fasion.
		 *@param pMsgOut The message to broadcast in hopes of a response.
		 *@param pSender The message that sent out the message and doesn't want to
		 * receive it's own message.  This should always just be "this".
		 *@returns The message that was returned by the first link to return a
		 * non-null response.  If all messages return null responses then this also
		 * returns null.  Please note that whoever calls this will be responsible
		 * for deleting the message returned by it, if non-null.
		 */
		class LinkMessage *broadcastIRM( LinkMessage *pMsgOut, ProgramLink *pSender );

	private:
		/**
		 * Shuts down all operation no matter what point in the operation we were.
		 */
		void emergencyShutdown();
		Bu::List<Bu::ProgramLink *> lLink; /**< The linked list that contains all of the links. */
	};
}


#endif
