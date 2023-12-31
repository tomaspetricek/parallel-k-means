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
void benchmark(std::size_t n_samples, std::size_t n_features, std::size_t n_clusters, std::size_t n_threads)
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

    auto begin = std::chrono::high_resolution_clock::now();
    mcc::clustering::k_means<Precision> cluster{n_samples, n_features, n_clusters, n_threads};
    cluster(samples, centroids, expect_labels, centroids);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin);

    // log results
    // use custom number separator
    std::cout.imbue(std::locale(std::cout.getloc(), new num_separator<char>()));

    std::cout << "n samples: " << n_samples << ", "
              << "n features: " << n_features << ", "
              << "n clusters: " << n_clusters << ", "
              << "n threads: " << n_threads << ", "
//              << "n iterations: " << cluster.n_iter() << std::endl
              << "duration: " << duration.count() << " ns" << std::endl;
//              << "mean iteration duration: " << duration.count()/cluster.n_iter() << " ns" << std::endl << std::endl;

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
//            {640'000, 2,   10},
//            {320'000, 4,   10},
//            {160'000, 8,   10},
//            {80'000,  16,  10},
            {40'000, 32, 10},
//            {20'000,  64,  10},
//            {10'000,  128, 10},
//            {5'000,   256, 10},
    };

    std::vector<std::size_t> thread_counts{1, 2, 4, 6, 8, 12, 16, 20};

    for (const auto& dim: dims)
        for (const auto& n_threads: thread_counts)
            benchmark<double>(dim.n_samples, dim.n_features, dim.n_clusters, n_threads);

    return EXIT_SUCCESS;
}
