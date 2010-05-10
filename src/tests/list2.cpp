#include <bu/list.h>
#include <bu/sio.h>

using namespace Bu;

int main()
{
	List<int64_t> lInt;

	lInt.append( 512 );
	lInt.append( 1024 );
	lInt.append( 4096 );
	lInt.append( 12 );
	lInt.erase( 12 );
	lInt.append( 12 );
	lInt.erase( 12 );
	lInt.append( 12 );
}

