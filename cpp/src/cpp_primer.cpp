#include <iostream>
#include <string>
using namespace std;

/*
 *
 * 5.3
 */
void test_5p3()
{
	string s = "abc";
	for (int i=0; i<=s.length()+100; i++)
		cout<<int(s[i])<<'\t'<<s[i]<<endl;
}

/*
 *
 * 5.5.1
 */
void test_5p5p1()
{
	char ch;
	cin>>ch;
	cout<<ch<<endl;
}

/*
 *
 * 5.5
 */
void test_5p5()
{
	char ch1, ch2;
	cin.get(ch1).get(ch2);
	cout<<"ch1: "<<ch1<<endl;
	cout<<"ch1(int): "<<int(ch1)<<endl;
	cout<<"ch2: "<<ch2<<endl;
	cout<<"ch2(int): "<<int(ch2)<<endl;

}

/*
 * 6.2
 */
void test_6p2(){
	char ch;

	cout<<"Type and I shall repeat.\n";
	cin.get(ch);
	while ('.' != ch){
		if ('\n' == ch){
			cout<<ch;
		}else{
			//cout<<++ch;
			cout<<ch+1;
		}
		cin.get(ch);
	}
}

/*
 * 10.3.2
 */
void test_10p3p2(){
	class Test{
		int i;
		public:
		//Test(){ }
		Test(int j){
			//this->i = i;
			i = j;
		}
		void show() const{
			cout<<"i = "<<i<<endl;
		}
	};

	const Test m_t2;
	m_t2.show();
}

int main()
{
	cout<<"This is a file doing test for <C++ Primer Plus>\n";
#ifdef TEST_5P3
	test_5p3();
#endif

#ifdef TEST_5P5
	test_5p5();
#endif

#ifdef TEST_5P5P1
	test_5p5p1();
#endif

#ifdef TEST_6P2
	test_6p2();
#endif

#ifdef TEST_10P3P2
	test_10p3p2();
#endif
	return 0;
}

