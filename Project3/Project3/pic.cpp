#include <opencv2/opencv.hpp>
#include <iostream>
#include "pic.h"
using namespace std;

#define X true
#define Y false
#define RED Color(255,0,0)
#define GREEN Color(0,255,0)
#define BLUE Color(0,0,255)

void Pic::input(string filename) {
	cout << "input " << filename << endl;
	cv::Mat image = cv::imread("./input/" + filename + ".jpg", -1);
	M = m = image.rows;
	N = n = image.cols;
	cout << M << ' ' << N << endl;
	table = new Node**[M];
	origin = new Node**[M];
	for(int i = 0; i < M; i++) {
		table[i] = new Node*[N];
		origin[i] = new Node*[N];
		cv::Vec3b * p = image.ptr<cv::Vec3b>(i); //获取第 i 行首像素指针
		for(int j = 0; j < N; j++) {
			//对第 i 行的每个像素(byte)操作
			table[i][j] = origin[i][j]= new Node(i, j, Color(p[j][2], p[j][1], p[j][0]));
		}
	}
}

void Pic::output(string filename) {
	cout << "output " << filename << endl;
	cv::Mat out(m, n, CV_8UC3);
	for(int i = 0; i < m; i++) {
		//获取第 i 行首像素指针
		cv::Vec3b * p = out.ptr<cv::Vec3b>(i); //获取第 i 行首像素指针
		for(int j = 0; j < n; j++) {
			//对第 i 行的每个像素(byte)操作
			p[j][0] = table[i][j]->c.b; //Blue
			p[j][1] = table[i][j]->c.g; //Green
			p[j][2] = table[i][j]->c.r; //Red
		}
	}
	cv::imwrite("./input/" + filename + "_out.png", out);

	cout << "sign " << filename << endl;
	cv::Mat out1(M, N, CV_8UC3);
	for(int i = 0; i < M; i++) {
		//获取第 i 行首像素指针
		cv::Vec3b * p = out1.ptr<cv::Vec3b>(i); //获取第 i 行首像素指针
		for(int j = 0; j < N; j++) {
			//对第 i 行的每个像素(byte)操作
			if(origin[i][j]->type == remY) {
				p[j][0] = 0;
				p[j][1] = 255;
				p[j][2] = 0;
			}
			else if(origin[i][j]->type == remX) {
				p[j][0] = 0;
				p[j][1] = 0;
				p[j][2] = 255;

			}
			else {
				p[j][0] = origin[i][j]->c.b; //Blue
				p[j][1] = origin[i][j]->c.g; //Green
				p[j][2] = origin[i][j]->c.r; //Red
			}
		}
	}
	cv::imwrite("./input/" + filename + "_sign.png", out1);
}

void Pic::expandoutput(string filename) {
	cout << "expandoutput " << filename << endl;
	cv::Mat out(m, n, CV_8UC3);
	for(int i = 0; i < m; i++) {
		//获取第 i 行首像素指针
		cv::Vec3b * p = out.ptr<cv::Vec3b>(i); //获取第 i 行首像素指针
		for(int j = 0; j < n; j++) {
			//对第 i 行的每个像素(byte)操作
			p[j][0] = enlarge[i][j]->c.b; //Blue
			p[j][1] = enlarge[i][j]->c.g; //Green
			p[j][2] = enlarge[i][j]->c.r; //Red
		}
	}
	cv::imwrite("./input/" + filename + "_eout.png", out);

	cout << "expandsign " << filename << endl;
	for(int i = 0; i < m; i++) {
		//获取第 i 行首像素指针
		cv::Vec3b * p = out.ptr<cv::Vec3b>(i); //获取第 i 行首像素指针
		for(int j = 0; j < n; j++) {
			//对第 i 行的每个像素(byte)操作
			if(enlarge[i][j]->type == insX) {
				p[j][0] = 0;
				p[j][1] = 255;
				p[j][2] = 0;
			}
			else if(enlarge[i][j]->type == insY) {
				p[j][0] = 0;
				p[j][1] = 0;
				p[j][2] = 255;
			}
		}
	}
	cv::imwrite("./input/" + filename + "_esign.png", out);
}


void Pic::set(int i, int j) {
	if(direct == X) 
		origin[i][j]->type = remX;
	else
		origin[i][j]->type = remY;
}

void Pic::carve(int start) {
	int prev = start;
	int tem = prev;
	if(direct == X) {
		for(int i = m - 1; i >= 0; i--) {
			prev = remove(i, tem);
			tem = prev;
		}
	}
	else {
		for(int j = n - 1; j >= 0; j--) {
			prev = remove(tem, j);
			tem = prev;
		}
	}
}

int Pic::remove(int i, int j) {
	int tem = table[i][j]->prev;
	set(table[i][j]->x, table[i][j]->y);
	if(direct == X) {
		for(int k = j; k < n - 1; k++)
			table[i][k] = table[i][k + 1];
	}
	else {
		for(int k = i; k < m - 1; k++)
			table[k][j] = table[k + 1][j];
	}
	return tem;
}

void Pic::expand(int scale_x, int scale_y, string filename) {
	cout << "start expand" << endl;
	if(direct == Y)
		m = M + scale_y;
	else
		n = N + scale_x;
	enlarge = new Node**[m];
	int* index = new int[n];
	for(int i = 0; i < n; i++)
		index[i] = 0;
	for(int i = 0; i < m; i++)
		enlarge[i] = new Node*[n];
	for(int i = 0; i < M; i++) {
		int dj = 0;
		for(int j = 0; j < N; j++) {
			enlarge[i+index[j+dj]][j + dj] = origin[i][j];
			enlarge[i + index[j + dj]][j + dj]->x = i + index[j + dj];
			enlarge[i + index[j + dj]][j + dj]->y = j + dj;
			if(direct == Y) {
				if(origin[i][j]->type == remY) {
					index[j + dj]++;
					cl[0] = getoricolor(i - 1, j);
					cl[1] = origin[i][j]->c;
					cl[2] = getoricolor(i + 1, j);
					int r = (cl[0].r + cl[1].r + cl[2].r) / 3;
					int g = (cl[0].g + cl[1].g + cl[2].g) / 3;
					int b = (cl[0].b + cl[1].b + cl[2].b) / 3;
					enlarge[i + index[j + dj]][j + dj] = new Node(i + index[j + dj], j + dj, Color(r, g, b), 0, 0, insY);
				}
			}
			else{
				if(origin[i][j]->type == remX) {
					dj++;
					cl[0] = getoricolor(i, j - 1);
					cl[1] = origin[i][j]->c;
					cl[2] = getoricolor(i, j + 1);
					int r = (cl[0].r + cl[1].r + cl[2].r) / 3;
					int g = (cl[0].g + cl[1].g + cl[2].g) / 3;
					int b = (cl[0].b + cl[1].b + cl[2].b) / 3;
					enlarge[i + index[j + dj]][j + dj] = new Node(i + index[j + dj], j + dj, Color(r, g, b), 0, 0, insX);
				}
			}
		}
	}
}

void Pic::setcut(int x1, int y1, int x2, int y2) {
	for(int i = x1; i<x2; i++)
		for(int j = y1; j < y2; j++) {
			table[i][j]->type = del;
			table[i][j]->e = -MAX;
		}
}

void Pic::setpro(int x1, int y1, int x2, int y2) {
	for(int i = x1; i<x2; i++)
		for(int j = y1; j < y2; j++) {
			table[i][j]->type = pro;
			table[i][j]->e = MAX;
		}
}

void Pic::recover() {
	M = m; N = n;
	table = new Node**[M];
	origin = new Node**[M];
	for(int i = 0; i < M; i++) {
		table[i] = new Node*[N];
		origin[i] = new Node*[N];
		for(int j = 0; j < N; j++)
			table[i][j] = origin[i][j] = enlarge[i][j];
	}
}

void Pic::reshape(int dm, int dn) {
	m += dm;
	n += dn;
}

Color Pic::getcolor(int i, int j) {
	if(i<0 || i>m - 1 || j<0 || j>n - 1)
		return Color();
	else
		return table[i][j]->c;
}

Color Pic::getoricolor(int i, int j) {
	if(i<0 || i>M - 1 || j<0 || j>N - 1)
		return Color();
	else
		return origin[i][j]->c;
}

int Pic::seam() {
	for(int i = 0; i < m; i++) {
		for(int j = 0; j < n; j++) {
			if(table[i][j]->type != del && table[i][j]->type != pro)
				table[i][j]->e = sobel(i, j);
		}
	}
	int start;
	if(direct == X) {
		for(int i = 0; i < m - 1; i++) {
			for(int j = 0; j < n; j++) {
				double el = (j == 0) ? MAX : table[i][j - 1]->e;
				double em = table[i][j]->e;
				double er = (j == n - 1) ? MAX : table[i][j + 1]->e;
				if(el < em) {
					if(el < er) {
						table[i + 1][j]->e += el;
						table[i + 1][j]->prev = j - 1;
					}
					else {
						table[i + 1][j]->e += er;
						table[i + 1][j]->prev = j + 1;
					}
				}
				else {
					if(em < er) {
						table[i + 1][j]->e += em;
						table[i + 1][j]->prev = j;
					}
					else {
						table[i + 1][j]->e += er;
						table[i + 1][j]->prev = j + 1;
					}
				}
			}
		}
		double min = MAX;
		for(int j = 0; j<n; j++) {
			if(min > table[m - 1][j]->e) {
				min = table[m - 1][j]->e;
				start = j;
			}
		}
	}
	else {
		for(int j = 0; j < n - 1; j++) {
			for(int i = 0; i < m; i++) {
				double eu = (i == 0) ? MAX : table[i - 1][j]->e;
				double em = table[i][j]->e;
				double ed = (i == m - 1) ? MAX : table[i + 1][j]->e;
				if(eu < em) {
					if(eu < ed) {
						table[i][j + 1]->e += eu;
						table[i][j + 1]->prev = i - 1;
					}
					else {
						table[i][j + 1]->e += ed;
						table[i][j + 1]->prev = i + 1;
					}
				}
				else {
					if(em < ed) {
						table[i][j + 1]->e += em;
						table[i][j + 1]->prev = i;
					}
					else {
						table[i][j + 1]->e += ed;
						table[i][j + 1]->prev = i + 1;
					}
				}
			}
		}
		double min = MAX;
		for(int i = 0; i<m; i++) {
			if(min > table[i][n - 1]->e) {
				min = table[i][n - 1]->e;
				start = i;
			}
		}
	}
	return start;
}

double Pic::sobel(int i, int j) {
	for(int k = 0; k < 3; k++) {
		cm[k][0] = getcolor(i + k - 1, j - 1);
		cm[k][1] = getcolor(i + k - 1, j);
		cm[k][2] = getcolor(i + k - 1, j + 1);
	}
	double rx = cm[2][0].r + 2 * cm[2][1].r + cm[2][2].r
		- (cm[0][0].r + 2 * cm[0][1].r + cm[0][2].r);
	double ry = cm[0][2].r + 2 * cm[1][2].r + cm[2][2].r
		- (cm[0][0].r + 2 * cm[1][0].r + cm[2][0].r);
	double gx = cm[2][0].g + 2 * cm[2][1].g + cm[2][2].g
		- (cm[0][0].g + 2 * cm[0][1].g + cm[0][2].g);
	double gy = cm[0][2].g + 2 * cm[1][2].g + cm[2][2].g
		- (cm[0][0].g + 2 * cm[1][0].g + cm[2][0].g);
	double bx = cm[2][0].b + 2 * cm[2][1].b + cm[2][2].b
		- (cm[0][0].b + 2 * cm[0][1].b + cm[0][2].b);
	double by = cm[0][2].b + 2 * cm[1][2].b + cm[2][2].b
		- (cm[0][0].b + 2 * cm[1][0].b + cm[2][0].b);

	double x = rx * rx + gx * gx + bx * bx;
	double y = ry * ry + gy * gy + by * by;
	double s = x + y;
	//return sqrt(s);
	//return s;
	return abs(rx) + abs(ry) + abs(bx) + abs(by) + abs(gx) + abs(gy);
	//return (direct == X)? sqrt(x): sqrt(y);
}