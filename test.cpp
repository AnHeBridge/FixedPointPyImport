#include <sg14/fixed_point>
#include <iostream>
#include <iomanip>
using namespace sg14;

int main() {
	int value = (1 << 29) - 1;
	//auto y = fixed_point<int, -2>{100.75};
	//auto z = fixed_point<int, -2>{2.5};
	auto y = fixed_point<int32_t, -2>{4};
	auto z = fixed_point<int32_t, -2>{0.125};
	
	std::cout << y << " " << y.data() << std::endl;
	std::cout << z << " " << z.data() << std::endl;
	auto x = y + z;
	z = 7 ^ y;

	std::cout << z  << std::endl;
}
