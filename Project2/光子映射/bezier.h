#ifndef BEZIER_H
#define BEZIER_H

#include "object.h"

class Bezier : public Object {
	int n;
	int sum;//��ĸ���
	Point* points;
	Point startpoint;
	Point startaxis;
	Point startverticle;
	double centermax, centermin, r;
	Sphere* box;
	double** getB(int n, double t);
	void delB(int n, double** B);
	Point rotateP(Point P, double v);
	Point getP(double u, double v);
	Ray dPu(double u, double v);//getP����u�ĵ���
	Ray dPv(double u, double v);
	//getP����v�ĵ���
	Ray dv(Point origin, Ray axis, double theta);//��v��
	bool newton(Point origin, Ray direction, double u, double v, double t);//��ţ�ٵ������󽻵�

	Color gettexture() {
		return Color();
	}
public:
	Bezier();
	~Bezier();

	void input(std::string, std::stringstream&);
	Object* copy() {
		Bezier* result = new Bezier(*this);
		return result;
	}
	bool collide(Point origin, Ray direction);
};

#endif
