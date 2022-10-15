#include <iostream>
#include <random>
#include <filesystem>
#include <chrono>
#include <locale>
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

template<typename CharType>
struct num_separator : public std::numpunct<CharType> {
    std::string do_grouping() const override { return "\003"; }
    CharType do_thousands_sep() const override { return ' '; }
};

int main()
{
    constexpr std::size_t n_samples{10'000}, n_features{50}, n_clusters{10};

    // prepare
    double samples[n_samples][n_features];
    double centroids[n_clusters][n_features];
    std::size_t actual_labels[n_samples], labels[n_samples];

    // read
    char delim{','};
    std::filesystem::path data_dir{"data"};
    mcc::read_csv(data_dir/"samples.csv", delim, samples);
    mcc::read_csv(data_dir/"labels.csv", delim, actual_labels);
    mcc::read_csv(data_dir/"init_centroids.csv", delim, centroids);

    // cluster
    mcc::clustering::k_means<n_samples, n_features, n_clusters> cluster;
    auto begin = std::chrono::high_resolution_clock::now();
    cluster(samples, labels, centroids);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin);

    // log results
    // use custom number separator
    std::cout.imbue(std::locale(std::cout.getloc(), new num_separator<char>()));

    std::cout << "n samples: " << n_samples << std::endl
              << "n features: " << n_features << std::endl
              << "n clusters: " << n_clusters << std::endl
              << "n iterations: " << cluster.n_iter() << std::endl
              << "duration: " << duration.count() << " ns" << std::endl
              << "centroids: " << std::endl;
    mcc::print(centroids);
    std::cout << "labels: " << std::endl;
    mcc::print(labels);
    return EXIT_SUCCESS;
}
