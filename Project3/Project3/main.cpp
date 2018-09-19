#include <opencv2/opencv.hpp>
#include <iostream>
#include "pic.h"
using namespace std;
typedef unsigned char uchar;

#define X true
#define Y false

enum Mode {
	shrink = 0,
	expand = 1,
	cut = 2,
	protect = 3
};

void shrinking(Pic* p, int scale_x, int scale_y, string filename);
void expanding(Pic* p, int scale_x, int scale_y, string filename);

void seamcarving(string filename, double ratio_y, double ratio_x, Mode mode) {
	Pic* p = new Pic(0, 0, X);
	p->input(filename);
	int scale_y = (int)(p->getM()*ratio_x);
	int scale_x = (int)(p->getN()*ratio_y);
	if(mode == shrink)
		shrinking(p, scale_x, scale_y, filename);
	else if(mode== expand) 
		expanding(p, scale_x, scale_y, filename);
	else if(mode == cut) {
		int x1 = 83, x2 = 103, y1 = 147, y2 = 177;
		p->setcut(x1, y1, x2, y2);
		shrinking(p, scale_x, scale_y, filename);
	}
	else {
		int x1=121, x2=245, y1=94, y2=209;
		p->setpro(x1, y1, x2, y2);
		shrinking(p, scale_x, scale_y, filename);
	}
}

void shrinking(Pic * p, int scale_x, int scale_y, string filename) {
	cout << "shrink " << filename << endl;
	p->setdirect(X);
	for(int i = 0; i < scale_x; i++) {
		int start = p->seam();
		p->carve(start);
		p->reshape(0, -1);
		cout << "X:" << i << endl;
	}
	p->setdirect(Y);
	for(int i = 0; i < scale_y; i++) {
		int start = p->seam();
		p->carve(start);
		p->reshape(-1, 0);
		cout << "Y:" << i << endl;
	}
	p->output(filename);
}

void expanding(Pic* p, int scale_x, int scale_y, string filename) {
	cout << "expand " << filename << endl;
	p->setdirect(X);
	for(int i = 0; i < scale_x; i++) {
		int start = p->seam();
		p->carve(start);
		p->reshape(0, -1);
		cout << "X:" << i << endl;
	}
	p->expand(scale_x, scale_y, filename);
	p->recover();
	p->setdirect(Y);
	for(int i = 0; i < scale_y; i++) {
		int start = p->seam();
		p->carve(start);
		p->reshape(-1, 0);
		cout << "Y:" << i << endl;
	}
	p->expand(scale_x,scale_y,filename);
	p->expandoutput(filename);
}




int main() {
	seamcarving("p15", 0.28, 0.01, cut);
	seamcarving("p14", 0.2, 0.2, expand);
	seamcarving("p13", 0.2, 0.2, shrink);

	return 0;
}