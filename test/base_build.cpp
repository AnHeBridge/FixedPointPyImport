#include <sg14/fixed_point>
#include "gtest/gtest.h"

//<>default represent int64_t,-32
TEST(fixed_base,construct_integer) {
	sg14::fixed_point<> value = sg14::fixed_point<>{1000};
	sg14::fixed_point<> cmp = sg14::fixed_point<>{1000};
	ASSERT_EQ(value, 1000);
	ASSERT_EQ(1000, value);
	ASSERT_EQ(value, cmp);
	ASSERT_EQ(-value, -1000);
}


TEST(fixed_base,construct_float) {
	sg14::fixed_point<> value = sg14::fixed_point<>{2000.657935};
	ASSERT_EQ(value, 2000.657935);
	ASSERT_EQ(2000.657935,value);
}

TEST(fixed_base,not_equal) {
	sg14::fixed_point<> value = sg14::fixed_point<>{1000};
	sg14::fixed_point<> newdata = sg14::fixed_point<>{2000};
	ASSERT_NE(value, 2000.65);
	ASSERT_NE(2000.65,value);
	ASSERT_NE(newdata,value);
}

TEST(fixed_base,lower_than) {
	sg14::fixed_point<> value = sg14::fixed_point<>{1000};
	sg14::fixed_point<> newdata = sg14::fixed_point<>{2000};
	sg14::fixed_point<> floatb = sg14::fixed_point<>{1235.4};
	ASSERT_LT(value,2000);
	ASSERT_LT(value,floatb);
	ASSERT_LT(value,newdata);
}

TEST(fixed_base,lower_equal_than) {
	sg14::fixed_point<> value = sg14::fixed_point<>{1000};
	sg14::fixed_point<> newdata = sg14::fixed_point<>{2000};
	sg14::fixed_point<> floatb = sg14::fixed_point<>{1235.4};
	ASSERT_LE(value,2000);
	ASSERT_LE(value,floatb);
	ASSERT_LE(value,newdata);
	ASSERT_LE(value,1000);
}

TEST(fixed_base,greater_than) {
	sg14::fixed_point<>value = sg14::fixed_point<>{1000};
	sg14::fixed_point<> newdata = sg14::fixed_point<>{2000};
	sg14::fixed_point<> floatb = sg14::fixed_point<>{1235.4};
	ASSERT_GT(2000,value);
	ASSERT_GT(newdata,1000);
	ASSERT_GT(newdata,floatb);
	ASSERT_GT(newdata,value);
}

TEST(fixed_base,greater_equal_than) {
	sg14::fixed_point<>value = sg14::fixed_point<>{1000};
	sg14::fixed_point<> newdata = sg14::fixed_point<>{2000};
	sg14::fixed_point<> floatb = sg14::fixed_point<>{1235.4};
	ASSERT_GE(2000,value);
	ASSERT_GE(newdata,2000);
	ASSERT_GE(newdata,value);
	ASSERT_GE(newdata,floatb);
}
