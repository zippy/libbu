#include "bu/archivebase.h"

Bu::ArchiveBase::ArchiveBase()
{
}

Bu::ArchiveBase::~ArchiveBase()
{
}

Bu::ArchiveBase &Bu::operator<<( Bu::ArchiveBase &ar, bool p)
{
	ar.write( &p, sizeof(p) );
	return ar;
}

Bu::ArchiveBase &Bu::operator<<( Bu::ArchiveBase &ar, char p)
{
	ar.write( &p, sizeof(p) );
	return ar;
}

Bu::ArchiveBase &Bu::operator<<( Bu::ArchiveBase &ar, signed char p)
{
	ar.write( &p, sizeof(p) );
	return ar;
}

Bu::ArchiveBase &Bu::operator<<( Bu::ArchiveBase &ar, unsigned char p)
{
	ar.write( &p, sizeof(p) );
	return ar;
}

Bu::ArchiveBase &Bu::operator<<( Bu::ArchiveBase &ar, signed short p)
{
	ar.write( &p, sizeof(p) );
	return ar;
}

Bu::ArchiveBase &Bu::operator<<( Bu::ArchiveBase &ar, unsigned short p)
{
	ar.write( &p, sizeof(p) );
	return ar;
}

Bu::ArchiveBase &Bu::operator<<( Bu::ArchiveBase &ar, signed int p)
{
	ar.write( &p, sizeof(p) );
	return ar;
}

Bu::ArchiveBase &Bu::operator<<( Bu::ArchiveBase &ar, unsigned int p)
{
	ar.write( &p, sizeof(p) );
	return ar;
}

Bu::ArchiveBase &Bu::operator<<( Bu::ArchiveBase &ar, signed long p)
{
	ar.write( &p, sizeof(p) );
	return ar;
}

Bu::ArchiveBase &Bu::operator<<( Bu::ArchiveBase &ar, unsigned long p)
{
	ar.write( &p, sizeof(p) );
	return ar;
}

Bu::ArchiveBase &Bu::operator<<( Bu::ArchiveBase &ar, signed long long p)
{
	ar.write( &p, sizeof(p) );
	return ar;
}

Bu::ArchiveBase &Bu::operator<<( Bu::ArchiveBase &ar, unsigned long long p)
{
	ar.write( &p, sizeof(p) );
	return ar;
}

Bu::ArchiveBase &Bu::operator<<( Bu::ArchiveBase &ar, float p)
{
	ar.write( &p, sizeof(p) );
	return ar;
}

Bu::ArchiveBase &Bu::operator<<( Bu::ArchiveBase &ar, double p)
{
	ar.write( &p, sizeof(p) );
	return ar;
}

Bu::ArchiveBase &Bu::operator<<( Bu::ArchiveBase &ar, long double p)
{
	ar.write( &p, sizeof(p) );
	return ar;
}

Bu::ArchiveBase &Bu::operator>>( Bu::ArchiveBase &ar, bool &p)
{
	ar.read( &p, sizeof(p) );
	return ar;
}

Bu::ArchiveBase &Bu::operator>>( Bu::ArchiveBase &ar, char &p)
{
	ar.read( &p, sizeof(p) );
	return ar;
}

Bu::ArchiveBase &Bu::operator>>( Bu::ArchiveBase &ar, signed char &p)
{
	ar.read( &p, sizeof(p) );
	return ar;
}

Bu::ArchiveBase &Bu::operator>>( Bu::ArchiveBase &ar, unsigned char &p)
{
	ar.read( &p, sizeof(p) );
	return ar;
}

Bu::ArchiveBase &Bu::operator>>( Bu::ArchiveBase &ar, signed short &p)
{
	ar.read( &p, sizeof(p) );
	return ar;
}

Bu::ArchiveBase &Bu::operator>>( Bu::ArchiveBase &ar, unsigned short &p)
{
	ar.read( &p, sizeof(p) );
	return ar;
}

Bu::ArchiveBase &Bu::operator>>( Bu::ArchiveBase &ar, signed int &p)
{
	ar.read( &p, sizeof(p) );
	return ar;
}

Bu::ArchiveBase &Bu::operator>>( Bu::ArchiveBase &ar, unsigned int &p)
{
	ar.read( &p, sizeof(p) );
	return ar;
}

Bu::ArchiveBase &Bu::operator>>( Bu::ArchiveBase &ar, signed long &p)
{
	ar.read( &p, sizeof(p) );
	return ar;
}

Bu::ArchiveBase &Bu::operator>>( Bu::ArchiveBase &ar, unsigned long &p)
{
	ar.read( &p, sizeof(p) );
	return ar;
}

Bu::ArchiveBase &Bu::operator>>( Bu::ArchiveBase &ar, signed long long &p)
{
	ar.read( &p, sizeof(p) );
	return ar;
}

Bu::ArchiveBase &Bu::operator>>( Bu::ArchiveBase &ar, unsigned long long &p)
{
	ar.read( &p, sizeof(p) );
	return ar;
}

Bu::ArchiveBase &Bu::operator>>( Bu::ArchiveBase &ar, float &p)
{
	ar.read( &p, sizeof(p) );
	return ar;
}

Bu::ArchiveBase &Bu::operator>>( Bu::ArchiveBase &ar, double &p)
{
	ar.read( &p, sizeof(p) );
	return ar;
}

Bu::ArchiveBase &Bu::operator>>( Bu::ArchiveBase &ar, long double &p)
{
	ar.read( &p, sizeof(p) );
	return ar;
}

