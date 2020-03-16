#if !defined(SG14_NUMBER_UTILS_H)
#define SG14_NUMBER_URILS_H 

#include <array>
#include <sg14/bits/type_traits.h>
#include <limits>
#include <sg14/num_traits.h>
#include <tuple>

# define likely(x)	__builtin_expect(!!(x), 1)
# define unlikely(x)	__builtin_expect(!!(x), 0)

namespace sg14 {

	unsigned clz_generic(uint32_t x) noexcept {
		unsigned n = 32;
		for (int i = 4; i >= 0; --i) {
			const auto s = unsigned{1} << i;
			const auto hi = x >> s;
			if (hi != 0) {
				n -= s;
				x = hi;
			}
		}
		return n - x;
	}

	unsigned clz_generic(uint64_t x) noexcept {
		unsigned n = 64;
		for (int i = 5; i >= 0; --i) {
			const auto s = unsigned{1} << i;
			const auto hi = x >> s;
			if (hi != 0) {
				n -= s;
				x = hi;
			}
		}
		return n - static_cast<unsigned>(x);
	}

	unsigned clz_generic(uint16_t x) noexcept {
		unsigned n = 16;
		for (int i = 3; i >= 0; --i) {
			const auto s = unsigned{1} << i;
			const auto hi = x >> s;
			if (hi != 0) {
				n -= s;
				x = hi;
			}
		}
		return n - x;
	}

	 inline unsigned clz(uint32_t x) noexcept {
#ifdef _MSC_VER
		return clz_generic(x);
#else
		return x != 0 ? unsigned(__builtin_clz(x)) : 32;
#endif
	 }

	  inline unsigned clz(uint64_t x) noexcept {
#ifdef _MSC_VER
		  return clz_generic(x);
#else
		  return x != 0 ? unsigned(__builtin_clzll(x)) : 64;
#endif
	  }

	  inline unsigned clz(uint16_t x) noexcept {
		  return clz_generic(x);
	  }
		
	  template<class BigInteger,class SmallInteger,typename = _impl::enable_if_t<std::numeric_limits<BigInteger>::is_integer>,typename = _impl::enable_if_t<std::numeric_limits<SmallInteger>::is_integer>>
	  inline std::tuple<BigInteger,BigInteger,BigInteger> shifted_big_divsmall(BigInteger a,SmallInteger b) {
		  int digits = std::numeric_limits<SmallInteger>::digits;
		  SmallInteger quo3 = static_cast<SmallInteger>(((a >> digits) / b));
		  SmallInteger rem3 = static_cast<SmallInteger>((a >> digits) % b);
		  BigInteger t = (static_cast<BigInteger>(rem3)) << digits | static_cast<SmallInteger>(a);
		  SmallInteger quo2 = static_cast<SmallInteger>(t / b);
		  SmallInteger rem2 = static_cast<SmallInteger>(t % b);
		  t = static_cast<BigInteger>(rem2) << digits;
		  SmallInteger quo1 = static_cast<SmallInteger>(t / b);
		  SmallInteger rem1 = static_cast<SmallInteger>(t % b);
		  return std::make_tuple(static_cast<BigInteger>(quo2) << digits | quo1,quo3,rem1);
	  }

	  template<class Integer,typename = _impl::enable_if_t<std::numeric_limits<Integer>::is_integer>, typename = _impl::enable_if_t<!std::numeric_limits<Integer>::is_signed>>
	  inline std::tuple<Integer,Integer,Integer> shifted_div(Integer a,Integer b) {
		//Knuth TAOCP Vol.2中描述的多精度除法算法
		//算法将96位整数视为3位2^half_digits进制的多精度数，进行多精度除法运算
		using small_int  = typename _num_traits_impl::set_digits_unsigned<std::numeric_limits<Integer>::digits / 2>::type;
		int digits = std::numeric_limits<Integer>::digits;
		int half_digits = digits / 2;

		if ((b & (b - 1)) == 0) {
			small_int shamt = half_digits - 1  - clz(b);
			if (shamt >= 0) 
				return std::make_tuple(a >> shamt, static_cast<Integer>(0), a & ((1 << shamt) - 1));
			else 
				return std::make_tuple(a << -shamt, a >> (digits + shamt), static_cast<Integer>(0));
		}
	
		if ((a >> half_digits) == 0) 
			return std::make_tuple((a << half_digits) / b, static_cast<Integer>(0), (a << half_digits) % b );
	
		if ((b >> half_digits) == 0) 
			return shifted_big_divsmall(a, (small_int)b);

		const unsigned n = 2;
		auto log_d = clz(b);
		std::array<small_int, 4> u;
		std::array<small_int, 2> v = { (small_int)(b << log_d), (small_int)(b << log_d >> half_digits) };
		if (log_d != 0) 
			u = { 0, (small_int)(a << log_d), (small_int)(a << log_d >> half_digits), (small_int)(a >> (digits - log_d)) };
		else 
			u = { 0, (small_int)a, (small_int)(a >> half_digits), 0 };
	
		std::array<small_int, 2> quotient = {};
		{
			const unsigned j = 1;
			Integer a = (Integer)u[j + n] << half_digits | u[j + n - 1];
			Integer q = a / v[n - 1];
			Integer r = a % v[n - 1];
			if (unlikely(q == std::numeric_limits<small_int>::max() || q * v[n - 2] > (((Integer)r << half_digits) | u[j + n - 2]))) {
				q -= 1;
				r += v[n - 1];
			}
			std::array<small_int, 3> qv;
			{
				Integer t = (Integer)v[0] * q;
				qv[0] = (small_int)t;
				small_int temp_carry = (small_int)(t >> half_digits);
				t = (Integer)v[1] * q + temp_carry;
				qv[1] = (small_int)t;
				temp_carry = (small_int)(t >> half_digits);
				qv[2] = temp_carry;
			}
			bool neg = std::make_tuple(u[3], u[2], u[1]) < std::make_tuple(qv[2], qv[1], qv[0]);

			qv[0] = ~qv[0];
			qv[1] = ~qv[1];
			qv[2] = ~qv[2];
			qv[2] += qv[1] == std::numeric_limits<small_int>::max() && qv[0] == std::numeric_limits<small_int>::max();
			qv[1] += qv[0] == std::numeric_limits<small_int>::max();
			qv[0] += 1;
			{
				Integer t = (Integer)u[1] + qv[0];
				u[1] = (small_int)t;
				small_int carry = (small_int)(t >> half_digits);
				t = (Integer)u[2] + qv[1] + carry;
				u[2] = (small_int)t;
				carry = t >> half_digits;
				u[3] += qv[2] + carry;
			}

			if (neg) {
				quotient[j] = (small_int)(q - 1);
				{
					Integer t = (Integer)u[1] + v[0];
					u[1] = (small_int)t;
					small_int carry = (small_int)(t >> half_digits);
					t = (Integer)u[2] + v[1] + carry;
					u[2] = (small_int)t;
					carry = t >> half_digits;
					u[3] += carry;
				}
			}
			else {
				quotient[j] = (small_int)q;
			}
		}


		{
			const unsigned j = 0;
			Integer a = (Integer)u[j + n] << half_digits | u[j + n - 1];
			Integer q = a / v[n - 1];
			Integer r = a % v[n - 1];
			if (unlikely(q == std::numeric_limits<small_int>::max() || q * v[n - 2] > ((Integer)r << half_digits | u[j + n - 2]))) {
				q -= 1;
				r += v[n - 1];
			}
			std::array<small_int, 3> qv;
			{
				Integer t = (Integer)v[0] * q;
				qv[0] = (small_int)t;
				small_int temp_carry = (small_int)(t >> half_digits);
				t = (Integer)v[1] * q + temp_carry;
				qv[1] = (small_int)t;
				temp_carry = (small_int)(t >> half_digits);
				qv[2] = temp_carry;
			}
			bool neg = std::make_tuple(u[2], u[1], u[0]) < std::make_tuple(qv[2], qv[1], qv[0]);

			qv[0] = ~qv[0];
			qv[1] = ~qv[1];
			qv[2] = ~qv[2];
			qv[2] += qv[1] == std::numeric_limits<small_int>::max() && qv[0] == std::numeric_limits<small_int>::max();
			qv[1] += qv[0] == std::numeric_limits<small_int>::max();
			qv[0] += 1;
			{
				Integer t = (Integer)u[0] + qv[0];
				u[0] = (small_int)t;
				small_int carry = (small_int)(t >> half_digits);
				t = (Integer)u[1] + qv[1] + carry;
				u[1] = (small_int)t;
				carry = t >> half_digits;
				u[2] += qv[2] + carry;
			}

			if (neg) {
				quotient[j] = (small_int)(q - 1);
				{
					Integer t = (Integer)u[0] + v[0];
					u[0] = (small_int)t;
					small_int carry = (small_int)(t >> half_digits);
					t = (Integer)u[1] + v[1] + carry;
					u[1] = (small_int)t;
					carry = t >> half_digits;
					u[2] += carry;
				}
			} else {
				quotient[j] = (small_int)q;
			}
		}

		if (log_d) {
			std::array<small_int, 2> remainder = { (u[0] >> log_d) | (u[1] << (half_digits - log_d)), (u[1] >> log_d) | (u[2] << (half_digits - log_d)) };
			return std::make_tuple(quotient[0] | ((Integer)quotient[1] << half_digits), static_cast<Integer>(0), remainder[0] | ((Integer)remainder[1] << half_digits) );
		}
		return std::make_tuple(quotient[0] | (Integer)quotient[1] << half_digits, static_cast<Integer>(0), u[0] | ((Integer)u[1] << half_digits) );
	}
}


#endif



