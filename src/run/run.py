import os
import sys
from RF_predict import *
from SVM_predict import *

def run(video):
	os.system('video2img.exe ' + video + ' 1.jpg 2.jpg')
	output = os.popen('getSingleFace.exe 1.jpg 2.jpg out1.jpg out2.jpg').read()
	if 'no face!' in output:
		print('no face!')
		return
	os.system('getSingleFeature.exe out1.jpg out2.jpg test.txt')
	#random_forset_predict('test.txt', 'out.pkl')
	svm_predict('test.txt', 'out.pkl')

if __name__ == '__main__':
	run(sys.argv[1])
