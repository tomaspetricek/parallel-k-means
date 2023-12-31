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
    template<class Precision>
    class k_means {
        std::size_t n_samples_;
        std::size_t n_features_;
        std::size_t n_clusters_;
        std::size_t n_threads_;
        Precision* sums_;
        Precision* counts_;
        std::size_t n_iter_{0};

    protected:
        void
        assign_centroids(const Precision* centroids, const Precision* samples, std::size_t* labels, bool& changed) const
        {
            changed = false;
            std::size_t min_idx;
            Precision dist, min_dist;

            #pragma omp parallel for schedule(static) private(min_idx, dist, min_dist) num_threads(n_threads_)
            for (std::size_t s = 0; s<n_samples_; s++) {
                min_dist = euclidean_distance(centroids, samples+index(s, 0, n_features_), n_features_);
                min_idx = 0;

                for (std::size_t c{1}; c<n_clusters_; c++) {
                    dist = euclidean_distance(centroids+index(c, 0, n_features_), samples+index(s, 0, n_features_),
                            n_features_);

                    if (dist<min_dist) {
                        min_dist = dist;
                        min_idx = c;
                    }
                }

                if (labels[s]!=min_idx) changed = true;

                labels[s] = min_idx;
            }
        }

        void compute_centroids(const Precision* samples, const std::size_t* labels, Precision* centroids)
        {
            // initialize to zero
            #pragma omp simd
            for (int i = 0; i<n_threads_*n_clusters_*n_features_; i++)
                sums_[i] = 0;

            #pragma omp simd
            for (int i = 0; i<n_threads_*n_clusters_; i++)
                counts_[i] = 0;

            std::size_t c;

            #pragma omp parallel private(c) num_threads(n_threads_)
            {
                int id = omp_get_thread_num();

                #pragma omp for schedule(static)
                for (std::size_t s = 0; s<n_samples_; s++) {
                    c = labels[s];

                    #pragma omp simd
                    for (std::size_t f = 0; f<n_features_; f++)
                        sums_[index(id, c, f, n_clusters_, n_features_)] += samples[index(s, f, n_features_)];

                    counts_[index(id, c, n_clusters_)] += 1;
                }
            }

            // mean
            Precision sum, count;

            for (c = 0; c<n_clusters_; c++) {
                for (std::size_t f = 0; f<n_features_; f++) {
                    sum = count = 0;

                    #pragma omp simd
                    for (std::size_t id = 0; id<n_threads_; id++) {
                        sum += sums_[index(id, c, f, n_clusters_, n_features_)];
                        count += counts_[index(id, c, n_clusters_)];
                    }

                    // if (!sum) throw std::runtime_error("Cluster without samples");
                    centroids[index(c, f, n_features_)] = sum/count;
                }
            }
        }

    public:
        explicit k_means(size_t n_samples, size_t n_features, size_t n_clusters, size_t n_threads)
                :n_samples_(n_samples), n_features_(n_features), n_clusters_(n_clusters), n_threads_(n_threads)
        {
            assert(n_samples>=n_clusters);
            sums_ = new Precision[n_threads*n_clusters*n_features];
            counts_ = new Precision[n_threads*n_clusters];
        }

        void operator()(const Precision* samples, Precision* init_centroids, std::size_t* labels, Precision* centroids)
        {
            bool changed;

            // assign initial centroids
            assign_centroids(init_centroids, samples, labels, changed);

            // fit
            do {
                compute_centroids(samples, labels, centroids);
                assign_centroids(centroids, samples, labels, changed);
                n_iter_++;
            }
            while (changed);
        }

        size_t n_iter() const
        {
            return n_iter_;
        }

        ~k_means()
        {
            delete[] sums_;
            delete[] counts_;
        }
    };
}

#endif //MCC_K_MEANS_HPP
