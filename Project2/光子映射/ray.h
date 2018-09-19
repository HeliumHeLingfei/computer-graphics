#ifndef RAY_H
#define RAY_H

#include <sstream>

const double ZERO = 1e-6;
const double PI = 3.141592653589793;


class Ray {
public:
	double x, y, z;
	Ray(double _x = 0, double _y = 0, double _z = 0) :x(_x), y(_y), z(_z) {}
	~Ray() {}

	friend Ray operator -(const Ray&);
	friend Ray operator +(const Ray&, const Ray&);
	friend Ray operator -(const Ray&, const Ray&);
	friend Ray operator *(const Ray&, const Ray&);
	friend Ray operator *(const Ray&, const double&);
	friend Ray operator /(const Ray&, const double&);
	friend Ray& operator +=(Ray&, const Ray&);
	friend Ray& operator -=(Ray&, const Ray&);
	friend Ray& operator *=(Ray&, const Ray&);
	friend Ray& operator *=(Ray&, const double&);
	friend Ray& operator /=(Ray&, const double&);

	void input(std::stringstream& fin);
	double dot(const Ray&);
	double M2();
	double M();
	double& dimension(int d);
	double D2(Ray&);
	Ray unit();
	Ray vertical();
	Ray random();
	bool iszero();
	Ray reflect(Ray N);
	Ray refract(Ray N, double n);
	Ray diffuse();
	Ray rotate(Ray axis, double theta);
};

typedef Ray Point;
#endif // !RAY_H

