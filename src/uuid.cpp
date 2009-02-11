#include "bu/uuid.h"

Bu::Uuid::Uuid()
{
	clear();
}

Bu::Uuid::~Uuid()
{
}

#define msb( i ) (1<<(7-i))

void Bu::Uuid::clear()
{
	data[7] = msb(0);
}

