#include "bu/hash.h"
#include "bu/fstring.h"
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

Bu::FString genThing()
{
	Bu::FString bob;
	bob.append("ab ");
	bob += "cd ";
	bob += "efg";

    printf("---bob------\n%08X: %s\n", (unsigned int)bob.c_str(), bob.c_str() );
	return bob;
}

void thing( Bu::FString str )
{
	printf("Hey:  %s\n", str.c_str() );
}

void copyfunc( std::string temp )
{
	temp += "Hi";
}

void copyfunc( Bu::FString temp )
{
	temp += "Hi";
}

void doTimings()
{
	Bu::FString fs1, fs2;
	std::string ss1, ss2;
	double dStart, dEnd, tfs1, tfs2, tfs3, tss1, tss2, tss3;
	int nChars = 500000, nChunks=5000, nCopies=5000000, nChunkSize=1024*4;
	char *buf = new char[nChunkSize];
	memset( buf, '!', nChunkSize );

	printf("Timing Bu::FString single chars...\n");
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

	printf("Timing Bu::FString %d char chunks...\n", nChunkSize);
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
	printf("Timing Bu::FString copies...\n");
	dStart = getTime();
	for( int j = 0; j < nCopies; j++ ) Bu::FString stmp = fs2;
	dEnd = getTime();
	tfs3 = dEnd-dStart;

	printf("Timing std::string copies...\n");
	dStart = getTime();
	for( int j = 0; j < nCopies; j++ ) std::string stpm = ss2;
	dEnd = getTime();
	tss3 = dEnd-dStart;

	printf(
		"Results:       singles:        chunks:         copies:\n"
		"Bu::FString    %10.2f/s   %10.2f/s   %10.2f/s\n"
		"std::string    %10.2f/s   %10.2f/s   %10.2f/s\n",
		nChars/tfs1, nChunks/tfs2, nCopies/tfs3,
		nChars/tss1, nChunks/tss2, nCopies/tss3 );

	delete[] buf;
}

#define pem printf("---------\n%08X: %s\n%08X: %s\n", (unsigned int)str.c_str(), str.c_str(), (unsigned int)str2.c_str(), str2.c_str() );
int main( int argc, char *argv )
{
	Bu::FString fs1;
	for( int j = 0; j < 500000; j++ ) fs1 += (char)('a'+(j%26));
	return 0;

	Bu::FString str("th");

	str.prepend("Hello ");
	str.append("ere.");

	Bu::FString str2( str );
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
	
	printf("%d == %d\n", Bu::__calcHashCode( str ), Bu::__calcHashCode( str.c_str() ) );

	doTimings();
}

