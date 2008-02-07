/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include <string>
#include "bu/ito.h"
#include "bu/itoqueue.h"

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
			std::string *pStr = q.dequeue( true );
			if( pStr == NULL )
			{
				printf("Null received...\n");
			}
			else
			{
				printf("[%d] read:  %s\n", id, pStr->c_str() );
				delete pStr;
			}
			usleep( (int)(((double)rand())/((double)RAND_MAX)*2000000.0) );
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
			usleep( (int)(((double)rand())/((double)RAND_MAX)*2000000.0) );
			q.enqueue( new std::string( strbase ) );
			printf("[%d] write: %s\n", id, strbase );
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
	Writer *wr[5];
	Reader *rd[5];
	const char bob[][7]={
		{"Test 1"},
		{"Test 2"},
		{"Test 3"},
		{"Test 4"},
		{"Test 5"}
	};

	Bu::ItoQueue<std::string *> q;

	for( int j = 0; j < 5; j++ )
	{
		wr[j] = new Writer( q, j, bob[j] );
		rd[j] = new Reader( q, j );
	}

	for( int j = 0; j < 5; j++ )
	{
		rd[j]->start();
	}

	for( int j = 0; j < 5; j++ )
	{
		wr[j]->start();
	}

	for( int j = 0; j < 5; j++ )
	{
		rd[j]->join();
	}

	for( int j = 0; j < 5; j++ )
	{
		delete wr[j];
		delete rd[j];
	}

	return 0;
}

