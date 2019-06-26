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
// Project sources
#include "bit_algorithm_details.hpp"
// Third-party libraries
// Miscellaneous

namespace bit {
// ========================================================================== //

template <std::size_t N> 
struct index_t : std::integral_constant<std::size_t, N>
{}; 

template <std::size_t N>
inline constexpr index_t<N> index = index_t<N>{};

/* still TODO (6/24):
    -> add more tests. particularly for cases where word types are different
    -> improve readability and/or documentation. clearly define how this should be used
       and, more importantly, where this should not be used. i.e. the user needs
       to check for the ranges are at least num_digits long.
    -> doxygen ?
*/
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
    using read_pair_t = std::pair<ReadType, ReadType>;
    static constexpr std::size_t num_digits = binary_digits<ReadType>::value;
private:
    /* first: first1_ is LSB aligned and first2_ is not
     * second: first2_ is LSB aligned and first1_ is not
     * both: first1_ and first2_ is LSB aligned
     * first_pos_greater: neither first1_ and first2_ are LSB aligned and 
     *     first1_.position() > first2_.position()
     * second_pos_greater: neither first1_ and first2_ are LSB aligned and
     *     first2_.position() > first1_.position()
     */
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
    std::size_t num_relevant_bits_;
    bool is_next_read_last_;

    // sets is_next_read_last_ flag. should be called at the end of read_first()
    // and read_last(). if is_next_read_last_ is true, your next read should be
    // done using read_last(). otherwise, just call read().
    void check_if_next_read_is_last() {
        if (alignment_ == alignment_type_::both 
            || alignment_ == alignment_type_::first) {
            is_next_read_last_ =
                std::next(first1_.base()) == last1_.base() && first1_.position() == 0
                || first1_.base() == last1_.base(); 
        } else {
            is_next_read_last_ =
                std::next(first2_.base()) == last2_.base() && first2_.position() == 0
                || first2_.base() == last2_.base();
        } 
    }

    // Sets all non relevant bits of a read to padding_value_. Non relevant bits 
    // are determined based on the value of num_relevant_bits_ which is set at 
    // the end of each read 
    read_pair_t pad_bits(read_pair_t read) const {
        ReadType mask;
        if (padding_value_ == bit0) {
            mask = _shift_towards_lsb(
                static_cast<ReadType>(_all_ones()), num_digits - num_relevant_bits_);
            read.first &= mask;
            read.second &= mask;
        } else {
            mask = _shift_towards_msb(
                static_cast<ReadType>(_all_ones()), num_relevant_bits_);
            read.first |= mask;
            read.second |= mask;
        }
        return read;
    }

public:
    
    dual_range_reader(
        bit_iterator<Iter1> first1, bit_iterator<Iter1> last1,
        bit_iterator<Iter2> first2, bit_iterator<Iter2> last2
    ) : first1_(first1), last1_(last1), 
        first2_(first2), last2_(last2),
        padding_value_(bit0), num_relevant_bits_(0) 
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

    // changes the bit value used in padded reads
    void set_padding_value(const bit_value bv) {
        padding_value_ = bv;
    }

    // gives the number of bits returned in the last read which
    // actually belonged to the two ranges 
    std::size_t get_num_relevant_bits() const {
        return num_relevant_bits_;
    }

    // returns true if the next read will read bits in the last
    // words of the ranges
    bool is_next_read_last() {
        return is_next_read_last_; 
    }

    /* this must be called before calling read() unless the beginning of
       both ranges are LSB aligned. 

       Important: this function does not check if the end of the range is in 
       this first read. i.e. you shouldn't use this class if you have 
       ranges smaller than num_digits 
    */
    read_pair_t read_first() {
        ReadType read1, read2;
        num_relevant_bits_ = num_digits;

        if (alignment_ == alignment_type_::both) {
            read1 = *first1_.base();
            read2 = *first2_.base();
            first1_ = bit_iterator(std::next(first1_.base()));
            first2_ = bit_iterator(std::next(first2_.base()));
        } else if (alignment_ == alignment_type_::first) {
            read1 = *first1_.base();
            read2 = get_word(first2_, num_relevant_bits_);
            first1_ = bit_iterator(std::next(first1_.base())); 
            first2_ += num_relevant_bits_;
        } else if (alignment_ == alignment_type_::second) {
            read1 = get_word(first1_, num_relevant_bits_);
            read2 = *first2_.base();
            first1_ += num_relevant_bits_;
            first2_ = bit_iterator(std::next(first2_.base()));
        } else if (alignment_ == alignment_type_::first_pos_greater) {
            num_relevant_bits_ = num_digits - first2_.position();
            read1 = get_word(first1_, num_relevant_bits_); 
            read2 = _shift_towards_lsb(
                static_cast<ReadType>(*first2_.base()), first2_.position());
            alignment_ = alignment_type_::second;
            first1_ += num_relevant_bits_;
            first2_ += num_relevant_bits_;
        } else { // alignment_ == alignment_type_::second_pos_greater
            num_relevant_bits_ = num_digits - first1_.position();
            read1 = _shift_towards_lsb(
                static_cast<ReadType>(*first1_.base()), first1_.position());
            read2 = get_word(first2_, num_relevant_bits_);
            alignment_ = alignment_type_::first;
            first1_ += num_relevant_bits_;
            first2_ += num_relevant_bits_;
        }

        check_if_next_read_is_last();

        return std::make_pair(read1, read2);
    } 

    // the first read of the ranges but will pad out irrelevant bits on the MSB 
    // side with padding_value_.
    read_pair_t read_first_padded() {
        read_pair_t read = read_first();
        const std::size_t bits_to_pad = num_digits - num_relevant_bits_;
        if (bits_to_pad) {
            read = pad_bits(read);
        }
        return read;
    }
 
    // should be called after read_first() and until is_next_read_last().
    // assuming used in this way, should always return full words 
    read_pair_t read() {
        ReadType read1, read2;
        num_relevant_bits_ = num_digits;

        if (alignment_ == alignment_type_::both) {
            read1 = *first1_.base();
            read2 = *first2_.base();
            first1_ = bit_iterator(std::next(first1_.base()));
            first2_ = bit_iterator(std::next(first2_.base()));
        } else if (alignment_ == alignment_type_::first) {
            read1 = *first1_.base();
            read2 = get_word(first2_, num_digits);
            first1_ = bit_iterator(std::next(first1_.base()));
            first2_ += num_relevant_bits_;
        } else {
            // either alignment is second or first_pos_greater
            read1 = get_word(first1_, num_digits);
            read2 = *first2_.base();
            first1_ += num_relevant_bits_;
            first2_ = bit_iterator(std::next(first2_.base()));
        }

        check_if_next_read_is_last();

        return std::make_pair(read1, read2);
    }

    // should be called when is_next_read_last(). you need to take care to check
    // the number of relative bits when this is used because this will in many
    // cases read past the end of the ranges. 
    read_pair_t read_last() {
        ReadType read1, read2;
        num_relevant_bits_ = num_digits;

        if (alignment_ == alignment_type_::both) {
            read1 = *first1_.base();
            read2 = *first2_.base();
        } else if (alignment_ == alignment_type_::first) {
            num_relevant_bits_ = 
                last1_.position() == 0 ? num_digits : last1_.position();
            read1 = *first1_.base();
            read2 = get_word(first2_, num_relevant_bits_);
        } else {
            num_relevant_bits_ =
                last2_.position() == 0 ? num_digits : last2_.position();
            read1 = get_word(first1_, num_relevant_bits_);
            read2 = *first2_.base();
        }

        return std::make_pair(read1, read2);
    } 

    read_pair_t read_last_padded() {
        read_pair_t read = read_last();
        const std::size_t bits_to_pad = num_digits - num_relevant_bits_;
        if (bits_to_pad) {
            read = pad_bits(read);
        }
        return read;
    }

    template <std::size_t N>
    using iter_selector_t = typename std::conditional<N == 0, Iter1, Iter2>::type;

    template <std::size_t N>
    bit_iterator<iter_selector_t<N>> get_bit_iterator(index_t<N>) const {
        return N == 0 ? first1_ : first2_;
    }

    template <std::size_t N>
    iter_selector_t<N> get_base_iterator(index_t<N>) const {
        return N == 0 ? first1_.base() : first2_.base();
    }

    template <std::size_t N>
    std::size_t get_position(index_t<N>) const {
        return N == 0 ? first1_.position() : first2_.position();
    }
};


// ========================================================================== //
} // namespace bit

#endif // _RANGE_READER_HPP_INCLUDED
// ========================================================================== //
