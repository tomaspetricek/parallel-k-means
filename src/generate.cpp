//
// Created by Tomáš Petříček on 19.11.2022.
//

#include <iostream>
#include <random>
#include <filesystem>
#include <array.hpp>
#include <k_means.hpp>


void generate_initial_centroids(const double* samples, std::size_t* labels, double* init_centroids,
        std::size_t n_samples, std::size_t n_features, std::size_t n_clusters)
{
    std::mt19937 rand_gen(std::random_device{}());
    std::uniform_int_distribution<int> sampler(0, n_samples-1);

    std::size_t s;
    for (std::size_t c{0}; c<n_clusters; c++) {
        s = sampler(rand_gen);
        mcc::copy(samples+mcc::index(s, 0, n_features), init_centroids+mcc::index(c, 0, n_features), n_features);
    }

    const std::size_t n_threads{static_cast<size_t>(omp_get_max_threads())};
    mcc::clustering::k_means cluster{n_samples, n_features, n_clusters, n_threads};
    cluster(samples, labels, init_centroids);

    mcc::print(init_centroids, n_clusters, n_features);
}

int main()
{
    constexpr std::size_t n_samples{640'000}, n_features{2}, n_clusters{10};
    double* samples = new double[n_samples*n_features];
    double* init_centroids = new double[n_clusters*n_features];
    std::size_t* labels = new std::size_t[n_samples];

    char sep{'-'};
    std::stringstream ss;
    ss << n_samples << sep << n_features << sep << n_clusters;
    std::string samples_dirname = ss.str();

    std::filesystem::path data_dir{"data"};
    std::filesystem::path samples_dir{data_dir/samples_dirname};

    // read
    char delim{','};
    mcc::read_csv(samples_dir/"samples.csv", delim, samples, n_samples, n_features);

    // generate
    generate_initial_centroids(samples, labels, init_centroids, n_samples, n_features, n_clusters);

    delete[] samples;
    delete[] labels;
    delete[] init_centroids;
}