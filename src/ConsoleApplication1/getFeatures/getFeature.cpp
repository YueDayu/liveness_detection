#include <opencv2/opencv.hpp>
#include <fstream>

using namespace std;
using namespace cv;

int main() {
 	int pic_number = 221;
	int dims = 5;
	int dimsquare = 25;
	//int tag[] = {0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1};
	ofstream out("..//feature//features.txt");
	for (int i = 0; i < pic_number; i++) {
		cout << "processing " << i << '\t';
		Mat origin_light = imread("..//face//" + to_string(i) + "_light.jpg", 1);
		if (origin_light.data == NULL) {
			continue;
		}
		blur(origin_light, origin_light, Size(dims, dims));
		Mat origin_no = imread("..//face//" + to_string(i) + "_nolight.jpg", 1);
		blur(origin_no, origin_no, Size(dims, dims));
		//cvtColor(origin_light, origin_light, CV_BGR2HSV);
		//cvtColor(origin_no, origin_no, CV_BGR2HSV);
		cvtColor(origin_light, origin_light, CV_BGR2Lab);
		cvtColor(origin_no, origin_no, CV_BGR2Lab);

		Mat *light_part = new Mat[dimsquare];
		Mat *no_part = new Mat[dimsquare];
		int row = origin_light.rows;
		int col = origin_light.cols;

		int row_part = row / dims;
		int col_part = col / dims;

		for (int j = 0; j < dimsquare; j++) {
			int xx = j % dims;
			int yy = j / dims;
			light_part[j] = origin_light(Rect(col_part * xx, row_part * yy, col_part, row_part));
			no_part[j] = origin_no(Rect(col_part * xx, row_part * yy, col_part, row_part));
		}
		/*light_part[0] = origin_light(Rect(0, 0, col / 2, row / 3));
		no_part[0] = origin_no(Rect(0, 0, col / 2, row / 3));
		light_part[1] = origin_light(Rect(col / 2, 0, col / 2, row / 3));
		no_part[1] = origin_no(Rect(col / 2, 0, col / 2, row / 3));
		light_part[2] = origin_light(Rect(0, row / 3, col / 3, 2 * row / 3));
		no_part[2] = origin_no(Rect(0, row / 3, col / 3, 2 * row / 3));
		light_part[3] = origin_light(Rect(col * 2 / 3, row / 3, col / 3, 2 * row / 3));
		no_part[3] = origin_no(Rect(col * 2 / 3, row / 3, col / 3, 2 * row / 3));
		light_part[4] = origin_light(Rect(col / 3, row / 3, col / 3, row / 3));
		no_part[4] = origin_no(Rect(col / 3, row / 3, col / 3, row / 3));
		light_part[5] = origin_light(Rect(col / 3, 2 * row / 3, col / 3, row / 3));
		no_part[5] = origin_no(Rect(col / 3, 2 * row / 3, col / 3, row / 3));*/
		int size = 255;

		int h_bins = size; int s_bins = size;
		int histSize[] = { h_bins, s_bins };

		// hue的取值范围从0到256, saturation取值范围从0到180
		float h_ranges[] = { 0, size };
		float s_ranges[] = { 0, size };
		float v_ranges[] = { 0, size };

		const float* ranges[] = { h_ranges, s_ranges, v_ranges };

		// 使用第0和第1通道
		int channels[] = { 0 };

		MatND hist1;
		MatND hist2;

		//double scale = mean(origin_no)[0]/255;

		double *res = new double[dimsquare];

		for (int j = 0; j < dimsquare; j++) {
			calcHist( &light_part[j], 1, channels, Mat(), hist1, 1, histSize, ranges, true, false );
			normalize( hist1, hist1, 0, 1, NORM_MINMAX, -1, Mat());

			calcHist( &no_part[j], 1, channels, Mat(), hist2, 1, histSize, ranges, true, false );
			normalize( hist2, hist2, 0, 1, NORM_MINMAX, -1, Mat());

			res[j] = compareHist(hist1, hist2, CV_COMP_BHATTACHARYYA);
		}

		/*for (int j = 0; j < dimsquare; j++)
			res[j] /= scale;*/

		/*Mat test = origin_light / origin_no;
		
		vector<Mat> mhsv(test.channels());
		split(test, mhsv);
		imwrite("res//" + to_string(i) + ".jpg", mhsv[2]);
		double check[5];
		for (int j = 0; j < 5; j++) {
			check[j] = 0;
		}
		int row = test.rows;
		int col = test.cols;
		int sum = row * col;
		for (int m = 0; m < row; m++) {
			for (int n = 0; n < col; n++) {
				if (mhsv[1].at<uchar>(m, n) == 128) {
					check[0]++;
					int num = (m * 2 / row) * 2 + (n * 2 / col) + 1;
					if (num < 9) {
						check[num]++;
					}
				}
			}
		}*/
		int tag;
		if (i <= 45 || (i > 100 && i < 183)) {
			tag = 0;
		} else if(i <= 55 || i == 99 || i > 182) {
			tag = 1;
		} else {
			tag = 2;
		}
		out << tag << '\t';
		out << i << '\t';
		for (int j = 0; j < dimsquare; j++) {
			out << res[j] << '\t';
		}
		//out << res;
		out << endl;
		cout << "done" << endl;
	}
	out.close();
}