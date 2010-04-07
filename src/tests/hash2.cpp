#include <bu/fstring.h>
#include <bu/hash.h>

int main()
{
	Bu::Hash<Bu::FString, int> hCmd;

	hCmd.insert("help", 5 );
	hCmd.insert("exit", 5 );
	hCmd.insert("getkey", 5 );
	hCmd.insert("setcrypt", 5 );
	hCmd.insert("user", 5 );
	hCmd.insert("pass", 5 );
	hCmd.erase("getkey");
	hCmd.erase("setcrypt");
	hCmd.insert("user", 5 );
	hCmd.insert("pass", 5 );
	hCmd.erase("pass");
	hCmd.erase("user");
	hCmd.insert("acl", 5 );
	hCmd.insert("owner", 5 );
	hCmd.insert("operator", 5 );
	hCmd.insert("admin", 5 );
	hCmd.insert("monitor", 5 );
	hCmd.insert("shutdown", 5 );

	return 0;
}
