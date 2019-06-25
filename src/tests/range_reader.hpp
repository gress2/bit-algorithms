// ======================= RANGE READER TESTS ========================== //
// Project:         The Experimental Bit Algorithms Library
// Name:            range_reader.hpp
// Description:     Tests for utility classes for reading bit iterator ranges
// Creator:         Vincent Reverdy
// Contributor(s):  Collin Gress [2019]
// License:         BSD 3-Clause License
// ========================================================================== //
#ifndef _RANGE_READER_TESTS_HPP_INCLUDED
#define _RANGE_READER_TESTS_HPP_INCLUDED
// ========================================================================== //



// =============================== PREAMBLE ================================= //
// C++ standard library
#include <bitset>
#include <iostream>
// Project sources
#include "test_root.cc"
#include "bit.hpp"
// Third-party libraries
// Miscellaneous
// ========================================================================== //

template <class T>
std::string to_bit_string(T t) {
    return std::bitset<bit::binary_digits<T>::value>(t).to_string();
}

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

    first_word_bit_strings.push_back(
        to_bit_string(*(drr.get_base_iterator(index<0>))));
    second_word_bit_strings.push_back(
        to_bit_string(*(drr.get_base_iterator(index<1>))));
    first_positions.push_back(drr.get_position(index<0>));
    second_positions.push_back(drr.get_position(index<1>));

    // move positions and words
    pair_type read = drr.read_first();

    first_word_bit_strings.push_back(
        to_bit_string(*(drr.get_base_iterator(index<0>))));
    second_word_bit_strings.push_back(
        to_bit_string(*(drr.get_base_iterator(index<1>))));
    first_positions.push_back(drr.get_position(index<0>));
    second_positions.push_back(drr.get_position(index<1>));

    while (!drr.is_next_read_last()) {
        read = drr.read();
        // move positions and words
        first_word_bit_strings.push_back(
            to_bit_string(*(drr.get_base_iterator(index<0>))));
        second_word_bit_strings.push_back(
            to_bit_string(*(drr.get_base_iterator(index<1>))));
        first_positions.push_back(drr.get_position(index<0>));
        second_positions.push_back(drr.get_position(index<1>));
    }

    read = drr.read_last();
    first_word_bit_strings.push_back(
        to_bit_string(*(drr.get_base_iterator(index<0>))));
    second_word_bit_strings.push_back(
        to_bit_string(*(drr.get_base_iterator(index<1>))));
    first_positions.push_back(drr.get_position(index<0>));
    second_positions.push_back(drr.get_position(index<1>));

    first_positions.push_back(drr.get_num_relevant_bits() + first_positions.back());
    second_positions.push_back(drr.get_num_relevant_bits() + second_positions.back());

    for (std::size_t i = 0; i < first_positions.size(); i++) {
        std::string to_be_inserted = "|";
        std::size_t string_idx = i;
        std::size_t position_to_insert = first_positions[i];

        if (i == 0) {
            to_be_inserted = "]";
        } else if (i == first_positions.size() - 1) {
            string_idx--;
            position_to_insert++;
            to_be_inserted = "[";
        }

        first_word_bit_strings[string_idx].insert(position_to_insert, to_be_inserted); 

        if (i != first_positions.size() - 2) {
            std::reverse(first_word_bit_strings[string_idx].begin(), 
                first_word_bit_strings[string_idx].end());
        }

    }

    for (std::size_t i = 0; i < second_positions.size(); i++) {
        std::string to_be_inserted = "|";
        std::size_t string_idx = i;
        std::size_t position_to_insert = second_positions[i];

        if (i == 0) {
            to_be_inserted = "]";
        } else if (i == second_positions.size() - 1) {
            string_idx--;
            position_to_insert++;
            to_be_inserted = "[";
        }

        second_word_bit_strings[string_idx].insert(position_to_insert, to_be_inserted); 

        if (i != second_positions.size() - 2) {
            std::reverse(second_word_bit_strings[string_idx].begin(), 
                second_word_bit_strings[string_idx].end());
        }
    }

    std::reverse(first_word_bit_strings.begin(), first_word_bit_strings.end());
    std::reverse(second_word_bit_strings.begin(),second_word_bit_strings.end());

    std::cout << "end";
    for (auto& e : first_word_bit_strings) {
        std::cout << " " << e;  
    }
    std::cout << " start\n";

    std::cout << "end";
    for (auto& e : second_word_bit_strings) {
        std::cout << " " << e;  
    }
    std::cout << " start\n\n";
}


// ------------------------ Range Reader Tests ------------------------- //

TEMPLATE_TEST_CASE("dual range reader: read_first() is ok for single(full) word case", 
    "[dual_range_reader]", unsigned short, unsigned int, unsigned long, 
    unsigned long long) {

    using namespace bit;

    TestType x = 3929; // [MSB] 111101011001 [LSB]
    TestType y = 2183; // [MSB] 100010000111 [LSB] 

    bit_iterator<TestType*> beg_x(&x);
    bit_iterator<TestType*> end_x(&x + 1);
    bit_iterator<TestType*> beg_y(&y);
    bit_iterator<TestType*> end_y(&y + 1);

    dual_range_reader reader1(beg_x, end_x, beg_y, end_y);

    auto pair = reader1.read_first();
    REQUIRE(pair.first == 3929);
    REQUIRE(pair.second == 2183);
}

TEMPLATE_PRODUCT_TEST_CASE("dual range reader: read_first() is ok for std containers", 
    "[template][product]", (std::vector, std::list, std::forward_list),
    (unsigned short, unsigned int, unsigned long)) {

    using namespace bit;
    
    using container_type = TestType;
    using value_type = typename container_type::value_type;
    using cont_iter_type = typename container_type::iterator;
    using bit_iter_type = bit_iterator<cont_iter_type>;

    container_type c1 = {3, 8, 22};
    container_type c2 = {8, 11111, 88};

    bit_iter_type c1_beg(c1.begin());
    bit_iter_type c1_end(c1.end()); 
    bit_iter_type c2_beg(c2.begin());
    bit_iter_type c2_end(c2.end());

    dual_range_reader reader(c1_beg, c1_end, c2_beg, c2_end);

    std::pair<value_type, value_type> read = reader.read_first();

    REQUIRE(read.first == 3);
    REQUIRE(read.second == 8);

    REQUIRE(!reader.is_next_read_last());

    container_type c3 = {static_cast<value_type>(_all_ones()), 0, 1024};
    constexpr std::size_t num_digits = binary_digits<value_type>::value;

    bit_iter_type c3a_beg(c3.begin(), num_digits / 2);
    bit_iter_type c3a_end(std::next(c3.begin()), 0);
    bit_iter_type c3b_beg(c3.begin(), num_digits / 2 + 1);
    bit_iter_type c3b_end(std::next(c3.begin()), 1);

    dual_range_reader reader2(c3a_beg, c3a_end, c3b_beg, c3b_end);

    auto read2 = reader2.read_first();
    REQUIRE(read2.first == 
        _shift_towards_lsb(static_cast<value_type>(_all_ones()), num_digits / 2));
    REQUIRE(read2.second ==
        _shift_towards_lsb(static_cast<value_type>(_all_ones()), num_digits / 2 + 1));

    REQUIRE(reader2.get_position(index<0>) == 0);
    REQUIRE(reader2.get_position(index<1>) == 1); 
}

TEMPLATE_TEST_CASE("dual range reader: read_first() is ok for c-style arrays",
    "[dual_range_reader]", unsigned short, unsigned int, unsigned long) {

    TestType ary1[3] = {1, 2, 3};
    TestType ary2[3] = {9, 10, 11};

    using bit_iter_t = typename bit::bit_iterator<TestType*>;
    
    bit_iter_t ary1_beg(ary1);
    bit_iter_t ary1_end(ary1 + 3);
    bit_iter_t ary2_beg(ary2);
    bit_iter_t ary2_end(ary2 + 3);

    bit::dual_range_reader reader(ary1_beg, ary1_end, ary2_beg, ary2_end);
    auto read = reader.read_first();
    REQUIRE(read.first == 1);
    REQUIRE(read.second == 9);
}

TEMPLATE_PRODUCT_TEST_CASE("dual range reader: read() is ok for std containers", 
    "[template][product]", (std::vector, std::list, std::forward_list),
    (unsigned short, unsigned int, unsigned long)) {

    using namespace bit;
    
    using container_type = TestType;
    using value_type = typename container_type::value_type;
    using cont_iter_type = typename container_type::iterator;
    using bit_iter_type = bit_iterator<cont_iter_type>;
    constexpr std::size_t num_digits = binary_digits<value_type>::value;

    container_type c1 = { 
        _shift_towards_msb(static_cast<value_type>(_all_ones()), num_digits - 5),
        _shift_towards_msb(static_cast<value_type>(_all_ones()), 7),
        _shift_towards_msb(static_cast<value_type>(_all_ones()), num_digits / 2)
    };

    dual_range_reader reader(
        bit_iter_type(c1.begin(), num_digits - 3),
        bit_iter_type(std::next(c1.begin(), 2), 10),
        bit_iter_type(c1.begin(), num_digits - 2),
        bit_iter_type(std::next(c1.begin(), 2), 11)
    ); 

    //        [2]              [1]               [0] 
    // 11111111100000000 1111111110000000 1111100000000000
    //       [         |                |   ]                                                   
    //      [         |                |   ]

    display(reader);

    reader.read_first();

    auto read = reader.read();
}

    
TEMPLATE_TEST_CASE("dual range reader: read() is fine", "[dual_range_reader]",
    unsigned short, unsigned int, unsigned long, unsigned long long) {

    using namespace bit;

    constexpr std::size_t num_digits = binary_digits<TestType>::value;

    TestType ary[3] = {0, 1024 - 1, _shift_towards_msb(static_cast<TestType>(-1), 2)};

    bit_iterator<TestType*> beg1(ary, 5);
    bit_iterator<TestType*> end1(ary + 2, 2);
    bit_iterator<TestType*> beg2(ary, 7);
    bit_iterator<TestType*> end2(ary + 2, 4);

    dual_range_reader reader(beg1, end1, beg2, end2);

    std::pair<TestType, TestType> p = reader.read_first();
    std::size_t relevant_bits = reader.get_num_relevant_bits();

    REQUIRE(relevant_bits == num_digits - 5);

    p = reader.read();

    relevant_bits = reader.get_num_relevant_bits();

    REQUIRE(relevant_bits == num_digits);
    REQUIRE(p.first == 1023);
    REQUIRE(p.second == _shift_towards_lsb(static_cast<TestType>(1023), 2));

    REQUIRE(reader.is_next_read_last());
    p = reader.read_last();
    relevant_bits = reader.get_num_relevant_bits();
    REQUIRE(relevant_bits == 2);

    TestType mask = 3;

    REQUIRE((mask & p.first) == 0);
    REQUIRE((mask & p.second) == 3);
}

// -------------------------------------------------------------------------- //



// ========================================================================== //
#endif // _RANGE_READER_TESTS_HPP_INCLUDED
// ========================================================================== //
