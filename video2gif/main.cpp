#include "StdAfx.h"


CvCapture* readVideo(String filename) {
	CvCapture* capture = cvCreateFileCapture(filename.c_str());
	if (!capture) {
		cout << "Failed to open video " << filename << endl;
	}
	return capture;
}

IplImage* readImg(String filename) {
	IplImage* img = cvLoadImage(filename.c_str());
	if (!img) {
		cout << "Failed to open image " << filename << endl;
	}
	return img;
}

int main(int argc, char * argv[]) {/*
	CvCapture* cap = readVideo("C:\\kankan\\Zootopia.2016.720p.BluRay.x264-SPARKS.mkv");
	if (!cap)return 0;
	cvSetCaptureProperty(cap, CV_CAP_PROP_POS_MSEC, 40 * 60 * 1000);*/
	IplImage* img = readImg("../gaojijian.jpg");
	gif* g = new gif(img);
	g->init();
	g->saveFile("test2.gif");

	return 0;
}