#include <sg14/fixed_point>
#include <iostream>
#include <iomanip>
#include <stdio.h>
using namespace sg14;

int main() {
	/*
	auto y = sg14::fixed_point<int32_t, -16>{1000};

	sg14::fixed_point<int64_t,-32> x(y);
	std::cout << x << " " << x.data() << std::endl;
	std::cout << y << " " << y.data() << std::endl;
	*/
	
	auto y = fixed_point<>{std::numeric_limits<std::int16_t>::max()};
	std::cout << y.data() << std::endl;
	std::cout << std::fixed << std::setprecision(0) << y << std::endl;
	auto result = y * y;
	auto result1 = result * 2;
	std::cout << result << std::endl;
	std::cout << result1 << std::endl;
	std::cout << result + 10 << std::endl;
	std::cout << 10 +  result << std::endl;
	std::cout << result - 10 << std::endl;
	std::cout << 10 - result << std::endl;
	/*
	auto x = y * y;
	auto res = y * 1000000.72; //进入的函数不对 需要加匹配规则
	*/
	return 0;
}

