#include<opencv2/opencv.hpp>
#include<iostream>
#include<fstream>
#include<sstream>
using namespace cv;
using namespace std;

const int frameRate = 25;
Mat getForeGround(const Mat &light, const Mat &unlight);


void savePhoto(const vector<int> &sums, const vector<Mat> &mats, string fileName);
int main()
{
    
    ifstream nameFile;
    stringstream sstream;
    string preDir = "NewData/false/";
    string afrDir = "NewData/falseplus/";
    nameFile.open(preDir + "name.txt", ios::in);
    int index = 48;
    while (true)
    {
        string name = "";
        nameFile >> name;
        if (name == "")
        {
            break;
        }

        //sstream << index;
        //string indexStr;
        //sstream >> indexStr;
        Mat lightMat;
        Mat unlightMat;

        Mat foreground;
        lightMat = imread(preDir + name);
        nameFile >> name;
        unlightMat = imread(preDir + name);
        

        BackgroundSubtractorMOG mog;
        mog(unlightMat, foreground, 0);
        mog(lightMat, foreground, 0);

        //cv::imwrite("foreground/" + to_string(index) + ".jpg", getForeGround(lightMat, unlightMat));
        getForeGround(lightMat, unlightMat);
        ++index;
    }
}


Mat getForeGround(const Mat &light, const Mat &unlight)
{
    Mat foreground(light.rows, light.cols, CV_8UC1);
    Mat lightHSV, unlightHSV;
    cvtColor(light,lightHSV , CV_BGR2HSV);
    cvtColor(unlight, unlightHSV, CV_BGR2HSV);
    for (int i = 0; i < light.rows; ++i)
    {
        for (int j = 0; j < light.cols; ++j)
        {
            int grayDegree;
            Vec3b lightVec = lightHSV.at<Vec3b>(i, j);
            Vec3b unlightVec = unlightHSV.at<Vec3b>(i, j);
        
            //dist(h's'v', hsv) = || (vscos(2PI*h), vsin(2PI*h),v) - (v's'cos(2PI*h'), v's'sin(2PI*h'),v')||
            //((double)lightVec[0] / 255.0 * 3.14)
            //foreground.at<uchar>(i, j) = grayDegree;
            
            
        }
    }
    return foreground;
}

