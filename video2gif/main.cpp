#include "StdAfx.h"

using namespace std;
using namespace cv;

CvCapture* readVideo(String filename) {
	CvCapture* capture = cvCreateFileCapture(filename.c_str());
	if (!capture) {
		cout << "Failed to open video " << filename << endl;
	}
	return capture;
}

int main(int argc, char * argv[]) {

}