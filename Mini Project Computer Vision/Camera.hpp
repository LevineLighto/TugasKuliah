#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

class Camera{
public:
    Camera(int);
    ~Camera();
    bool capture(OutputArray);
private:
    VideoCapture cap;
    Mat foto;
};

