__author__ = 'Yue Dayu'

from sklearn import svm
import pickle
import numpy as np
import sys


def readfile(filename):
    list_file = open(filename, 'r')
    # _ = list_file.readline()
    lines = list_file.readlines()
    type_list = []
    feature_list = []
    for line in lines:
        current_line = line.split('\t')
        type_list.append(current_line[0])
        # temp_list = map(float, current_line[2:-1])
        temp_list = []
        i = 0
        for x in current_line:
            if i != 0 and i != 1 and x != '\n':
                temp_list.append(float(x))
            i += 1
        feature_list.append(temp_list)
    result = np.array(feature_list)
    return type_list, result


def svm_train(filename, output):
    tag, data = readfile(filename)
    # for x in data:
    #     print(x)
    clf = svm.SVC(kernel='linear')
    clf.fit(data, tag)
    print(clf)
    with open(output, 'wb') as f:
        pickle.dump(clf, f)


if __name__ == '__main__':
    svm_train(sys.argv[1], sys.argv[2])
