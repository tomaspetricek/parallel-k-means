#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>

namespace mcc::clustering {

    // inspired: https://towardsdatascience.com/k-means-clustering-algorithm-applications-evaluation-methods-and-drawbacks-aa03e644b48a
    template<std::size_t n_dims, std::size_t n_clusters, std::size_t n_samples>
    class k_means {
        static_assert(n_samples>=n_clusters);

        void initialize_centroids(const double (& samples)[n_samples][n_dims], double (& centroids)[n_clusters][n_dims]) const
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
                std::copy(&samples[s][0], &samples[s][0]+n_dims, &centroids[c][0]);
            }
        }

        void
        compute_distances(const double (& centroids)[n_clusters][n_dims], const double (& samples)[n_samples][n_dims],
                double (& dists)[n_clusters][n_samples]) const
        {
            double sum;
            double diff;

            for (std::size_t s{0}; s<n_samples; s++) {
                for (std::size_t c{0}; c<n_clusters; c++) {
                    sum = 0;

                    for (std::size_t d{0}; d<n_dims; d++) {
                        diff = centroids[c][d]-samples[s][d];
                        sum += diff*diff;
                    }

                    dists[s][c] = std::sqrt(sum);
                }
            }
        }

        void find_closest(const double (& dists)[n_clusters][n_samples], std::size_t (& labels)[n_samples]) const
        {
            double min;

            for (std::size_t s{0}; s<n_samples; s++) {
                min = dists[s][0];

                for (std::size_t c{1}; c<n_clusters; c++)
                    if (dists[s][c]<min)
                        min = dists[s][c];

                labels[s] = min;
            }
        }

        void compute_centroids(const double (& samples)[n_samples][n_dims], const std::size_t (& labels)[n_samples],
                double (& centroids)[n_clusters][n_dims]) const
        {
            // initialize to zero
            double sums[n_clusters][n_dims] = {0};
            double counts[n_dims] = {0};
            std::size_t c;

            for (std::size_t s{0}; s<n_samples; s++) {
                c = labels[s];

                for (std::size_t d{0}; d<n_dims; d++)
                    sums[c][d] += samples[s][d];

                counts[c] += 1;
            }

            // mean
            for (c = 0; c<n_clusters; c++)
                for (std::size_t d{0}; d<n_dims; d++)
                    centroids[c][d] = sums[c][d]/counts[c];
        }

    public:
        void operator()(const double (& samples)[n_samples][n_dims], std::size_t (& labels)[n_samples])
        {
            // init centroids
            double centroids[n_clusters][n_dims];
            initialize_centroids(samples, centroids);

            // prepare variables
            double prev_centroids[n_clusters][n_dims];
            double dists[n_clusters][n_samples];
            bool changed{false};

            // fit
            do {
                std::copy(&centroids[0][0], &centroids[0][0]+n_clusters*n_dims, &prev_centroids[0][0]);
                compute_distances(centroids, samples, dists);
                find_closest(dists, labels);
                compute_centroids(samples, labels, centroids);

                // check if changed
                for (std::size_t c{0}; c<n_clusters && !changed; c++)
                    for (std::size_t d{0}; d<n_dims && !changed; d++)
                        if (prev_centroids[c][d]!=centroids[c][d])
                            changed = true;
            }
            while (changed);
        }
    };
}

int main()
{
    constexpr std::size_t n_dims{2}, n_clusters{3}, n_samples{10};
    double samples[n_samples][n_dims];
    std::size_t labels[n_samples];

    mcc::clustering::k_means<n_dims, n_clusters, n_samples> cluster;
    cluster(samples, labels);
    return 0;
}
