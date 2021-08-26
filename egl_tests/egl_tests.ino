#include <SPI.h>
#include <GD2.h>
#include "egl.h"


void setup() {
	GD.begin();

	GD.BitmapHandle(0);
	GD.cmd_loadimage(0, 0);
	GD.load("healsky3.jpg");

}


static void zigzag(int x)
{
GD.Vertex2ii(x - 10, 10); GD.Vertex2ii(x + 10, 60);
GD.Vertex2ii(x - 10, 110); GD.Vertex2ii(x + 10, 160);
GD.Vertex2ii(x - 10, 210); GD.Vertex2ii(x + 10, 260);
}


void loop()
{
GD.Clear();


GD.cmd_rotate(DEGREES(22.5));
GD.cmd_setmatrix();

GD.Begin(LINES);
zigzag(140);
GD.Begin(LINE_STRIP);
zigzag(240);
GD.LineWidth(16 * 10);
GD.Begin(LINE_STRIP);
zigzag(340);

GD.Begin(BITMAPS);
GD.Vertex2ii(200, 100, 0); // handle 1: healsky3


GD.swap();
}

