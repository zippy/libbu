#include <bu/sio.h>

int upper()
{
	static int iVal = 1;
	return iVal++;
}

int main()
{
	Bu::print("hello there %1!\n").arg("Bob");

	Bu::println("This is %1 crazy, like %2 times over!").
		arg("totally").arg( 47.2 ).end();
	Bu::println("This is unsubstituted?");

	Bu::serr << "This is error text." << Bu::serr.nl;
	Bu::println( Bu::serr, "This is also error text?");

	Bu::println("This is %{1}000  - %{1}.").arg( 34, Bu::Fmt().width(10).fill('0') );

	Bu::String s = Bu::String("hello %1").arg("bob %1").end().toLower().arg("yo");

	Bu::println( s );
	Bu::println("Hello %%1");

	Bu::println("Nums: %1, %2, %3, %4, %5, %6").arg( upper() ).arg( upper() ).arg( upper() ).arg( upper() ).arg( upper() ).arg( upper() );

	return 0;
}

