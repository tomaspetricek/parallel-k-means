//
// Created by Tomáš Petříček on 01.10.2022.
//

#ifndef MCC_ARRAY_HPP
#define MCC_ARRAY_HPP

#include <stddef.h>
#include <iostream>

template<size_t n_rows, size_t n_cols>
bool are_equal(const double (& a)[n_rows][n_cols], const double (& b)[n_rows][n_cols])
{
    for (size_t r{0}; r<n_rows; r++)
        for (size_t c{0}; c<n_cols; c++)
            if (a[r][c]!=b[r][c])
                return false;
    return true;
}

template<class Type, size_t n>
void print(const Type (& arr)[n])
{
    for (size_t i{0}; i<n; i++)
        std::cout << arr[i] << ", ";
    std::cout << std::endl;
}

template<class Type, std::size_t n_rows, std::size_t n_cols>
void print(const Type (& arr)[n_rows][n_cols])
{
    for (std::size_t r{0}; r<n_rows; r++)
        print(arr[r]);
    std::cout << std::endl;
}

#endif //MCC_ARRAY_HPP
