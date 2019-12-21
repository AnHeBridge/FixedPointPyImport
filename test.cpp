#include <sg14/fixed_point>
#include <iostream>
using namespace sg14;

int main() {
	auto z = fixed_point<int, -2>{0.3};
	auto y = fixed_point<int, -4>{10};
	
	std::cout << y.data() << std::endl;
	std::cout << z.data() << std::endl;
	
	auto res = y / z;
	std::cout << res.data() << std::endl;
	std::cout << res << std::endl;
	return 0;
}
