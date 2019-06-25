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

TEMPLATE_TEST_CASE("dual range reader: read_first() is fine", 
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

    REQUIRE(!reader1.is_next_read_last());

    auto pair = reader1.read_first();
    REQUIRE(pair.first == 3929);
    REQUIRE(pair.second == 2183);

    std::vector<TestType> vec1 = {3, 8, 22};
    std::vector<TestType> vec2 = {8, 11111, 88};

    using vec_iter_t = typename std::vector<TestType>::iterator;

    bit_iterator<vec_iter_t> vec1_beg(vec1.begin());
    bit_iterator<vec_iter_t> vec1_end(vec1.end());
    bit_iterator<vec_iter_t> vec2_beg(vec2.begin());
    bit_iterator<vec_iter_t> vec2_end(vec2.end());

    dual_range_reader reader2(vec1_beg, vec1_end, vec2_beg, vec2_end);

    auto pair2 = reader2.read_first(); 

    REQUIRE(pair2.first == 3);
    REQUIRE(pair2.second == 8);

    REQUIRE(!reader2.is_next_read_last());

    TestType ary1[3] = {1, 2, 3};
    TestType ary2[3] = {9, 10, 11}; 
    bit_iterator<TestType*> ary1_beg(ary1);
    bit_iterator<TestType*> ary1_end(ary1 + 3);
    bit_iterator<TestType*> ary2_beg(ary2);
    bit_iterator<TestType*> ary2_end(ary2 + 3);

    dual_range_reader reader3(ary1_beg, ary1_end, ary2_beg, ary2_end);
    auto pair3 = reader3.read_first();
    REQUIRE(pair3.first == 1);
    REQUIRE(pair3.second == 9);

    std::vector<TestType> vec3 = {static_cast<TestType>(_all_ones()), 0, 1024};

    constexpr std::size_t num_digits = binary_digits<TestType>::value;
    bit_iterator<vec_iter_t> vec3a_beg(vec3.begin(), num_digits / 2);
    bit_iterator<vec_iter_t> vec3a_end(vec3.begin() + 1, 0);
    bit_iterator<vec_iter_t> vec3b_beg(vec3.begin(), num_digits / 2 + 1); 
    bit_iterator<vec_iter_t> vec3b_end(vec3.begin() + 1, 0);

    dual_range_reader reader4(vec3a_beg, vec3a_end, vec3b_beg, vec3b_end);
    auto pair4 = reader4.read_first();
    REQUIRE(pair4.first == 
        _shift_towards_lsb(static_cast<TestType>(_all_ones()), num_digits / 2));
    REQUIRE(pair4.second == 
        _shift_towards_lsb(static_cast<TestType>(_all_ones()), num_digits / 2 + 1));

    REQUIRE(reader4.template get_position<0>() == 0);
    REQUIRE(reader4.template get_position<1>() == 1); 

    reader4 = dual_range_reader(vec3b_beg, vec3b_end, vec3a_beg, vec3a_end);
    auto pair5 = reader4.read_first();
    REQUIRE(pair5.first == 
        _shift_towards_lsb(static_cast<TestType>(_all_ones()), num_digits / 2 + 1));
    REQUIRE(pair5.second == 
        _shift_towards_lsb(static_cast<TestType>(_all_ones()), num_digits / 2));

    REQUIRE(reader4.template get_position<0>() == 1);
    REQUIRE(reader4.template get_position<1>() == 0); 

    TestType t = 0;
    bit_iterator<TestType*> t1_beg(&t, 4);
    bit_iterator<TestType*> t1_end(&t, 5);
    bit_iterator<TestType*> t2_beg(&t, 6); 
    bit_iterator<TestType*> t2_end(&t, 7);

    dual_range_reader reader5(t1_beg, t1_end, t2_beg, t2_end);

    auto pair7 = reader5.read_first();
    REQUIRE(pair7.first == 0);
    REQUIRE(pair7.second == 0);
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
