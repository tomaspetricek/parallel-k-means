from sklearn.datasets import make_blobs
import numpy as np


def compute_distances(samples, centroids):
    n_clusters = centroids.shape[0]
    distances = np.empty(shape=(samples.shape[0], n_clusters))

    for c in range(n_clusters):
        distances[:, c] = np.sqrt(np.sum((samples - centroids[c, :]) ** 2, axis=1))

    return distances


def compute_centroids(samples, labels, n_clusters):
    centroids = np.empty((n_clusters, samples.shape[1]))

    for c in range(n_clusters):
        centroids[c, :] = np.mean(samples[labels == c, :], axis=0)

    return centroids


def generate_samples(n_samples, n_features, centers):
    samples, labels, centers = make_blobs(n_samples=n_samples,
                                          n_features=n_features,
                                          centers=centers,
                                          cluster_std=0.8,
                                          shuffle=True,
                                          return_centers=True)

    np.savetxt("data/labels.csv", labels, delimiter=",")
    np.savetxt("data/samples.csv", samples, delimiter=",")
    np.savetxt("data/centers.csv", centers, delimiter=",")


def main():
    samples = np.genfromtxt("data/test/samples.csv", delimiter=',')
    labels = np.genfromtxt("data/test/labels.csv", delimiter=',')
    centers = np.genfromtxt("data/test/centers.csv", delimiter=',')

    centroids = compute_centroids(samples, labels, centers.shape[0])
    np.savetxt("data/test/centroids.csv", centroids, delimiter=',')

    dists = compute_distances(samples, centroids)
    np.savetxt("data/test/distances.csv", dists, delimiter=',')


if __name__ == "__main__":
    main()
