// ================================= COPY =================================== //
// Project:     The Experimental Bit Algorithms Library
// Name:        copy.hpp
// Description: Implementation of copy, copy_if, copy_n and copy_backward 
// Creator:     Vincent Reverdy
// Contributor: Bryce Kille [2019]
// License:     BSD 3-Clause License
// ========================================================================== //
#ifndef _COPY_HPP_INCLUDED
#define _COPY_HPP_INCLUDED
// ========================================================================== //



// ================================ PREAMBLE ================================ //
// C++ standard library
// Project sources
#include "bit.hpp"
// Third-party libraries
// Miscellaneous
namespace bit {
// ========================================================================== //



// ---------------------------- Copy Algorithms ----------------------------- //

// Status: complete
template <class InputIt, class OutputIt>
bit_iterator<OutputIt> copy(bit_iterator<InputIt> first,
                            bit_iterator<InputIt> last,
                            bit_iterator<OutputIt> d_first
)
{
    // Assertions
    _assert_range_viability(first, last);
    if (first == last) return d_first;

    // Types and constants
    using dst_word_type = typename bit_iterator<OutputIt>::word_type;
    using dst_size_type = typename bit_iterator<OutputIt>::size_type;
    constexpr dst_size_type dst_digits = binary_digits<dst_word_type>::value;

    // Initialization
    const bool is_d_first_aligned = d_first.position() == 0;
    dst_size_type total_bits_to_copy = std::distance(first, last);
    auto it = d_first.base();

    // d_first is not aligned. Copy partial word to align it
    if (!is_d_first_aligned) {
        dst_size_type partial_bits_to_copy = std::min(
                total_bits_to_copy,
                dst_digits - d_first.position()
                );
        *it = _bitblend(
                *it,
                static_cast<dst_word_type>(
                  get_word<dst_word_type>(first, partial_bits_to_copy) 
                    << static_cast<dst_word_type>(d_first.position())
                ),
                static_cast<dst_word_type>(d_first.position()),
                static_cast<dst_word_type>(partial_bits_to_copy)
                );
        total_bits_to_copy -= partial_bits_to_copy;
        std::advance(first, partial_bits_to_copy);
        it++;
    }

    while (total_bits_to_copy >= dst_digits) {
        *it = get_word<dst_word_type>(first, dst_digits);
        total_bits_to_copy -= dst_digits;
        it++; 
        std::advance(first, dst_digits);
    }
    if (total_bits_to_copy) {
        *it = _bitblend(
                *it,
                get_word<dst_word_type>(first, total_bits_to_copy),
                static_cast<dst_word_type>(
                    (static_cast<dst_word_type>(1) << total_bits_to_copy) - 1)
        );
    }
    return bit::bit_iterator<OutputIt>(it, total_bits_to_copy);
}

// Status: complete
template <class InputIt, class OutputIt, class UnaryPredicate>
constexpr bit_iterator<OutputIt> copy_if(bit_iterator<InputIt> first,
    bit_iterator<InputIt> last, bit_iterator<OutputIt> d_first,
    UnaryPredicate pred) {
    return std::copy_if(first, last, d_first, pred);
}

// Status: complete
template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2,
    class UnaryPredicate> bit_iterator<ForwardIt2> copy_if(
    ExecutionPolicy&& policy,
    bit_iterator<ForwardIt1> first, bit_iterator<ForwardIt1> last,
    bit_iterator<ForwardIt2> d_first, UnaryPredicate pred) {
    return std::copy_if(std::forward<ExecutionPolicy>(policy), first, last,
        d_first, pred);
}  

// -------------------------------------------------------------------------- //



// ========================================================================== //
} // namespace bit
#endif // _COPY_HPP_INCLUDED
// ========================================================================== //
