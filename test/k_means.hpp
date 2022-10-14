//
// Created by Tomáš Petříček on 02.10.2022.
//

#ifndef MCC_TEST_K_MEANS_HPP
#define MCC_TEST_K_MEANS_HPP

#include <boost/test/unit_test.hpp>
#include <k_means.hpp>

constexpr std::size_t n_samples{2'000}, n_features{2}, n_clusters{3};
class fixture : public mcc::clustering::k_means<n_samples, n_features, n_clusters> {
};

BOOST_FIXTURE_TEST_SUITE(k_means_tests, fixture)

    BOOST_AUTO_TEST_CASE(k_means_test) {
        double samples[n_samples][n_features];
        double actual_dists[n_samples][n_clusters], expect_dists[n_samples][n_clusters];
        std::size_t actual_labels[n_samples], expect_labels[n_samples];
        double actual_centroids[n_clusters][n_features], expect_centroids[n_clusters][n_features];

        // read
        char delim{','};
        std::filesystem::path data_dir{"../../test/data"};
        mcc::read_csv(data_dir/"samples.csv", delim, samples);
        mcc::read_csv(data_dir/"labels.csv", delim, actual_labels);
        mcc::read_csv(data_dir/"centroids.csv", delim, actual_centroids);
        mcc::read_csv(data_dir/"distances.csv", delim, actual_dists);

        double tolerance{0.0000001};
        int n_changed;

        mcc::copy(actual_labels, expect_labels);
        assign_centroids(actual_centroids, samples, expect_labels, n_changed);
        BOOST_TEST(!n_changed);
        BOOST_TEST(mcc::are_equal(actual_labels, expect_labels));

        compute_centroids(samples, actual_labels, expect_centroids);
        for (std::size_t c{0}; c<n_clusters; c++)
            for (std::size_t f{0}; f<n_features; f++)
                BOOST_REQUIRE_CLOSE(actual_centroids[c][f], expect_centroids[c][f], tolerance);
    }
BOOST_AUTO_TEST_SUITE_END()

#endif //MCC_TEST_K_MEANS_HPP
