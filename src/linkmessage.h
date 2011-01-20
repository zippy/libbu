/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

/**\file linkmessage.h
 */

#ifndef BU_LINKMESSAGE_H
#define BU_LINKMESSAGE_H

namespace Bu
{
	/**
	 * A message to be broadcast accross ProgramLinks in a ProgramChain.  Generally
	 * one would make a subclass of this in order to transmit more useful
	 * information, but sometimes it isn't necesarry.
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
}

#endif
