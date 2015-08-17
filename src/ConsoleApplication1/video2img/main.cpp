#include<opencv2/opencv.hpp>
#include<iostream>
#include <cstring>
#include<fstream>
using namespace cv;
using namespace std;

const int frameRate = 25;


void savePhoto(const vector<int> &sums, const vector<Mat> &mats, int &begin, int &end);

int main(int argc, char* args[])
{
    char cmd[100];

    char* inName = args[1];
    char* outputUnLight = args[2];
    char* outputLight = args[3];

    char outvideoName[100];
    strcpy(outvideoName, "output/");
    strcat(outvideoName, inName);
    strcat(outvideoName, ".avi");
    sprintf(cmd, "ffmpeg -i %s %s", inName, outvideoName);

    cout << cmd << endl;

	system(cmd);

    vector<int> sumVec;
    vector<Mat> originMat;

    VideoCapture cap(outvideoName);
    int frameIndex = 0;
    int thisSum = 0;
    int lastSum = 0;
    while (true)
    {
        Mat aFrame;
        if (!cap.read(aFrame))
        {
            break;
        }
        originMat.push_back(aFrame);
        Mat frameSmall;
        Mat targetGrey;


        resize(aFrame, frameSmall, Size(10, 10), 0, 0, CV_INTER_LINEAR);
        cvtColor(frameSmall, targetGrey, CV_BGR2GRAY);
        int sum = 0;
        for (int i = 0; i < targetGrey.rows; ++i)
        {
            for (int j = 0; j < targetGrey.cols; ++j)
            {
                sum += targetGrey.at<uchar>(i, j);
            }
        }
        sum = sum / (targetGrey.rows * targetGrey.cols);
        sumVec.push_back(sum);
    }

    int beg;
    int end;

    savePhoto(sumVec, originMat, beg, end);
    //system("pause");
    cout << outputUnLight << "  " << outputLight << endl;
    imwrite(outputUnLight, originMat[beg]);
    imwrite(outputLight, originMat[end]);
    cap.release();

}


/*
int main(int argc, char* args[])
{
    char cmd[100];
    
    char* inName = args[1];
    char* outputUnLight = args[2];
    char* outputLight = args[3];
    
    ifstream nameFile;
    nameFile.open("example/false/name.txt");

    while (true)
    {
        string name = "";
        nameFile >> name;
        if (name =="")
        {
            break;
        }
        string completeName = "example/false/" + name;
        const char  *cname = completeName.c_str();
        sprintf(cmd, "ffmpeg -i %s output/%s.avi", cname, name.c_str());
        system(cmd);

        vector<int> sumVec;
        vector<Mat> originMat;

        VideoCapture cap(completeName);
        int frameIndex = 0;
        int thisSum = 0;
        int lastSum = 0;
        while (true)
        {
            Mat aFrame;
            if (!cap.read(aFrame))
            {
                break;
            }
            originMat.push_back(aFrame);
            Mat frameSmall;
            Mat targetGrey;


            resize(aFrame, frameSmall, Size(10, 10), 0, 0, CV_INTER_LINEAR);
            cvtColor(frameSmall, targetGrey, CV_BGR2GRAY);
            int sum = 0;
            for (int i = 0; i < targetGrey.rows; ++i)
            {
                for (int j = 0; j < targetGrey.cols; ++j)
                {
                    sum += targetGrey.at<uchar>(i, j);
                }
            }
            sum = sum / (targetGrey.rows * targetGrey.cols);
            sumVec.push_back(sum);
        }


        savePhoto(sumVec, originMat, "false/" + name);
        //system("pause");
        cap.release();
        
    }
}*/

void savePhoto(const vector<int> &sums, const vector<Mat> &mats, int &begin, int &end)
{
    int maxIndex = 0;
    int maxIncrease = 0;
    for (unsigned i = 2; i < sums.size(); ++i)
    {
        if (sums[i] - sums[i - 1] > maxIncrease)
        {
            maxIndex = i;
            maxIncrease = sums[i] - sums[i - 1];
        }
    }

    int beginFrame= 0;
    int endFrame = 0;
    //
    int lastIncrease = sums[maxIndex - 1] - sums[maxIndex - 2];
    int nextIncrease = sums[maxIndex + 1] - sums[maxIndex];


    if (lastIncrease > 9 || abs(lastIncrease - maxIncrease) < 4 && lastIncrease > 0)
    {
        beginFrame = maxIndex - 2;
        endFrame = maxIndex;
    }

    else if (nextIncrease > 9 || abs(nextIncrease - maxIncrease) < 4 && nextIncrease > 0)
    {
        beginFrame = maxIndex - 1;
        endFrame = maxIndex + 1;
    }

    else
    {
        beginFrame = maxIndex - 1;
        endFrame = maxIndex;
    }
    if (beginFrame < 0)
    {
        beginFrame = 0;
    }
    if (endFrame > mats.size() - 1)
    {
        endFrame = mats.size() - 1;
    }

    begin = beginFrame;
    end = endFrame;
} 