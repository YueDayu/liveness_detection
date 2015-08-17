#include <opencv2/opencv.hpp>
#include <fstream>

using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {
	int dims = 5;
	int dimsquare = 25;
	string input_no(argv[1]);
	string input_light(argv[2]);
	string output_file(argv[3]);
	ofstream out(output_file);
		cout << "processing " << '\t';
		Mat origin_light = imread(input_light, 1);
		if (origin_light.data == NULL) {
			cout << "no data" << endl;
			return 0;
		}
		blur(origin_light, origin_light, Size(dims, dims));
		Mat origin_no = imread(input_no, 1);
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

		double *res = new double[dimsquare];

		for (int j = 0; j < dimsquare; j++) {
			calcHist( &light_part[j], 1, channels, Mat(), hist1, 1, histSize, ranges, true, false );
			normalize( hist1, hist1, 0, 1, NORM_MINMAX, -1, Mat());

			calcHist( &no_part[j], 1, channels, Mat(), hist2, 1, histSize, ranges, true, false );
			normalize( hist2, hist2, 0, 1, NORM_MINMAX, -1, Mat());

			res[j] = compareHist(hist1, hist2, CV_COMP_BHATTACHARYYA);
		}
		int tag = 0;
		out << tag << '\t';
		out << 1 << '\t';
		for (int j = 0; j < dimsquare; j++) {
			out << res[j] << '\t';
		}
		out << endl;
		cout << "done" << endl;
	out.close();
}