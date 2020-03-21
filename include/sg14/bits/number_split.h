#if !defined(SG14_NUMBER_SPLIT_H)
#define SG14_NUMBER_SPLIT_H 
//数字拆分为低位和高位 eg 64-32,32

#include <sg14/num_traits.h>
#include <sg14/bits/number_utils.h>
#include <tuple>

namespace sg14 {
	//namespace _impl {
		template<class Rep>//int64,uint64,int32,uint32...
		class number_split {
		public :
			using rep = Rep;

			number_split() = default;
			
			using highrep = typename _num_traits_impl::set_digits_integer<Rep,std::numeric_limits<Rep>::digits / 2>::type;
			using lowrep = typename _num_traits_impl::set_digits_unsigned<std::numeric_limits<Rep>::digits / 2>::type;

			explicit number_split(const rep& r);
			
			constexpr const Rep get_data() const;

			number_split operator + (const number_split& rhs) const;
			number_split operator - (const number_split& rhs) const;
			number_split operator * (const number_split& rhs) const;
			number_split operator / (const number_split& rhs) const;
			explicit constexpr operator bool() const;
			highrep constexpr get_high() const { return _high ;}
			lowrep constexpr get_low() const { return _low; }

		private :
			lowrep _low;
			highrep _high; 
		};

		template<class Rep>
		number_split<Rep>::number_split(const Rep& r) {
			this->_low = r & (std::numeric_limits<lowrep>::max());
			this->_high = r >> std::numeric_limits<lowrep>::digits;
		}

		template<class Rep>
		constexpr const Rep number_split<Rep>::get_data() const {
			return (static_cast<Rep>(this->_high) << std::numeric_limits<lowrep>::digits) + this->_low;
		}
		
		template<class Rep>
		constexpr number_split<Rep>::operator bool() const {
			return static_cast<bool>(this->_low) || static_cast<bool>(this->_high);
		}

		//do not support not equal Rep operator 
		template<class Rep>
		number_split<Rep> number_split<Rep>::operator +(const number_split<Rep>& rhs) const {
			Rep new_rep = (static_cast<Rep>(this->_high) << std::numeric_limits<lowrep>::digits) + (static_cast<Rep>(rhs._high) << std::numeric_limits<lowrep>::digits) + this->_low + rhs._low;
			return number_split<Rep>(new_rep);
		}

		template<class Rep>
		number_split<Rep> number_split<Rep>::operator / (const number_split<Rep>& rhs) const {
			using unsigned_rep = typename _num_traits_impl::set_digits_unsigned<std::numeric_limits<Rep>::digits>::type;
			bool f_signed = this->_high >= 0;
			bool s_signed = rhs._high >= 0;
			bool is_signed = f_signed ^ s_signed;
			unsigned_rep dividened = static_cast<unsigned_rep>(std::abs(this->get_data()));
			unsigned_rep divisor = static_cast<unsigned_rep>(std::abs(rhs.get_data()));
			unsigned_rep result = 0,tmp1 = 0,tmp2 = 0;
			std::tie(result,tmp1,tmp2) = shifted_div(dividened,divisor);
			return number_split<Rep>(static_cast<Rep>(result) * (is_signed ? -1 : 1));
		}

		template<class Rep>
		number_split<Rep> number_split<Rep>::operator -(const number_split<Rep>& rhs) const {
			Rep new_rep = (static_cast<Rep>(this->_high) << std::numeric_limits<lowrep>::digits) - (static_cast<Rep>(rhs._high) << std::numeric_limits<lowrep>::digits) + this->_low - rhs._low;
			return number_split<Rep>(new_rep);
		}

		template<class Rep>
		number_split<Rep> number_split<Rep>::operator *(const number_split<Rep>& rhs) const {
			bool nneg_ = this->_high < 0, dneg_ = rhs._high < 0;
			bool is_negative = nneg_ ^ dneg_;

			using unsigned_rep = typename _num_traits_impl::set_digits_unsigned<std::numeric_limits<Rep>::digits>::type;

			unsigned_rep t0 = static_cast<unsigned_rep>(this->_low) * rhs._low;
			unsigned_rep t1 = static_cast<unsigned_rep>(this->_high) * rhs._low;
			unsigned_rep t2 = static_cast<unsigned_rep>(this->_low) * rhs._high;
			unsigned_rep t3 = static_cast<unsigned_rep>(this->_high) * rhs._high;
			
			int digitval = std::numeric_limits<lowrep>::digits;
			lowrep digitmax = std::numeric_limits<lowrep>::max();
			unsigned_rep u1 = t1 + (t0 >> digitval);
			unsigned_rep u2 = t2 + (u1 & digitmax);
			unsigned_rep low = (u2 << digitval) | (t0 & digitmax);
			unsigned_rep high = t3 + (u2 >> digitval) + (u1 >> digitval);
			Rep result = static_cast<Rep>(((high & digitmax) << digitval) | (low >> digitval));
			return number_split<Rep>(is_negative ? -result : result);
		}
			
	//}
}
#endif
