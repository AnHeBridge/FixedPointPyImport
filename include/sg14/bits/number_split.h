#if !defined(SG14_NUMBER_SPLIT_H)
#define SG14_NUMBER_SPLIT_H 
//数字拆分为低位和高位 eg 64-32,32

#include <sg14/num_traits.h>
//#include <sg14/num_utils.h>

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
			Rep new_rep = this->get_data() / rhs.get_data();
			return number_split<Rep>(new_rep);
		}

		template<class Rep>
		number_split<Rep> number_split<Rep>::operator -(const number_split<Rep>& rhs) const {
			Rep new_rep = (static_cast<Rep>(this->_high) << std::numeric_limits<lowrep>::digits) - (static_cast<Rep>(rhs._high) << std::numeric_limits<lowrep>::digits) + this->_low - rhs._low;
			return number_split<Rep>(new_rep);
		}

		template<class Rep>
		number_split<Rep> number_split<Rep>::operator *(const number_split<Rep>& rhs) const {
			Rep t0 = static_cast<Rep>(this->_low) * rhs._low;
			Rep t1 = static_cast<Rep>(this->_high) * rhs._low;
			Rep t2 = static_cast<Rep>(this->_low) * rhs._high;
			Rep t3 = static_cast<Rep>(this->_high) * rhs._high;
			
			int digitval = std::numeric_limits<lowrep>::digits;
			lowrep digitmax = std::numeric_limits<lowrep>::max();
			Rep u1 = t1 + (t0 >> digitval);
			Rep u2 = t2 + (u1 & digitmax);
			Rep low = (u2 << digitval) | (t0 & digitmax);
			Rep high = t3 + (u2 >> digitval) + (u1 >> digitval);
			Rep result = ((high & digitmax) << digitval) | (low >> digitval);
			return number_split<Rep>(result);
		}
			
	//}
}
#endif
