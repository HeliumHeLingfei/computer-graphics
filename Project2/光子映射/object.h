#ifndef OBJECT_H
#define OBJECT_H

#include "color.h"
#include "ray.h"
#include "picture.h"
#include <iostream>
#include <algorithm>

extern const double ZERO;
extern const double PI;

struct Material {
	Color color,absor;
	double reflect_rate, refract_rate, diffuse_rate, specular_rate;//简化，环境光、漫反射相同，各分量相同
	double n;//折射率
	Picture* texture;
	Material():reflect_rate(0.0), refract_rate(0.0), diffuse_rate(0.0), specular_rate(0.0),n(0.0), color(Color()), absor(Color()),texture(NULL){}
	void input(std::string para, std::stringstream& fin);
};

class Object {
protected:
	int code;//哈希值，用于判断某个光子或光是否经过同一序列的物体和光，若不同则有可能出现锯齿
	Material* material;
	Object* next;
	virtual Color gettexture() = 0;
public:
	Point crash_point;//碰撞点
	Ray crash_normal_vector;//碰撞点法向量
	double crash_distance;//碰撞点离出发点的距离，用来判断两个物体的远近
	bool front;//在内/外或前/后碰撞

	Object() {
		code = rand();
		material = new Material();
		next = NULL;
	}
	Object(const Object& origin_object) {
		*this = origin_object;
		material = new Material();
		*material = *origin_object.material;
	}
	~Object() {
		delete material;
	}

	int getcode() {
		return code;
	}
	Material* getmaterial() {
		return material;
	}
	Object* getnext() {
		return next;
	}
	void setnext(Object* next_object) {
		next = next_object;
	}
	Color getcolor() {
		Color result = material->color;
		if(material->texture != NULL)
			result *= gettexture();
		return result;
	}
	virtual void input(std::string para, std::stringstream& fin) {
		material->input(para, fin);
	}
	virtual bool collide(Ray origin, Ray direction) = 0;
	virtual Object* copy() = 0;

};

class Sphere :public Object {
	Point center;//球心
	double r;//半径
	Color gettexture();
public:
	Sphere() :Object(){}
	~Sphere() {}

	Point getcenter() {
		return center;
	}
	void setcenter(Point A) {
		center = A;
	}
	void setR(double R) {
		r = R;
	}
	void input(std::string para, std::stringstream& fin);
	bool collide(Point origin, Ray direction);
	Object * copy() {
		return new Sphere(*this);
	}
};

class Plain :public Object {
	Ray normal_vector;//平面法向量
	Ray vector_x, vector_y;//两个向量构成一个平面
	double distance;//离原点的距离
	Color gettexture();

public:
	Plain():Object(){}
	~Plain() {}

	void input(std::string para, std::stringstream& fin);
	bool collide(Point origin, Ray direction);
	Object * copy() {		
		return new Plain(*this);
	}
};
#endif // !OBJECT_H

