#include <iostream>
#include <random>
#include <filesystem>
#include <array.hpp>
#include <k_means.hpp>


int main()
{
    constexpr std::size_t n_samples{10'000}, n_features{50}, n_clusters{10};

    // prepare
    double samples[n_samples][n_features];
    double actual_centroids[n_clusters][n_features], expect_centroids[n_clusters][n_features];
    std::size_t actual_labels[n_samples], expect_labels[n_samples];

    // read
    char delim{','};
    std::filesystem::path data_dir{"../../data"};
    mcc::read_csv(data_dir/"samples.csv", delim, samples);
    mcc::read_csv(data_dir/"labels.csv", delim, actual_labels);
    mcc::read_csv(data_dir/"centers.csv", delim, actual_centroids);

    // cluster
    mcc::clustering::k_means<n_samples, n_features, n_clusters> cluster;
    cluster(samples, expect_labels, expect_centroids);

    // log results
    std::cout << "n iterations: " << cluster.n_iter() << std::endl;
    std::cout << "centroids: " << std::endl;
    mcc::print(expect_centroids);
    std::cout << "labels: " << std::endl;
    mcc::print(expect_labels);
    return EXIT_SUCCESS;
}
