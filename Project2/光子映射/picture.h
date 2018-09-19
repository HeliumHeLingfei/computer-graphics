#ifndef PICTURE_H
#define PICTURE_H

#include "color.h"
#include <cmath>
#include <opencv2/opencv.hpp>
#include <string>
typedef unsigned char uchar;

extern const double ZERO;
extern const double PI;

struct Pix {
	uchar r, g, b;
	Pix(uchar _r = 0, uchar _g = 0, uchar _b = 0) :r(_r), g(_g), b(_b) {}
	Color color() {
		return Color(r, g, b) / 256;
	}
};

class Picture {
	Pix** image;
public:
	int h, w;

	Picture(int _h = 0,int _w = 0):h(_h),w(_w){
		if(h != 0 && w != 0)
			init(h,w);
	}
	~Picture() {
		for(int i = 0; i < h; i++)
			delete[] image[i];
		delete[] image;
	}
	Color getcolor(int i, int j) {
		return image[i][j].color();
	}
	Color getcolor(double u, double v);
	void init(int _h,int _w){
		h=_h;
		w=_w;
		image = new Pix*[h];
		for(int i = 0; i < h; i++)
			image[i] = new Pix[w];
	}
	void setcolor(int i, int j, Color color) {
		image[i][j].r = (uchar)(color.r * 255);
		image[i][j].g = (uchar)(color.g * 255);
		image[i][j].b = (uchar)(color.b * 255);
		std::cout<< (int)image[i][j].r<<" "<< (int)image[i][j].g<<" "<< (int)image[i][j].b<<std::endl;
	}
	void input(std::string filename);
	void output(std::string filename);


private:

};

#endif // !PICTURE_H

