#include "Bezier.h"
#include <algorithm>

Bezier::Bezier() {
	n = sum = 0;
	points = NULL;
	box = NULL;
	crash_distance = 1e7;
	centermax = 0, r = 0;
	centermin = 1e7;
}

Bezier::~Bezier() {
	if(points != NULL) {
		delete[] points;
	}
}

void Bezier::input(std::string para, std::stringstream & fin) {
	if(para == "N=")
		fin >> n;
	else if(para == "startpoint=")
		startpoint.input(fin);
	else if(para == "startaxis=") {
		startaxis.input(fin);
		startaxis = startaxis.unit();
		startverticle = startaxis.vertical();
		startverticle = startverticle.unit();
	}
	else if(para == "newpoint=") {
		if(points == NULL) {
			points = new Point[n + 1];
		}
		double a, b;
		fin >> a >> b;
		Point pt = startpoint + a * startaxis + b * startverticle;
		points[sum] = pt;
		sum++;
		if(a > centermax)
			centermax = a;
		if(a < centermin)
			centermin = a;
	}
	else
		Object::input(para, fin);
}

double** Bezier::getB(int n, double t) {
	double** B = new double*[n + 1];
	for(int i = 0; i <= n; i++) {
		B[i] = new double[n + 1];
	}

	B[0][0] = 1;
	for(int j = 1; j <= n; j++) {
		B[0][j] = (1 - t) * B[0][j - 1];
		for(int i = 1; i < j; i++) {
			B[i][j] = (1 - t) * B[i][j - 1] + t * B[i - 1][j - 1];
		}
		B[j][j] = t * B[j - 1][j - 1];
	}

	return B;
}

void Bezier::delB(int n, double ** B) {
	for(int i = 0; i <= n; i++) {
		delete[] B[i];
	}
	delete B;
}

Point Bezier::rotateP(Point P, double v) {
	Point sp = P - startpoint;
	return startpoint + sp.rotate(startaxis, v);
}

Point Bezier::getP(double u, double v) {
	Point P;
	double** B = getB(n, u);

	for(int i = 0; i <= n; i++) {
		P += points[i] * B[i][n];
	}
	delB(n, B);
	return rotateP(P, v);
}

Point Bezier::dPu(double u, double v) {
	Point Pu;
	double** Bu = getB(n, u);
	for(int i = 0; i <= n; i++) {
		double dB = 0;
		if(i - 1 >= 0) {
			dB += n * Bu[i - 1][n - 1];
		}
		if(i <= n - 1) {
			dB -= n * Bu[i][n - 1];
		}
		Pu += points[i] * dB;
	}
	delB(n, Bu);
	return Pu.rotate(startaxis, v);
}

Ray Bezier::dv(Point origin, Ray axis, double theta) {
	Ray result;
	double cost = cos(theta);
	double sint = sin(theta);
	result.x += origin.x * ((1 - axis.x * axis.x) * -sint);
	result.x += origin.y * (axis.x * axis.y * sint - axis.z * cost);
	result.x += origin.z * (axis.x * axis.z * sint + axis.y * cost);
	result.y += origin.x * (axis.y * axis.x * sint + axis.z * cost);
	result.y += origin.y * ((1 - axis.y * axis.y) *  -sint);
	result.y += origin.z * (axis.y * axis.z * sint - axis.x * cost);
	result.z += origin.x * (axis.z * axis.x * sint - axis.y * cost);
	result.z += origin.y * (axis.z * axis.y * sint + axis.x * cost);
	result.z += origin.z * ((1 - axis.z * axis.z) * -sint);
	return result;

}
Point Bezier::dPv(double u, double v) {
	Point Pv = getP(u, v);
	Point ret = dv((rotateP(Pv, -v) - startpoint), startaxis, v);
	if(ret.x == 0 && ret.y == 0 && ret.z == 0)
		ret = startverticle.rotate(startaxis, v + PI / 2);
	return ret;
}

bool Bezier::newton(Point origin, Ray direction, double u, double v, double t) {
	Point P = getP(u, v);
	Point Pu = dPu(u, v);
	Point Pv = dPv(u, v);
	Point df = P - (origin + direction * t); 
	for(int k = 0; k < 5; k++){
		if(df.M() < ZERO)
			break;
		double D = direction.dot(Pu*Pv);
		t = t + Pu.dot(Pv*(df)) / D;
		u = u + direction.dot(Pv*(df)) / D;
		v = v - direction.dot(Pu*(df)) / D;
		P = getP(u, v);
		Pu = dPu(u, v);
		Pv = dPv(u, v);
		df = P - (origin + direction * t);
	}

	if(df.M() >= ZERO || t < ZERO || u < ZERO || u>1)
		return false;
	if(crash_distance == 0 || crash_distance > t) {
		crash_point = P;
		crash_distance = t;
		crash_normal_vector = (Pv*(Pu)).unit();
		front = direction.dot(crash_normal_vector) < 0;
	}
	return true;
}

bool Bezier::collide(Point origin, Ray direction) {
	crash_distance = 0;
	if(box == NULL) {
		box = new Sphere();
		box->setcenter(startpoint + startaxis * (centermax + centermin) / 2);
		for(int i = 0; i <= n; i++) {
			double d = (points[i] - box->getcenter()).M();
			if(d > r)
				r = d;
		}
		box->setR(r);
	}
	if(!box->collide(origin, direction))
		return false;
	Ray d = box->crash_point - box->getcenter();
	Ray c = startaxis * startaxis.dot(d);
	Ray p = (d - c).unit();
	double theta = acos(p.dot(startverticle));
	if((p*startverticle).dot(startaxis) < 0)
		theta = 2 * PI - theta;
	bool result = false;
	if(!result) {
		for(double i = 0.0; i <= 4.0; i++) {
			if(newton(origin, direction, 0 + i / 3, theta + PI / 4, box->crash_distance))
				result = true;
			if(newton(origin, direction, 0 + i / 3, theta - PI / 4, box->crash_distance))
				result = true;
			if(result)
				continue;
			if(newton(origin, direction, 0 + i / 3, theta + 3 * PI / 4, box->crash_distance))
				result = true;
			if(newton(origin, direction, 0 + i / 3, theta - 3 * PI / 4, box->crash_distance))
				result = true;
		}
	}

	return result;
}
