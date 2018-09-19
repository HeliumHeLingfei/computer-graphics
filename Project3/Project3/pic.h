#ifndef PIC_H
#define PIC_H

#include <iostream>

using namespace::std;
const double MAX = 10000000.0;

enum Type {
	ori = 0, //oringin
	remX = 1, //removeX
	remY = 2,  //removeY
	del = 3, //delete
	pro = 4, //protect
	insX = 5, //insertX
	insY = 6 //insertY
};

struct Color {
	uchar r, g, b;
	Color(uchar _r = 0, uchar _g = 0, uchar _b = 0) :r(_r), g(_g), b(_b) {}
};

struct Node {
	Color c;
	double e; //energy
	int x; //location on oringin picture
	int y; //location on oringin picture
	int prev; //previous index, used to backtrack
	Type type;
	Node(int _x, int _y, Color _c = Color(), double _e = MAX, int _n = 0, Type _t = ori) :x(_x), y(_y), c(_c), e(_e), prev(_n), type(_t) {}
};

class Pic {
	int n, m, N, M;//m-row,n-col,M-origin row,N-origin col
	bool direct; //X or Y
	Node*** table;
	Node*** origin;
	Node*** enlarge;
	Color cm[3][3];//color_matrix
	Color cl[3];//color list
	Color getcolor(int i, int j);
	Color getoricolor(int i, int j);
	double sobel(int i, int j);
	void set(int i, int j);//set node type, used to output sign.
	int remove(int i, int j);//remove node
public:
	Pic(int _M, int _N, bool _d) :M(_M), N(_N), direct(_d), m(_M), n(_N), table(NULL) {}
	void input(string filename);
	void output(string filename);
	void expandoutput(string filename);
	void carve(int start);
	void recover();//set the expanded picture as the origin picture
	void expand(int scale_x, int scale_y, string filename);
	void setcut(int x1, int y1, int x2, int y2);
	void setpro(int x1, int y1, int x2, int y2);
	int seam();
	void reshape(int dm, int dn);
	int getM() {
		return M;
	}
	int getN() {
		return N;
	}
	void setdirect(bool d) {
		direct = d;
	}
};
#endif // !PIC_H


