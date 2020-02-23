#if !defined(SG14_NUMBER_SPLIT_H)
#define SG14_NUMBER_SPLIT_H 
//数字拆分为低位和高位 eg 64-32,32

#include <sg14/num_traits.h>

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
			explicit constexpr operator bool() const;


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

		//do not support not equal Rep operator temporarily 加上一个负数
		template<class Rep>
		number_split<Rep> number_split<Rep>::operator +(const number_split<Rep>& rhs) const {
			Rep new_rep = (static_cast<Rep>(this->_high) << std::numeric_limits<lowrep>::digits) + (static_cast<Rep>(rhs._high) << std::numeric_limits<lowrep>::digits) + this->_low + rhs._low;
			return number_split<Rep>(new_rep);
		}

		template<class Rep>
		number_split<Rep> number_split<Rep>::operator -(const number_split<Rep>& rhs) const {
			Rep new_rep = (static_cast<Rep>(this->_high) << std::numeric_limits<lowrep>::digits) - (static_cast<Rep>(rhs._high) << std::numeric_limits<lowrep>::digits) + this->_low - rhs._low;
			return number_split<Rep>(new_rep);
		}

		template<class Rep>
		number_split<Rep> number_split<Rep>::operator *(const number_split<Rep>& rhs) const {
			return 1;
		}
			
	//}
}
#endif