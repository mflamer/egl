#include "mlib.h"

//// V2 class /////////////////////////////////////////////////







//// TMat2 | transformation matrix ////////////////////////////
// a b | x
// c d | y


TMat2::TMat2() : a(1), b(0), x(0), c(0), d(1), y(0){}

TMat2& TMat2::Translate(float x, float y){}

TMat2& TMat2::Rotate(float a){}
TMat2& TMat2::RotateAbout(float a, float x, float y){}
TMat2& TMat2::Scale(float x, float y){}