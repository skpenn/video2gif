#pragma once

class Color {
public:
	uchar v0;
	uchar v1;
	uchar v2;

	Color();
	Color(const Color& c);
	Color(const uchar& v0, const uchar&  v1, const uchar&  v2);
};

class Node {
	void init();
public:
	int level;
	
	//color
	Color c;

	int freq;
	int count;

	int index;

	Node* children[8];

	Node();
	Node(const int& level);
	Node(const int& level, const uchar& v0, const uchar& v1, const uchar& v2);
	Node(const int& level, const Color& c);

	friend bool less_than(Node* a, Node* b);
	friend bool great_than(Node* a, Node* b);
};

class Octree {
	IplImage* InitImg;
	static const uchar mask[8];
	static const uchar mask2[8];
	list<Node*>* Levels = new list<Node*>[9];
	int leaflevel = 8;
	Node* insert(Node* pNode, const Color& c);
	void sum(Node* p, int& s0, int& s1, int& s2);
public:
	Octree();
	Octree(IplImage* InitImg);
	void init(IplImage* InitImg);
	void reduce();
	void addIndex();
	Node* find(const Color& c);
	void qunatization(IplImage* src, IplImage* dst);
	void qunatizationIndex(IplImage* src, IplImage* dst);
	list<Node*>* getTable();
	void test();

	~Octree();
};