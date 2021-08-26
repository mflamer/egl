#include "math.h"

struct V2{
			V2(){;}
			V2(float _x, float _y){x = _x; y = _y;}
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
	TMat2();

	TMat2& 		Translate(float x, float y);
	TMat2& 		Rotate(float a);
	TMat2&		RotateAbout(float a, float x, float y);
	TMat2&		Scale(float x, float y);

	V2			operator*(const V2& v){return V2((v.x * a) + (v.y * b) + x, (v.x * c) + (v.y * d) + y);}	


float a, b, x, c, d, y;

};