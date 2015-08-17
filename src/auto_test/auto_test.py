__author__ = 'Yue Dayu'

from randomFeatureList import *
from RF_train import *
from RF_predict import *

if __name__ == '__main__':
    res = 0
    for x in range(0, 200):
        randomList('features.txt', 40)
        random_forset_train('features_train.txt', 'out.pkl')
        res += random_forset_predict('features_test.txt', 'out.pkl', 'res.txt')
    res /= 200
    print(res)
