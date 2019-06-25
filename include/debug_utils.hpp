// =========================== DEBUG_UTILS ============================== //
// Project:     The Experimental Bit Algorithms Library
// Name:        debug_utils.hpp
// Description: Utilities useful for debugging
// Creator:     Vincent Reverdy
// Contributor: Bryce Kille [2019]
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _DEBUG_HPP_INCLUDED
#define _DEBUG_HPP_INCLUDED
// ========================================================================== //



// ================================ PREAMBLE ================================ //
// C++ standard library
#include <bitset>
// Project sources
#include "bit.hpp"
// Third-party libraries
// Miscellaneous
namespace bit {
// ========================================================================== //



// --------------------------- Utility Functions ---------------------------- //
template <class T1>
std::string word_to_vec(T1 word) {
    std::bitset<binary_digits<T1>::value> word_c(word);
    std::string out = word_c.to_string();
    std::reverse(out.begin(), out.end());
    return out;
}

template <class T>
void print_as_binary(T num) {
  constexpr std::size_t num_digits = binary_digits<T>::value;
  std::cout << "[MSB] " << (std::bitset<num_digits>(num)) << " [LSB]\n"; 
}
// -------------------------------------------------------------------------- //



// ========================================================================== //
} // namespace bit
#endif // _COPY_HPP_INCLUDED
// ========================================================================== //
