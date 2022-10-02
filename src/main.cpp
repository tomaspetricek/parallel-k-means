#include <iostream>
#include <random>
#include <algorithm>
#include <filesystem>
#include <array.hpp>
#include <k_means.hpp>

template<std::size_t n_samples, std::size_t n_features, std::size_t n_clusters>
void initialize_centroids(const double (& samples)[n_samples][n_features],
        double (& centroids)[n_clusters][n_features])
{
    std::size_t indices[n_samples];

    for (std::size_t i{0}; i<n_samples; i++)
        indices[i] = i;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(&indices[0], &indices[n_samples-1], gen);

    std::size_t s;
    for (std::size_t c{0}; c<n_clusters; c++) {
        s = indices[c];
        std::copy(&samples[s][0], &samples[s][0]+n_features, &centroids[c][0]);
    }
}

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

    // initialize centroids
    initialize_centroids(samples, expect_centroids);

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
