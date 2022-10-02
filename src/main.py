from sklearn.datasets import make_blobs
import numpy as np


def main():
    samples, labels, centers = make_blobs(n_samples=10_000,
                                          n_features=50,
                                          centers=10,
                                          cluster_std=0.8,
                                          shuffle=True,
                                          return_centers=True)

    np.savetxt("data/labels.csv", labels, delimiter=",")
    np.savetxt("data/samples.csv", samples, delimiter=",")
    np.savetxt("data/centers.csv", centers, delimiter=",")


if __name__ == "__main__":
    main()
