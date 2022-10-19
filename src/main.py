from sklearn.datasets import make_blobs
import numpy as np
import os


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


def generate_same_size_samples(min_exp, max_exp, n_centers, min_n_samples):
    assert (min_exp < max_exp)
    n_nums = (2 ** max_exp) * min_n_samples
    res = list()

    for exp in range(min_exp, max_exp + 1, 1):
        n_features = (2 ** exp)
        n_samples = int(n_nums / n_features)
        samples, labels, centers = make_blobs(n_samples=n_samples,
                                              n_features=n_features,
                                              centers=n_centers,
                                              cluster_std=0.8,
                                              shuffle=True,
                                              return_centers=True)
        res.append((n_samples, n_features, n_centers, samples, labels, centers))

    return res


def save_data(data, dir_path, data_dir_fmt):
    for n_samples, n_features, n_centers, samples, labels, centers in data:
        # create data directory
        data_dir_name = data_dir_fmt.format(n_samples=n_samples, n_features=n_features, n_centers=n_centers)
        data_dir_path = os.path.join(dir_path, data_dir_name)
        os.mkdir(data_dir_path)

        # save data
        np.savetxt(os.path.join(data_dir_path, "labels.csv"), labels, delimiter=",")
        np.savetxt(os.path.join(data_dir_path, "samples.csv"), samples, delimiter=",")
        np.savetxt(os.path.join(data_dir_path, "centers.csv"), centers, delimiter=",")


def main():
    data = generate_same_size_samples(1, 8, 10, 5_000)
    src_data_dir_path = "data"
    data_dir_fmt = "{n_samples}-{n_features}-{n_centers}"
    save_data(data, src_data_dir_path, data_dir_fmt)


if __name__ == "__main__":
    main()
