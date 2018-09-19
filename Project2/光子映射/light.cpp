#include "light.h"
#define ran() (double(rand() % 32768) / 32768)
const int HASH_MOD = 10006481;

void Light::input(std::string para, std::stringstream & fin) {
	if(para == "color=")
		fin >> color.r >> color.g >> color.b;
	else if(para == "center=") {
		fin >> center.x >> center.y >> center.z;
		std::cout << center.x << " " << center.y << " " << center.z << std::endl;
	}
}

void Pointlight::input(std::string para, std::stringstream & fin) {
	Light::input(para, fin);
}

Color Light::irradiance(Object* object, Ray direction, int* hash){
	Color result;
	double dist2 = direction.M2();
	Ray ray_v = direction.unit();
	double dot = ray_v.dot(object->crash_normal_vector);

	if (dot > ZERO) {
		if (hash != NULL)
			*hash = (*hash + code) % HASH_MOD;
		if (object->getmaterial()->diffuse_rate > ZERO) {
			result += color * object->getmaterial()->diffuse_rate * dot / dist2;
		}
		if (object->getmaterial()->specular_rate > ZERO) {
			result += color * object->getmaterial()->specular_rate * pow(dot, SPEC_POWER) / dist2;
		}
	}
	return result;	
}

bool Pointlight::collide(Point origin, Ray direction) {
	return false;
}

Color Pointlight::getirr(Object* object , Object* object_head , int shade_quality , int* hash){
	Ray ray_v = center - object->crash_point;
	double dist = ray_v.M();

	Object* head=object_head;
	while(head) {
		if(head->collide(object->crash_point, ray_v) && head->crash_distance < dist)//有物体挡着
			return Color();
		head = head->getnext();
	}	
	return irradiance(object,ray_v,hash);
}

Photon Pointlight::emit() {
	Photon result;
	result.position = center;
	result.direction= result.direction.random();
	result.intensity = color / color.intensity();
	return result;
}

void Plainlight::input(std::string para, std::stringstream & fin) {
	if(para == "vector_x=")
		fin >> vector_x.x >> vector_x.y >> vector_x.z;
	if(para == "vector_y=")
		fin >> vector_y.x >> vector_y.y >> vector_y.z;
	Light::input(para, fin);
}

bool Plainlight::collide(Point origin, Ray direction) {//与平面相同
	Ray ray_v = direction.unit();
	Ray normal_vector = (vector_x*vector_y).unit();
	double theta = normal_vector.dot(ray_v);
	if(fabs(theta) < ZERO)
		return false;
	double dist = (normal_vector * center.dot(normal_vector) - origin).dot(normal_vector) / theta;
	if(dist < ZERO)
		return false;
	Point crash_point = (origin + direction * dist) - center;
	if(fabs(vector_x.dot(crash_point)) > vector_x.dot(vector_x))
		return false;
	if(fabs(vector_y.dot(crash_point)) > vector_y.dot(vector_y))
		return false;
	crash_distance = dist;
	return true;
}

Photon Plainlight::emit() {
	Photon result;
	result.position = center + vector_x * (ran() * 2 - 1) + vector_y * (ran() * 2 - 1);
	result.direction = result.direction.random();
	result.intensity = color / color.intensity();
	return result;
}

Color Plainlight::getirr(Object* object , Object* object_head , int shade_quality , int* hash){
	Color result;
	bool shade;
	for(int i = -2; i < 2; i++)
		for(int j = -2; j < 2; j++)
			for(int k = 0; k < shade_quality; k++) {//多次采样
				Ray ray_v = center + vector_x * ((ran() + i) / 2) + vector_y * ((ran() + j) / 2) - object->crash_point;//在方格内随机取点
				shade=false;
				double dist = ray_v.M();
				Object* head = object_head;
				while(head) {
					if(head->collide(object->crash_point, ray_v) && head->crash_distance < dist) {//挡着
						shade=true;
						break;
					}
					head = head->getnext();
				}
				if(!shade)
					result += irradiance(object, ray_v, NULL);
			}

	return result/ (16.0 * shade_quality);
}
