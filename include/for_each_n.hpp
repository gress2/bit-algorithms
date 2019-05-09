// =============================== FOR EACH N =============================== //
// Project: The Experimental Bit Algorithms Library
// Name: for_each_n.hpp
// Description: bit_iterator overloads for std::for_each_n
// Creator: Vincent Reverdy
// Contributor(s): 
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _FOR_EACH_N_HPP_INCLUDED
#define _FOR_EACH_N_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
// Third-party libraries
// Miscellaneous
namespace bit {
// ========================================================================== //

template <class InputIt, class Size, class UnaryFunction>
constexpr bit_iterator<InputIt> for_each_n(bit_iterator<InputIt> first,
    Size n, UnaryFunction f) {
    return std::for_each_n(first, n, f);
}

template <class ExecutionPolicy, class ForwardIt, class Size, 
    class UnaryFunction2> bit_iterator<ForwardIt> for_each_n (
    ExecutionPolicy&& policy, bit_iterator<ForwardIt> first, Size n,
    UnaryFunction2 f) {
    return std::for_each_n(std::forward<ExecutionPolicy>(policy), first, n, f);
}

// ========================================================================== //
} // namespace bit

#endif // _FOR_EACH_N_HPP_INCLUDED
// ========================================================================== //
