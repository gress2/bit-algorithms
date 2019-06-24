// ================================= RANGE READER ================================= //
// Project: The Experimental Bit Algorithms Library
// Name: range_reader.hpp
// Description: A utility class for low overhead bit_iterator range reading
// Creator: Vincent Reverdy
// Contributor(s): Collin Gress [2019]
// License: BSD 3-Clause License
// ========================================================================== //
#ifndef _RANGE_READER_HPP_INCLUDED
#define _RANGE_READER_HPP_INCLUDED
// ========================================================================== //



// ============================== PREAMBLE ================================== //
// C++ standard library
#include <fstream>
// Project sources
// Third-party libraries
// Miscellaneous

namespace bit {
// ========================================================================== //
template <
    class Iter1, 
    class Iter2, 
    class ReadType = typename std::conditional<
      (binary_digits<typename bit_iterator<Iter1>::word_type>::value > 
       binary_digits<typename bit_iterator<Iter2>::word_type>::value),
      typename bit_iterator<Iter1>::word_type,
      typename bit_iterator<Iter2>::word_type
    >::type
>
class dual_range_reader {
public:

private:
    enum alignment_type_ {
        first,
        second,
        both,
        first_pos_greater,
        second_pos_greater
    };

    bit_iterator<Iter1> first1_;
    bit_iterator<Iter1> last1_;
    bit_iterator<Iter2> first2_;
    bit_iterator<Iter2> last2_;
    alignment_type_ alignment_;
    bit_value padding_value_;
    std::size_t relevant_bits_;

public:

    using read_pair_t = std::pair<ReadType, ReadType>;
    static constexpr std::size_t num_digits = binary_digits<ReadType>::value;

    dual_range_reader(
        bit_iterator<Iter1> first1, bit_iterator<Iter1> last1,
        bit_iterator<Iter2> first2, bit_iterator<Iter2> last2
    ) : first1_(first1), last1_(last1), 
        first2_(first2), last2_(last2),
        padding_value_(bit0), relevant_bits_(0) 
    {
        if (_is_aligned_lsb(first1_) && _is_aligned_lsb(first2_)) {
            alignment_ = alignment_type_::both;
        } else if (_is_aligned_lsb(first1_)) {
            alignment_ = alignment_type_::first;
        } else if (_is_aligned_lsb(first2_)) {
            alignment_ = alignment_type_::second;
        } else {
            if (first1_.position() > first2_.position()) {
                alignment_ = alignment_type_::first_pos_greater;
            } else {
                alignment_ = alignment_type_::second_pos_greater;
            }
        }
    }

    void set_padding_value(const bit_value bv) {
        padding_value_ = bv;
    }

    std::size_t get_relevant_bits() const {
        return relevant_bits_;
    }

    bool has_full_word() const {
        return first1_ != last1_;
    }

    read_pair_t pad_bits(read_pair_t read) const {
        ReadType mask;
        if (padding_value_ == bit0) {
            mask = _shift_towards_lsb(
                static_cast<ReadType>(_all_ones()), num_digits - relevant_bits_);
            read.first &= mask;
            read.second &= mask;
        } else {
            mask = _shift_towards_msb(
                static_cast<ReadType>(_all_ones()), relevant_bits_);
            read.first |= mask;
            read.second |= mask;
        }
        return read;
    }

    // TODO: look into having two overloads to avoid the bits_read shit
    read_pair_t read_first() {
        ReadType read1, read2;
        relevant_bits_ = num_digits; // assume full read until we know otherwise

        if (alignment_ == alignment_type_::both) {
            read1 = *first1_.base();
            read2 = *first2_.base();
        } else if (alignment_ == alignment_type_::first) {
            if (_in_same_word(first1_, last1_)) {
                relevant_bits_ = num_digits - last1_.position();
            } else {
                relevant_bits_ = num_digits;
            }
            read1 = *first1_.base();
            read2 = get_word(first2_, relevant_bits_);
        } else if (alignment_ == alignment_type_::second) {
            if (_in_same_word(first2_, last2_)) {
                relevant_bits_ = num_digits - last2_.position();
            } else {
                relevant_bits_ = num_digits;
            } 
            read1 = get_word(first1_, relevant_bits_);
            read2 = *first2_.base();
        } else if (alignment_ == alignment_type_::first_pos_greater) {
            if (_in_same_word(first2_, last2_)) {
                relevant_bits_ = last2_.position() - first2_.position(); 
            } else {
                relevant_bits_ = num_digits - first2_.position();
            }
            read1 = get_word(first1_, relevant_bits_); 
            read2 = _shift_towards_lsb(
                static_cast<ReadType>(*first2_.base()), first2_.position());
            alignment_ = alignment_type_::second;
        } else { // alignment_ == alignment_type_::second_pos_greater
            if (_in_same_word(first1_, last1_)) {
                relevant_bits_ = last1_.position() - first1_.position();
            } else {
                relevant_bits_ = num_digits - first1_.position();
            }
            read1 = _shift_towards_lsb(
                static_cast<ReadType>(*first1_.base()), first1_.position());
            read2 = get_word(first2_, relevant_bits_);
            alignment_ = alignment_type_::first;
        }

        // TODO: for the aligned one just use std::next()
        first1_ += relevant_bits;
        first2_ += relevant_bits;

        return std::make_pair(read1, read2);
    } 

    read_pair_t read_first(bit_value bv, std::size_t* const bits_read = nullptr) {
        read_pair_t read = read_first(bits_read);
        const std::size_t bits_to_pad = num_digits - *bits_read;
        if (bits_to_pad) {
            read = pad_bits(read, bits_to_pad, bv);
        }
        return read;
    }
 
    read_pair_t read(std::size_t* const bits_read = nullptr) {
        ReadType read1, read2;
        constexpr std::size_t num_digits = binary_digits<ReadType>::value;
        std::size_t relevant_bits = num_digits;

        if (alignment_ == alignment_type_::both) {
            read1 = *first1_.base();
            read2 = *first2_.base();
            if (last1_.base() == first1_.base()) {
                relevant_bits = last1_.position();
            }         
        } else if (alignment_ == alignment_type_::first) {
            read1 = *first1_.base();
            read2 = get_word(first2_, num_digits);
            if (first1_.base() == last1_.base()) {
                relevant_bits = last1_.position();
            }
        } else {
            // either alignment is second or first_pos_greater
            read1 = get_word(first1_, num_digits);
            read2 = *first2_.base();
            if (first2_.base() == last2_.base()) {
                relevant_bits = last2_.position();
            }
        }
        

        /*
        auto reader = ...
        auto p = reader.read_first();

        while (reader.ok) {
            p = reader.read();
        }

        p = reader.read_last();
        */


        first1_ += relevant_bits;
        first2_ += relevant_bits;

        if (bits_read != nullptr) {
            *bits_read = relevant_bits;
        }

        return std::make_pair(read1, read2);
    }

    read_pair_t read(bit_value bv, std::size_t* const bits_read = nullptr) {
        read_pair_t read = read(bits_read);
        const std::size_t bits_to_pad = num_digits - *bits_read;
        if (bits_to_pad) {
            read = pad_bits(read, bits_to_pad, bv);
        }
        return read;
    }

    template <int N>
    using iter_selector_t = typename std::conditional<N == 0, Iter1, Iter2>::type;

    template <int N>
    bit_iterator<iter_selector_t<N>> get_bit_iterator() const {
        return N == 0 ? first1_ : first2_;
    }

    template <int N>
    iter_selector_t<N> get_base_iterator() const {
        return N == 0 ? first1_.base() : first2_.base();
    }

    template <int N>
    std::size_t get_position() const {
        return N == 0 ? first1_.position() : first2_.position();
    }
};


// ========================================================================== //
} // namespace bit

#endif // _EQUAL_HPP_INCLUDED
// ========================================================================== //
