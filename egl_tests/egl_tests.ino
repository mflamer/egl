
#include <SPI.h>
#include <GD2.h>
#include "egl.h"


auto root = Group::Make();;

void setup() {
	GD.begin();

	GD.BitmapHandle(0);
	GD.cmd_loadimage(0, 0);
	GD.load("healsky3.jpg");

	
	auto p = Parametric::Make();
	p->SetF("px", 80);
	p->SetF("py", 20);
	auto a = Attributes::Make();
	a->SetColor(darkorange);
	a->SetLineWidth(48);
	a->SetLayer(UIBase);
	p->Add(a);

	auto r = Rectangle::Make(40, 40, false);



	//root.Add(p);

	auto t = Transform::Make(TMat2().Rotate(45));
	//t->Add(Line::Make(-20, -20, -100, 100));
	a->Add(t);
	t->Add(r);
	
	auto g = Group::Make();
	g->Add(Line::Make(0, 0, "px", 0));
	g->Add(Line::Make("px", 0, "px", "py"));
	g->Add(Line::Make("px", "py", 0, "py"));
	g->Add(Line::Make(0, "py", 0, 0));	

	t->Add(g);

	auto p1 = Parametric::Make();
	p1->SetF("px", 40);
	p1->SetF("py", -20);	

	auto a1 = Attributes::Make();
	a1->SetColor(darkred);
	a1->Add(p1);

	p1->Add(g);

	auto poly = Polygon::Make();
	poly->AddVertex(-100, 0);
	poly->AddVertex(-100, 45);
	poly->AddVertex(-50, -20);
	poly->AddVertex(-20, -30);

	p1->Add(poly);

	auto rect = Rectangle::Make(15, 60, true);
	auto a2 = Attributes::Make();
	a2->SetColor(darkseagreen);
	a2->Add(rect);

	auto butt = Button::Make("test", 1, 100, 75, 40, 20);

	root->Add(p);
	root->Add(a1);
	root->Add(a2);
	root->Add(butt);
	
	
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

	BatchSet batches;
	root->DrawToBatch(batches);
	batches.Draw();

	// origin 
	GD.Begin(POINTS);
	GD.ColorRGB(30, 30, 30);
	GD.Vertex2ii(GD.w/2, GD.h/2, 0);
	GD.Vertex2ii(GD.w/2 +50, GD.h/2 +50, 0);

	GD.swap();
}

