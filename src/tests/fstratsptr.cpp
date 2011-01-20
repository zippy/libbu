/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/string.h"
#include "bu/atom.h"
#include "bu/sptr.h"

class Person
{
public:
	Person(){};
	virtual ~Person(){};

	Bu::Atom<Bu::String> sFirstName;
	Bu::Atom<Bu::String> sLastName;
};

typedef Bu::SPtr<Person> PersonPtr;

void Swap(PersonPtr one, PersonPtr two)
{
	PersonPtr three(new Person);
	three->sFirstName = one->sFirstName;
	three->sLastName = two->sLastName;

	printf(
			"%s %s\n",
			three->sFirstName->getStr(),
			three->sLastName->getStr() );
}

int main()
{
/*	PersonPtr one(new Person);
	PersonPtr two(new Person);
	one->sFirstName = "Bob";
	one->sLastName = "Smith";
	two->sFirstName = "Fred";
	two->sLastName = "Carpenter";

	Swap(one, two);
*/

	Bu::Atom<Bu::String> sOne, sTwo;
	sOne = "Hello";
	sTwo = sOne;

	return 0;
}
