//
// Created by Tomáš Petříček on 02.10.2022.
//

#ifndef MCC_TEST_ARRAY_HPP
#define MCC_TEST_ARRAY_HPP

#include <boost/test/unit_test.hpp>
#include <array.hpp>

BOOST_AUTO_TEST_SUITE(array_tests)
    BOOST_AUTO_TEST_CASE(euclidean_distance_test)
    {
        constexpr std::size_t n{3};
        double x[n]{4, 4, 2};
        double y[n]{1, 2, 1};
        BOOST_REQUIRE_CLOSE(mcc::euclidean_distance(x, y), 3.7416573867739413, 0.000001);
    }
BOOST_AUTO_TEST_SUITE_END()

#endif //MCC_TEST_ARRAY_HPP
