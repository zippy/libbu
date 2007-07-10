#include "bu/fstring.h"
#include "bu/atom.h"
#include "bu/sptr.h"

class Person
{
public:
	Person(){};
	virtual ~Person(){};

	Bu::Atom<Bu::FString> sFirstName;
	Bu::Atom<Bu::FString> sLastName;
};

typedef Bu::SPtr<Person> PersonPtr;

void Swap(PersonPtr one, PersonPtr two)
{
	PersonPtr three(new Person);
	three->sFirstName = one->sFirstName;
	three->sLastName = two->sLastName;

	printf(
			"%s %s\n",
			three->sFirstName->c_str(),
			three->sLastName->c_str() );
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

	Bu::Atom<Bu::FString> sOne, sTwo;
	sOne = "Hello";
	sTwo = sOne;

	return 0;
}
