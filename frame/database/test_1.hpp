//
// Created by gaosm on 1/8/24.
//

#ifndef PLUTO_TEST_1_HPP
#define PLUTO_TEST_1_HPP

#include <iostream>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/static_assert.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/placeholders.hpp>

template <typename T>
constexpr T Factorial(T x) {
    return (x <= 1) ? 1 : (x + Factorial(x - 1));
}

template<int N>
struct Sum {
    static constexpr int value = N + Sum<N - 1>::value;
};

// 特化模板结构体，用于终止递归
template<>
struct Sum<1> {
    static constexpr int value = 1;
};

#endif //PLUTO_TEST_1_HPP
