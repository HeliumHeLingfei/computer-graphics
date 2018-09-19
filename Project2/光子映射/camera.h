#ifndef CAMERA_H
#define CAMERA_H
#include "ray.h"
#include "picture.h"
#include "color.h"

class Camera {
	Ray normal_vector, vector_x, vector_y;//视点方向，视点范围
	double lens_h, lens_w;
	Color** image;
public:
	int max_photons, emit_photons, sample_photons;//最大光子数，发射数，单次采样数
	double sample_dist;//采样半径
	int h, w;//图片大小
	Point center;
	int shade_quality;//计算软阴影的次数
	Camera();
	~Camera();

	void setcolor(int i, int j, Color color) {
		image[i][j] = color;
	}
	Color getcolor(int i,int j){
		return image[i][j];
	}
	Ray getview(double i, double j);//发出视线
	void init();
	void input(std::string para, std::stringstream& fin);
	void output(std::string);//输出图片
};

#endif // !CAMERA_H