#include "StdAfx.h"

Color::Color() {};
Color::Color(const uchar& v0, const uchar&  v1, const uchar&  v2) : v0(v0), v1(v1), v2(v2) {};
Color::Color(const Color& c) {
	v0 = c.v0;
	v1 = c.v1;
	v2 = c.v2;
}

void Node::init() {
	freq = 0;
	count = 0;
	for (int i = 0; i < 8; i++) {
		children[i] = NULL;
	}
}

Node::Node() { init(); }
Node::Node(const int& level) :level(level) { init();}
Node::Node(const int& level, const uchar& v0, const uchar& v1, const uchar& v2) : level(level), c(Color(v0, v1, v2)) { init(); }
Node::Node(const int& level, const Color& c) : level(level), c(c) { init(); }

bool less_than(Node* a, Node* b) {
	if (a->count <= 0)return true;
	if (b->count <= 0)return false;
	return float(a->freq) / (a->count) < float(b->freq) / (b->count);
}


const uchar Octree::mask[8] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };

Octree::Octree() {};
Octree::Octree(IplImage* img) {
	init(img);
	reduce();
}

//Insert a color to node
Node* Octree::insert(Node* pNode, const Color& c) {
	Node* t;
	int lev = pNode->level;
	if (lev < 8) {
		pNode->freq++;
		int index = ((c.v2&mask[lev]) >> (7 - lev) << 2) + ((c.v1&mask[lev]) >> (7 - lev) << 1) + ((c.v2&mask[lev]) >> (7 - lev));
		if (pNode->children[index] == NULL) {
			t = new Node(lev + 1, c);
			Levels[lev + 1].push_back(t);
			pNode->children[index] = t;
			pNode->count++;
		}
		else {
			t = pNode->children[index];
		}
		return t;
	}
	else {
		pNode->freq++;
		return NULL;
	}
}

//Sum to get average color
void Octree::sum(Node* p, int& s0, int& s1, int& s2) {
	if (p->level == 8) {
		s0 += p->c.v0*p->freq;
		s1 += p->c.v1*p->freq;
		s2 += p->c.v2*p->freq;
	}
	else {
		for (int i = 0; i < 8;i++) {
			if (p->children[i] != NULL) {
				sum(p->children[i], s0, s1, s2);
				p->children[i] = NULL;
			}
		}
	}
}

//reduce node to 256
void Octree::reduce() {
	int i = 0;
	for (i = 0; i < 9 && Levels[i].size() <= 256; i++);
	if (i == 9)return;
	leaflevel = i;
	int t = i;
	if (t < 8) {
		for (list<Node*>::iterator it = Levels[t].begin(); it != Levels[t].end(); it++) {
			Node* p = *it;
			int s0 = 0, s1 = 0, s2 = 0, freq = p->freq;
			sum(p, s0, s1, s2);
			Color c(s0 / freq, s1 / freq, s2 / freq);
			p->c = c;
			p->count = 0;
		}
		for (i = t + 1; i < 9; i++) {
			for (list<Node*>::iterator it = Levels[i].begin(); it != Levels[i].end(); it++) {
				delete *it;
			}
			Levels[i].clear();
		}
	}
	Levels[t - 1].sort(less_than);
	for (list<Node*>::iterator it = Levels[t - 1].begin(); it != Levels[t - 1].end() && Levels[t].size() > 256; it++) {
		Node* p = *it;
		if (p->count <= 1)continue;
		int s0 = 0, s1 = 0, s2 = 0;
		for (int i = 0; i < 8; i++) {
			if (p->children[i] != NULL) {
				s0 += p->children[i]->c.v0*p->children[i]->freq;
				s1 += p->children[i]->c.v1*p->children[i]->freq;
				s2 += p->children[i]->c.v2*p->children[i]->freq;
				Levels[t].remove(p->children[i]);
				delete p->children[i];
				p->children[i] = NULL;
			}
		}
		Color c(s0 / p->freq, s1 / p->freq, s2 / p->freq);
		p->count = 1;
		insert(p, c);
	}
}

//Generate Octree from a image
void Octree::init(IplImage* img) {
	int lev = 0;
	InitImg = img;
	Node* root = new Node(0);
	Levels[lev].push_back(root);

	//Generate Octree
	for (int i = 0; i < img->height; i++) {
		for (int j = 0; j < img->width; j++) {
			uchar* pt = (uchar*)(img->imageData + i*img->widthStep + j*img->nChannels);
			Color c(*pt, *(pt + 1), *(pt + 2));
			Node* pnt = root;
			for (int k = 0; k < 9; k++) {
				pnt = insert(pnt, c);
			}
		}
	}
}

void Octree::qunatization(IplImage* src, IplImage* dst) {
	for (int i = 0; i < src->height; i++) {
		for (int j = 0; j < src->width; j++) {
			uchar* pt = (uchar*)(src->imageData + i*src->widthStep + j*src->nChannels);
			Color c(*pt, *(pt + 1), *(pt + 2));
			Node* p = *Levels[0].begin();
			int lev = 0;
			for (; lev < leaflevel-1; lev++) {
				int index = ((c.v2&mask[lev]) >> (7 - lev) << 2) + ((c.v1&mask[lev]) >> (7 - lev) << 1) + ((c.v2&mask[lev]) >> (7 - lev));
				p = p->children[index];
				if (p == NULL) {
					cout << "Error: at level of " << lev << endl;
					exit(2);
				}
			}
			int index = ((c.v2&mask[lev]) >> (7 - lev) << 2) + ((c.v1&mask[lev]) >> (7 - lev) << 1) + ((c.v2&mask[lev]) >> (7 - lev));
			if (p->children[index] != NULL)p = p->children[index];
			else {
				int k = 0;
				for (; k < 8 && p->children[k] == NULL; k++);
				if (k == 8) {
					cout << "Error: at final level" << endl;
					exit(2);
				}
				p = p->children[k];
			}
			pt = (uchar*)(dst->imageData + i*dst->widthStep + j*dst->nChannels);
			*pt = p->c.v0;
			*(pt + 1) = p->c.v1;
			*(pt + 2) = p->c.v2;
		}
	}
}

void Octree::test() {
	for (int i = 0; i < 9; i++) {
		cout << "level" << i << ": " << Levels[i].size() << endl;
	}

}

Octree::~Octree() {
	for (int i = 0; i < 9; i++) {
		for (list<Node*>::iterator it = Levels[i].begin(); it != Levels[i].end(); it++) {
			delete *it;
		}
	}
	delete[] Levels;
}