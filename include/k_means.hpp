//
// Created by Tomáš Petříček on 01.10.2022.
//

#ifndef MCC_K_MEANS_HPP
#define MCC_K_MEANS_HPP

#include <cmath>
#include <random>
#include <array.hpp>
#include <cassert>
#include <omp.h>

namespace mcc::clustering {

    // inspired: https://towardsdatascience.com/k-means-clustering-algorithm-applications-evaluation-methods-and-drawbacks-aa03e644b48a
    template<std::size_t n_samples, std::size_t n_features, std::size_t n_clusters>
    class k_means {
        static_assert(n_samples>=n_clusters);
        std::size_t n_iter_{0};

    protected:
        void assign_centroids(const double (& centroids)[n_clusters][n_features],
                const double (& samples)[n_samples][n_features], std::size_t (& labels)[n_samples],
                int& n_changed) const
        {
            n_changed = 0;
            std::size_t min_idx;
            double dist, min_dist;

            #pragma omp parallel for schedule(static) private(min_idx, dist, min_dist)
            for (std::size_t s = 0; s<n_samples; s++) {
                min_dist = euclidean_distance(centroids[0], samples[s]);
                min_idx = 0;

                for (std::size_t c{1}; c<n_clusters; c++) {
                    dist = euclidean_distance(centroids[c], samples[s]);

                    if (dist<min_dist) {
                        min_dist = dist;
                        min_idx = c;
                    }
                }

                if (labels[s]!=min_idx) {
                    #pragma omp atomic update
                    n_changed++;
                }

                labels[s] = min_idx;
            }
        }

        void compute_centroids(const double (& samples)[n_samples][n_features], const std::size_t (& labels)[n_samples],
                double (& centroids)[n_clusters][n_features])
        {
            // initialize to zero
            const int n_threads{omp_get_max_threads()};
            double sums[n_threads][n_clusters][n_features] = {0};
            double counts[n_threads][n_clusters] = {0};
            std::size_t c;

            omp_set_num_threads(n_threads);

            #pragma omp parallel private(c)
            {
                int id = omp_get_thread_num();

                #pragma omp for schedule(static)
                for (std::size_t s = 0; s<n_samples; s++) {
                    c = labels[s];

                    #pragma omp simd
                    for (std::size_t f = 0; f<n_features; f++)
                        sums[id][c][f] += samples[s][f];

                    counts[id][c] += 1;
                }
            }

            // mean
            double sum, count;

            #pragma omp for schedule(static)
            for (c = 0; c<n_clusters; c++) {
                for (std::size_t f = 0; f<n_features; f++) {
                    sum = 0;
                    count = 0;

                    #pragma omp simd
                    for (std::size_t id = 0; id<n_threads; id++) {
                        sum += sums[id][c][f];
                        count += counts[id][c];
                    }

                    centroids[c][f] = sum/count;
                }
            }
        }

    public:
        void operator()(const double (& samples)[n_samples][n_features], std::size_t (& labels)[n_samples],
                double (& centroids)[n_clusters][n_features])
        {
            int n_changed;

            // assign initial centroids
            assign_centroids(centroids, samples, labels, n_changed);

            // fit
            do {
                compute_centroids(samples, labels, centroids);
                assign_centroids(centroids, samples, labels, n_changed);
                n_iter_++;
            }
            while (n_changed);
        }

        size_t n_iter() const
        {
            return n_iter_;
        }
    };
}

#endif //MCC_K_MEANS_HPP
