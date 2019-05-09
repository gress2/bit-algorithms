// =============================== GENERATE N =============================== //
// Project: The Experimental Bit Algorithms Library
// Name: generate_n.hpp
// Description: bit_iterator overloads for std::generate_n
// Creator: Vincent Reverdy
// Contributor(s): 
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _GENERATE_N_HPP_INCLUDED
#define _GENERATE_N_HPP_INCLUDED
// ========================================================================== //

// ============================== PREAMBLE ================================== //
// C++ standard library
// Project sources
// Third-party libraries
// Miscellaneous
namespace bit {
// ========================================================================== //

// Status: complete
template <class OutputIt, class Size, class Generator>
constexpr bit_iterator<OutputIt> generate_n(bit_iterator<OutputIt> first,
    Size count, Generator g) {
    return std::generate_n(first, count, g);
}

// Status: complete
template <class ExecutionPolicy, class ForwardIt, class Size, class Generator>
bit_iterator<ForwardIt> generate_n(ExecutionPolicy&& policy,
    bit_iterator<ForwardIt> first, Size count, Generator g) {
    return std::generate_n(std::forward<ExecutionPolicy>(policy), first, 
        count, g);
}

// ========================================================================== //
} // namespace bit

#endif // _GENERATE_N_HPP_INCLUDED
// ========================================================================== //
