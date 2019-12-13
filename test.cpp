#include <sg14/fixed_point>
#include <iostream>
using namespace sg14;

int main() {
	int value = (1 << 29) - 1;
	//auto y = fixed_point<int, -2>{100.75};
	//auto z = fixed_point<int, -2>{2.5};
	auto y = fixed_point<int, -2>{6};
	auto z = fixed_point<int, -2>{1.2};
	
	std::cout << y.data() << std::endl;
	std::cout << z.data() << std::endl;
	
	auto res = sqrt(y);
	auto res1 = y / z;
	std::cout << res1 << std::endl;
}
