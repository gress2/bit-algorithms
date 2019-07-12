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
        std::size_t cursor_pos_1_, cursor_pos_2_;
        std::size_t last_pos_1_, last_pos_2_;
        std::size_t num_relevant_bits_;
        bit_value padding_value_;
        Iter1 word_cursor_1_, last_word_cursor_1_;
        Iter2 word_cursor_2_, last_word_cursor_2_;

    public:
        dual_range_reader(
            bit_iterator<Iter1> first1, bit_iterator<Iter1> last1, 
            bit_iterator<Iter2> first2, bit_iterator<Iter2> last2
        ) : word_cursor_1_(first1.base()), last_word_cursor_1_(last1.base()),
            word_cursor_2_(first2.base()), last_base_2_(last2.base()),
            cursor_pos_1_(first1.position()), cursor_pos_2_(first2.position()),
            last_pos_1_(last1.position()), last_pos_2_(last2.position()),
            is_mismatched_(false)
        {
        }

        bool is_mismatched() const {
            
        }

        template <std::size_t N>
        bool will_word_read_reach_last(std::size_t bits_to_read) const {
            auto [word_cursor, last_word_cursor, cursor_pos, last_pos] = N == 1 ?
                std::tie(word_cursor_1_, last_word_cursor_1_, cursor_pos_1_, last_pos_1_) :
                std::tie(word_cursor_2_, last_word_cursor_2_, cursor_pos_2_, last_pos_2_);

            std::size_t new_position = cursor_pos + bits_to_read - num_digits;

            if (new_position < cursor_pos) {
                return std::next(word_cursor) == last_word_cursor && new_position == last_pos;
            } else {
                return word_cursor == last_word_cursor && new_position == last_pos;
            }
        }

        template <std::size_t N>
        bool will_full_word_read_reach_last() {
            return will_word_read_reach_last<N>(num_digits);
        }

        template <std::size_t N>
        struct alignment {};

        using aligned_first = alignment<1>;
        using aligned_second = alignment<2>;
        using aligned_first_ahead = alignment<3>;
        using aligned_second_ahead = alignment<4>;
        using aligned_symmetric_unaligned = alignment<5>;

        bool aligned_first() const {
            return cursor_pos_1_ == 0;
        }

        bool aligned_second() const {
            return cursor_pos_2_ == 0;
        }

        bool aligned_first_ahead() const {
            return cursor_pos_1_ > cursor_pos_2_;
        }

        bool aligned_second_ahead() const {
            return cursor_pos_1_ < cursor_pos_2_;
        }

        bool aligned_symmetric_unaligned() const {
            return cursor_pos_1_ == cursor_pos_2_;
        }

        template <class Lambda>
        auto dispatch_by_alignment(Lambda lambda) {
            if (aligned_first()) {
                return lambda(aligned_first{});
            } else if (aligned_second()) {
                return lambda(aligned_second{});
            } else if (aligned_symmetric_unaligned()) {
                return lambda(aligned_symmetric_unaligned{});
            } else if (aligned_second_ahead()) {
                return lambda(aligned_second_ahead{});
            } else {
                return lambda(aligned_first_ahead{});
            }
        }

        std::pair<std::size_t, std::size_t> get_read_len(aligned_first) {
            
            std::size_t first_len, second_len;
            
            if (word_cursor_1_ == last_word_cursor_1_) {
                first_len = last_pos_1_ - cursor_pos_1_;
            } else {
                first_len = num_digits;
            }

            if (Careful) {
                std::size_t new_pos_2 = (cursor_pos_2_ + first_len) % num_digits;

                if (word_cursor_2_ == last_word_cursor_2_) {
                    if ()


                }



                if (first_len < num_digits) {
                    
                    
                    
                    
                    // the new position of the second cursor should be the end position
                    if (new_pos_2 < cursor_pos_2_) {

                    }

                }







                if (new_pos_2 < cursor_pos_2_) {
                    if (word_cursor_2_ == last_word_cursor_2_) {
                        second_len = last_pos_2_ - cursor_pos_2_;
                    } else if (std::next(word_cursor_2_) == last_word_cursor_2_ 
                               && new_pos_2 > last_pos_2_) {
                        second_len = num_digits - (new_pos_2 - last_pos_2_);
                    } else {
                        second_len = num_digits;
                    }
                } else {
                    
                }
            } else {
                second_len = first_len;
            }


            if (new_pos_2 < cursor_pos_2_) {
                if (word_cursor_2_ == last_word_cursor_2_) {
                    second_len = last_pos_2_ - cursor_pos_2_;
                } else if (std::next(word_cursor_2_) == last_word_cursor_2_
                    && )


                if (std::next(word_cursor_2_) == last_word_cursor_2_) {

                }


            } else {

            }
        }

        std::pair<std::size_t, std::size_t> get_read_len(aligned_second) {

        }

        std::pair<std::size_t, std::size_t> get_read_len(aligned_symmetric_unaligned) {

        }

        std::pair<std::size_t, std::size_t> get_read_len(aligned_first_ahead) {

        }

        std::pair<std::size_t, std::size_t> get_read_len(aligned_second_ahead) {

        }

        std::pair<std::size_t, std::size_t> get_read_len() const {
            return dispatch_by_alignment(
                [] (auto alignment) { return get_read_len(alignment); }
            );
        }


        bool is_next_read_last() const {
        
            if (cursor_pos_1_ )



            auto [word_cursor, last_word_cursor, ]
        
        
        
        
        
        
            if (cursor_pos_1_ == 0) {
                return _in_same_word(word_cursor_1_, last_word_cursor_1_)
                    || _in_same_word(std::next(word_cursor_1_), last_word_cursor_1_) && ;



                return will_full_word_read_reach_last<1>();
            } else if (cursor_pos_2_ == 0) {
                return will_full_word_read_reach_last<2>();
            } else {
                if (cursor_pos_1_ > cursor_pos_2_) {
                    return will_full_word_read_reach_last<2>();
                } else {
                    return will_full_word_read_reach_last<1>();
                }
            }
        }

        read_pair_t read_first() {
            read_pair_t reads = std::make_pair(*word_cursor_1_, *word_cursor_2_);

            num_relevant_bits_ = std::max(
                num_digits - cursor_pos_1,
                num_digits - cursor_pos_2,
            );

            word_cursor_1_++;
            word_cursor_2_++;

            std::tie(cursor_pos_1_, cursor_pos_2_) = {0, 0};
        
            return reads;
        }

        read_pair_t read_first_as_last() {
            read_pair_t reads = std::make_pair(*word_cursor_1_, *word_cursor_2_);

            if (cursor_pos_1_ > cursor_pos_2_) {
                num_relevant_bits_ = last_pos_2_ - cursor_pos_2_; 
            } else {
                num_relevant_bits_ = last_pos_1_ - cursor_pos_1_;
            }

            return reads;
        }



        read_pair_t read_first_padded();

        read_pair_t read();
        read_pair_t read_padded();

        std::size_t get_num_relevant_bits() const;

        void set_padding_value(const bit_value bv);

        template <std::size_t N>
            using iter_selector_t = typename std::conditional<N == 0, Iter1, Iter2>::type;
        
        template <std::size_t N>
        bit_iterator<iter_selector_t<N>> get_bit_iterator(index<N>) const;

        template <std::size_t N>
        iter_selector_t<N> get_base_iterator(index_t<N>) const;

        template <std::size_t N>
        std::size_t get_position(index_t<N>) const;
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
