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

template <
    class Iter1, 
    class Iter2, 
    class ReadType = typename std::conditional<
      (binary_digits<typename bit_iterator<Iter1>::word_type>::value > 
       binary_digits<typename bit_iterator<Iter2>::word_type>::value),
      typename bit_iterator<Iter1>::word_type,
      typename bit_iterator<Iter2>::word_type
    >::type,
    bool Careful = false
>
class dual_range_reader {
    public:
        using read_pair_t = std::pair<ReadType, ReadType>;
        static constexpr std::size_t num_digits = binary_digits<ReadType>::value;
    private:
        
        std::size_t num_relevant_bits_;
        bit_value padding_value_;
        Iter1 word_cursor_1_, last_word_cursor_1_;
        Iter2 word_cursor_2_, last_word_cursor_2_;
        std::size_t cursor_pos_1_, cursor_pos_2_;
        std::size_t last_pos_1_, last_pos_2_;

        std::pair<std::size_t, std::size_t> next_read_lengths_, prev_read_lengths_;
        bool needs_next_lengths_updated_;
        bool is_mismatched_;

        read_pair_t pad_reads(read_pair_t unpadded_reads) const {
            ReadType mask_1, mask_2;
            read_pair_t padded_reads;

            if (Careful) {
                if (padding_value_ == bit0) {
                    mask_1 = _shift_towards_lsb(static_cast<ReadType>(_all_ones()), 
                        num_digits - prev_read_lengths_.first);
                    mask_2 = _shift_towards_lsb(static_cast<ReadType>(_all_ones()), 
                        num_digits - prev_read_lengths_.second);

                    padded_reads.first = unpadded_reads.first & mask_1;
                    padded_reads.second = unpadded_reads.second & mask_2;
                } else {
                    mask_1 = _shift_towards_lsb(static_cast<ReadType>(_all_ones()), 
                        prev_read_lengths_.first);
                    mask_2 = _shift_towards_lsb(static_cast<ReadType>(_all_ones()), 
                        prev_read_lengths_.second);

                    padded_reads.first = unpadded_reads.first | mask_1;
                    padded_reads.second = unpadded_reads.second | mask_2;
                }
            } else {
                ReadType mask;
                if (padding_value_ == bit0) {
                    mask = _shift_towards_lsb(static_cast<ReadType>(_all_ones()), 
                        num_digits - prev_read_lengths_.first);

                    padded_reads.first = unpadded_reads.first & mask;
                    padded_reads.second = unpadded_reads.second & mask;
                } else {
                    mask = _shift_towards_lsb(static_cast<ReadType>(_all_ones()), 
                        prev_read_lengths_.first);

                    padded_reads.first = unpadded_reads.first & mask;
                    padded_reads.second = unpadded_reads.second & mask;
                }
            }

            return padded_reads;  
        }

        template <std::size_t N>
        struct alignment {};

        using aligned_both = alignment<0>;
        using aligned_first = alignment<1>;
        using aligned_second = alignment<2>;
        using aligned_first_ahead = alignment<3>;
        using aligned_second_ahead = alignment<4>;
        using aligned_symmetric_unaligned = alignment<5>;

        bool is_aligned_both() const {
            return cursor_pos_1_ == 0
                && cursor_pos_2_ == 0;
        }

        bool is_aligned_first() const {
            return cursor_pos_1_ == 0;
        }

        bool is_aligned_second() const {
            return cursor_pos_2_ == 0;
        }

        bool is_aligned_first_ahead() const {
            return cursor_pos_1_ > cursor_pos_2_;
        }

        bool is_aligned_second_ahead() const {
            return cursor_pos_1_ < cursor_pos_2_;
        }

        bool is_aligned_symmetric_unaligned() const {
            return cursor_pos_1_ == cursor_pos_2_;
        }

        template <class Lambda>
        auto dispatch_by_alignment(Lambda lambda) const {
            if (is_aligned_both()) {
                return lambda(aligned_both{});
            } else if (is_aligned_first()) {
                return lambda(aligned_first{});
            } else if (is_aligned_second()) {
                return lambda(aligned_second{});
            } else if (is_aligned_symmetric_unaligned()) {
                return lambda(aligned_symmetric_unaligned{});
            } else if (is_aligned_second_ahead()) {
                return lambda(aligned_second_ahead{});
            } else {
                return lambda(aligned_first_ahead{});
            }
        }

        template <std::size_t N>
        auto get_positionals() const {
            if (N == 1) {
                return std::make_tuple(
                    word_cursor_1_, cursor_pos_1_, 
                    last_word_cursor_1_, last_pos_1_
                );
            } else {
                return std::make_tuple(
                    word_cursor_2_, cursor_pos_2_,
                    last_word_cursor_2_, last_pos_2_
                );
            }
        }

        std::pair<std::size_t, std::size_t> get_next_read_lengths(aligned_both) const {
            std::size_t first_len, second_len;

            if (word_cursor_1_ == last_word_cursor_1_) {
                first_len = last_pos_1_ - cursor_pos_1_;
            } else {
                first_len = num_digits;
            }

            if (Careful) {
                if (word_cursor_2_ == last_word_cursor_2_) {
                    second_len = last_pos_2_ - cursor_pos_2_;
                } else {
                    second_len = num_digits;
                }
            } else {
                second_len = first_len;
            }

            return std::make_pair(first_len, second_len);
        }


        template <std::size_t Aligned, std::size_t Unaligned>
        std::pair<std::size_t, std::size_t> get_next_read_lengths_one_aligned() const {
            std::size_t aligned_len, unaligned_len;

            auto [cursor_aligned, pos_aligned, 
                last_cursor_aligned, last_pos_aligned] = get_positionals<Aligned>();
            auto [cursor_unaligned, pos_unaligned, 
                last_cursor_unaligned, last_pos_unaligned] = get_positionals<Unaligned>();
            
            if (cursor_aligned == last_cursor_aligned) {
                aligned_len = last_pos_aligned - pos_aligned;
            } else {
                aligned_len = num_digits;
            }

            if (Careful) {
                if (last_cursor_unaligned == cursor_unaligned) {
                    unaligned_len = last_pos_unaligned - pos_unaligned;
                } else if (std::next(cursor_unaligned) == last_cursor_unaligned) {
                    unaligned_len = std::min(pos_unaligned, last_pos_unaligned);
                } else {
                    unaligned_len = pos_unaligned;
                }
            } else {
                unaligned_len = aligned_len;
            }

            return std::make_pair(aligned_len, unaligned_len);
        }

        std::pair<std::size_t, std::size_t> get_next_read_lengths(aligned_first) const {
            return get_next_read_lengths_one_aligned<1, 2>();
        }

        std::pair<std::size_t, std::size_t> get_next_read_lengths(aligned_second) const {
            std::pair<std::size_t, std::size_t> one_aligned_lens = 
                get_next_read_lengths_one_aligned<2, 1>();
            return std::make_pair(one_aligned_lens.second, one_aligned_lens.first);
        }

        std::pair<std::size_t, std::size_t> get_next_read_lengths(aligned_symmetric_unaligned) const {
            std::size_t first_len, second_len;

            if (word_cursor_1_ == last_word_cursor_1_) {
                first_len = last_pos_1_ - cursor_pos_1_;
            } else {
                first_len = num_digits - cursor_pos_1_;
            }

            if (Careful) {
                if (word_cursor_2_ == last_word_cursor_2_) {
                    second_len = last_pos_2_ - cursor_pos_2_;
                } else {
                    second_len = num_digits - cursor_pos_2_;
                }
            }

            return std::make_pair(first_len, second_len);
        }

        template <std::size_t Ahead, std::size_t Behind>
        std::pair<std::size_t, std::size_t> get_next_read_lengths_ahead_behind() const {
            auto [cursor_ahead, pos_ahead, 
                last_cursor_ahead, last_pos_ahead] = get_positionals<Ahead>();
            auto [cursor_behind, pos_behind, 
                last_cursor_behind, last_pos_behind] = get_positionals<Behind>();

            std::size_t ahead_len, behind_len;

            if (cursor_behind == last_cursor_behind) {
                behind_len = last_pos_behind - pos_behind;
            } else {
                behind_len = num_digits - pos_behind;
            }

            if (Careful) {
                if (cursor_ahead == last_cursor_ahead) {
                    ahead_len = last_pos_ahead - pos_ahead;
                } else if (std::next(cursor_ahead) == last_cursor_ahead) {
                    std::size_t new_pos_ahead = (pos_ahead + behind_len) % num_digits;
                    ahead_len = std::min(new_pos_ahead, last_pos_ahead);
                } else {
                    ahead_len = behind_len;
                }
            } else {
                ahead_len = behind_len;
            }

            return std::make_pair(ahead_len, behind_len);
        } 

        std::pair<std::size_t, std::size_t> get_next_read_lengths(aligned_first_ahead) const {
            return get_next_read_lengths_ahead_behind<1, 2>();
        }

        std::pair<std::size_t, std::size_t> get_next_read_lengths(aligned_second_ahead) const {
            std::pair<std::size_t, std::size_t> ahead_behind_lens =
                get_next_read_lengths_ahead_behind<2, 1>();
            return std::make_pair(ahead_behind_lens.second, ahead_behind_lens.first);
        }

    public:
        dual_range_reader(
            bit_iterator<Iter1> first1, bit_iterator<Iter1> last1, 
            bit_iterator<Iter2> first2, bit_iterator<Iter2> last2
        ) : word_cursor_1_(first1.base()), last_word_cursor_1_(last1.base()),
            word_cursor_2_(first2.base()), last_word_cursor_2_(last2.base()),
            cursor_pos_1_(first1.position()), cursor_pos_2_(first2.position()),
            last_pos_1_(last1.position()), last_pos_2_(last2.position()),
            prev_read_lengths_(std::make_pair(-1, -1)),
            needs_next_lengths_updated_(true), is_mismatched_(false)
        {
        }

        std::pair<std::size_t, std::size_t> get_prev_read_lengths() const {
            return prev_read_lengths_;
        }

        std::pair<std::size_t, std::size_t> get_next_read_lengths() {
            if (needs_next_lengths_updated_) {
                std::cout << "updating lengths.\n";
                next_read_lengths_ = dispatch_by_alignment(
                    [this] (auto alignment) { return this->get_next_read_lengths(alignment); }
                );
            }
            return next_read_lengths_;
        }

        read_pair_t read(aligned_both)



        read_pair_t read() {

            std::pair<std::size_t, std::size_t> next_read_lengths = get_next_read_lengths();

            if (next_read_lengths_.first != next_read_lengths_.second) {
                is_mismatched_ = true;
            }

            read_pair_t reads = dispatch_by_alignment(
                [this, next_read_lengths] (auto alignment) { 
                    return this->read(next_read_lengths, alignment);
                }
            );



            read_pair_t reads = std::make_pair(*word_cursor_1_, *word_cursor_2_);

            std::size_t new_pos_1 = (cursor_pos_1_ + next_read_lengths_.first) % num_digits;
            std::size_t new_pos_2 = (cursor_pos_2_ + next_read_lengths_.second) % num_digits;




            if (new_pos_1 <= cursor_pos_1_) {
                word_cursor_1_++;
            }

            if (new_pos_2 <= cursor_pos_2_) {
                word_cursor_2_++;
            }

            cursor_pos_1_ = new_pos_1;
            cursor_pos_2_ = new_pos_2;

            prev_read_lengths_ = next_read_lengths_;

            needs_next_lengths_updated_ = true;

            std::cout << "pos1_after: " << cursor_pos_1_ << std::endl;
            std::cout << "pos2_after: " << cursor_pos_2_ << std::endl;

            return reads;
        }

        read_pair_t read_padded() {
            read_pair_t unpadded_reads = read();
            return pad_reads(unpadded_reads);
        }

        bool has_more() {
            std::pair<std::size_t, std::size_t> next_read_lengths = get_next_read_lengths();
            return next_read_lengths.first > 0 || next_read_lengths.second > 0;
        }

        bool is_mismatched() const {
            return is_mismatched_;
        }

        void set_padding_value(const bit_value bv) {
            padding_value_ = bv;
        }

        template <std::size_t N>
            using iter_selector_t = typename std::conditional<N == 0, Iter1, Iter2>::type;
        
        template <std::size_t N>
        bit_iterator<iter_selector_t<N>> get_bit_iterator(index_t<N>) const {
            if (N == 0) {
                return bit_iterator(word_cursor_1_, cursor_pos_1_);
            } else {
                return bit_iterator(word_cursor_2_, cursor_pos_2_);
            }
        }

        template <std::size_t N>
        iter_selector_t<N> get_base_iterator(index_t<N>) const {
            if (N == 0) {
                return word_cursor_1_;
            } else {
                return word_cursor_2_;
            }
        }

        template <std::size_t N>
        std::size_t get_position(index_t<N>) const {
            if (N == 0) {
                return cursor_pos_1_;
            } else {
                return cursor_pos_2_;
            }
        }
};

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
dual_range_reader<Iter1, Iter2, ReadType, false> 
get_safe_dual_range_reader(Iter1&& first1, Iter1&& last1, 
    Iter2&& first2, Iter2&& last2) {
    return dual_range_reader<Iter1, Iter2, ReadType, false>
      (std::forward<Iter1>(first1), std::forward<Iter1>(last1),
       std::forward<Iter2>(first2), std::forward<Iter2>(last2));
}

}

#endif