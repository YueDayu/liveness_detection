#include <opencv2/opencv.hpp>
#include "mcv_facesdk.h"
#include <fstream>

using namespace std;
using namespace cv;

#define CMat Mat

bool facePic(Mat& origin, Mat& img, Mat& light_pic);
void rot90(cv::Mat &matImage, int rotflag);

int main(int argc, char* argv[]) {
	Mat img;
	Mat grayImg;
	Mat light_pic_mat;
	string no_light_pic;
	string light_pic;
	int j = 0;
	string input_nolight(argv[1]);
	string input_light(argv[2]);
	string output_nolight(argv[3]);
	string output_light(argv[4]);
	img = imread(input_nolight, 1);
	light_pic_mat = imread(input_light, 1);
	rot90(img, 1);
	cvtColor(img, grayImg, CV_RGB2GRAY);
	rot90(light_pic_mat, 1);
	if (facePic(img, grayImg, light_pic_mat)) {
		imwrite(output_nolight, img);
		imwrite(output_light, light_pic_mat);
		j++;
	}
	cout << "done" << endl;
	return 0;
}

bool facePic(Mat& origin, Mat& img, Mat& light_pic) {
	mcv_handle_t detector;
	PMCV_FACERECT pface = NULL;
	unsigned face_count = 0;
	detector = mcv_facesdk_create_frontal_detector_instance_from_resource(1);
	mcv_facesdk_frontal_detector(detector, img.data, img.cols, img.rows, img.cols, &pface, &face_count);
	bool res;
	if (face_count == 0) {
		cout << "no face!" << '\t';
		res = false;
	} else {
		cout << pface[0].Rect.right << '\t';
		int right = pface[0].Rect.right;
		if (right >= origin.cols) {
			right = origin.cols - 1;
		}
		origin = origin(Rect(pface[0].Rect.left, pface[0].Rect.top, right - pface[0].Rect.left - 1, pface[0].Rect.bottom - pface[0].Rect.top - 1));
		light_pic = light_pic(Rect(pface[0].Rect.left, pface[0].Rect.top, right - pface[0].Rect.left - 1, pface[0].Rect.bottom - pface[0].Rect.top - 1));
		res = true;
	}
	mcv_facesdk_release_frontal_result(pface, face_count);
	mcv_facesdk_destroy_frontal_instance(detector);
	return res;
}

void rot90(cv::Mat &matImage, int rotflag){
	//1=CW, 2=CCW, 3=180
	if (rotflag == 1){
		transpose(matImage, matImage);  
		flip(matImage, matImage,1); //transpose+flip(1)=CW
	} else if (rotflag == 2) {
		transpose(matImage, matImage);  
		flip(matImage, matImage,0); //transpose+flip(0)=CCW     
	} else if (rotflag ==3){
		flip(matImage, matImage,-1);    //flip(-1)=180          
	} else if (rotflag != 0){ //if not 0,1,2,3:
		cout  << "Unknown rotation flag(" << rotflag << ")" << endl;
	}
}