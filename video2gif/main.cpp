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
	if (argc <= 1) {
		cout << "USAGE: video2gif filename  " << endl;
		return 0;
	}
	CvCapture* cap = readVideo(argv[1]);
	if (!cap)return 0;
	//IplImage* img = readImg("../gaojijian.jpg");
	gif* g = new gif(cap);
	g->Resize(1280 * 0.3, 536 * 0.3);
	g->init();
	g->saveFile("a.gif");

	return 0;
}