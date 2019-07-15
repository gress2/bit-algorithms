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



// ------------------------ Range Reader Tests ------------------------- //

TEMPLATE_TEST_CASE("dual range reader: read() is ok for single(full) word case", 
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

    REQUIRE(reader1.has_more());

    auto pair = reader1.read();
    REQUIRE(pair.first == 3929);
    REQUIRE(pair.second == 2183);

    std::pair<std::size_t, std::size_t> read_lens = reader1.get_read_lengths();

    REQUIRE(read_lens.first == read_lens.second);
    REQUIRE(!reader1.has_more());
    REQUIRE(!reader1.is_mismatched());
}

/*

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

    container_type c3 = {static_cast<value_type>(_all_ones()), 0, 1024, 0};
    constexpr std::size_t num_digits = binary_digits<value_type>::value;

    bit_iter_type c3a_beg(c3.begin(), num_digits / 2);
    bit_iter_type c3a_end(std::next(c3.begin(), 2), 0);
    bit_iter_type c3b_beg(c3.begin(), num_digits / 2 + 1);
    bit_iter_type c3b_end(std::next(c3.begin(), 2), 1);

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

    constexpr value_type all_ones = static_cast<value_type>(_all_ones());

    container_type c1 = { 
        _shift_towards_msb(all_ones, num_digits - 5),
        _shift_towards_msb(all_ones, 7),
        _shift_towards_msb(all_ones, num_digits / 2)
    };

    dual_range_reader reader(
        bit_iter_type(c1.begin(), num_digits - 3),
        bit_iter_type(std::next(c1.begin(), 2), 10),
        bit_iter_type(c1.begin(), num_digits - 2),
        bit_iter_type(std::next(c1.begin(), 2), 11)
    ); 

    reader.read_first();

    auto read = reader.read();

    value_type r1_expected = _shift_towards_msb(all_ones, 7);
    value_type r2_expected = _shift_towards_lsb(r1_expected, 1);

    REQUIRE(read.first == r1_expected);
    REQUIRE(read.second == r2_expected);

    REQUIRE(reader.is_next_read_last());

    c1 = { 
      _shift_towards_lsb(all_ones, num_digits / 2),
      _shift_towards_lsb(all_ones, num_digits / 4),
      _shift_towards_lsb(all_ones, num_digits / 8), 
      all_ones
    };
      
    reader = dual_range_reader(
        bit_iter_type(c1.begin(), num_digits - 3),
        bit_iter_type(std::next(c1.begin(), 3), 10),
        bit_iter_type(c1.begin(), num_digits - 2),
        bit_iter_type(std::next(c1.begin(), 3), 11)
    );


    reader.read_first(); 

    read = reader.read();

    r1_expected = _shift_towards_lsb(all_ones, num_digits / 4);
    r2_expected = _set_nth_bit(
        _shift_towards_lsb(all_ones, num_digits / 4 + 1), 
        num_digits - 1
    );

    REQUIRE(read.first == r1_expected);
    REQUIRE(read.second == r2_expected); 

    read = reader.read();

    r1_expected = _shift_towards_lsb(all_ones, num_digits / 8);
    r2_expected = _set_nth_bit(
        _shift_towards_lsb(all_ones, num_digits / 8 + 1),
        num_digits - 1
    );

    REQUIRE(read.first == r1_expected);
    REQUIRE(read.second == r2_expected);
}
    
TEMPLATE_TEST_CASE("dual range reader: read() is fine for c-style arrays", 
    "[dual_range_reader]", unsigned short, unsigned int, unsigned long, 
    unsigned long long) {

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
}

TEMPLATE_TEST_CASE("dual range reader: read_last() is fine for c-style arays", 
    "[dual_range_reader]", unsigned short, unsigned int, unsigned long, 
    unsigned long long) {
    using namespace bit;

    constexpr TestType all_ones = static_cast<TestType>(_all_ones());
    constexpr std::size_t num_digits = binary_digits<TestType>::value;

    TestType ary[3] = {
        0, 1024 - 1, _shift_towards_msb(all_ones, 2)};

    bit_iterator<TestType*> beg1(ary, 5);
    bit_iterator<TestType*> end1(ary + 2, 4);
    bit_iterator<TestType*> beg2(ary, 7);
    bit_iterator<TestType*> end2(ary + 2, 6);

    dual_range_reader reader(beg1, end1, beg2, end2);

    reader.read_first();
    reader.read();
    REQUIRE(reader.is_next_read_last());

    std::pair<TestType, TestType> read = reader.read_last();

    std::size_t num_relevant_bits = reader.get_num_relevant_bits();
    std::size_t bits_to_mask_off = num_digits - num_relevant_bits;

    TestType mask = _shift_towards_lsb(all_ones, bits_to_mask_off); 

    REQUIRE((read.first & mask) == 12);
    REQUIRE((read.second & mask) == 15);

    TestType ary2[2] = { all_ones, 1024 };
    TestType ary3[2] = { 1024, all_ones };

    beg1 = bit_iterator<TestType*>(ary2);
    end1 = bit_iterator<TestType*>(ary2 + 1, 5); 
    beg2 = bit_iterator<TestType*>(ary3, num_digits - 5);
    end2 = bit_iterator<TestType*>(ary3 + 1);

    dual_range_reader reader2(beg1, end1, beg2, end2);

    read = reader2.read_first();

    REQUIRE(reader2.is_next_read_last());

    read = reader2.read_last();

    mask = _shift_towards_lsb(all_ones, num_digits - reader2.get_num_relevant_bits());

    REQUIRE((mask & read.first) == 0); 
    REQUIRE((mask & read.second) == _shift_towards_lsb(all_ones, num_digits - 5));
}

*/
// -------------------------------------------------------------------------- //



// ========================================================================== //
#endif // _RANGE_READER_TESTS_HPP_INCLUDED
// ========================================================================== //