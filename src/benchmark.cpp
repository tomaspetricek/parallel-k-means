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

template<class Precision>
void benchmark(std::size_t n_samples, std::size_t n_features, std::size_t n_clusters)
{
    auto samples = new Precision[n_samples*n_features];
    auto centroids = new Precision[n_clusters*n_features];
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
    mcc::read_csv<Precision>(samples_dir/"samples.csv", delim, samples, n_samples, n_features);
    mcc::read_csv<std::size_t>(samples_dir/"labels.csv", delim, actual_labels, n_samples);
    mcc::read_csv<Precision>(samples_dir/"init_centroids.csv", delim, centroids, n_clusters, n_features);

    const std::size_t n_threads{static_cast<size_t>(omp_get_max_threads())};

    auto begin = std::chrono::high_resolution_clock::now();
    mcc::clustering::k_means<Precision> cluster{n_samples, n_features, n_clusters, n_threads};
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

    delete[] samples;
    delete[] centroids;
    delete[] expect_labels;
    delete[] actual_labels;
}

struct dimensions {
    std::size_t n_samples;
    std::size_t n_features;
    std::size_t n_clusters;
};

int main()
{
    std::vector<dimensions> dims{
//            {5'000,   256, 10},
//            {10'000,  128, 10},
//            {20'000,  64,  10},
//            {40'000,  32,  10},
//            {80'000,  16,  10},
//            {160'000, 8,   10},
//            {320'000, 4,   10},
//            {640'000, 2,   10},
            {10'000, 50, 10}
    };

    for (const auto& dim: dims)
        benchmark<double>(dim.n_samples, dim.n_features, dim.n_clusters);

    return EXIT_SUCCESS;
}
