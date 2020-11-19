#include "GSColoring.hpp"

GSColoring::GSColoring(){
	string protofile = "./models/colorization_deploy_v2.prototxt",
		weightsFile = "./models/colorization_release_v2.caffemodel";

	net = readNetFromCaffe(protofile, weightsFile);

	int sz[] = { 2, 313, 1, 1 };
	const Mat pts_in_hull(4, sz, CV_32F, hull_pts);
	class8_ab = net.getLayer("class8_ab");
	class8_ab->blobs.push_back(pts_in_hull);
	conv8_313_rh = net.getLayer("conv8_313_rh");
	conv8_313_rh->blobs.push_back(Mat(1, 313, CV_32F, Scalar(2.606)));


}

GSColoring::~GSColoring(){
	
}

void GSColoring::apply(InputArray src, OutputArray dst){
	Mat source;
	src.copyTo(source);

	Mat lab, L, input;
	source.convertTo(source, CV_32F, 1.0 / 255);
	cvtColor(source, lab, COLOR_BGR2Lab);
	extractChannel(lab, L, 0);
	resize(L, input, Size(W_in, H_in));
	input -= 50;

	Mat inputBlob = blobFromImage(input);
	net.setInput(inputBlob);
	Mat result = net.forward();

	Size siz(result.size[2], result.size[3]);
	Mat a = Mat(siz, CV_32F, result.ptr(0, 0));
	Mat b = Mat(siz, CV_32F, result.ptr(0, 1));
	resize(a, a, source.size());
	resize(b, b, source.size());

	Mat color, chn[] = { L, a, b };
	merge(chn, 3, lab);
	cvtColor(lab, color, COLOR_Lab2BGR);

	color = color * 255;
	color.convertTo(color, CV_8U);
	color.copyTo(dst);
}
