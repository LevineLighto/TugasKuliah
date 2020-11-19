#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <sstream>

using namespace std;
using namespace cv;

class Segmentation{
public:
	Segmentation();
	~Segmentation();

	void createWin(int);
	string winName(int);
	void getBGR(OutputArray);
	void updateWindow(Vec3b, Scalar, int, int);
	void updateData(Vec3b);
	void storeIMG(InputArray, InputArray);
	void preview(int, int, int);
	void destroyWin(int);

private:
	string get_win_name, set_win_name;
	Mat hsv, bgr, mask, inverted_mask, buffer;
	int hsv_value[3];
};