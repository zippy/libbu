#include "confpair.h"
#include <iostream>

using namespace std;

int main()
{
	ConfPair<float> p1("DebugMode");
	p1.value() = 12;
	cout << p1.value() << "\n";
	p1.value() = 55;
	cout << p1.value() << "\n";

	ConfPairBase &p = p1;

	p = "33.12";
	cout << p.getAsString();
}

