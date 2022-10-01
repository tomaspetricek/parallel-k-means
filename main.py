from sklearn.datasets import make_blobs
import numpy as np


def main():
    samples, labels, centers = make_blobs(n_samples=2000,
                                          n_features=2,
                                          centers=3,
                                          cluster_std=0.4,
                                          shuffle=True,
                                          return_centers=True)

    np.savetxt("data/labels.csv", labels, delimiter=",")
    np.savetxt("data/samples.csv", samples, delimiter=",")
    np.savetxt("data/centers.csv", centers, delimiter=",")


if __name__ == "__main__":
    main()
