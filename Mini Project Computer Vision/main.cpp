#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

#include "Camera.hpp"
#include "GSColoring.hpp"
#include "Transform.hpp"
#include "Segmentation.hpp"

using namespace std;
using namespace cv;

Mat img, hsv, cropped, stretched, gray, claheRes, dst;
Segmentation crop;
int h = 8, s = 8, v = 8;

static void eventMouse(int event, int x, int y, int, void*) {
	if (event == EVENT_MOUSEMOVE) {
		Vec3b hsvPixel = hsv.at<Vec3b>(y, x);
		Vec3b bgrPixel = img.at<Vec3b>(y, x);
		crop.updateWindow(hsvPixel, Scalar(bgrPixel.val[0], bgrPixel.val[1], bgrPixel.val[2]), x, y);
	}
	if (event == EVENT_LBUTTONDOWN) {
		Vec3b pixel = hsv.at<Vec3b>(y, x);
		crop.updateData(pixel);
	}
}

static void trackbar(int, void*) {
	crop.preview(h, s, v);
}


int main() {
ambilCam: 
	/*
	Camera* cam = new Camera(0);
	if (!cam->capture(img)) {
		return 0;
	}
	delete cam;
	// */
	 img = imread("gambar.png");

ambilColor: 
	cvtColor(img, hsv, COLOR_BGR2HSV);
	crop.storeIMG(hsv, img);
	crop.createWin(0);
	setMouseCallback(crop.winName(0), eventMouse);
	crop.updateWindow(Vec3b(0, 0, 0), Scalar(0, 0, 0), 0, 0);

	char c = waitKey();
	if (c == 27) {
		crop.destroyWin(0);
		goto ambilCam;
	} else if (c == 's') {
		crop.destroyWin(0);
		goto segmentasi;
	} else {
		crop.destroyWin(0);
		cerr << "Tombol lain ditekan" << endl;
		return -1;
	}

segmentasi:
	crop.createWin(1);
	createTrackbar("H", crop.winName(1), &h, 255, trackbar);
	createTrackbar("S ", crop.winName(1), &s, 255, trackbar);
	createTrackbar("V", crop.winName(1), &v, 255, trackbar);
	crop.preview(h, s, v);
	c = waitKey();
	if (c == 27) {
		crop.destroyWin(1);
		goto ambilColor;
	} else if (c == 's') {
		crop.destroyWin(1);
		crop.getBGR(cropped);
		goto stretch;
	}

stretch:
	Transform squarize;
	squarize.apply(cropped, stretched);

enhance:
	cvtColor(stretched, gray, COLOR_BGR2GRAY);
	Ptr<CLAHE> clahe = createCLAHE(4, Size(2, 2));
	clahe->apply(gray, claheRes);

mewarnai:
	cvtColor(claheRes, claheRes, COLOR_GRAY2BGR);
	GSColoring colorize;
	colorize.apply(claheRes, dst);

	imshow("Hasil", dst);
	waitKey();
	return 0;
}