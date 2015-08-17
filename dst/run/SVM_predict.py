__author__ = 'Yue Dayu'

from SVM_train import *
from collections import Counter


def svm_predict(filename, train_file):
    type_list, data = readfile(filename)
    with open(train_file, 'rb') as f:
        clf = pickle.load(f)
    result = clf.predict(data)
    if result[0] == '0':
        print ('true')
    else:
        print ('false')

if __name__ == '__main__':
    svm_predict(sys.argv[1], sys.argv[2], sys.argv[3])
