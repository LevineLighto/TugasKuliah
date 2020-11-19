#include "Camera.hpp"

Camera::Camera(int cameraIndex){
	this->cap.open(cameraIndex);
	if (!this->cap.isOpened()) {
		cerr << "ERROR::Kamera tidak dapat dibuka" << endl;
		this->~Camera();
		return;
	}
}

Camera::~Camera(){
	this->cap.release();
}

bool Camera::capture(OutputArray dst){
	bool success = false;

	while (true) {
		Mat frame;
		this->cap >> frame;
		if (frame.empty()) {
			cerr << "ERROR::Kamera tidak menangkap gambar" << endl;
			break;
			success = false;
		}
		Mat display(frame.rows + 100, frame.cols, CV_8UC3, Scalar(245, 245, 245));
		frame.copyTo(display(Rect(0, 0, frame.cols, frame.rows)));
		putText(display, "Tekan [S] untuk menangkap gambar", Point(10, display.rows - 50), 
			FONT_HERSHEY_SIMPLEX, 1.0, Scalar(125, 125, 125));
		imshow("CAMERA", display);

		char click = waitKey(5);

		if(click == 's'){
			frame.copyTo(this->foto);
			success = true;
			destroyWindow("CAMERA");
			break;
		} else if (click == 27) {
			success = false;
			cout << "Pengambilan gambar dibatalkan" << endl;
			destroyWindow("CAMERA");
			break;
		}
	}
	if (success) this->foto.copyTo(dst);
	else cout << "Tidak terjadi pengambilan gambar" << endl;
	this->cap.release();

	return success;
}
