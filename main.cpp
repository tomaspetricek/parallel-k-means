#include <iostream>
#include <random>
#include <algorithm>
#include <filesystem>
#include <k_means.hpp>
#include <reader.hpp>


template<class Type, std::size_t n_rows, std::size_t n_cols>
void read(const std::filesystem::path& path, char delim, Type (& mat)[n_rows][n_cols])
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
void read(const std::filesystem::path& path, char delim, Type (& arr)[n])
{
    mcc::csv::reader reader{path, delim};
    std::string data[1];

    for (std::size_t r{0}; r<n; r++) {
        reader(data);
        arr[r] = std::stod(data[0]);
    }
}

int main()
{
    constexpr std::size_t n_features{2}, n_clusters{3}, n_samples{2000};

    // prepare
    double samples[n_samples][n_features];
    double actual_centroids[n_clusters][n_features], expect_centroids[n_clusters][n_features];
    std::size_t actual_labels[n_samples], expect_labels[n_samples];

    // read
    char delim{','};
    read({"../data/samples.csv"}, delim, samples);
    read({"../data/labels.csv"}, delim, actual_labels);
    read({"../data/centers.csv"}, delim, actual_centroids);

    // cluster
    mcc::clustering::k_means<n_features, n_clusters, n_samples> cluster;
    cluster(samples, expect_labels, expect_centroids);

    // log results
    std::cout << "centroids: " << std::endl;
    print(expect_centroids);
    std::cout << "labels: " << std::endl;
    print(expect_labels);
    return 0;
}
