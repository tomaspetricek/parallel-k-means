//
// Created by Tomáš Petříček on 19.11.2022.
//

#include <iostream>
#include <random>
#include <filesystem>
#include <array.hpp>
#include <k_means.hpp>

template<class Precision>
void generate_initial_centroids(const Precision* samples, std::size_t* labels, Precision* init_centroids,
        Precision* centroids, std::size_t n_samples, std::size_t n_features, std::size_t n_clusters)
{
    std::mt19937 rand_gen(std::random_device{}());
    std::uniform_int_distribution<int> sampler(0, n_samples-1);

    std::size_t s;
    for (std::size_t c{0}; c<n_clusters; c++) {
        s = sampler(rand_gen);
        mcc::copy(samples+mcc::index(s, 0, n_features), init_centroids+mcc::index(c, 0, n_features), n_features);
    }

    mcc::copy(init_centroids, centroids, n_clusters*n_features);

    const std::size_t n_threads{static_cast<size_t>(omp_get_max_threads())};
    mcc::clustering::k_means<Precision> cluster{n_samples, n_features, n_clusters, n_threads};
    cluster(samples, labels, centroids);
}

template<class Precision>
void generate(std::size_t n_samples, std::size_t n_features, std::size_t n_clusters)
{
    auto* samples = new Precision[n_samples*n_features];
    auto* init_centroids = new Precision[n_clusters*n_features];
    auto* centroids = new Precision[n_clusters*n_features];
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

    bool generated{false};

    while (!generated) {
        try {
            generate_initial_centroids(samples, labels, init_centroids, centroids, n_samples, n_features, n_clusters);
            generated = true;
        }
        catch (...) {
            std::cout << "here" << std::endl;
        }
    }

    mcc::write_csv(samples_dir/"init_centroids.csv", delim, init_centroids, n_clusters, n_features);

    delete[] samples;
    delete[] labels;
    delete[] init_centroids;
    delete[] centroids;
}

struct dimensions {
    std::size_t n_samples;
    std::size_t n_features;
    std::size_t n_clusters;
};

int main()
{
    std::vector<dimensions> dims{
            {5'000,   256, 10},
            {10'000,  128, 10},
            {20'000,  64,  10},
            {40'000,  32,  10},
            {80'000,  16,  10},
            {160'000, 8,   10},
            {320'000, 4,   10},
            {640'000, 2,   10},
    };

    for (const auto& dim: dims)
        generate<double>(dim.n_samples, dim.n_features, dim.n_clusters);

    return EXIT_SUCCESS;
}