#include <bu/string.h>
#include <bu/regexengine.h>
#include <bu/sio.h>

using namespace Bu;

void compile( const Bu::String &s, Bu::RegExEngine<char> &ree )
{
	int iRoot = ree.addState();
	int iCur = iRoot;
	for( Bu::String::const_iterator i = s.begin(); i; i++ )
	{
		int iNext = -1;
		if( i+1 )
			iNext = ree.addState();
		ree.addCompletion( iCur, *i, *i, iNext );
		iCur = iNext;
	}
}

int main()
{
	Bu::String sRegEx("abcd");
	Bu::String sMatch("abcdefg");

	Bu::RegExEngine<char> ree;

	compile( sRegEx, ree );

	bool bRet;
	int iSize, iCompletion;
	bRet = ree.match( sMatch, iSize, iCompletion );

	sio << "Matched: " << bRet << sio.nl
		<< "Size: " << iSize << sio.nl
		<< "Completion: " << iCompletion << sio.nl;

	return 0;
}

