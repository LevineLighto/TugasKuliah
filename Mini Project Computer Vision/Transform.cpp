#include "Transform.hpp"

Transform::Transform(){
	this->largest_contour_index = 0;
	this->largest_area = 0;
}

Transform::~Transform(){

}

void Transform::apply(InputArray src, OutputArray dst){
	Mat img, thr, out, cntr;
	src.copyTo(img);
	this->getEdges(img, thr);
	this->createContours(thr);
	this->transform(img, out);
	out.copyTo(dst);
}

void Transform::createContours(InputArray src){
	Mat source;
	src.copyTo(source);
	findContours(source.clone(), this->contours, this->hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < this->contours.size(); i++) {
		double a = contourArea(this->contours[i], false);
		if (a > this->largest_area) {
			this->largest_area = a;
			this->largest_contour_index = i;
		}
	}
}

void Transform::getEdges(InputArray src, OutputArray dst){
	Mat source, dest;
	src.copyTo(source);
	cvtColor(source, dest, COLOR_BGR2GRAY);
	threshold(dest, dest, 70, 255, THRESH_BINARY);
	dest.copyTo(dst);
}

void Transform::transform(InputArray src, OutputArray dst){
	Mat source;
	src.copyTo(source);
	vector<vector<Point>> contours_poly(1);
	approxPolyDP(Mat(this->contours[this->largest_contour_index]), contours_poly[0], 5, true);
	Rect boundRect = boundingRect(this->contours[this->largest_contour_index]);

	if (contours_poly[0].size() == 4) {
		vector<Point2f> quad_pts, squre_pts;
		quad_pts.push_back(Point2f(contours_poly[0][0].x, contours_poly[0][0].y));
		quad_pts.push_back(Point2f(contours_poly[0][1].x, contours_poly[0][1].y));
		quad_pts.push_back(Point2f(contours_poly[0][3].x, contours_poly[0][3].y));
		quad_pts.push_back(Point2f(contours_poly[0][2].x, contours_poly[0][2].y));
		squre_pts.push_back(Point2f(boundRect.x, boundRect.y));
		squre_pts.push_back(Point2f(boundRect.x, boundRect.y + boundRect.height));
		squre_pts.push_back(Point2f(boundRect.x + boundRect.width, boundRect.y));
		squre_pts.push_back(Point2f(boundRect.x + boundRect.width, boundRect.y + boundRect.height));

		Mat transmtx = getPerspectiveTransform(quad_pts, squre_pts);
		Mat transformed = Mat::zeros(source.rows, source.cols, CV_8UC3);
		warpPerspective(source, transformed, transmtx, source.size());
		cout << "AA" << endl;
		transformed.copyTo(dst);
	} else {
		cerr << "Persegi tidak ditemukan" << endl;
	}
}
