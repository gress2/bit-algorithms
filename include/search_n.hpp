// ================================ SEARCH N ================================ //
// Project: The Experimental Bit Algorithms Library
// Name: search_n.hpp
// Description: bit_iterator overloads for std::search_n
// Creator: Vincent Reverdy
// Contributor(s): 
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _SEARCH_N_HPP_INCLUDED
#define _SEARCH_N_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
// Third-party libraries
// Miscellaneous
namespace bit {
// ========================================================================== //

// Status: to do
template <class ForwardIt, class Size, class T>
constexpr bit_iterator<ForwardIt> search_n(bit_iterator<ForwardIt> first,
    bit_iterator<ForwardIt> last, Size count, bit::bit_value bv) {
    (last, count, bv);
    return first;
}

// Status: to do
template <class ExecutionPolicy, class ForwardIt, class Size, class T>
bit_iterator<ForwardIt> search_n(ExecutionPolicy&& policy,
    bit_iterator<ForwardIt> first, bit_iterator<ForwardIt> last, Size count,
    bit::bit_value bv) {
    (policy, last, count, bv);
    return first;
}

// Status: complete
template <class ForwardIt, class Size, class T, class BinaryPredicate>
constexpr bit_iterator<ForwardIt> search_n(bit_iterator<ForwardIt> first,
    bit_iterator<ForwardIt> last, Size count, bit::bit_value bv, 
    BinaryPredicate p) {
    return std::search_n(first, last, count, bv, p);
}

// Status: complete
template <class ExecutionPolicy, class ForwardIt, class Size, class T,
         class BinaryPredicate> bit_iterator<ForwardIt> search_n(
    ExecutionPolicy&& policy, bit_iterator<ForwardIt> first, 
    bit_iterator<ForwardIt> last, Size count, bit::bit_value bv, 
    BinaryPredicate p) {
    return std::search_n(std::forward<ExecutionPolicy>(policy), first, last,
        count, bv, p);
}

// ========================================================================== //
} // namespace bit

#endif // _SEARCH_N_HPP_INCLUDED
// ========================================================================== //
