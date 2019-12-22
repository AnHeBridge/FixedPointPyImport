#include <sg14/fixed_point>
#include <iostream>
using namespace sg14;

int main() {
	auto z = fixed_point<int, -2>{0.3};
	auto y = fixed_point<int, -4>{10};
	
	std::cout << y.data() << std::endl;
	std::cout << z.data() << std::endl;
	
	y = z + 2;
	y -= 2;
	y *= 2;
	y /= 2;

	std::cout << y << std::endl;
	return 0;
}
