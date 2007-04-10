#include <stdio.h>
#include "sptr.h"

template <typename T>
class DataBase
{
public:
	DataBase():
		_bHas(false)
	{
	}
	
	virtual ~DataBase()
	{
		clr();
	}
	
	virtual bool has() const
	{
		return _bHas;
	}
	
	virtual void clr()
	{
		_bHas = false;
	}

	virtual void set(T d)
	{
		_tVal = d;
		_bHas = true;
	}

	virtual T const &get() const
	{
		if(!has())
			throw "no data";
		return _tVal;
	}

	virtual T &get()
	{
		if(!has())
			throw "no data";
		return _tVal;
	}
	
protected:
	bool _bHas;
	T _tVal;
};


class Test
{
public:
	Test(){};
	virtual ~Test(){};

	void set(int i)
	{
		_i = i;
	}
	
	int get() const
	{
		return _i;
	}
	
private:
	int _i;
};

int main()
{
	typedef SPtr<Test> TestPtr;

	TestPtr t1(new Test);
	t1->set(42);

	printf("t1: %d.\n", t1->get());

	const TestPtr t2 = t1;

	printf("t2: %d.\n", t2->get());

	typedef DataBase<const TestPtr> DBTP;

	DBTP db;
	db.set(t1);

	printf("dbt1: %d.\n", db.get()->get());
}

