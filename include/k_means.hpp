//
// Created by Tomáš Petříček on 01.10.2022.
//

#ifndef MCC_K_MEANS_HPP
#define MCC_K_MEANS_HPP

#include <cmath>
#include <array.hpp>

namespace mcc::clustering {

    // inspired: https://towardsdatascience.com/k-means-clustering-algorithm-applications-evaluation-methods-and-drawbacks-aa03e644b48a
    template<std::size_t n_features, std::size_t n_clusters, std::size_t n_samples>
    class k_means {
        static_assert(n_samples>=n_clusters);

        void initialize_centroids(const double (& samples)[n_samples][n_features],
                double (& centroids)[n_clusters][n_features]) const
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

        void compute_distances(const double (& centroids)[n_clusters][n_features],
                const double (& samples)[n_samples][n_features],
                double (& dists)[n_samples][n_clusters]) const
        {
            double sum;
            double diff;

            for (std::size_t s{0}; s<n_samples; s++) {
                for (std::size_t c{0}; c<n_clusters; c++) {
                    sum = 0;

                    for (std::size_t f{0}; f<n_features; f++) {
                        diff = centroids[c][f]-samples[s][f];
                        sum += diff*diff;
                    }

                    dists[s][c] = std::sqrt(sum);
                }
            }
        }

        void find_closest(const double (& dists)[n_samples][n_clusters], std::size_t (& labels)[n_samples]) const
        {
            double min;
            std::size_t min_idx;

            for (std::size_t s{0}; s<n_samples; s++) {
                min = dists[s][0];
                min_idx = 0;

                for (std::size_t c{1}; c<n_clusters; c++)
                    if (dists[s][c]<min) {
                        min = dists[s][c];
                        min_idx = c;
                    }

                labels[s] = min_idx;
            }
        }

        void compute_centroids(const double (& samples)[n_samples][n_features], const std::size_t (& labels)[n_samples],
                double (& centroids)[n_clusters][n_features]) const
        {
            // initialize to zero
            double sums[n_clusters][n_features] = {0};
            double counts[n_features] = {0};
            std::size_t c;

            for (std::size_t s{0}; s<n_samples; s++) {
                c = labels[s];

                for (std::size_t f{0}; f<n_features; f++)
                    sums[c][f] += samples[s][f];

                counts[c] += 1;
            }

            // mean
            for (c = 0; c<n_clusters; c++)
                for (std::size_t f{0}; f<n_features; f++) {
                    assert(sums[c][f]);
                    centroids[c][f] = sums[c][f]/counts[c];
                }
        }

    public:
        void operator()(const double (& samples)[n_samples][n_features], std::size_t (& labels)[n_samples],
                double (& centroids)[n_clusters][n_features])
        {
            initialize_centroids(samples, centroids);

            // prepare
            double prev_centroids[n_clusters][n_features];
            double dists[n_samples][n_clusters];

            // fit
            do {
                std::copy(&centroids[0][0], &centroids[0][0]+n_clusters*n_features, &prev_centroids[0][0]);
                compute_distances(centroids, samples, dists);
                find_closest(dists, labels);
                compute_centroids(samples, labels, centroids);
            }
            while (!are_equal(prev_centroids, centroids));
        }
    };
}

#endif //MCC_K_MEANS_HPP
