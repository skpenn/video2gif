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

int main(int argc, char * argv[]) {
	CvCapture* cap = readVideo("C:\\kankan\\test4.mkv");
	if (!cap)return 0;
	//IplImage* img = readImg("../gaojijian.jpg");
	gif* g = new gif(cap);
	g->init();
	g->saveFile("test4.gif");

	return 0;
}