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
	double reflect_rate, refract_rate, diffuse_rate, specular_rate;//�򻯣������⡢��������ͬ����������ͬ
	double n;//������
	Picture* texture;
	Material():reflect_rate(0.0), refract_rate(0.0), diffuse_rate(0.0), specular_rate(0.0),n(0.0), color(Color()), absor(Color()),texture(NULL){}
	void input(std::string para, std::stringstream& fin);
};

class Object {
protected:
	int code;//��ϣֵ�������ж�ĳ�����ӻ���Ƿ񾭹�ͬһ���е�����͹⣬����ͬ���п��ܳ��־��
	Material* material;
	Object* next;
	virtual Color gettexture() = 0;
public:
	Point crash_point;//��ײ��
	Ray crash_normal_vector;//��ײ�㷨����
	double crash_distance;//��ײ���������ľ��룬�����ж����������Զ��
	bool front;//����/���ǰ/����ײ

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
	Point center;//����
	double r;//�뾶
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
	Ray normal_vector;//ƽ�淨����
	Ray vector_x, vector_y;//������������һ��ƽ��
	double distance;//��ԭ��ľ���
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

