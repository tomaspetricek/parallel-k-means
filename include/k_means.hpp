//
// Created by Tomáš Petříček on 01.10.2022.
//

#ifndef MCC_K_MEANS_HPP
#define MCC_K_MEANS_HPP

#include <cmath>
#include <array.hpp>

namespace mcc::clustering {

    // inspired: https://towardsdatascience.com/k-means-clustering-algorithm-applications-evaluation-methods-and-drawbacks-aa03e644b48a
    template<std::size_t n_samples, std::size_t n_features, std::size_t n_clusters>
    class k_means {
        static_assert(n_samples>=n_clusters);
        std::size_t n_iter_{0};
        std::mt19937 rand_gen_;
        std::uniform_int_distribution<int> sampler_;

    protected:
        void initialize_centroids(const double (& samples)[n_samples][n_features],
                double (& centroids)[n_clusters][n_features])
        {
            std::size_t s;
            for (std::size_t c{0}; c<n_clusters; c++) {
                s = sampler_(rand_gen_);
                std::copy(&samples[s][0], &samples[s][0]+n_features, &centroids[c][0]);
            }
        }

        void compute_distances(const double (& centroids)[n_clusters][n_features],
                const double (& samples)[n_samples][n_features],
                double (& dists)[n_samples][n_clusters]) const
        {
            for (std::size_t s{0}; s<n_samples; s++)
                for (std::size_t c{0}; c<n_clusters; c++)
                    dists[s][c] = euclidean_distance(centroids[c], samples[s]);
        }

        void label_samples(const double (& dists)[n_samples][n_clusters], std::size_t (& labels)[n_samples]) const
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
                double (& centroids)[n_clusters][n_features])
        {
            // initialize to zero
            double sums[n_clusters][n_features] = {0};
            double counts[n_clusters] = {0};
            std::size_t c;

            for (std::size_t s{0}; s<n_samples; s++) {
                c = labels[s];

                for (std::size_t f{0}; f<n_features; f++)
                    sums[c][f] += samples[s][f];

                counts[c] += 1;
            }

            // mean
            std::size_t s;
            for (c = 0; c<n_clusters; c++) {
                // src: https://stackoverflow.com/questions/11075272/k-means-empty-cluster
                if (sums[c][0]) {
                    for (std::size_t f{0}; f<n_features; f++)
                        centroids[c][f] = sums[c][f]/counts[c];
                }
                // empty -> replace with random sample
                else {
                    s = sampler_(rand_gen_);
                    std::copy(&samples[s][0], &samples[s][0]+n_features, &centroids[c][0]);
                }
            }
        }

    public:
        explicit k_means()
                :rand_gen_(std::random_device{}()), sampler_(0, n_samples-1) { }

        void operator()(const double (& samples)[n_samples][n_features], std::size_t (& labels)[n_samples],
                double (& centroids)[n_clusters][n_features])
        {
            initialize_centroids(samples, centroids);

            // prepare
            double prev_centroids[n_clusters][n_features];
            double dists[n_samples][n_clusters];

            // fit
            do {
                mcc::copy(centroids, prev_centroids);
                compute_distances(centroids, samples, dists);
                label_samples(dists, labels);
                compute_centroids(samples, labels, centroids);
                n_iter_++;
            }
            while (!are_equal(prev_centroids, centroids));
        }

        size_t n_iter() const
        {
            return n_iter_;
        }
    };
}

#endif //MCC_K_MEANS_HPP
