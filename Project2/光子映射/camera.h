#ifndef CAMERA_H
#define CAMERA_H
#include "ray.h"
#include "picture.h"
#include "color.h"

class Camera {
	Ray normal_vector, vector_x, vector_y;//�ӵ㷽���ӵ㷶Χ
	double lens_h, lens_w;
	Color** image;
public:
	int max_photons, emit_photons, sample_photons;//���������������������β�����
	double sample_dist;//�����뾶
	int h, w;//ͼƬ��С
	Point center;
	int shade_quality;//��������Ӱ�Ĵ���
	Camera();
	~Camera();

	void setcolor(int i, int j, Color color) {
		image[i][j] = color;
	}
	Color getcolor(int i,int j){
		return image[i][j];
	}
	Ray getview(double i, double j);//��������
	void init();
	void input(std::string para, std::stringstream& fin);
	void output(std::string);//���ͼƬ
};

#endif // !CAMERA_H