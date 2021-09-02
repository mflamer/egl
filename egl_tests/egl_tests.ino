#include <SPI.h>
#include <GD2.h>
#include "egl.h"


auto root = Group::Make();;

void setup() {
	GD.begin();

	GD.BitmapHandle(0);
	GD.cmd_loadimage(0, 0);
	GD.load("healsky3.jpg");

	
	//auto p = Parametric::Make();
	//p->SetF("px", 25);
	//p->SetF("py", -50);
	//root.Add(p);

	auto t = Transform::Make(TMat2().Rotate(45), Line::Make(0, 0, 75, 0));
	t->Add(Line::Make(-20, -20, -100, 100));
	
	auto p = Attributes::Make();
	p->Add(Line::Make(20, 20, 100, 100));
	p->SetColor(darkorange);
	t->Add(p);
	root->Add(t);
	
}




void loop()
{
	GD.ClearColorRGB(0xDFA07A);
	GD.Clear();



	//GD.cmd_translate(F16(100), F16(100));
	//GD.cmd_rotate(DEGREES(45));
	//GD.cmd_translate(F16(-64), F16(-64));
	//GD.cmd_setmatrix();
	//GD.BitmapSize(BILINEAR, BORDER, BORDER, 200, 200);
	//GD.Begin(BITMAPS);
	//GD.Vertex2ii(140, 36, 0); // handle 1: healsky3


	//state.tmat.Rotate(135);
	//state.tmat.Translate(50, 50);
	//state.tmat.Scale(1, 2);

	BatchPass batcher;
	root->Accept(batcher);
	batcher.Draw();

	// origin 
	GD.Begin(POINTS);
	GD.ColorRGB(30, 30, 30);
	GD.Vertex2ii(GD.w/2, GD.h/2, 0);
	GD.Vertex2ii(GD.w/2 +50, GD.h/2 +50, 0);

	GD.swap();
}

