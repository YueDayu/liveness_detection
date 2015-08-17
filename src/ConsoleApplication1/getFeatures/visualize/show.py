__author__ = 'Yue Dayu'

from time import time

import os
import sys
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import offsetbox
from matplotlib.colors import ListedColormap
from sklearn import (manifold, datasets, decomposition, ensemble, lda,
                     random_projection)


def load_feature(filename):
    fin = open(filename)
    lines = fin.readlines()
    Xlst = []
    ylst = []
    name = []
    lasttype = ""
    num = -1
    for line in lines:
        raw = line.split("\t")
        # vec = map(float, raw[1:])
        vec = []
        i = 0
        for x in raw:
            if x != '\n' and i != 0 and i != 1:
                vec.append(float(x))
            i += 1
        num = int(raw[0])
        tempname = raw[1]
        # if (lasttype != imgtype):
        #     num += 1
        #     lasttype = imgtype
        Xlst.append(vec)
        ylst.append(num)
        name.append(tempname)

    X = np.array(Xlst)
    y = np.array(ylst)
    return X, y, name


def plot_embedding(X, y, name, title=None, shownum=False):
    cm = plt.cm.Set1
    # print(cm)
    plt.figure()
    ax = plt.subplot(111)
    if shownum:
        for i in range(X.shape[0]):
            plt.text(X[i, 0], X[i, 1], str(name[i]),
                     color=plt.cm.Set1(y[i] / 10.),
                     fontdict={'weight': 'bold', 'size': 9})
    else:
        for i in range(0, 1):
            ax.scatter(X[i::1, 0], X[i::1, 1], marker='x', c=y[i::1], cmap=cm, alpha = 1)
    plt.xticks([]), plt.yticks([])
    if title is not None:
        plt.title(title)


def visualize(X, y, name):
    X2 = X.copy()
    X2.flat[::X.shape[1] + 1] += 0.01  # Make X invertible
    t0 = time()
    X_lda = lda.LDA(n_components=2).fit_transform(X2, y)
    plot_embedding(X_lda, y, name, "haha", True)
    plt.show()

if __name__ == '__main__':
    X, y, name = load_feature(sys.argv[1])
    visualize(X, y, name)
