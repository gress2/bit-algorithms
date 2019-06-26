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
#include "range_reader.hpp"
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

template <class T>
std::string to_bit_string(T t) {
    return std::bitset<bit::binary_digits<T>::value>(t).to_string();
}

// lord forgive me for creating this abomination
template <class T>
void display(T drr) {
    using namespace bit;

    using pair_type = typename T::read_pair_t;
    using word_type = typename pair_type::first_type;
    constexpr std::size_t num_digits = bit::binary_digits<word_type>::value;
    
    std::vector<std::string> first_word_bit_strings;
    std::vector<std::string> second_word_bit_strings;
    std::vector<std::size_t> first_positions;
    std::vector<std::size_t> second_positions;

    // before any reads are done
    first_word_bit_strings.push_back(
        to_bit_string(*(drr.get_base_iterator(index<0>))));
    second_word_bit_strings.push_back(
        to_bit_string(*(drr.get_base_iterator(index<1>))));
    first_positions.push_back(drr.get_position(index<0>));
    second_positions.push_back(drr.get_position(index<1>));

    drr.read_first();
    
    do {
        first_word_bit_strings.push_back(
            to_bit_string(*(drr.get_base_iterator(index<0>))));
        second_word_bit_strings.push_back(
            to_bit_string(*(drr.get_base_iterator(index<1>))));
        first_positions.push_back(drr.get_position(index<0>));
        second_positions.push_back(drr.get_position(index<1>));

        drr.read();
    } while (!drr.is_next_read_last()); 

    first_word_bit_strings.push_back(
        to_bit_string(*(drr.get_base_iterator(index<0>))));
    second_word_bit_strings.push_back(
        to_bit_string(*(drr.get_base_iterator(index<1>))));
    first_positions.push_back(drr.get_position(index<0>));
    second_positions.push_back(drr.get_position(index<1>));

    drr.read_last();

    std::size_t last_read_rel_bits = drr.get_num_relevant_bits();

    if (last_read_rel_bits + first_positions.back() > num_digits) {
        first_positions.push_back(last_read_rel_bits + first_positions.back() - num_digits); 
        first_word_bit_strings.push_back(
            to_bit_string(*std::next(drr.get_base_iterator(index<0>))));
    } else {
        first_positions.push_back(drr.get_num_relevant_bits() + first_positions.back());
    }

    if (last_read_rel_bits + second_positions.back() > num_digits) {
        second_positions.push_back(last_read_rel_bits + second_positions.back() - num_digits); 
        second_word_bit_strings.push_back(
            to_bit_string(*std::next(drr.get_base_iterator(index<1>))));
    } else {
        second_positions.push_back(drr.get_num_relevant_bits() + second_positions.back());
    }

    for (std::size_t i = 0; i < first_positions.size(); i++) {
        std::string to_be_inserted = "][";
        std::size_t string_idx = i;
        std::size_t position_to_insert = num_digits - first_positions[i];

        if (position_to_insert > num_digits) { break; }

        if (i == 0) {
            to_be_inserted = "]";
        } else if (i == first_positions.size() - 1) {
            if (first_positions.size() > first_word_bit_strings.size()) {
                string_idx--;
            }
            to_be_inserted = "[";
        }

        first_word_bit_strings[string_idx].insert(position_to_insert, to_be_inserted); 
    }

    for (std::size_t i = 0; i < second_positions.size(); i++) {
        std::string to_be_inserted = "][";
        std::size_t string_idx = i;
        std::size_t position_to_insert = num_digits - second_positions[i];

        if (position_to_insert > num_digits) { break; }

        if (i == 0) {
            to_be_inserted = "]";
        } else if (i == second_positions.size() - 1) {

            if (second_positions.size() > second_word_bit_strings.size()) {
                string_idx--;
            }
            to_be_inserted = "[";
        }

        second_word_bit_strings[string_idx].insert(position_to_insert, to_be_inserted); 
    }

    std::reverse(first_word_bit_strings.begin(), first_word_bit_strings.end());
    std::reverse(second_word_bit_strings.begin(),second_word_bit_strings.end());

    std::string first_concat;
    for (std::string& s : first_word_bit_strings) {
        first_concat += " ";
        first_concat += s;
    }

    std::string second_concat;
    for (std::string& s : second_word_bit_strings) {
        second_concat += " ";
        second_concat += s;
    }

    if (first_concat.size() < second_concat.size()) {
        std::size_t diff = second_concat.size() - first_concat.size();
        first_concat.insert(0, diff, ' ');
    } else if (second_concat.size() < first_concat.size()) {
        std::size_t diff = first_concat.size() - second_concat.size();
        second_concat.insert(0, diff, ' ');
    }

    std::cout << "end" << first_concat << " start\n";
    std::cout << "end" << second_concat << " start\n\n";
}




// -------------------------------------------------------------------------- //



// ========================================================================== //
} // namespace bit
#endif // _COPY_HPP_INCLUDED
// ========================================================================== //
