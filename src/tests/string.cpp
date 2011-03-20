/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/hash.h"
#include "bu/string.h"
#include <sys/time.h>
#include <string>

#ifndef WIN32
inline double getTime()
{
	struct timeval tv;
	gettimeofday( &tv, NULL );
	return ((double)tv.tv_sec) + ((double)tv.tv_usec/1000000.0);
}
#else
#include "windows.h"
#include "winbase.h"
inline double getTime()
{
       uint32_t t = (uint32_t) GetTickCount();
       return (double) t / 1000.0;
}
#endif

Bu::String genThing()
{
	Bu::String bob;
	bob.append("ab ");
	bob += "cd ";
	bob += "efg";

    printf("---bob------\n%08tX: %s\n", (ptrdiff_t)bob.getStr(),
			bob.getStr() );
	return bob;
}

void thing( Bu::String str )
{
	printf("Hey:  %s\n", str.getStr() );
}

void copyfunc( std::string temp )
{
	temp += "Hi";
}

void copyfunc( Bu::String temp )
{
	temp += "Hi";
}

void doTimings()
{
	Bu::String fs1, fs2;
	std::string ss1, ss2;
	double dStart, dEnd, tfs1, tfs2, tfs3, tss1, tss2, tss3;
	int nChars = 50000000, nChunks=50000, nCopies=500000000, nChunkSize=1024*4;
	char *buf = new char[nChunkSize];
	memset( buf, '!', nChunkSize );

	printf("Timing Bu::String single chars...\n");
	dStart = getTime();
	for( int j = 0; j < nChars; j++ ) fs1 += (char)('a'+(j%26));
	fs1.getStr();
	dEnd = getTime();
	tfs1 = dEnd-dStart;

	printf("Timing std::string single chars...\n");
	dStart = getTime();
	for( int j = 0; j < nChars; j++ ) ss1 += (char)('a'+(j%26));
	ss1.c_str();
	dEnd = getTime();
	tss1 = dEnd-dStart;

	printf("Timing Bu::String %d char chunks...\n", nChunkSize);
	dStart = getTime();
	for( int j = 0; j < nChunks; j++ ) fs2.append(buf, nChunkSize);
	fs2.getStr();
	dEnd = getTime();
	tfs2 = dEnd-dStart;

	printf("Timing std::string %d char chunks...\n", nChunkSize);
	dStart = getTime();
	for( int j = 0; j < nChunks; j++ ) ss2.append(buf, nChunkSize);
	ss2.c_str();
	dEnd = getTime();
	tss2 = dEnd-dStart;

	fs2 = "Hello there.";
	ss2 = "Hello there.";
	printf("Timing Bu::String copies...\n");
	dStart = getTime();
	for( int j = 0; j < nCopies; j++ ) Bu::String stmp = fs2;
	dEnd = getTime();
	tfs3 = dEnd-dStart;

	printf("Timing std::string copies...\n");
	dStart = getTime();
	for( int j = 0; j < nCopies; j++ ) std::string stpm = ss2;
	dEnd = getTime();
	tss3 = dEnd-dStart;

	printf(
		"Results:          singles:              chunks:            copies:\n"
		"Bu::String    %15.2f/s   %15.2f/s   %15.2f/s\n"
		"std::string   %15.2f/s   %15.2f/s   %15.2f/s\n",
		nChars/tfs1, nChunks/tfs2, nCopies/tfs3,
		nChars/tss1, nChunks/tss2, nCopies/tss3 );

	delete[] buf;
}

#define pem printf("---------\n%08tX: %s\n%08tX: %s\n", (ptrdiff_t)str.getConstStr(), str.getConstStr(), (ptrdiff_t)str2.getConstStr(), str2.getConstStr() );
int main( )
{
//	Bu::String fs1;
//	for( int j = 0; j < 500000; j++ ) fs1 += (char)('a'+(j%26));
//	return 0;

	Bu::String str("th");

	str.prepend("Hello ");
	str.append("ere.");

	Bu::String str2( str );
	pem;
	str += "  What's up?";
	pem;
	str2 += "  How are you?";
	pem;
	str = str2;
	pem;

	str2 = genThing();
	pem;

	str = str2;
	pem;

	thing( str2 );
	thing("test.");
	
	printf("%d == %d\n", Bu::__calcHashCode( str ), Bu::__calcHashCode( str.getStr() ) );

	doTimings();

	return 0;
}

