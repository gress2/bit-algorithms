// =============================== TRANSFORM =============================== //
// Project: The Experimental Bit Algorithms Library
// Name: transform.hpp
// Description: bit_iterator overloads for std::transform
// Creator: Vincent Reverdy
// Contributor(s): 
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _TRANSFORM_HPP_INCLUDED
#define _TRANSFORM_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
// Third-party libraries
// Miscellaneous
namespace bit {
// ========================================================================== //

// Status: complete
template <class InputIt, class OutputIt, class UnaryOperation>
constexpr bit_iterator<OutputIt> transform(bit_iterator<InputIt> first1,
    bit_iterator<InputIt> last1, bit_iterator<OutputIt> d_first,
    UnaryOperation unary_op) {
    return std::transform(first1, last1, d_first, unary_op);
} 

// Status: complete
template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2,
    class UnaryOperation> bit_iterator<ForwardIt2> transform(
    ExecutionPolicy&& policy, bit_iterator<ForwardIt1> first1, 
    bit_iterator<ForwardIt1> last1, bit_iterator<ForwardIt2> d_first, 
    UnaryOperation unary_op) {
    return std::transform(std::forward<ExecutionPolicy>(policy), first1,
       last1, d_first, unary_op); 
}

// Status: complete
template <class InputIt1, class InputIt2, class OutputIt, class BinaryOperation>
constexpr bit_iterator<OutputIt> transform(bit_iterator<InputIt1> first1,
    bit_iterator<InputIt1> last1, bit_iterator<InputIt2> first2,
    bit_iterator<OutputIt> d_first, BinaryOperation binary_op) {
    return std::transform(first1, last1, first2, d_first, binary_op);
}

// Status: complete
template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2,
    class ForwardIt3, class BinaryOperation> bit_iterator<ForwardIt3>
    transform(ExecutionPolicy&& policy, bit_iterator<ForwardIt1> first1, 
    bit_iterator<ForwardIt1> last1, bit_iterator<ForwardIt2> first2,
    bit_iterator<ForwardIt3> d_first, BinaryOperation binary_op) {
    return std::transform(std::forward<ExecutionPolicy>(policy), first1,
        last1, first2, d_first, binary_op);
}


// ========================================================================== //
} // namespace bit

#endif // _TRANSFORM_HPP_INCLUDED
// ========================================================================== //
