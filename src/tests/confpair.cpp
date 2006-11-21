#include "confpair.h"
#include <iostream>

using namespace std;

int main()
{
	ConfPair<bool> p1("DebugMode");
	p1.value() = true;
	cout << p1.value() << "\n";
	p1.value() = false;
	cout << p1.value() << "\n";
}

