//
// Created by Tomáš Petříček on 01.10.2022.
//

#ifndef MCC_ARRAY_HPP
#define MCC_ARRAY_HPP

#include <iostream>
#include <reader.hpp>
#include <writer.hpp>

namespace mcc {
    template<class Type, size_t n>
    inline bool are_equal(const Type (& a)[n], const Type (& b)[n])
    {
        for (std::size_t i{0}; i<n; i++)
            if (a[i]!=b[i])
                return false;
        return true;
    }

    template<class Type, size_t n_rows, size_t n_cols>
    inline bool are_equal(const Type (& a)[n_rows][n_cols], const Type (& b)[n_rows][n_cols])
    {
        for (size_t r{0}; r<n_rows; r++)
            for (size_t c{0}; c<n_cols; c++)
                if (a[r][c]!=b[r][c])
                    return false;
        return true;
    }

//    template<class Type, std::size_t n_rows, std::size_t n_cols>
//    inline void copy(const Type (& from)[n_rows][n_cols], Type (& to)[n_rows][n_cols])
//    {
//        std::copy(&from[0][0], &from[0][0]+n_rows*n_cols, &to[0][0]);
//    }

    inline std::size_t index(std::size_t r, std::size_t c, std::size_t n_cols)
    {
        return r*n_cols+c;
    }

    inline std::size_t index(std::size_t l, std::size_t r, std::size_t c, std::size_t n_rows, std::size_t n_cols)
    {
        return l*n_rows*n_cols+r*n_cols+c;
    }

    template<class Type>
    inline void copy(const Type* from, Type* to, std::size_t n_cols)
    {
        for (std::size_t c{0}; c<n_cols; c++)
            to[c] = from[c];
    }

    template<class Type>
    inline void print(const Type* arr, std::size_t n)
    {
        for (size_t i{0}; i<n; i++)
            std::cout << arr[i] << ", ";
        std::cout << std::endl;
    }

    template<class Type>
    inline void print(const Type* arr, std::size_t n_rows, std::size_t n_cols)
    {
        for (std::size_t r{0}; r<n_rows; r++) {
            for (std::size_t c{0}; c<n_cols; c++)
                std::cout << arr[index(r, c, n_cols)] << ", ";
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    template<class Type>
    inline double euclidean_distance(const Type* x, const Type* y, std::size_t n)
    {
        double sum{0}, diff;

        for (std::size_t i{0}; i<n; i++) {
            diff = x[i]-y[i];
            sum += diff*diff;
        }

        return std::sqrt(sum);
    }

    template<class Type>
    inline void
    read_csv(const std::filesystem::path& path, char delim, Type* mat, std::size_t n_rows, std::size_t n_cols)
    {
        mcc::csv::reader reader{path, delim};
        auto* data = new std::string[n_cols];

        for (std::size_t r{0}; r<n_rows; r++) {
            reader(data, n_cols);

            for (std::size_t c{0}; c<n_cols; c++)
                mat[index(r, c, n_cols)] = std::stod(data[c]);
        }

        delete[] data;
    }

    template<class Type>
    inline void read_csv(const std::filesystem::path& path, char delim, Type* arr, std::size_t n)
    {
        mcc::csv::reader reader{path, delim};
        auto data = std::string();

        for (std::size_t i{0}; i<n; i++) {
            reader(&data, 1);
            arr[i] = std::stod(data);
        }
    }

    template<class Type, std::size_t n_rows, std::size_t n_cols>
    inline void write_csv(const std::filesystem::path& path, char delim, const Type (& mat)[n_rows][n_cols])
    {
        mcc::csv::writer writer{path, delim};
        std::string data[n_cols];

        for (std::size_t r{0}; r<n_rows; r++) {
            for (std::size_t c{0}; c<n_cols; c++)
                data[c] = std::to_string(mat[r][c]);

            writer(data);
        }
    }
}

#endif //MCC_ARRAY_HPP
