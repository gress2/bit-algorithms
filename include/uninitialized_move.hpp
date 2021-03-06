// =========================== UNINITIALIZED MOVE =========================== //
// Project: The Experimental Bit Algorithms Library
// Name: uninitialized_move.hpp
// Description: bit_iterator overloads for std::uninitialized_move
// Creator: Vincent Reverdy
// Contributor(s): 
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _UNINITIALIZED_MOVE_HPP_INCLUDED
#define _UNINITIALIZED_MOVE_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
// Third-party libraries
// Miscellaneous

namespace bit {
// ========================================================================== //



// Status: to do
template <class InputIt, class ForwardIt>
bit_iterator<ForwardIt> uninitialized_move(bit_iterator<InputIt> first,
    bit_iterator<InputIt> last, bit_iterator<ForwardIt> d_first) {
    (first, last);
    return d_first;
}

// Status: to do
template <class ExecutionPolicy, class InputIt, class ForwardIt>
bit_iterator<ForwardIt> uninitialized_move(ExecutionPolicy&& policy,
    bit_iterator<InputIt> first, bit_iterator<InputIt> last,
    bit_iterator<ForwardIt> d_first) {
    (policy, first, last);
    return d_first;
}


// ========================================================================== //
} // namespace bit

#endif // _UNINITIALIZED_MOVE_HPP_INCLUDED
// ========================================================================== //
