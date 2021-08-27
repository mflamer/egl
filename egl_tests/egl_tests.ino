#include <SPI.h>
#include <GD2.h>
#include "egl.h"


void setup() {
	GD.begin();

	GD.BitmapHandle(0);
	GD.cmd_loadimage(0, 0);
	GD.load("healsky3.jpg");
	
}




void loop()
{
	GD.ClearColorRGB(0xFFA07A);
	GD.Clear();

	//GD.cmd_translate(F16(100), F16(100));
	//GD.cmd_rotate(DEGREES(45));
	//GD.cmd_translate(F16(-64), F16(-64));
	//GD.cmd_setmatrix();
	//GD.BitmapSize(BILINEAR, BORDER, BORDER, 200, 200);
	//GD.Begin(BITMAPS);
	//GD.Vertex2ii(140, 36, 0); // handle 1: healsky3

	DrawState state;
	//state.tmat.Rotate(45);
	state.tmat.Translate(50, 50);
	Line l = Line(-100, 50, 100, -50);
	l.Draw(state);

	GD.swap();
}

