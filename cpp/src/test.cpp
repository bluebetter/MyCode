/*************************************************************************
    > File Name: test.cpp
    > Author: lxf
    > Mail: blueskytoday@126.com 
    > Created Time: Mon 25 May 2015 10:26:43 AM CST
 ************************************************************************/

#include<iostream>
#include <cstring>
#include <string.h>
using namespace std;

int _test(){
	string s = "abc\0def";
	s += "def";
	s += 'g';
	cout<<s.length()<<endl;
	for (int i=0; i<7; i++){
		cout<<i<<":"<<s[i]<<endl;
	}
	return 0;
}

char *my_strsep(char **stringp, const char *delim)
{
	cout<<"in "<< __func__ <<endl;
	char *s;
	const char *spanp;
	int c, sc;
	char *tok;
	if ((s = *stringp)== NULL)
		return (NULL);
	for (tok = s;;) {

		c = *s++;
		spanp = delim;
		do {

			if ((sc =*spanp++) == c) {

				if (c == 0)
					s = NULL;
				else
					s[-1] = 0;
				*stringp = s;
				return (tok);
			}
		} while (sc != 0);
	}
	/* NOTREACHED */
}

void test_my_strsep(){
	char *s = "a\tb\tc";
	cout << my_strsep(&s, "\t") << endl;
}
void test_strsep(){
	char *s = "a\tb\tc";
	cout << strsep(&s, "\t") << endl;
}

int main(){
	test_strsep();
	return 0;
}

