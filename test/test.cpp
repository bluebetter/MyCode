/*************************************************************************
    > File Name: test.cpp
    > Author: lxf
    > Mail: blueskytoday@126.com 
    > Created Time: Tue 31 Mar 2015 01:46:40 PM CST
 ************************************************************************/

#include<iostream>
#include <stdio.h>
using namespace std;

/*
 */
void test_big_little_endian(){
	short a = 1;
	char c = *(char *)&a;
	printf("%x\n", c);
	if (c == 1){
		cout<<"little endian.\n";
	}
	cout<<c<<endl;
	int len = sizeof(a);
	/*
	char *s = new char[len];
	for (int i=0; i<len; i++){
		s[i] = *(((char *)&a)+i);
	}
	cout<<s[0]<<endl;
	cout<<s[1]<<endl;
	cout<<s[2]<<endl;
	cout<<s[3]<<endl;
	delete []s;
	*/

	/*
	short s_a = 0x1122;
	cout<<s_a<<":"<<sizeof(s_a)<<endl;
	char x0, x1;
	x0 = ((char*)&s_a)[0];
	x1 = ((char*)&s_a)[1];

	printf("%x-%x", x0, x1);
	cout<<(int)x0<<endl<<(int)x1<<endl;
	*/
}

int main(){
	//test_big_little_endian();
	
	//test for
	for (int i=0; i<10; i++){
		static int j = 1;
		cout<<"i at:"<<&i<<"\t"<<i<<endl;
		cout<<"j at:"<<&j<<"\t"<<j<<endl;
		j = 2;
	}

	return 0;
}
