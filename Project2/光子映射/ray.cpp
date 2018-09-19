#include "ray.h"
#include <cmath>

extern const double ZERO;
extern const double PI;
#define ran() (double(rand() % 32768) / 32768)

Ray operator-(const Ray& M) {
	return Ray(-M.x, -M.y, -M.z);
}

Ray operator+(const Ray & A, const Ray & B) {
	return Ray(A.x + B.x, A.y + B.y, A.z + B.z);
}

Ray operator-(const Ray &A, const Ray &B) {
	return Ray(A.x - B.x, A.y - B.y, A.z - B.z);
}

Ray operator*(const Ray &A, const Ray &B) {
	return Ray(A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x);
}

Ray operator*(const Ray &M, const double &k) {
	return Ray(M.x * k, M.y * k, M.z * k);
}

Ray operator/(const Ray &M, const double &k) {
	return Ray(M.x / k, M.y / k, M.z / k);
}

Ray& operator+=(Ray &A, const Ray &B) {
	A = A + B;
	return A;
}

Ray& operator-=(Ray &A, const Ray &B) {
	A = A - B;
	return A;
}

Ray& operator*=(Ray &A, const Ray &B) {
	A = A * B;
	return A;
}

Ray& operator*=(Ray &A, const double &k) {
	A = A * k;
	return A;
}

Ray& operator/=(Ray &A, const double &k) {
	A = A / k;
	return A;
}

void Ray::input(std::stringstream& fin){
	fin >> x >> y >> z;
}

double Ray::dot(const Ray &M) {
	return x * M.x + y * M.y + z * M.z;
}

double Ray::M2() {
	return x * x + y * y + z * z;
}

double Ray::M() {
	return sqrt(M2());
}

double Ray::D2(Ray& T) {
	return (T - *this).M2();
}

Ray Ray::unit() {
	return *this / M();
}

Ray Ray::vertical() {
	Ray ray = *this*Ray(0, 0, 1);
	if(ray.iszero())
		ray = Ray(1, 0, 0);
	return ray.unit();
}

Ray Ray::random() {
	do {
		x = 1 - 2 * ran();
		y = 1 - 2 * ran();
		z = 1 - 2 * ran();
	} while(M2() > 1 || M2() < ZERO);
	*this=unit();
	return *this;
}

double& Ray::dimension(int d){
	if(d==0)
		return x;
	else if(d==1)
		return y;
	else
		return z;
}

bool Ray::iszero() {
	return fabs(x) < ZERO && fabs(y) < ZERO && fabs(z) < ZERO;
}

Ray Ray::reflect(Ray N) {
	return *this - N * (2 * dot(N));
}

Ray Ray::refract(Ray N, double n) {
	Ray V = unit();
	double cosi = -N.dot(V), cosr2 = 1 - (n * n) * (1 - cosi * cosi);
	if(cosr2 > ZERO)
		return V * n + N * (n * cosi - sqrt(cosr2));
	return V.reflect(N);
}

Ray Ray::diffuse() {
	Ray Vert = vertical();
	double theta = acos(sqrt(ran()));
	double fi = ran() * 2 * PI;
	Ray tem = rotate(Vert, theta);//水平方向
	return tem.rotate(*this , fi);//竖直方向
}

Ray Ray::rotate(Ray axis , double theta) {
	Ray result;
	double cost = cos(theta);
	double sint = sin(theta);
	result.x += x * (axis.x * axis.x + (1 - axis.x * axis.x) * cost);
	result.x += y * (axis.x * axis.y * (1 - cost) - axis.z * sint);
	result.x += z * (axis.x * axis.z * (1 - cost) + axis.y * sint);
	result.y += x * (axis.y * axis.x * (1 - cost) + axis.z * sint);
	result.y += y * (axis.y * axis.y + (1 - axis.y * axis.y) * cost);
	result.y += z * (axis.y * axis.z * (1 - cost) - axis.x * sint);
	result.z += x * (axis.z * axis.x * (1 - cost) - axis.y * sint);
	result.z += y * (axis.z * axis.y * (1 - cost) + axis.x * sint);
	result.z += z * (axis.z * axis.z + (1 - axis.z * axis.z) * cost);
	return result;
}
