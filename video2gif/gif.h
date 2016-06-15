#pragma once

#define ENABLE 1
#define DISABLE 0

//version
#define GIF_87A 0
#define GIF_89A 1

//block info define
#define GIF_IMG_DSCPT 0x2c
#define GIF_EXT_BLK 0x21
#define GIF_GRAPH_CTRL_EXT 0xf9
#define GIF_COMM_EXT 0xfe
#define GIF_APP_EXT 0xff
#define GIF_PLAIN_TXT_EXT 0x01
#define GIF_TRAILER 0x3b
#define BLK_TERM 0x00

//header

//graphic control extension
//disposal method
#define DSP_NO_ACT 0
#define DSP_NO_DSP 1
#define DSP_RST_BG 2
#define DSP_RST_PREV 3

#define TRANSP Color c(0xff, 0xff, 0xff);

class Image_info {
public:
	unsigned short left;
	unsigned short top;
	unsigned short width;
	unsigned short height;
	bool localcolor=false;
	bool interlace=false;
	bool sorted=false;
	uchar size=0;
};

class Graph_ctrl {
public:
	unsigned int size;
	uchar disp_meth=DSP_NO_ACT;
	bool user_input=false;
	bool transp=false;
	unsigned short delay;
	unsigned char transp_index=0xff;

};

class gif {
	ofstream fout;

	unsigned char tch;
	unsigned short tsh;
	unsigned int tin;

	float RealDelay;
	int count = 0;
	float NowTime = 0;
	float speed = 1;

	void writeHead();
	void writeColorTab();
	void writeImgDesp();
	void writeGraphCtrl();
	int lzw_encode(IplImage* img);
	void diffBuf(IplImage* old, IplImage* ne);
	void writeDate(int size);
	IplImage* getNextFrame();
	void saveLocal();

	void display(IplImage* img);
public:
	unsigned int IndSize = 8;
	bool version=GIF_89A;
	bool IsVideo=false;
	bool Local = false;
	unsigned short width;
	unsigned short height;
	bool rescale = false;
	int frames=1;
	float startframe = 0;
	bool IsKeyFrame = false;
	Octree* octree;
	bool FullQuant=false;
	float CapFps = -1;
	float outFps = -1;
	uchar* charbuf;

	IplImage* buf = NULL;
	IplImage* buf2 = NULL;
	IplImage* buf3 = NULL;
	IplImage* ResizeBuf;

	IplImage* InitImg = NULL;
	IplImage* tmp = NULL;
	list<Node*>* GlobalTab;
	CvCapture* Cap;

	Image_info* im;
	Graph_ctrl* Graph;

	gif(IplImage* img);
	gif(CvCapture* cap);
	void Resize(short width, short height);
	void setFps(float fps);
	void split(float start, float end);
	void init();
	void saveFile(String);

	~gif();
};
