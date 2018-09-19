#ifndef COLOR_H
#define COLOR_H
#include<sstream>

class Color {
public:
	double r, g, b;

	Color(double _r = 0, double _g = 0, double _b = 0):r(_r),g(_g),b(_b){}
	~Color(){}

	friend Color operator - (const Color&);
	friend Color operator + (const Color&, const Color&);
	friend Color operator - (const Color&, const Color&);
	friend Color operator * (const Color&, const Color&);
	friend Color operator * (const Color&, const double&);
	friend Color operator / (const Color&, const double&);
	friend Color& operator += (Color&, const Color&);
	friend Color& operator -= (Color&, const Color&);
	friend Color& operator *= (Color&, const Color&);
	friend Color& operator *= (Color&, const double&);
	friend Color& operator /= (Color&, const double&);

	void input(std::stringstream& fin){
		fin >> r >> g >> b;
	}
	void limit();
	Color exp() {
		return Color(std::exp(r), std::exp(g), std::exp(b));
	}
	double intensity(){//π‚«ø
		return (r+g+b)/3;
	}
};

#endif // !COLOR_H

