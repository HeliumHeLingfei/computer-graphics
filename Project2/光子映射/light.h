#ifndef LIGHT_H
#define LIGHT_H

#include "ray.h"
#include "object.h"
#include "color.h"
#include "photon.h"
#include <cmath>

extern const double ZERO;
const double SPEC_POWER = 20;

class Light {
protected:
	int code;
	Color color;
	Light* next;
	Point center;
public:
	bool pointlight;
	double crash_distance;

	Light() {
		code = rand();
		next = NULL;
	}
	~Light() {}

	int getcode() {
		return code;
	}
	Color getcolor() {
		return color;
	}
	Light* getnext() {
		return next;
	}
	void setnext(Light* next_light) {
		next = next_light;
	}
	Point getcenter() {
		return center;
	}
	Color irradiance(Object* object, Ray direction, int* hash);//光直接照射的光通量
	virtual void input(std::string para, std::stringstream& fin);
	virtual bool collide(Point origin, Ray direction) = 0;
	virtual Color getirr(Object* object ,Object* object_head , int shade_quality , int* hash)=0;//该光在物体某点上的光通量，0即为该光造成阴影
	virtual Photon emit() = 0;
};

class Pointlight:public Light {
public:
	Pointlight() :Light() {
		pointlight = true;
	}
	~Pointlight() {}
	
	Color getirr(Object* object , Object* object_head , int shade_quality , int* hash);
	Photon emit();
	void input(std::string para, std::stringstream& fin);
	bool collide(Point origin, Ray direction);
};

class Plainlight : public Light {
	Ray vector_x, vector_y;
public:
	Plainlight() :Light() {
		pointlight = false;
	}
	~Plainlight() {}

	void input(std::string para, std::stringstream& fin);
	bool collide(Point origin, Ray direction);
	Color getirr(Object* object , Object* object_head , int shade_quality , int* hash);
	Photon emit();
};


#endif // !LIGHT_H
