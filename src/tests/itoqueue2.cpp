/*
 * Copyright (C) 2007 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include <string>
#include "bu/ito.h"
#include "bu/itoqueue.h"
#include <errno.h>

class Reader : public Bu::Ito
{
public:
	Reader( Bu::ItoQueue<std::string *> &q, int id ) :
		q( q ),
		id( id )
	{
	}

	void *run()
	{
		for( int i = 0; i < 10; i++ )
		{
			std::string *pStr = q.dequeue( 0, 500000 );
			if( pStr == NULL )
			{
				printf("Null received...\n");
				i--;
			}
			else
			{
				printf("[%d] read:  %s\n", id, pStr->c_str() );
				delete pStr;
			}
		}

		return NULL;
	}

private:
	Bu::ItoQueue<std::string *> &q;
	int id;
};

class Writer : public Bu::Ito
{
public:
	Writer( Bu::ItoQueue<std::string *> &q, int id, const char *strbase ) :
		q( q ),
		strbase( strbase ),
		id( id )
	{
	}

	void *run()
	{
		for( int i = 0; i < 11; i++ )
		{
			sleep( 2 );
			printf("[%d] write: %s\n", id, strbase );
			q.enqueue( new std::string( strbase ) );
		}

		return NULL;
	}

private:
	Bu::ItoQueue<std::string *> &q;
	const char *strbase;
	int id;
};

int main()
{
	printf("ETIMEDOUT: %d\n", ETIMEDOUT );
	Bu::ItoQueue<std::string *> q;
	Writer wr( q, 0, "writer" );
	Reader rd( q, 0 );

	rd.start();
	wr.start();

	rd.join();
	wr.join();

	return 0;
}

