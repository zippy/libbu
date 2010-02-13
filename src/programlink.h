/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_PROGRAMLINK_H
#define BU_PROGRAMLINK_H

#include "bu/linkmessage.h"
#include "bu/programchain.h"

namespace Bu
{
	/**
	 * Program Link is the base class for any object that will be a piece of the
	 * main program chain loop.
	 */
	class ProgramLink
	{
	friend class Bu::ProgramChain;
	public:
		/**
		 * Construct a program link.
		 */
		ProgramLink();

		/**
		 * Deconstruct.
		 */
		virtual ~ProgramLink();

		/**
		 * Initialization code required for a link that wasn't performed in the
		 * constructor.
		 *@returns true if initialization was successful.  A false value will halt
		 * the chain.
		 */
		virtual bool init()=0;

		/**
		 * DeInitialization code that should happen, but doesn't belong in the
		 * destructor.
		 *@returns true means everything worked, false means failure, but is
		 * meaningless.
		 */
		virtual bool deInit()=0;

		/**
		 * Executed once per link per chain pass.  Contains the guts of the program.
		 *@returns true if everything went well.  A false value will halt the chain.
		 */
		virtual bool timeSlice()=0;

		/**
		 * This must be handled in order to process Instant Response Messages.
		 * This function should return null on all messages that it doesn't
		 * understand how to handle, and construct new messages to return to sender
		 * in the cases where it does understand.
		 *@param pMsgIn The message that must be processed.
		 *@returns Either a new message in cases where a response is required,
		 * or null if nothing needs to be done by this link.
		 */
		virtual LinkMessage *processIRM( LinkMessage *pMsgIn ) = 0;

		/**
		 * Broadcast a LinkMessage to all other links in the system.  Each other
		 * link will get a call of their processIRM function.  If the message gets
		 * a response then you will regain control immediately, otherwise the system
		 * will give all other Links a chance to respond before returning NULL.
		 *@param pMsgOut The message to broadcast.
		 *@returns The message response, or NULL if no Link understood your message.
		 */
		LinkMessage *sendIRM( LinkMessage *pMsgOut );

	private:
		/**
		 * Set which chain we're assosiated with.  This is how IRM messages make
		 * it out to the rest of the world.
		 *@param pNewChain A pointer to the containing program chain.
		 */
		void setChain( class ProgramChain *pNewChain );

		/**
		 * The pointer to the containing chain.
		 */
		class ProgramChain *pChain;
	/*
		void postMessage( LinkMessage *pMsg, int nLvl );
		LinkMessage *getMessage( int nLvl );

		enum
		{
			msgToChain,
			msgToLink
		};

	private:
		Queue qMsgToChain;
		Queue qMsgToLink;
	*/
	};
}

#endif
