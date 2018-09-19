#include "picture.h"
#define ran() (double(rand() % 32768) / 32768)

const double ZERO = 1e-6;
const double PI = 3.141592653589793;

Color Picture::getcolor(double _u, double _v) {
	double u = (_u - floor(_u + ZERO)) * h, v = (_v - floor(_v + ZERO)) * w;
	int u1 = (int)floor(u), u2 = u1 + 1, v1 = (int)floor(v), v2 = v1 + 1;
	double rate_u1 = u2 - u, rate_v1 = v2 - v, rate_u2 = u - u1, rate_v2 = v - v1;
	if(u1 < 0)
		u1 = h - 1;
	if(u2 == h)
		u2 = 0;
	if(v1 < 0)
		v1 = w - 1;
	if(v2 == w)
		v2 = 0;
	Color result;
	result += image[u1][v1].color() * rate_u1 * rate_v1;
	result += image[u1][v2].color() * rate_u1 * rate_v2;
	result += image[u2][v1].color() * rate_u2 * rate_v1;
	result += image[u2][v2].color() * rate_u2 * rate_v2;
	return result;
}

void Picture::input(std::string filename) {
	if(h > 0) {
		for(int i = 0; i < h; i++)
			delete[] image[i];
		delete[] image;
	}
	cv::Mat ima = cv::imread("./" + filename + ".jpg", -1);
	h = ima.rows;
	w = ima.cols;
	image = new Pix*[h];
	for(int i = 0; i < h; i++){
		image[i] = new Pix[w];
		cv::Vec3b * p = ima.ptr<cv::Vec3b>(i); //获取第 i 行首像素指针
		for(int j = 0; j < w; j++) {
			//对第 i 行的每个像素(byte)操作
			image[i][j]=Pix(p[j][2], p[j][1], p[j][0]);
		}
	}
}

void Picture::output(std::string filename) {
	cv::Mat out(h, w, CV_8UC3);
	for(int i = 0; i < h; i++) {
		cv::Vec3b * p = out.ptr<cv::Vec3b>(i); 
		for(int j = 0; j < w; j++) {
			p[j][0] = image[i][j].b; //Blue
			p[j][1] = image[i][j].g; //Green
			p[j][2] = image[i][j].r; //Red
		}
	}
	cv::imwrite("./picture/" + filename + ".jpg", out);
}
