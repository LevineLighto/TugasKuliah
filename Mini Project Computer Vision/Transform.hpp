#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

class Transform{
public:
	Transform();
	~Transform();
	void apply(InputArray, OutputArray);
	void createContours(InputArray);
	void getEdges(InputArray, OutputArray);
	void transform(InputArray, OutputArray);

private:
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	int largest_contour_index;
	int largest_area;

	Mat buffer;
};

