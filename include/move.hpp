// ================================== MOVE ================================== //
// Project: The Experimental Bit Algorithms Library
// Name: move.hpp
// Description: bit_iterator overloads for std::move
// Creator: Vincent Reverdy
// Contributor(s): 
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _MOVE_HPP_INCLUDED
#define _MOVE_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
// Third-party libraries
// Miscellaneous
namespace bit {
// ========================================================================== //

// Status: to do
// Notes: can we implement this using bit::copy?
template <class InputIt, class OutputIt>
constexpr bit_iterator<OutputIt> move(bit_iterator<InputIt> first,
    bit_iterator<InputIt> last, bit_iterator<OutputIt> d_first) {
    (first, last);
    return d_first;
}

// Status: to do
template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2>
bit_iterator<ForwardIt2> move(ExecutionPolicy&& policy, 
    bit_iterator<ForwardIt1> first, bit_iterator<ForwardIt1> last,
    bit_iterator<ForwardIt2> d_first) {
    (policy, first, last);
    return d_first;
}

// ========================================================================== //
} // namespace bit

#endif // _MOVE_HPP_INCLUDED
// ========================================================================== //
