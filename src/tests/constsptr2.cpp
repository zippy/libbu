#include "sptr.h"

void nonsptr()
{
	int a = 5;
	int b = 10;
	const int *p = &a;
	p = &b;
	printf("p = %d\n", (*p) );
	//(*p)++;
}

void sptr()
{
	int a = 5;
	int b = 10;
	const SPtr<int> p = new int(a);
	p = new int(b);
	printf("p = %d\n", (*p) );
	//(*p)++;
}

int main()
{
	printf("Non-sptr:\n");
	nonsptr();
	printf("sptr:\n");
	sptr();
}

