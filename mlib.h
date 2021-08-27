#include "math.h"

#define degToRad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)
#define radToDeg(angleInRadians) ((angleInRadians) * 180.0 / M_PI)

struct V2{
			V2(){;}
			V2(float x, float y): x(x), y(y){}
	V2	  	Unit(){float m = Mag(); return V2(x / m, y / m);}
	float 	Distance(const V2& v){return sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y));}
	float 	DistanceSq(const V2& v){return (x - v.x) * (x - v.x) + (y - v.y) * (y - v.y);}
	float 	Mag(){return sqrt(x * x + y * y);}
	float 	Dot(const V2& v){return (x * v.x) + (y * v.y);}
	float	operator*(const V2& v){return Dot(v);}
	V2		operator*(float a){return V2(a * x, a * y);}	
	V2		operator-(){return V2(-x, -y);}	

	float x;
	float y;
};



struct TMat2{
	TMat2() : a(1), b(0), x(0), c(0), d(1), y(0){}
	TMat2(float a, float b, float x, float c, float d, float y) : a(a), b(b), x(x), c(c), d(d), y(y){}

	TMat2& 		Translate(float _x, float _y){x += _x; y += _y; return *this;}
	TMat2& 		Rotate(float _a);
	TMat2&		RotateAbout(float _a, float _x, float _y);
	TMat2&		Scale(float _x, float _y);
	TMat2 		Reflect(V2& l);
	TMat2&      operator-();
	V2			operator*(const V2& v){return V2((a * v.x) + (b * v.y) + x, (c * v.x) + (d * v.y) + y);}	
	TMat2&		operator*=(const TMat2& m);


float a, b, x, c, d, y;

};

TMat2 operator*(const TMat2& l, const TMat2& r);	
V2 operator*(const TMat2& m, const V2& v);