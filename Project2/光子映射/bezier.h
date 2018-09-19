#ifndef BEZIER_H
#define BEZIER_H

#include "object.h"

class Bezier : public Object {
	int n;
	int sum;//点的个数
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
	Ray dPu(double u, double v);//getP关于u的导数
	Ray dPv(double u, double v);
	//getP关于v的导数
	Ray dv(Point origin, Ray axis, double theta);//对v求导
	bool newton(Point origin, Ray direction, double u, double v, double t);//用牛顿迭代法求交点

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
