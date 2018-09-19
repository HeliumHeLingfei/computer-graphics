#include "camera.h"

#define ran() (double(rand() % 32768) / 32768)
Camera::Camera() {
	center = Point(0, 0, 0);
	normal_vector = Ray(0, 1, 0);
	lens_w = 0.88;
	lens_h = 0.88;
	w = 100;
	h = 100;
	shade_quality = 4;
	max_photons = 2000000;
	emit_photons = 1000000;
	sample_photons = 100;
	sample_dist = 1;
	image = NULL;
}

Camera::~Camera() {
	if(image) {
		for(int i = 0; i < h; i++)
			delete[] image[i];
		delete[] image;
	}
}

Ray Camera::getview(double i, double j) {
	return normal_vector + vector_y * (2 * i / h - 1) + vector_x * (2 * j / w - 1);
}

void Camera::init() {
	vector_x = normal_vector.vertical() * lens_w / 2;
	vector_y = vector_x * normal_vector * lens_h / 2;
	normal_vector = normal_vector.unit();
	image = new Color*[(int)h];
	for(int i = 0; i < h; i++)
		image[i] = new Color[(int)w];
}

void Camera::input(std::string para, std::stringstream & fin) {
	if(para == "center=") {
		fin >> center.x >> center.y >> center.z;
		std::cout << center.x << " " << center.y << " " << center.z << std::endl;
	}
	else if(para == "normal_vector=") {
		fin >> normal_vector.x >> normal_vector.y >> normal_vector.z;
		std::cout << normal_vector.x << " " << normal_vector.y << " " << normal_vector.z << std::endl;
	}
	else if(para == "lens_w=")
		fin >> lens_w;
	else if(para == "lens_h=")
		fin >> lens_h;
	else if(para == "image_w=")
		fin >> w;
	else if(para == "image_h=")
		fin >> h;
	else if(para == "shade_quality=")
		fin >> shade_quality;
	else if(para == "max_photons=")
		fin >> max_photons;
	else if(para == "emit_photons=")
		fin >> emit_photons;
	else if(para == "sample_photons=")
		fin >> sample_photons;
	else if(para == "sample_dist=")
		fin >> sample_dist;
}

void Camera::output(std::string filename) {
	Picture* picture = new Picture(h, w);
	for(int i = 0; i < h; i++)
		for(int j = 0; j < w; j++)
			picture->setcolor(i, j, image[i][j]);
	picture->output(filename);
}
