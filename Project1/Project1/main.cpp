#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;
Vec4b white(255, 255, 255, 255);
Vec4b black(0, 0, 0, 255);
Vec4b grey(100, 100, 100, 255);
Vec4b green(0, 255, 0, 255);
Vec4b lgreen(0, 255, 0, 100);

struct seed {
	int x;
	int y;
	seed(int _x = 0, int _y = 0) {
		x = _x, y = _y;
	}
};

class stack {
	seed* s;
	int top;
public:
	stack() {
		s = new seed[200];
		top = 0;
	}
	void setstackempty() {
		top = 0;
	}
	void stackpush(seed q) {
		s[top++] = q;
	}
	bool isstackempty() {
		return top==0;
	}
	seed stackpop() {
		return s[--top];
	}
}s;

void drawline(Mat m,int x0,int y0,int x1,int y1, Vec4b color) {
	int x, y, dx, dy, ax,ay,e;
	dx = x1 - x0, dy = y1 - y0;
	x = x0, y = y0;
	ax = abs(dx), ay = abs(dy);
	if(ax> ay) {
		if(dx > 0) {
			e = -dx;
			if(dy >= 0) {
				for(int i = 0; i <= ax; i++) {
					m.at<Vec4b>(x, y) = color;
					x++, e += (2 * dy);
					if(e >= 0)
						y++, e -= (2 * dx);
				}
			}
			else {
				for(int i = 0; i <= ax; i++) {
					m.at<Vec4b>(x, y) = color;
					x++, e -= (2 * dy);
					if(e >= 0)
						y--, e -= (2 * dx);
				}
			}
		}
		else {
			e = dx;
			if(dy >= 0) {
				for(int i = 0; i <= ax; i++) {
					m.at<Vec4b>(x, y) = color;
					x--, e += (2 * dy);
					if(e >= 0)
						y++, e += (2 * dx);
				}
			}
			else {
				for(int i = 0; i <= ax; i++) {
					m.at<Vec4b>(x, y) = color;
					x--, e -= (2 * dy);
					if(e >= 0)
						y--, e += (2 * dx);
				}
			}
		}
	}
	else {
		if(dy > 0) {
			e = -dy;
			if(dx >= 0) {
				for(int i = 0; i <= ay; i++) {
					m.at<Vec4b>(x, y) = color;
					y++, e += (2 * dx);
					if(e >= 0)
						x++, e -= (2 * dy);
				}
			}
			else {
				for(int i = 0; i <= ay; i++) {
					m.at<Vec4b>(x, y) = color;
					y++, e -= (2 * dx);
					if(e >= 0)
						x--, e -= (2 * dy);
				}
			}
		}
		else {
			e = dy;
			if(dx >= 0) {
				for(int i = 0; i <= ay; i++) {
					m.at<Vec4b>(x, y) = color;
					y--, e += (2 * dx);
					if(e >= 0)
						x++, e += (2 * dy);
				}
			}
			else {
				for(int i = 0; i <= ay; i++) {
					m.at<Vec4b>(x, y) = color;
					y--, e -= (2 * dx);
					if(e >= 0)
						x--, e += (2 * dy);
				}
			}
		}
	}
}

void fill(Mat &m,int &x, int &y, Vec4b oldcolor, Vec4b newcolor) {
	int yl, yr;
	bool spanNeedFill;
	seed pt(x,y);
	s.setstackempty();
	s.stackpush(pt);
	while(!s.isstackempty()) {
		pt = s.stackpop();
		y = pt.y, x = pt.x;
		while(m.at<Vec4b>(x, y) == oldcolor) {
			m.at<Vec4b>(x, y) = newcolor;
			if(x < 255)
				m.at<Vec4b>(x, y)[0] = x;
			else
				m.at<Vec4b>(x, y)[0] = 510 - x;
			if(y < 255)
				m.at<Vec4b>(x, y)[2] = y;
			else
				m.at<Vec4b>(x, y)[2] = 510 - y;
			y++;
		}
		yr = y - 1;
		y = pt.y - 1;
		while(m.at<Vec4b>(x, y) == oldcolor) {
			m.at<Vec4b>(x, y) = newcolor;
			if(x < 255)
				m.at<Vec4b>(x, y)[0] = x;
			else
				m.at<Vec4b>(x, y)[0] = 510 - x;
			if(y < 255)
				m.at<Vec4b>(x, y)[2] = y;
			else
				m.at<Vec4b>(x, y)[2] = 510 - y;
			y--;
		}
		yl = y + 1;
		y = yl, x = x + 1;
		while(y <= yr) {
			spanNeedFill = false;
			while(m.at<Vec4b>(x, y) == oldcolor) {
				spanNeedFill = true;
				y++;
			}
			if(spanNeedFill) {
				pt.y = y - 1, pt.x = x;
				s.stackpush(pt);
				spanNeedFill = false;
			}
			while(m.at<Vec4b>(x, y) != oldcolor && y <= yr) {
				y++;
			}
		}
		y = yl, x = x - 2;
		while(y <= yr) {
			spanNeedFill = false;
			while(m.at<Vec4b>(x, y) == oldcolor) {
				spanNeedFill = true;
				y++;
			}
			if(spanNeedFill) {
				pt.y = y - 1, pt.x = x;
				s.stackpush(pt);
				spanNeedFill = false;
			}
			while(m.at<Vec4b>(x, y) != oldcolor && y <= yr) {
				y++;
			}
		}
	}	
}

int main(int argc, char** argv) {
	Mat pic(500, 500, CV_8UC4, white);
	int x[50], y[50],a,b;
	scanf("%d %d", &x[0], &y[0]);
	char q = getchar();
	int i = 1;
	while(q != '\n') {
		scanf("%d %d", &x[i], &y[i]);
		q = getchar();
		drawline(pic, x[i - 1], y[i - 1], x[i], y[i],black);
		i++;
	}
	drawline(pic, x[i-1], y[i-1], x[0], y[0],black);
	scanf("%d %d", &a, &b);
	fill(pic, a, b, white, green);
	imshow("pic.png", pic);
	imwrite("pic3.png", pic);
	waitKey(0);
	return 0;
}