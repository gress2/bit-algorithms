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
#include <iostream>
// Project sources
#include "test_root.cc"
#include "bit.hpp"
// Third-party libraries
// Miscellaneous
// ========================================================================== //


// ------------------------ Range Reader Tests ------------------------- //

TEMPLATE_TEST_CASE("dual range reader: read_first is fine", 
    "[dual_range_reader]", unsigned short, unsigned int, unsigned long, 
    unsigned long long) {

    using namespace bit;

    TestType x = 3929; // 111101011001
    TestType y = 2183; // 100010000111 

    bit_iterator<TestType*> beg_x(&x);
    bit_iterator<TestType*> end_x(&x + 1);
    bit_iterator<TestType*> beg_y(&y);
    bit_iterator<TestType*> end_y(&y + 1);

    dual_range_reader reader1(beg_x, end_x, beg_y, end_y);

    REQUIRE(reader1.has_more());

    auto pair = reader1.read_first();
    REQUIRE(pair.first == 3929);
    REQUIRE(pair.second == 2183);

    REQUIRE(!reader1.has_more());

    std::vector<TestType> vec1 = {3, 8, 22};
    std::vector<TestType> vec2 = {8, 11111, 88};

    using vec_iter_t = typename std::vector<TestType>::iterator;

    bit_iterator<vec_iter_t> vec1_beg(vec1.begin());
    bit_iterator<vec_iter_t> vec1_end(vec1.end());
    bit_iterator<vec_iter_t> vec2_beg(vec2.begin());
    bit_iterator<vec_iter_t> vec2_end(vec2.end());

    dual_range_reader reader2(vec1_beg, vec1_end, vec2_beg, vec2_end);

    REQUIRE(reader2.has_more());
    auto pair2 = reader2.read_first(); 

    REQUIRE(pair2.first == 3);
    REQUIRE(pair2.second == 8);

    REQUIRE(reader2.has_more());

    TestType ary1[3] = {1, 2, 3};
    TestType ary2[3] = {9, 10, 11}; 
    bit_iterator<TestType*> ary1_beg(ary1);
    bit_iterator<TestType*> ary1_end(ary1 + 3);
    bit_iterator<TestType*> ary2_beg(ary2);
    bit_iterator<TestType*> ary2_end(ary2 + 3);

    dual_range_reader reader3(ary1_beg, ary1_end, ary2_beg, ary2_end);
    REQUIRE(reader3.has_more());
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
    REQUIRE(reader4.has_more());
    auto pair4 = reader4.read_first();
    REQUIRE(pair4.first == 
        _shift_towards_lsb(static_cast<TestType>(_all_ones()), num_digits / 2));
    REQUIRE(pair4.second == 
        _shift_towards_lsb(static_cast<TestType>(_all_ones()), num_digits / 2 + 1));

    REQUIRE(reader4.template get_position<0>() == 0);
    REQUIRE(reader4.template get_position<1>() == 1); 

    reader4 = dual_range_reader(vec3b_beg, vec3b_end, vec3a_beg, vec3a_end);
    REQUIRE(reader4.has_more());
    auto pair5 = reader4.read_first();
    REQUIRE(pair5.first == 
        _shift_towards_lsb(static_cast<TestType>(_all_ones()), num_digits / 2 + 1));
    REQUIRE(pair5.second == 
        _shift_towards_lsb(static_cast<TestType>(_all_ones()), num_digits / 2));

    REQUIRE(reader4.template get_position<0>() == 1);
    REQUIRE(reader4.template get_position<1>() == 0); 

    TestType t = -1; 
    bit_iterator<TestType*> t1_beg(&t, 3);
    bit_iterator<TestType*> t1_end(&t, 8);
    bit_iterator<TestType*> t2_beg(&t, 5);
    bit_iterator<TestType*> t2_end(&t, 10);

    dual_range_reader reader5(t1_beg, t1_end, t2_beg, t2_end);
    std::size_t relevant_bits;
    auto pair6 = reader5.read_first(&relevant_bits);

    REQUIRE(relevant_bits == 5);

    std::size_t bits_to_mask = num_digits - relevant_bits; 
    TestType mask = 
        _shift_towards_lsb(static_cast<TestType>(_all_ones()), bits_to_mask); 
    
    REQUIRE((pair6.first & mask) == 
        _shift_towards_lsb(static_cast<TestType>(_all_ones()), num_digits - 5));
    REQUIRE((pair6.second & mask) == 
        _shift_towards_lsb(static_cast<TestType>(_all_ones()), num_digits - 5));

    REQUIRE(reader5.template get_position<0>() == 8);
    REQUIRE(reader5.template get_position<1>() == 10);

    REQUIRE(*(reader5.template get_base_iterator<0>()) 
        == static_cast<TestType>(_all_ones())); 
    REQUIRE(*(reader5.template get_base_iterator<1>()) 
        == static_cast<TestType>(_all_ones())); 

    REQUIRE(reader5.template get_bit_iterator<0>() == t1_end);
    REQUIRE(reader5.template get_bit_iterator<1>() == t2_end);

    t = 0;
    t1_beg = bit_iterator<TestType*>(&t, 4);
    t1_end = bit_iterator<TestType*>(&t, 5);
    t2_beg = bit_iterator<TestType*>(&t, 6); 
    t2_end = bit_iterator<TestType*>(&t, 7);

    reader5 = dual_range_reader(t1_beg, t1_end, t2_beg, t2_end);
    REQUIRE(reader5.has_more());

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

    std::size_t relevant_bits;
    std::pair<TestType, TestType> p = reader.read_first(&relevant_bits);
    
    REQUIRE(relevant_bits == num_digits - 5);

    p = reader.read(&relevant_bits);

    REQUIRE(relevant_bits == num_digits);
    REQUIRE(p.first == 1023);
    REQUIRE(p.second == _shift_towards_lsb(static_cast<TestType>(1023), 2));

    p = reader.read(&relevant_bits);
    REQUIRE(relevant_bits == 2);

    TestType mask = 3;

    REQUIRE((mask & p.first) == 0);
    REQUIRE((mask & p.second) == 3);

    REQUIRE(reader.template get_position<0>() == 2);
    REQUIRE(reader.template get_position<1>() == 4);


    TestType ary2[2] = {0, 0};

    bit_iterator<TestType*> invalid(ary2 + 30000, 5);
    std::cout << *invalid << std::endl;
}

// -------------------------------------------------------------------------- //



// ========================================================================== //
#endif // _RANGE_READER_TESTS_HPP_INCLUDED
// ========================================================================== //
