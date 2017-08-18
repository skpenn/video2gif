#include "StdAfx.h"


CvCapture* readVideo(String filename) {
	CvCapture* capture = cvCreateFileCapture(filename.c_str());
	if (!capture) {
		cout << "Error:Failed to open video " << filename << endl;
	}
	return capture;
}

IplImage* readImg(String filename) {
	IplImage* img = cvLoadImage(filename.c_str());
	if (!img) {
		cout << "Error:Failed to open image " << filename << endl;
	}
	return img;
}

float timeFormatter(String time) {
	float seconds = 0;
	int minutes = 0, hours = 0;
	size_t index = time.find_last_of(':');
	if (index == std::string::npos){
		seconds =  atof(time.c_str());
		if (seconds == 0 && time[0] != '0') return -1;
	}
	else{
		String strSeconds = time.substr(index + 1); 
		seconds = atof(strSeconds.c_str());
		if (seconds == 0 && strSeconds[0] != '0') return -1;
		time = time.substr(0, index);
		index = time.find_last_of(':');
		if (index == std::string::npos){
			minutes = atoi(time.c_str());
			if (minutes == 0 && time[0] != '0') return -1;
		}
		else{
			String strMinutes = time.substr(index + 1);
			minutes = atoi(strMinutes.c_str());
			if (minutes == 0 && strMinutes[0] != '0') return -1;
			time = time.substr(0, index);
			hours = atoi(time.c_str());
			if (hours == 0 && time[0] != '0') return -1;
		}
	}
	return ((hours * 60) + minutes) * 60 + seconds;

}

int main(int argc, char * argv[]) {
	String inputfile = "", outputfile = "", filetype="";
	float speed = 1, starttime=0, endtime=0, resize=0, fps=0;
	bool Local = false, keyfram=false;
	int count = 0;
	if (argc <= 1) {
		cout << "USAGE: video2gif filename  [-options] [param]                            " << endl;
		cout << "    This is a program to convert video or image to Graphics Interchanged " << endl;
		cout << "Format(GIF) image. Supported video format include `.mp4`, `.avi`, `.rm`, " << endl;
		cout << "`.rmvb`, `.mkv`(partial). Supported image format include `.jpg`, `.bmp`. " << endl;
		cout << "Options:"                                                                  << endl;
		cout << "-o [Filename]     Output filename. If not given, original input file name" << endl;
		cout << "                  will be used as output filename                        " << endl;
		cout << "-r [Size Ratio]   Rescale the image size. Only shrink is supported so the" << endl;
		cout << "                  given size ratio should be less than 1                 " << endl;
		cout << "-f [FPS]          Setting FPS. Given FPS should not be great than the    " << endl;
		cout << "                  original one                                           " << endl;
		cout << "-s [Speed Ratio]  Change playing speed. Can speed up or slow down your   " << endl;
		cout << "                  output gif. Notice that this will change your FPS set- " << endl;
		cout << "                  ting, the real FPS is Speed Ratio multiple set FPS     " << endl;
		cout << "-c [start-end]    Give a cut range and convert only the cut range to GIF." << endl;
		cout << "                  you should provide 2 time points with their units  se- " << endl;
		cout << "                  cond, connected by symbol `-`. Time point can have de- " << endl;
		cout << "                  cimals. For example, `30.1-32.5`                       " << endl;
		cout << "-l                use local color table instead of global color table,   " << endl;
		cout << "                  this will cause your GIF file larger                   " << endl;
		cout << "-k                use firest frame as key frame, all other frame will ba-" << endl;
		cout << "                  se on this frame. This will reduce the size of your    " << endl;
		cout << "                  image under some special condition.                    " << endl;
		return 0;
	}
	else {
		while (++count < argc) {
			String str = argv[count];
			if (str[0] == '-') {
				if (str == "-o") {
					count++;
					if (count >= argc || argv[count][0]=='-') {
						cout << "Error:Illegal parameter of -o" << endl;
						return 0;
					}
					else {
						outputfile = argv[count];
					}
				}
				else if (str == "-r") {
					count++;
					if (count >= argc || argv[count][0] == '-') {
						cout << "Error:Illegal parameter of -r" << endl;
						return 0;
					}
					else {
						resize = atof(argv[count]);
						if (resize == 0) {
							cout << "Error:Illegal parameter of -r" << endl;
							return 0;
						}
					}
				}
				else if (str == "-f") {
					count++;
					if (count >= argc || argv[count][0] == '-') {
						cout << "Error:Illegal parameter of -f" << endl;
						return 0;
					}
					else {
						fps = atof(argv[count]);
						if (fps == 0) {
							cout << "Error:Illegal parameter of -f" << endl;
							return 0;
						}
					}
				}
				else if (str == "-s") {
					count++;
					if (count >= argc || argv[count][0] == '-') {
						cout << "Error:Illegal parameter of -s" << endl;
						return 0;
					}
					else {
						speed = atof(argv[count]);
						if (speed == 0) {
							cout << "Error:Illegal parameter of -s" << endl;
							return 0;
						}
					}
				}
				else if (str == "-c") {
					count++;
					if (count >= argc || argv[count][0] == '-') {
						cout << "Error:Illegal parameter of -c" << endl;
						return 0;
					}
					else {
						String tmp = argv[count];
						int t = tmp.find('-');
						if (t == -1) {
							cout << "Error:Illegal parameter of -c" << endl;
							return 0;
						}
						String tmp1 = tmp.substr(0, t);
						String tmp2 = tmp.substr(t + 1);
						starttime = timeFormatter(tmp1);
						if (starttime == -1) {
							cout << "Error:Illegal parameter of -c" << endl;
							return 0;
						}
						endtime = timeFormatter(tmp2);
						if (endtime == -1) {
							cout << "Error:Illegal parameter of -c" << endl;
							return 0;
						}
						if (starttime >= endtime) {
							cout << "Error:Illegal parameter of -c" << endl;
							return 0;
						}
					}
				
				}
				else if (str == "-l") {
					Local = true;
				}
				else if (str == "-k") {
					keyfram = true;
				}
				else {
					cout << "Error:Unknown command " << str << endl;
					return 0;
				}
			}
			else {
				if (inputfile != "") {
					cout << "Error:Invalid parameter " << str << endl;
					return 0;
				}
				else {
					inputfile = str;
				}
			}
		}
	}
	if (inputfile == "") {
		cout << "Error:Please give an input file" << endl;
		return 0;
	}
	int t = inputfile.rfind('.');
	if (t == -1) {
		cout << "Error:Unknown input file format" << endl;
		return 0;
	}
	filetype = inputfile.substr(t);
	if (outputfile == "") {
		outputfile = inputfile.substr(0, t) + ".gif";
	}
	if (filetype == ".mp4" || filetype == ".avi" || filetype == ".rm" || filetype == ".rmvb" || filetype == ".mkv") {

		CvCapture* cap = readVideo(inputfile);
		if (!cap)return 0;
		float width = cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_WIDTH);
		float height = cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_HEIGHT);
		gif* g = new gif(cap);
		if (resize != 0 && resize < 1) {
			g->Resize(width * resize, height * resize);
		}
		if (fps != 0) {
			g->setFps(fps);
		}
		if (endtime != 0) {
			g->split(starttime, endtime);
		}
		g->Local = Local;
		g->IsKeyFrame = keyfram;
		g->speed = speed;
		g->init();
		g->saveFile(outputfile);

	}
	else if (filetype == ".jpg" || filetype == ".bmp") {
		IplImage* img = readImg(inputfile);
		if (!img)return 0;
		float width = img->width;
		float height = img->height;
		gif* g = new gif(img);
		if (resize != 0 && resize < 1) {
			g->Resize(width * resize, height * resize);
		}
		g->init();
		g->saveFile(outputfile);
	}
	else {
		cout << "Error:Unsupported file format" << endl;
	}

	return 0;
}