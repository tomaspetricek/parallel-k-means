#include <iostream>
#include <random>
#include <filesystem>
#include <array.hpp>
#include <k_means.hpp>


template<std::size_t n_samples, std::size_t n_features, std::size_t n_clusters>
void initialize_centroids(const double (& samples)[n_samples][n_features],
        double (& centroids)[n_clusters][n_features])
{
    std::mt19937 rand_gen(std::random_device{}());
    std::uniform_int_distribution<int> sampler(0, n_samples-1);

    std::size_t s;
    for (std::size_t c{0}; c<n_clusters; c++) {
        s = sampler(rand_gen);
        mcc::copy(samples[s], centroids[c]);
    }
}


int main()
{
    constexpr std::size_t n_samples{10'000}, n_features{50}, n_clusters{10};

    // prepare
    double samples[n_samples][n_features];
    double centroids[n_clusters][n_features];
    std::size_t actual_labels[n_samples], labels[n_samples];

    // read
    char delim{','};
    std::filesystem::path data_dir{"src/data"};
    mcc::read_csv(data_dir/"samples.csv", delim, samples);
    mcc::read_csv(data_dir/"labels.csv", delim, actual_labels);
    mcc::read_csv(data_dir/"init_centroids.csv", delim, centroids);

    // cluster
    mcc::clustering::k_means<n_samples, n_features, n_clusters> cluster;
    cluster(samples, labels, centroids);

    // log results
    std::cout << "n iterations: " << cluster.n_iter() << std::endl;
    std::cout << "centroids: " << std::endl;
    mcc::print(centroids);
    std::cout << "labels: " << std::endl;
    mcc::print(labels);
    return EXIT_SUCCESS;
}
