#include "hash.h"
#include <string.h>

typedef struct CC
{
	const char *sKey;
	const char *sData;
} CC;

char *c(const char *s)
{
	int l = strlen(s);
	char *o = new char[l+1];
	o[l] = '\0';
	memcpy(o, s, l);

	return o;
}

CC *mkCC(const char *k, const char *d)
{
	CC *o = new CC;
	o->sKey = c(k);
	o->sData = c(d);
	return o;
}

void klCC(CC *c)
{
	delete[] c->sKey;
	delete[] c->sData;
	delete c;
}

typedef Hash<const char *, CC *> CCHash;

int main()
{
	char buf1[200];
	char buf2[200];
	CCHash h;
	CC *tmp;

	for(int i=0;i<10;i++)
	{
		sprintf(buf1, "key_%d", i);
		sprintf(buf2, "data_%d", i);
		tmp = mkCC(buf1, buf2);
		h[tmp->sKey] = tmp;
	}

	for(int i=0;i<12;i++)
	{
		sprintf(buf1, "key_%d", i);
		if(h.has(buf1))
		{
			tmp = h[buf1];
			printf("GOT %s = %s\n", tmp->sKey, tmp->sData);
		}
		else
			printf("%s is not in the table.\n", buf1);
	}

	CCHash::iterator it = h.begin();
	for(;it!=h.end();it++)
	{
		tmp = (*it).second;
		klCC(tmp);
	}

	return 0;
}

