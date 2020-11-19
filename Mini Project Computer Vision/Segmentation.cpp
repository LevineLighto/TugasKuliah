#include "Segmentation.hpp"


Segmentation::Segmentation(){
	this->get_win_name = "Hapus Background";
	this->set_win_name = "Atur Nilai HSV";
}

Segmentation::~Segmentation(){
	destroyWindow(this->get_win_name);
	destroyWindow(this->set_win_name);
}

void Segmentation::createWin(int id){
	if(id == 0)
		namedWindow(this->get_win_name);
	else if (id == 1)
		namedWindow(this->set_win_name);
}

void Segmentation::destroyWin(int id) {
	if (id == 0)
		destroyWindow(this->get_win_name);
	else if (id == 1)
		destroyWindow(this->set_win_name);
}

string Segmentation::winName(int id){
	if (id == 0) return this->get_win_name;
	else if (id == 1) return this->set_win_name;
}


void Segmentation::getBGR(OutputArray dst){
	this->buffer.copyTo(dst);
	destroyWindow(this->get_win_name);
}

void Segmentation::updateWindow(Vec3b hsv, Scalar bgr, int x, int y){
	Mat display;
	bool left = false;
	this->bgr.copyTo(display);
	if (x > display.cols / 2) left = true;
	string contentH = "H: " + to_string((int)hsv.val[0]),
		contentS = "S: " + to_string((int)hsv.val[1]),
		contentV = "V: " + to_string((int)hsv.val[2]);
	if (left) {
		rectangle(display, Rect(0, display.rows - 75, 200, 75), Scalar(255, 255, 255), FILLED);
		rectangle(display, Rect(10, display.rows - 65, 55, 55), Scalar(0, 0, 0), FILLED);
		rectangle(display, Rect(12, display.rows - 63, 51, 51), bgr, FILLED);
		putText(display, contentH, Point(85, display.rows - 50), FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(125, 125, 125));
		putText(display, contentS, Point(85, display.rows - 30), FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(125, 125, 125));
		putText(display, contentV, Point(85, display.rows - 10), FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(125, 125, 125));
	}
	else {
		rectangle(display, Rect(display.cols - 200, display.rows - 75, 200, 75), Scalar(255, 255, 255), FILLED);
		rectangle(display, Rect(display.cols - 190, display.rows - 65, 55, 55), Scalar(0, 0, 0), FILLED);
		rectangle(display, Rect(display.cols - 188, display.rows - 63, 51, 51), bgr, FILLED);
		putText(display, contentH, Point(display.cols - 115, display.rows - 50), FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(125, 125, 125));
		putText(display, contentS, Point(display.cols - 115, display.rows - 30), FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(125, 125, 125));
		putText(display, contentV, Point(display.cols - 115, display.rows - 10), FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(125, 125, 125));
	}

	imshow(this->get_win_name, display);
}

void Segmentation::updateData(Vec3b hsv){
	this->hsv_value[0] = hsv.val[0];
	this->hsv_value[1] = hsv.val[1];
	this->hsv_value[2] = hsv.val[2];
}

void Segmentation::storeIMG(InputArray hsv, InputArray bgr){
	hsv.copyTo(this->hsv);
	bgr.copyTo(this->bgr);
}

void Segmentation::preview(int h, int s, int v){
	int h_min, h_max, s_min, s_max, v_min, v_max;
	Mat buff;
	h_min = (this->hsv_value[0] == 0) ? 0 : (this->hsv_value[0] - h >= 0) ? this->hsv_value[0] - h : 0;
	s_min = (this->hsv_value[1] == 0) ? 0 : (this->hsv_value[1] - s >= 0) ? this->hsv_value[1] - s : 0;
	v_min = (this->hsv_value[2] == 0) ? 0 : (this->hsv_value[2] - v >= 0) ? this->hsv_value[2] - v : 0;

	h_max = (this->hsv_value[0] == 255) ? 255 : (this->hsv_value[0] + h <= 255) ? this->hsv_value[0] + h : 255;
	s_max = (this->hsv_value[1] == 255) ? 255 : (this->hsv_value[1] + s <= 255) ? this->hsv_value[1] + s : 255;
	v_max = (this->hsv_value[2] == 255) ? 255 : (this->hsv_value[2] + v <= 255) ? this->hsv_value[2] + v : 255;

	cout << "H: " << h_min << ", " << h_max << endl;
	cout << "S: " << s_min << ", " << s_max << endl;
	cout << "V: " << v_min << ", " << v_max << endl;

	inRange(this->hsv,
		Scalar(h_min, s_min, v_min),
		Scalar(h_max, s_max, v_max),
		this->mask);
	bitwise_not(this->mask, this->inverted_mask);
	bgr.copyTo(buff, this->inverted_mask);
	imshow(this->set_win_name, buff);
	buff.copyTo(this->buffer);
}
