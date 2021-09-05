#include "egl.h"


//// PValue ///////////////////////////////////////////////////

std::list<PEnv*> PValue::scope;



//// Node /////////////////////////////////////////////////////


Node::Node(){
}

Node::~Node(){

}


//// Line /////////////////////////////////////////////////////

Line::Line(PFloat x0, PFloat y0, PFloat x1, PFloat y1): x0(x0), y0(y0), x1(x1), y1(y1){

}

std::shared_ptr<Line> Line::Make(PFloat x0, PFloat y0, PFloat x1, PFloat y1){
	auto l = std::shared_ptr<Line>(new Line(x0, y0, x1, y1));
	return l;
}

void Line::Draw(){	
	GD.Vertex2f(ModelToScreen_X(x0), ModelToScreen_Y(y0));
	GD.Vertex2f(ModelToScreen_X(x1), ModelToScreen_Y(y1));
	Serial.print("Vertex "); Serial.print(x0); Serial.print(", "); Serial.print(y0); Serial.print("\n");
	Serial.print("Vertex "); Serial.print(x1); Serial.print(", "); Serial.print(y1); Serial.print("\n");
}	

void Line::DrawToBatch(BatchSet& batchSet){
	V2 s = batchSet.Tstk.front() * V2(x0, y0);
	V2 e = batchSet.Tstk.front() * V2(x1, y1);	
	auto l = Line::Make(s.x, s.y, e.x, e.y);
	batchSet.FindBatchOrMakeNewAndAdd(LINES, l);
}

void Line::Accept(NodeVisitor& v){
	//Serial.print(" Line _ Accept \n");
	v.Visit_Line(*this);
}	


//// Polygon //////////////////////////////////////////////////////

std::shared_ptr<Polygon> Polygon::Make(){
	auto p = std::shared_ptr<Polygon>(new Polygon());
	p->filled = false;
	return p;
}

void Polygon::Draw(){
	if(filled){

	}
	else{
		for(auto itr = verticies.begin(); itr != verticies.end(); itr++){			
			GD.Vertex2f(ModelToScreen_X(itr->x), ModelToScreen_Y(itr->y)); 
			Serial.print("Vertex "); Serial.print(itr->x); Serial.print(", "); Serial.print(itr->y); Serial.print("\n");
		}
	}
}	

void Polygon::DrawToBatch(BatchSet& batchSet){
	auto p = Polygon::Make();
	*p = *this;
	p->TransformVerticies(batchSet.Tstk.front());
	batchSet.FindBatchOrMakeNewAndAdd(LINE_STRIP, p);
}

void Polygon::Accept(NodeVisitor& v){
	v.Visit_Polygon(*this);
}

void Polygon::AddVertex(PFloat x, PFloat y){
	verticies.push_back(PPnt(x,y));
}	

void Polygon::SetFilled(bool f){
	filled = f;
}	

void Polygon::TransformVerticies(const TMat2& mat){
	for(auto itr = verticies.begin(); itr != verticies.end(); itr++){
		*itr = PPnt(mat * V2(*itr)); 
	}
}


//// Rectangle ////////////////////////////////////////////////////

std::shared_ptr<Rectangle> Rectangle::Make(PFloat width, PFloat height, bool filled){
	return std::shared_ptr<Rectangle>(new Rectangle(width, height, filled));
}

void Rectangle::Update(){
	
}

void Rectangle::Draw(){

}

void Rectangle::DrawToBatch(BatchSet& batchSet) {
	if (!filled) {
		auto poly = Polygon::Make();
		poly->AddVertex(0, 0);
		poly->AddVertex(w, 0);
		poly->AddVertex(w, h);
		poly->AddVertex(0, h);
		poly->AddVertex(0, 0);		
		poly->TransformVerticies(batchSet.Tstk.front());
		batchSet.FindBatchOrMakeNewAndAdd(LINE_STRIP, poly);
	}
	else if (abs(batchSet.Tstk.front().a) != 1) { // filled & rotated
		//implement using GD.Poly or a solid bitmap
	}
	else {
		V2 bl = batchSet.Tstk.front() * V2(0, 0);
		V2 tr = batchSet.Tstk.front() * V2(w, h);
		auto r = AAFilledRect::Make(bl, tr);
		batchSet.FindBatchOrMakeNewAndAdd(RECTS, r);
	}
}

//// AAFilledRect /////////////////////////////////////////////


std::shared_ptr<AAFilledRect> AAFilledRect::Make(V2 bl, V2 tr) {
	return std::shared_ptr<AAFilledRect>(new AAFilledRect(bl, tr));
}

void AAFilledRect::Draw() {
	GD.Vertex2f(ModelToScreen_X(bl.x), ModelToScreen_Y(bl.y));
	GD.Vertex2f(ModelToScreen_X(tr.x), ModelToScreen_Y(tr.y));
}
	

//// Group ////////////////////////////////////////////////////

std::shared_ptr<Group> Group::Make(NPtr n){
	auto g = std::shared_ptr<Group>(new Group());
	if(n) g->Add(n);
	return g;
}

Group::~Group(){
	std::for_each(nodes.begin(), nodes.end(), [](NPtr n){n.reset();});
}

void Group::Draw(){
	std::for_each(nodes.begin(), nodes.end(), [](NPtr n){n->Draw();});
}

void Group::Update(){
	std::for_each(nodes.begin(), nodes.end(), [](NPtr n){n->Update();});	
}

void Group::DrawToBatch(BatchSet& batchSet){
	std::for_each(nodes.begin(), nodes.end(), [&batchSet](NPtr n){n->DrawToBatch(batchSet);});
}

void Group::Accept(NodeVisitor& v){
	//Serial.print("Group - Accepted \n");
	v.Visit_Group(*this);	
}

Group& Group::Add(NPtr n){
	if(n) nodes.push_front(n);
	return *this;
}

void Group::Clear(){
	std::for_each(nodes.begin(), nodes.end(), [](NPtr n){n.reset();});
	nodes.clear();
}



//// Transform ////////////////////////////////////////////////////

Transform::Transform(){

}

Transform::~Transform(){

}

std::shared_ptr<Transform> Transform::Make(TMat2& m, NPtr n){
	auto t = std::shared_ptr<Transform>(new Transform());
	t->Matrix() = m;
	if(n) t->Add(n);
	return t;  
}

void Transform::DrawToBatch(BatchSet& batchSet){
	TMat2 m = Matrix() *= batchSet.Tstk.front();
	batchSet.Tstk.push_front(m);
	Group::DrawToBatch(batchSet);
	batchSet.Tstk.pop_front();
}

void Transform::Accept(NodeVisitor& v){
	//Serial.print("Transform - Accepted \n");
	v.Visit_Transform(*this);
}


//// Parametric ////////////////////////////////////////////////////


Parametric::~Parametric(){

}

std::shared_ptr<Parametric> Parametric::Make(NPtr n){
	auto p = std::shared_ptr<Parametric>(new Parametric());
	if(n) p->Add(n);
	return p;
}

void Parametric::DrawToBatch(BatchSet& batchSet){
	PValue::PushScope(&env);
	Group::DrawToBatch(batchSet);
	PValue::PopScope();	
}

void Parametric::Accept(NodeVisitor& v){
	//erial.print("Parametric - Accepted \n");
	v.Visit_Parametric(*this);
}

Parametric&	Parametric::SetI(const std::string& p, int v){
	env.PInts[Key(p)] = v;
	return *this;
}

Parametric&	Parametric::SetF(const std::string& p, float v){
	env.PFloats[Key(p)] = v;
	return *this;
}

Parametric&	Parametric::SetS(const std::string& p, std::string& v){
	env.PStrings[Key(p)] = v;
	return *this;
}


//// Attributes ///////////////////////////////////////////////

Attributes::Attributes(){ 
	//Serial.print("Attributes - Normal Constructor \n");
	layer_set =false; color_set = false;
	tag_set = false; line_width_set = false; point_size_set = false;
	line_width = 16;
	color = Color();
	point_size = 16;
}

Attributes::Attributes(const Attributes& copy){
	//Serial.print("Attributes - Copy Constructor \n");
	layer = copy.layer;
	color = copy.color;
	tag = copy.tag;
	line_width = copy.line_width;
	point_size = copy.point_size;

	layer_set = copy.layer_set;
 	color_set = copy.color_set;
	tag_set = copy.tag_set; 
	line_width_set = copy.line_width_set; 
	point_size_set = copy.point_size_set;

}

Attributes::~Attributes(){

}

std::shared_ptr<Attributes> Attributes::Make(NPtr n){	
	auto p = std::shared_ptr<Attributes>(new Attributes());
	if(n) p->Add(n);
	//Serial.print(" Make Attributs ID = "); Serial.print(p->ID()); Serial.print("\n");
	return p;
}

void Attributes::DrawToBatch(BatchSet& batchSet){
	batchSet.Astk.push_front(batchSet.Astk.front().CopyWithSetAttributes(*this));
	Group::DrawToBatch(batchSet);
	batchSet.Astk.pop_front();
}

void Attributes::Accept(NodeVisitor& v){
	v.Visit_Attributes(*this);
}

Attributes&	Attributes::SetLayer(Layer l){
	layer = l;
	layer_set = true;
	return *this;
}

Attributes&	 Attributes::SetColor(Color c){
	color = c;
	color_set = true;
	return *this;
}

Attributes&	 Attributes::SetTag(unsigned char t){
	tag = t;
	tag_set = true;
	return *this;
}

Attributes&	 Attributes::SetLineWidth(uint16_t lw){
	line_width = lw;
	line_width_set = true;
	return *this;
}

Attributes&	 Attributes::SetPointSize(uint16_t ps){
	point_size = ps;
	point_size_set = true;
	return *this;
}

Attributes Attributes::CopyWithSetAttributes(const Attributes& orig){
	Attributes copy(orig);
	if(layer_set) 		copy.layer = layer;
	if(color_set) 		copy.color = color;
	if(tag_set) 		copy.tag = tag;
	if(line_width_set) 	copy.line_width = line_width;
	if(point_size_set) 	copy.point_size = point_size;
	return copy;
}

bool Attributes::operator==(Attributes& rhs){
	//Serial.print(" Attributes == \n");
	return 	layer == rhs.layer &&
			color == rhs.color &&
			tag == rhs.tag &&
			line_width == rhs.line_width &&
			point_size == rhs.point_size;
}

//// BatchSet ////////////////////////////////////////////////////

BatchSet::BatchSet(){
	Astk.push_front(Attributes());
	Tstk.push_front(TMat2());
}

void BatchSet::FindBatchOrMakeNewAndAdd(Primitive prim, NPtr n){
	//Serial.print(" FindBatchOrMakeNewAndAdd "); Serial.print(n->ID()); Serial.print("\n");
	bool found = false;
	for(auto itr = batches.begin(); itr != batches.end(); itr++){
		//Serial.print("Try match Batch "); Serial.print(itr->prim); Serial.print("\n");
		if(itr->prim == prim && ((Attributes)(*itr) == *Astk.begin())){
			//Serial.print("Found match "); Serial.print(itr->ID()); Serial.print("\n");
			itr->Add(n);
			found = true; break;
		}
	}
	if(!found){
		// no matching batch, make a new one and add node
		//Serial.print(" Astk # "); Serial.print(Astk.size()); Serial.print("\n");
		auto b = Batch(*Astk.begin(), prim);	
		batches.push_front(b);
		batches.front().Add(n);
		//Serial.print(" Make New Batch, Primitive = "); Serial.print(prim); Serial.print("\n");
		
	}
}

void BatchSet::Draw(){
	Serial.print("\n\nBatchPass Draw "); Serial.print(nodes.size()); Serial.print("\n");
	Batch* last_batch = NULL;
	batches.sort();
	for(auto batch = batches.begin(); batch != batches.end(); batch++){
		//Serial.print("Try to draw batch "); Serial.print(batch->ID()); Serial.print("\n");
		batch->Draw(last_batch);
		last_batch = &(*batch);
	}
}

//// Batch ////////////////////////////////////////////////////



std::shared_ptr<Batch> Batch::Make(Attributes& attr, Primitive p) {
	auto b = std::shared_ptr<Batch>(new Batch(attr, p));	
	return b;
}


void Batch::Draw(Batch* last_batch){
	Serial.print("Batch "); Serial.print(prim); Serial.print("\n");
	if(last_batch == NULL){
		GD.Begin(prim);
		Serial.print("Begin "); Serial.print(prim); Serial.print("\n");
		GD.ColorRGB(color.rgb());
		Serial.print("Color "); Serial.print(color.rgb()); Serial.print("\n");
		GD.LineWidth(line_width);
		Serial.print("Line Width "); Serial.print(line_width); Serial.print("\n");
		// finish these
	}
	else{
		if(prim != last_batch->prim) {
			GD.Begin(prim);
			Serial.print("Begin "); Serial.print(prim); Serial.print("\n");
		}
		if(color != last_batch->color){
			GD.ColorRGB(color.rgb());
			Serial.print("Color "); Serial.print(color.rgb()); Serial.print("\n");
		}
		if(line_width != last_batch->line_width){
			GD.LineWidth(line_width);
			Serial.print("Line Width "); Serial.print(line_width); Serial.print("\n");
		}
		// finish these
	}	
	
	std::for_each(nodes.begin(), nodes.end(), [](NPtr n){n->Draw();});	
}

bool Batch::operator==(const Batch& rhs){
	return prim == rhs.prim && ((Attributes)(*this) == (Attributes&)(rhs));
}

bool Batch::operator<(const Batch& rhs) {
	if (layer < rhs.layer) return true;
	if (prim < rhs.prim) return true;
	if (color < rhs.color) return true;
	if (line_width < rhs.line_width) return true;
	if (point_size < rhs.point_size) return true;
	return false;
}


//// PEnv /////////////////////////////////////////////////////

Key::Key(const std::string& s){
	h = 5381;
    for (unsigned int i = 0; i < s.size(); ++i)
        h = 33 * h + (unsigned char)s[i];
}

PValue::~PValue(){}

void PValue::PopScope(){
	if(scope.begin() != scope.end()) scope.pop_front();
}


//// Color ////////////////////////////////////////////////////

Color::Color(U8 r, U8 g, U8 b, U8 a){
	rgba = (a << 24) | (b << 16) | (g << 8) | r;
}




//// Layer ////////////////////////////////////////////////////

Layer& Layer::MoveUP(U8 d){
	layer |= ((layer += d) & 0x0F);
	return *this;
}

Layer& Layer::MoveDN(U8 d){
	layer |= ((layer -= d) & 0x0F);
	return *this;
}

Layer& Layer::MoveTop(){
	layer |= 0x0F;
	return *this;
}

Layer& Layer::MoveBot(){
	layer |= 0x0F;
	return *this;
}	

Layer& Layer::Set(Layers l, unsigned char sub){
	layer = l | (sub & 0x0F);
	return *this;
}



//// utility functions ////////////////////////////////////////

int ModelToScreen_X(float x){return (int)(((x * 16) + 0.5) + (GD.w * 8));}
int ModelToScreen_Y(float y){return (int)(((-y * 16) + 0.5) + (GD.h * 8));}