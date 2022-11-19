#include <iostream>
#include <random>
#include <filesystem>
#include <chrono>
#include <locale>
#include <array.hpp>
#include <k_means.hpp>


template<typename CharType>
struct num_separator : public std::numpunct<CharType> {
    std::string do_grouping() const override { return "\003"; }
    CharType do_thousands_sep() const override { return ' '; }
};

int main()
{
    constexpr std::size_t n_samples{10'000}, n_features{50}, n_clusters{10};
    double* samples = new double[n_samples*n_features];
    double* centroids = new double[n_clusters*n_features];
    std::size_t* expect_labels = new std::size_t[n_samples];
    std::size_t* actual_labels = new std::size_t[n_samples];

    char sep{'-'};
    std::stringstream ss;
    ss << n_samples << sep << n_features << sep << n_clusters;
    std::string samples_dirname = ss.str();

    std::filesystem::path data_dir{"data"};
    std::filesystem::path samples_dir{data_dir/samples_dirname};

    // read
    char delim{','};
    mcc::read_csv(samples_dir/"samples.csv", delim, samples, n_samples, n_features);
    mcc::read_csv(samples_dir/"labels.csv", delim, actual_labels, n_samples);
    mcc::read_csv(samples_dir/"init_centroids.csv", delim, centroids, n_clusters, n_features);

    // cluster
    const std::size_t n_threads{static_cast<size_t>(omp_get_max_threads())};
    mcc::clustering::k_means cluster{n_samples, n_features, n_clusters, n_threads};
    cluster(samples, expect_labels, centroids);

    auto begin = std::chrono::high_resolution_clock::now();
    cluster(samples, expect_labels, centroids);
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

    mcc::print(centroids, n_clusters, n_features);
    std::cout << "labels: " << std::endl;
    mcc::print(expect_labels, n_samples);

    delete[] samples;
    delete[] centroids;
    delete[] expect_labels;
    delete[] actual_labels;
    return EXIT_SUCCESS;
}
