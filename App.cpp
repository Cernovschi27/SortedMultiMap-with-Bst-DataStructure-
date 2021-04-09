////#include <iostream>
////
////#include "ShortTest.h"
////#include "ExtendedTest.h"
//
//int main(){
//    testAll();
//	testAllExtended();
//
//    std::cout<<"Finished SMM Tests!"<<std::endl;
//	system("pause");
//}
#include <iostream>
using namespace std;
class Foo
{
public:
	int id_foo;
	Foo(int id) {
		id_foo = id;
	}
};
class Bar : public Foo
{
public:
	int id_bar;
	Bar(int id1, int id2) :Foo(id1) {
		id_bar = id2;
	}
};
int main()
{
	Bar obj1 = Bar(4, 20);
	cout << obj1.id_foo << ' ' << obj1.id_bar;
	return 0;
}