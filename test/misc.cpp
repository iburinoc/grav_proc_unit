#include <iostream>
#include <string>
#include <initializer_list>

using std::string;

class Test {
public:
	Test(std::initializer_list<std::string> t, int a) {
		
	}
};

int main() {
	string s = "abc";
	std::cout << (s == "abc") << std::endl;

	Test t({"a", "b", "c"}, 5);
}

