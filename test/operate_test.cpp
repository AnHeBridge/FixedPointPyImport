#include <sg14/fixed_point>
#include "gtest/gtest.h"
#include <limits>
#include <iostream>

TEST(fixed_op,fixed_add) {
	sg14::fixed_point<> value = sg14::fixed_point<>{1000};
	sg14::fixed_point<> cmp = sg14::fixed_point<>{2000};
	sg14::fixed_point<> fl = sg14::fixed_point<>{10.625};
	ASSERT_EQ(value + value, 2000);
	ASSERT_EQ(2000, value + 1000);
	ASSERT_EQ(cmp,1000 + value);
	ASSERT_EQ(2010.625,fl + cmp);
}

TEST(fixed_op,fixed_minus) {
	sg14::fixed_point<> value = sg14::fixed_point<>{1000};
	sg14::fixed_point<> cmp = sg14::fixed_point<>{2000};
	sg14::fixed_point<> fl = sg14::fixed_point<>{10.625};
	ASSERT_EQ(value - 500,500);
	ASSERT_EQ(cmp - value, value);
	ASSERT_EQ(value - fl,989.375);
}

TEST(fixed_op,fixed_multi) {
	sg14::fixed_point<> value = sg14::fixed_point<>{1000};
	sg14::fixed_point<> fl = sg14::fixed_point<>{10.625};
	sg14::fixed_point<> mm = sg14::fixed_point<>{std::numeric_limits<int32_t>::max()};
	ASSERT_EQ(value * 500,500000);
	ASSERT_EQ(value * fl, 10625);
	ASSERT_EQ(0.5 * value,sg14::fixed_point<>{500});
	ASSERT_EQ(mm * mm,std::numeric_limits<int32_t>::max() * std::numeric_limits<int32_t>::max());
}

TEST(fixed_op,fixed_div) {
	sg14::fixed_point<> value = sg14::fixed_point<>{1000};
	sg14::fixed_point<> cmp = sg14::fixed_point<>{1073741824};
	sg14::fixed_point<> cmp1 = sg14::fixed_point<>{-1073741824};
	sg14::fixed_point<> fl = sg14::fixed_point<>{0.125};
	ASSERT_EQ(value / fl,8000);
	ASSERT_EQ(cmp / 32768,32768);
	ASSERT_EQ(cmp1 / 32768,-32768);
}
