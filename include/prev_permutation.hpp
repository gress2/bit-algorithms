// ============================ PREV PERMUTATION ============================ //
// Project: The Experimental Bit Algorithms Library
// Name: prev_permutation.hpp
// Description: bit_iterator overloads for std::prev_permutation
// Creator: Vincent Reverdy
// Contributor(s): 
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _PREV_PERMUTATION_HPP_INCLUDED
#define _PREV_PERMUTATION_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
// Third-party libraries
// Miscellaneous

namespace bit {
// ========================================================================== //



// Status: to do
template <class BidirIt>
constexpr bool prev_permutation(bit_iterator<BidirIt> first, 
    bit_iterator<BidirIt> last) {
    (first, last);
    return true;
}

// Status: on hold
template <class BidirIt, class Compare>
constexpr bool prev_permutation(bit_iterator<BidirIt> first, 
    bit_iterator<BidirIt> last, Compare comp) {
    (first, last, comp);
    return true;
}

// ========================================================================== //
} // namespace bit

#endif // _PREV_PERMUTATION_HPP_INCLUDED
// ========================================================================== //
