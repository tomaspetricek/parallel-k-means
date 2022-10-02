//
// Created by Tomáš Petříček on 02.10.2022.
//

#ifndef MCC_TEST_K_MEANS_HPP
#define MCC_TEST_K_MEANS_HPP

#include <boost/test/unit_test.hpp>
#include <k_means.hpp>

constexpr std::size_t n_samples{2000}, n_features{2}, n_clusters{3};
class fixture : public mcc::clustering::k_means<n_samples, n_features, n_clusters> {
};

BOOST_FIXTURE_TEST_SUITE(k_means_tests, fixture)
    BOOST_AUTO_TEST_CASE(label_samples_test)
    {
        // prepare
        double samples[n_samples][n_features];
        double actual_centroids[n_clusters][n_features];
        std::size_t actual_labels[n_samples], expect_labels[n_samples];

        // read
        char delim{','};
        std::filesystem::path data_dir{"../../data/test"};
        mcc::read_csv(data_dir/"samples.csv", delim, samples);
        mcc::read_csv(data_dir/"labels.csv", delim, actual_labels);
        mcc::read_csv(data_dir/"centers.csv", delim, actual_centroids);

        double dists[n_samples][n_clusters];
        compute_distances(actual_centroids, samples, dists);
        label_samples(dists, expect_labels);
        BOOST_TEST(mcc::are_equal(actual_labels, expect_labels));
    }

    BOOST_AUTO_TEST_CASE(compute_centroids_test)
    {
        // prepare
        double samples[n_samples][n_features];
        double actual_centroids[n_clusters][n_features], expect_centroids[n_clusters][n_features];
        std::size_t actual_labels[n_samples];

        // read
        char delim{','};
        std::filesystem::path data_dir{"../../data/test"};
        mcc::read_csv(data_dir/"samples.csv", delim, samples);
        mcc::read_csv(data_dir/"labels.csv", delim, actual_labels);
        mcc::read_csv(data_dir/"centers.csv", delim, actual_centroids);

        compute_centroids(samples, actual_labels, expect_centroids);

        for (std::size_t c{0}; c<n_clusters; c++)
            for (std::size_t f{0}; f<n_features; f++)
                BOOST_REQUIRE_CLOSE(actual_centroids[c][f], expect_centroids[c][f], 5);
    }
BOOST_AUTO_TEST_SUITE_END()

#endif //MCC_TEST_K_MEANS_HPP
