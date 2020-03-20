#include <sg14/fixed_point>
#include "gtest/gtest.h"
#include <limits>
#include <stdlib.h>

constexpr float eps = 1e-4;
TEST(fixed_extra,fixed_sin) {
	sg14::fixed_point<> value = sg14::fixed_point<>{3.14};
	sg14::fixed_point<> value2 = sg14::fixed_point<>{1.57};
	ASSERT_EQ(abs(sin(value) - sin(3.14)) < 1e-4, true);
	ASSERT_EQ(abs(sin(value2) - sin(1.57)) < 1e-4, true);
}

TEST(fixed_extra,fixed_cos) {
	sg14::fixed_point<> value = sg14::fixed_point<>{3.14};
	sg14::fixed_point<> value2 = sg14::fixed_point<>{1.57};
	ASSERT_EQ(abs(cos(value) - cos(3.14)) < 1e-4, true);
	ASSERT_EQ(abs(cos(value2) - cos(1.57)) < 1e-4, true);
}

TEST(fixed_extra,fixed_tan) {
	sg14::fixed_point<> value = sg14::fixed_point<>{0};
	ASSERT_EQ(abs(tan(value) - tan(0.0)) < 1e-4, true);
}


TEST(fixed_extra,fixed_sqrt) {
	sg14::fixed_point<> value = sg14::fixed_point<>{1000250.015625};
	ASSERT_EQ(sqrt(value),1000.125);
}
