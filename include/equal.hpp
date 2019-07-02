// ================================= EQUAL ================================= //
// Project: The Experimental Bit Algorithms Library
// Name: equal.hpp
// Description: bit_iterator overloads for std::equal
// Creator: Vincent Reverdy
// Contributor(s): 
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _EQUAL_HPP_INCLUDED
#define _EQUAL_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
//#include <execution>
// Project sources
#include "type_traits.hpp"
// Third-party libraries
// Miscellaneous

namespace bit {
// ========================================================================== //



// Status: to do
template <class InputIt1, class InputIt2>
constexpr bool equal(bit_iterator<InputIt1> first1, bit_iterator<InputIt1> last, 
    bit_iterator<InputIt2> first2) {
    (first1, last, first2); 
    return true;
}

/*
// Status: to do
template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2,
          class = typename std::enable_if<std::is_execution_policy_v<ExecutionPolicy>::type>
bool equal(ExecutionPolicy policy, bit_iterator<ForwardIt1> first1,
    bit_iterator<ForwardIt1> last1, bit_iterator<ForwardIt2> first2) {
    (policy, first1, last1, first2);
    return true;
}


// Status: on hold
template <class InputIt1, class InputIt2, class BinaryPredicate>
constexpr bool equal(bit_iterator<InputIt1> first1, bit_iterator<InputIt1> last1,
    bit_iterator<InputIt2> first2, BinaryPredicate p) {

    if(!static_cast<bool>(p(*first1, *first2))) {
        return false;  
    }

}
*/

// Status: on hold
template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2,
    class BinaryPredicate> bool equal(ExecutionPolicy&& policy,
    bit_iterator<ForwardIt1> first1, bit_iterator<ForwardIt1> last1,
    BinaryPredicate p) {
    (policy, first1, last1, p);
    return true;
}

// Status: on hold
template <class InputIt1, class InputIt2>
constexpr bool equal(bit_iterator<InputIt1> first1, bit_iterator<InputIt1> last1,
    bit_iterator<InputIt2> first2, bit_iterator<InputIt2> last2) {
    
    using word_type1 = typename bit_iterator<InputIt1>::word_type;
    using word_type2 = typename bit_iterator<InputIt2>::word_type; 

    constexpr std::size_t num_digits1 = binary_digits<word_type1>::value;
    constexpr std::size_t num_digits2 = binary_digits<word_type2>::value;

    // distance equality checks
    if (is_random_access_iterator_v<InputIt1> &&
        is_random_access_iterator_v<InputIt2>) {
        if (distance(first1, last1) != distance(first2, last2)) {
            return false;
        }
    } else {
      // this won't always catch inequal distance ranges
      // so we'll need to catch it later

      std::size_t border_word_bits1 = num_digits1 - first1.position() + last1.position();
      std::size_t border_word_bits2 = num_digits2 - first2.position() + last2.position();

      if (border_word_bits1 != border_word_bits2) {
          return false;
      }
    }

    if (first1.base() == last1.base()) {
        // not suitable for dual_range_reader
        

    } else {

    }

    if (first1.base() == last1.base()) {
        std::cout << "this thing!\n";
    }

    (first1, last1, first2, last2);
    return true;
}

// Status: to do
template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2>
bool equal(ExecutionPolicy&& policy, bit_iterator<ForwardIt1> first1,
    bit_iterator<ForwardIt1> last1, bit_iterator<ForwardIt2> first2,
    bit_iterator<ForwardIt2> last2) {
    (policy, first1, last1, first2, last2); 
    return true;
} 

// Status: on hold
template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2,
    class BinaryPredicate> constexpr bool equal(bit_iterator<ForwardIt1> first1,
    bit_iterator<ForwardIt1> last1, bit_iterator<ForwardIt2> first2,
    bit_iterator<ForwardIt2> last2, BinaryPredicate p) {
    (first1, last1, first2, last2, p);
    return true;
} 

// Status: on hold
template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2,
    class BinaryPredicate> bool equal(ExecutionPolicy&& policy,
    bit_iterator<ForwardIt1> first1, bit_iterator<ForwardIt1> last1,
    bit_iterator<ForwardIt2> first2, bit_iterator<ForwardIt2> last2,
    BinaryPredicate p) {
    (policy, first1, last1, first2, last2, p);
    return true;
}

// ========================================================================== //
} // namespace bit

#endif // _EQUAL_HPP_INCLUDED
// ========================================================================== //
