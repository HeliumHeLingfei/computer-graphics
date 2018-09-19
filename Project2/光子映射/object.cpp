#include "object.h"
#define ran() (double(rand() % 32768) / 32768)

void Material::input(std::string para, std::stringstream & fin) {
	if(para == "reflect_rate=") 
		fin >> reflect_rate;
	else if(para == "refract_rate=")
		fin >> refract_rate;
	else if(para == "diffuse_rate=")
		fin >> diffuse_rate;
	else if(para == "specular_rate=")
		fin >> specular_rate;
	else if(para == "n=")
		fin >> n;
	else if(para == "color=")
		fin >> color.r >> color.g >> color.b;
	else if(para == "absor=")
		fin >> absor.r >> absor.g >> absor.b;
	else if(para == "texture=") {
		std::string file; 
		fin >> file;
		texture = new Picture();
		texture->input(file);
	}
}

void Sphere::input(std::string para, std::stringstream & fin) {
	if(para == "center=") {
		fin >> center.x >> center.y >> center.z;
		std::cout << center.x << " " << center.y << " " << center.z << std::endl;
	}
	else if(para == "r=")
		fin >> r;
	else
		material->input(para, fin);
}

bool Sphere::collide(Point origin, Ray direction) {
	Ray ray_v = direction.unit();
	Ray ray_p = origin - center;
	double b = -ray_p.dot(ray_v);
	double discriminant = b * b - ray_p.M2() + r * r;//判别式 b2-4ac
	if(discriminant > ZERO) {
		discriminant = sqrt(discriminant);
		double t1 = b - discriminant, t2 = b + discriminant;
		if(t2 < ZERO)
			return false;
		else if(t1 > ZERO) {
			crash_distance = t1;
			front = true;
		}
		else {
			crash_distance = t2;
			front = false;
		}
	}
	else
		return false;
	crash_point = origin + ray_v * crash_distance;
	crash_normal_vector = (crash_point - center).unit();
	if(!front)
		crash_normal_vector = -crash_normal_vector;
	return true;
}

Color Sphere::gettexture() {//映射到球坐标系
	Ray vector_y = Ray(0, 1, 0);
	Ray vector_z = Ray(0, 0, 1);
	Ray p = (crash_point - center).unit();
	double a = acos(-p.dot(vector_z));
	double b = acos(std::min(std::max(p.dot(vector_y) / sin(a), -1.0), 1.0));
	double u = a / PI;
	double v = b / 2 / PI;
	if(p.dot(vector_y * vector_z) < 0)
		v = 1 - v;
	return material->texture->getcolor(u, v);
}

void Plain::input(std::string para, std::stringstream & fin) {
	if(para == "normal_vector=")
		fin >> normal_vector.x >> normal_vector.y >> normal_vector.z;
	else if(para == "distance=")
		fin >> distance;
	else if(para == "vector_x=")
		fin >> vector_x.x >> vector_x.y >> vector_x.z;
	else if(para == "vector_y=")
		fin >> vector_y.x >> vector_y.y >> vector_y.z;
	else
		material->input(para, fin);

}

bool Plain::collide(Point origin, Ray direction) {
	Ray ray_v = direction.unit();
	normal_vector = normal_vector.unit();
	//std::cout << "start collide" << std::endl;
	//std::cout << ray_v.x << " " << ray_v.y << " " << ray_v.z << " " << normal_vector.x << " " << normal_vector.y << " " << normal_vector.z << " " << std::endl;
	double theta = normal_vector.dot(ray_v);
	if(fabs(theta) < ZERO)//平行
		return false;
	double dist = (normal_vector * distance - origin).dot(normal_vector) / theta;
	if(dist < ZERO)//点在面内
		return false;
	crash_distance = dist;
	front = (theta < 0);
	crash_point = origin + ray_v * crash_distance;
	crash_normal_vector = (front) ? normal_vector : -normal_vector;
	//std::cout << theta << " " << dist << std::endl;
	//std::cout << ray_v.x << " " << ray_v.y << " " << ray_v.z << " " << origin.x << " " << origin.y << " " << origin.z << " " << std::endl;
	return true;
}

Color Plain::gettexture() {
	//std::cout << "crash_point " << crash_point.x << " " << crash_point.y << " " << crash_point.z << std::endl;
	double u = crash_point.dot(vector_x) / vector_x.M2();
	double v = crash_point.dot(vector_y) / vector_y.M2();
	//std::cout << "u " << u << "v " << v<<std::endl;
	return material->texture->getcolor(u, v);
}
