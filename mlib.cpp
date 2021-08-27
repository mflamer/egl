#include "mlib.h"

//// V2 class /////////////////////////////////////////////////







//// TMat2 | transformation matrix ////////////////////////////
// c s 0   a b x   w 0 0   a b x  a b x   x                       
//-s c 0 * c d y   0 h 0 * c d y  c d y * y
// 0 0 1   0 0 1   0 0 1   0 0 1  0 0 1   0                      

TMat2& TMat2::operator*=(const TMat2& m){
	a = (a * m.a) + (b * m.c); 
	b = (a * m.b) + (b * m.d); 
	x = (a * m.x) + (b * m.y) + x;
	c = (c * m.a) + (d * m.c);
	d = (c * m.b) + (d * m.d);
	y = (c * m.x) + (d * m.y) + y;  
	return *this;
}

TMat2& TMat2::Rotate(float r){
	r = degToRad(r);
	a = ( cos(r) * a) + (sin(r) * c); 
	b = ( cos(r) * b) + (sin(r) * d); 
	x = ( cos(r) * x) + (sin(r) * y);
	c = (-sin(r) * a) + (cos(r) * c);
	d = (-sin(r) * b) + (cos(r) * d);
	y = (-sin(r) * x) + (cos(r) * y);
	return *this;
}

TMat2& TMat2::RotateAbout(float r, float _x, float _y){
	r = degToRad(r);
	x -= _x; y -= _y;
	Rotate(r);
	x += _x; y += _y;
	return *this;
}

TMat2& TMat2::Scale(float w, float h){
	a = w * a;
	b = w * b;
	x = w * x;
	c = h * c;
	d = h * d;
	y = h * y;
	return *this; 
}

TMat2 TMat2::Reflect(V2& l){
	// implement
	return TMat2();
}


TMat2& TMat2::operator-(){
	// implement
	return *this;
}


TMat2 operator*(const TMat2& l, const TMat2& r){ 
	return TMat2((l.a * r.a) + (l.b * r.c), (l.a * r.b) + (l.b * r.d), (l.a * r.x) + (l.b * r.y) + r.x,
				 (l.c * r.a) + (l.d * r.c), (l.c * r.b) + (l.d * r.d), (l.c * r.x) + (l.d * r.y) + r.y);
}

V2 operator*(const TMat2& m, const V2& v){
	return V2((m.a * v.x) + (m.b * v.y) + m.x, (m.c * v.x) + (m.d * v.y) + m.y);
}