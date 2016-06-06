#include "StdAfx.h"

int gif::lzw_encode(IplImage* img) {
	uchar* p = charbuf, t;
	int size = 0;
	unsigned local=0, ne;
	map<int, int> tab;
	int length = IndSize + 1, label=0;
	int index = (1 << (length-1)) + 2;
	for(int i=0;i<img->height;i++)
		for (int j = 0; j < img->width; j++) {
			if ((i | j) == 0) {
				ne= (img->imageData + i*img->widthStep)[j*img->nChannels];
				continue;
			}
			t = (img->imageData + i*img->widthStep)[j*img->nChannels];
			int tmp = (ne << IndSize) | t;
			int in = tab[tmp];
			if (in != 0) {
				ne = in;
				continue;
			}
			tab[tmp] = index;
			local |= (ne << label);
			label += length;
			while (label >= 8) {
				charbuf[size++] = local & 0xff;
				local >>= 8;
				label -= 8;
			}
			index++;
			if (index >= (1 << length) ){
				if (length >= 12) {
					local |= (1<< IndSize) << label;
					label += length;
					while (label >= 8) {
						charbuf[size++] = local & 0xff;
						local >>= 8;
						label -= 8;
					}
					length = IndSize + 1;
					index = (1 << (length - 1)) + 2;
					tab.clear();
				}
				else if (index >= (1 <<length)+1) {
					length++;
				}
			}
			ne = t;
		}
		local |= (ne) << label;
		label += length;
		while (label > 0) {
			charbuf[size++] = local & 0xff;
			local >>= 8;
			label -= 8;
		}
		return size;
}

IplImage* gif::getNextFrame() {
	NowTime += RealDelay;
	while (++count < int(NowTime*CapFps)) {
		cvQueryFrame(Cap);
	}
	return cvQueryFrame(Cap);
}

gif::gif(IplImage* img) {
	InitImg = img;
}

gif::gif(CvCapture* cap) {
	InitImg = cvQueryFrame(cap);
	float Frames = cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_COUNT);
	if (Frames > 2) {
		cvSetCaptureProperty(cap, CV_CAP_PROP_POS_FRAMES, Frames-1);
		buf2 = cvQueryFrame(cap);
		if (buf2 == NULL) {
			cout << "Warning: final frame not find" << endl;
			frames = 1;
		}
		else {
			frames = 2;
		}
	}
	if (Frames > 3 && frames > 1) {
		cvSetCaptureProperty(cap, CV_CAP_PROP_POS_FRAMES, int(Frames /2));
		buf3 = cvQueryFrame(cap);
		frames = 3;
	}
	Cap = cap;
	cvSetCaptureProperty(Cap, CV_CAP_PROP_POS_FRAMES, 0);
}


void gif::init() {
	im = new Image_info();
	Graph = new Graph_ctrl();
	if (!rescale) {
		width = InitImg->width;
		height = InitImg->height;
	}
	else {
		buf = cvCreateImage(CvSize(width, height), InitImg->depth, InitImg->nChannels);
		cvResize(InitImg, buf);
		InitImg = buf;
	}
	if(frames>1){
		CapFps = cvGetCaptureProperty(Cap, CV_CAP_PROP_FPS);
		RealDelay = 4 / CapFps;
		NowTime = 0;
		Graph->delay = (unsigned short)(RealDelay * 100);
	}
	charbuf = new uchar[width*height + ((width*height) >> 1)];
	octree = new Octree();
	octree->init(InitImg);
	octree->test();
	if (!FullQuant) {
		if (frames > 1) {
			octree->init(buf2);
			octree->test();
		}
		if (frames > 2) {
			octree->init(buf3);
			octree->test();
		}
	}
	octree->reduce();
	octree->test();
	octree->addIndex();
	GlobalTab = octree->getTable();
	if (GlobalTab->size()<256){
		cout << "Octree of color quantization generated " <<GlobalTab->size()<<" colors"<< endl;
	}
	else {
		cout << "Failed to generate an octree" << endl;
		exit(3);
	}
	buf = buf2 = buf3 = NULL;
}

void gif::saveFile(String filename) {
	fout = ofstream(filename.c_str(), ios::out | ios::binary);
	if (fout.is_open() == false) {
		cout << "Open file failed" << endl;
		exit(4);
	}
	buf = cvCreateImage(CvSize(width, height), IPL_DEPTH_8U, 1);
	octree->qunatizationIndex(InitImg, buf);
	writeHead();
	writeColorTab();
	im->top = im->left = 0;
	im->width = width;
	im->height = height;
	writeGraphCtrl();
	writeImgDesp();
	int size = lzw_encode(buf);
	tch = IndSize;
	fout.write((char*)(&tch), 1);
	writeDate(size);
	if (frames > 1) {
		buf2 = cvCreateImage(CvSize(width, height), IPL_DEPTH_8U, 1);
		while ((tmp = getNextFrame()) != NULL) {
			octree->qunatizationIndex(tmp, buf2);
			writeGraphCtrl();
			writeImgDesp();
			size = lzw_encode(buf2);
			tch = IndSize;
			fout.write((char*)(&tch), 1);
			writeDate(size);
		}
	}
	tch = GIF_TRAILER;
	fout.write((char*)(&tch), 1);
	fout.close();
}

void gif::writeHead() {
	fout.write("GIF89a", 6);
	fout.write((char*)&width, 2);
	fout.write((char*)&height, 2);
	tch = 0x88 | (0x70 & ((IndSize - 1) << 4)) | (0x07 & (IndSize - 1));
	fout.write((char*)&tch, 1);
	tch = 0xff;
	fout.write((char*) & tch, 1);
	if (width / height < 4 && height / width < 4) {
		tch = (unsigned char)(((unsigned)width << 6) / height) - 15;
	}
	else {
		tch = 0;
	}
	fout.write((char*)&tch, 1);
}

void gif::writeColorTab() {
	int i = 0;
	for (list<Node*>::iterator it = GlobalTab->begin(); it != GlobalTab->end(); it++, i++) {
		fout.write((char*)(&(*it)->c.v2), 1);
		fout.write((char*)(&(*it)->c.v1), 1);
		fout.write((char*)(&(*it)->c.v0), 1);
	}
	tch = 0xff;
	while (i < 256) {
		fout.write((char*)(&tch), 1);
		fout.write((char*)(&tch), 1);
		fout.write((char*)(&tch), 1);
		i++;
	}
}

void gif::writeImgDesp() {
	tch = GIF_IMG_DSCPT;
	fout.write((char*)&tch, 1);
	fout.write((char*)&im->left, 2);
	fout.write((char*)&im->top, 2);
	fout.write((char*)&im->width, 2);
	fout.write((char*)&im->height, 2);
	tch = 0x0;
	fout.write((char*)&tch, 1);
}

void gif::writeDate(int size) {
	int ind = 0;
	while (size-ind > 255) {
		tch = 0xff;
		fout.write((char*)&tch, 1);
		fout.write((char*)(charbuf+ind), 255);
		ind += 255;
	}
	tch = size - ind;
	fout.write((char*)&tch, 1);
	fout.write((char*)(charbuf + ind), tch);
	tch = 0;
	fout.write((char*)&tch, 1);
}

void gif::writeGraphCtrl() {
	tch = GIF_EXT_BLK;
	fout.write((char*)&tch, 1);
	tch = GIF_GRAPH_CTRL_EXT;
	fout.write((char*)&tch, 1);
	tch = 0x04;
	fout.write((char*)&tch, 1);
	tch = (DSP_NO_ACT << 2) | 1;
	fout.write((char*)&tch, 1);
	fout.write((char*)&Graph->delay, 2);
	fout.write((char*)&Graph->transp_index, 1);
	tch = BLK_TERM;
	fout.write((char*)&tch, 1);
}

gif::~gif() {
	if(buf != NULL)cvReleaseImage(&buf);
	if (buf2 != NULL)cvReleaseImage(&buf2);
	if (buf3 != NULL)cvReleaseImage(&buf3);

	delete im;
	delete Graph;
	delete[] charbuf;
	delete octree;
}
