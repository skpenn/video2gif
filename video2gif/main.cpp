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
	CvCapture* cap = readVideo("C:\\kankan\\Zootopia.2016.720p.BluRay.x264-SPARKS.mkv");
	if (!cap)return 0;
	cvSetCaptureProperty(cap, CV_CAP_PROP_POS_MSEC, 40 * 60 * 1000);
	IplImage* img = cvQueryFrame(cap);
	IplImage* buf = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);
	Octree oct(img);
	//oct.test();
	oct.qunatization(img, buf);

	cvNamedWindow("result");
	cvShowImage("result", img);
	cvNamedWindow("result2");
	cvShowImage("result2", buf);
	cvWaitKey(0);
	cvDestroyAllWindows();

	return 0;
}