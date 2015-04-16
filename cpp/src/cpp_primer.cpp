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
		Test(){ }
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

/*
 * test class private
 */
void test_class(){
	class Test{
		private:
			int i;
	};
	Test c_t;
	//c_t.i = 1;
	//cout<<"c_t.i = "<<c_t.i<<endl;	//Error
}

/*
 * test 11.5.2
 */
class Vector{
	double x,y;
	public:
	Vector(){
		x = y = 100;
	}
	Vector(double x, double y){
		this->x = x;
		this->y = y;
	}
	Vector operator-(const Vector &b)const;
	void show(){
		cout<<"x = "<<x<<","<<"y = "<<y<<endl;
	}
};
Vector Vector::operator-(const Vector &b)const{
	return Vector(x-b.x, y-b.y);
	//return Vector(b.x-x, b.y-y);		//result error
}
void test_11p5p2(){

	Vector v_a(10,10);
	Vector v_b(6,6);
	Vector v_c = v_a-v_b;
	v_c.show();
}

/*
 */
class Star{
	string str;

	public:
	Star(){
		str = "s";
	};
	Star & operator=(Star &s);
	Star(string s);
};
Star::Star(string s){
	str = s;
}
Star & Star::operator=(Star &s){
	str = s.str;
	return *this;
}
void test_13p8p2(){
	Star t;
	string s = "ac";
	t = s;
	//t = Star(s);
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
#ifdef TEST_11P5P2
	test_11p5p2();
#endif

#ifdef TEST_13P8P2
	test_13p8p2();
#endif
	return 0;
}

