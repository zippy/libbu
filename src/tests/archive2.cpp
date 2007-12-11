#include "bu/archive.h"
#include "bu/archival.h"
#include "bu/file.h"

int giId = 0;

class A : public Bu::Archival
{
public:
	A() :
		iId( giId++ )
	{
	}

	virtual ~A()
	{
	}

	virtual void archive( Bu::Archive &ar )
	{
		ar && iId;
	}

	int iId;
};

class B : public Bu::Archival
{
public:
	B() :
		iId( giId++ ),
		a1( new A ),
		a2( new A )
	{
	}

	virtual ~B()
	{
		delete a1;
		delete a2;
	}

	virtual void archive( Bu::Archive &ar )
	{
		//ar && iId && a1 && a2;
		ar << iId << a1 << a2;
	}

	int iId;
	A *a1, *a2;
};

class C : public Bu::Archival
{
public:
	C() :
		iId( giId++ ),
		a( new A ),
		b( new B )
	{
	}

	virtual ~C()
	{
		delete a;
		delete b;
	}

	virtual void archive( Bu::Archive &ar )
	{
		//ar && iId && a && b;
		ar << iId;
		ar << a << b;
	}

	int iId;
	A *a;
	B *b;
};

void write()
{
	C *c = new C;
	
	Bu::File f( "test.archive", "wb");
	Bu::Archive ar( f, Bu::Archive::save );
	ar << c;
}

int main()
{
	write();

}

