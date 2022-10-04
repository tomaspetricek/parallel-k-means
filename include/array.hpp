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

    template<class Type, std::size_t n_rows, std::size_t n_cols>
    inline void copy(const Type (& from)[n_rows][n_cols], Type (& to)[n_rows][n_cols])
    {
        std::copy(&from[0][0], &from[0][0]+n_rows*n_cols, &to[0][0]);
    }

    template<class Type, std::size_t n>
    inline void copy(const Type (& from)[n], Type (& to)[n])
    {
        std::copy(&from[0], &from[0]+n, &to[0]);
    }

    template<class Type, size_t n>
    inline void print(const Type (& arr)[n])
    {
        for (size_t i{0}; i<n; i++)
            std::cout << arr[i] << ", ";
        std::cout << std::endl;
    }

    template<class Type, std::size_t n_rows, std::size_t n_cols>
    inline void print(const Type (& arr)[n_rows][n_cols])
    {
        for (std::size_t r{0}; r<n_rows; r++)
            print(arr[r]);
        std::cout << std::endl;
    }

    template<class Type, std::size_t n>
    inline double euclidean_distance(const Type (& x)[n], const Type (& y)[n])
    {
        double sum{0}, diff;

        for (std::size_t i{0}; i<n; i++) {
            diff = x[i]-y[i];
            sum += diff*diff;
        }

        return std::sqrt(sum);
    }

    template<class Type, std::size_t n_rows, std::size_t n_cols>
    inline void read_csv(const std::filesystem::path& path, char delim, Type (& mat)[n_rows][n_cols])
    {
        mcc::csv::reader reader{path, delim};
        std::string data[n_cols];

        for (std::size_t r{0}; r<n_rows; r++) {
            reader(data);

            for (std::size_t c{0}; c<n_cols; c++)
                mat[r][c] = std::stod(data[c]);
        }
    }

    template<class Type, std::size_t n>
    inline void read_csv(const std::filesystem::path& path, char delim, Type (& arr)[n])
    {
        mcc::csv::reader reader{path, delim};
        std::string data[1];

        for (std::size_t r{0}; r<n; r++) {
            reader(data);
            arr[r] = std::stod(data[0]);
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
