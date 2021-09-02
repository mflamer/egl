#include "egl.h"


//// PValue ///////////////////////////////////////////////////

std::list<PEnv*> PValue::scope;



//// Node /////////////////////////////////////////////////////

int Node::next_id = 0;

Node::Node(): id(0xFFFFFFFF){
}

Node::~Node(){

}

NPtr Node::Find(int _id){
	if(_id == id) return NPtr(this);
	return NULL;
}

//// Line /////////////////////////////////////////////////////

Line::Line(PFloat x0, PFloat y0, PFloat x1, PFloat y1): x0(x0), y0(y0), x1(x1), y1(y1){

}

std::shared_ptr<Line> Line::Make(PFloat x0, PFloat y0, PFloat x1, PFloat y1){
	auto l = std::shared_ptr<Line>(new Line(x0, y0, x1, y1));
	l->Mark();
	return l;
}

void Line::Draw(){	
	Serial.print("Line.Draw "); Serial.print(ID()); Serial.print("\n");
	GD.Vertex2f(ModelToScreen_X(x0), ModelToScreen_Y(y0));
	GD.Vertex2f(ModelToScreen_X(x1), ModelToScreen_Y(y1));
}	

void Line::Accept(NodeVisitor& v){
	Serial.print(" Line _ Accept \n");
	v.Visit_Line(*this);
}	


//// Rectangle ////////////////////////////////////////////////////


Rectangle::Rectangle(PFloat width, PFloat height){

}




//// Group ////////////////////////////////////////////////////

Group::Group(int id){

}

std::shared_ptr<Group> Group::Make(NPtr n){
	auto g = std::shared_ptr<Group>(new Group());
	g->Mark();
	if(n) g->Add(n);
	return g;
}

Group::~Group(){
	std::for_each(nodes.begin(), nodes.end(), [](NPtr n){n.reset();});
}

void Group::Draw(){
	std::for_each(nodes.begin(), nodes.end(), [](NPtr n){n->Draw();});
}

void Group::Accept(NodeVisitor& v){
	Serial.print("Group - Accepted \n");
	v.Visit_Group(*this);	
}

Group& Group::Add(NPtr n){
	if(n) nodes.push_front(n);
	return *this;
}

void Group::Remove(NPtr n){

}

NPtr Group::Find(int _id){
	if(_id == id) return NPtr(this);	
	NPtr found = NULL;
	for(auto n = nodes.begin(); n != nodes.end(); n++){ 		
		if(found = (*n)->Find(_id)) break;
	}
	return found;	
}


//// Transform ////////////////////////////////////////////////////

Transform::Transform(){

}

Transform::~Transform(){

}

std::shared_ptr<Transform> Transform::Make(TMat2& m, NPtr n){
	auto t = std::shared_ptr<Transform>(new Transform());
	t->Matrix() = m;
	t->Mark();
	if(n) t->Add(n);
	return t;  
}

void Transform::Accept(NodeVisitor& v){
	Serial.print("Transform - Accepted \n");
	v.Visit_Transform(*this);
}


//// Parametric ////////////////////////////////////////////////////

Parametric::Parametric(){

}

Parametric::~Parametric(){

}

std::shared_ptr<Parametric> Parametric::Make(NPtr n){
	auto p = std::shared_ptr<Parametric>(new Parametric());
	p->Mark();
	if(n) p->Add(n);
	return p;
}

void Parametric::Draw(){
	
}

void Parametric::Accept(NodeVisitor& v){
	Serial.print("Parametric - Accepted \n");
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
	Serial.print("Attributes - Normal Constructor \n");
	layer_set =false; color_set = false;
	tag_set = false; line_width_set = false; point_size_set = false;
}

Attributes::Attributes(const Attributes& copy){
	Serial.print("Attributes - Copy Constructor \n");
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
	p->Mark();
	if(n) p->Add(n);
	Serial.print(" Make Attributs ID = "); Serial.print(p->ID()); Serial.print("\n");
	return p;
}

void Attributes::Accept(NodeVisitor& v){
	Serial.print("Attributes - Accepted \n");
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
	Serial.print(" Attributes == \n");
	return 	layer == rhs.layer &&
			color == rhs.color &&
			tag == rhs.tag &&
			line_width == rhs.line_width &&
			point_size == rhs.point_size;
}


//// Batch ////////////////////////////////////////////////////

BatchPass::BatchPass(){
	Astk.push_front(Attributes());
	Tstk.push_front(TMat2());
}	

void BatchPass::Visit_Group(Group& grp){
	Serial.print(" Visit - Group, Children = "); Serial.print(grp.Children()); Serial.print("\n");
	std::for_each(grp.Nodes().begin(), grp.Nodes().end(), [this](NPtr n){n->Accept(*this);});
}

void BatchPass::Visit_Transform(Transform& n){	
	Serial.print(" Visit - Transform \n");
	TMat2 m = Tstk.front() *= n.Matrix();
	Tstk.push_front(m);
	Visit_Group(n);
	Tstk.pop_front();
}

void BatchPass::Visit_Attributes(Attributes& n){
	Serial.print(" Visit - Attributes \n");	
	Astk.push_front(Astk.front().CopyWithSetAttributes(n));
	Visit_Group(n);
	Astk.pop_front();
}

// void BatchPass::Visit_Batch(Batch& n){

// }

void BatchPass::Visit_Parametric(Parametric& n){
	Serial.print(" Visit - Parametric \n");
	PValue::PushScope(&(n.Env()));
	Visit_Group(n);
	PValue::PopScope();
	
}

void BatchPass::Visit_Line(Line& n){
	Serial.print(" Visit - Line \n");
	V2 s = Tstk.front() * n.Start();
	V2 e = Tstk.front() * n.End();	
	auto l = Line::Make(s.x, s.y, e.x, e.y);
	FindBatchOrMakeNewAndAdd(LINES, l);
}

void BatchPass::Visit_Rectangle(Rectangle& n){

}

void BatchPass::FindBatchOrMakeNewAndAdd(Primitive prim, NPtr n){
	Serial.print(" FindBatchOrMakeNewAndAdd "); Serial.print(n->ID()); Serial.print("\n");
	bool found = false;
	for(auto itr = batches.begin(); itr != batches.end(); itr++){
		Serial.print("Try match Batch "); Serial.print(itr->prim); Serial.print("\n");
		if(itr->prim == prim && ((Attributes)(*itr) == *Astk.begin())){
			Serial.print("Found match "); Serial.print(itr->ID()); Serial.print("\n");
			itr->Add(n);
			found = true; break;
		}
	}
	if(!found){
		// no matching batch, make a new one and add node
		Serial.print(" Astk # "); Serial.print(Astk.size()); Serial.print("\n");
		Batch b(*Astk.begin(), prim);	
		batches.push_front(b);	
		batches.front().Add(n);
		Serial.print(" Make New Batch, Primitive = "); Serial.print(prim); Serial.print("\n");
		
	}
}

void BatchPass::Draw(){
	Serial.print("BatchPass.Draw "); Serial.print(batches.size()); Serial.print("\n");
	Batch* last_batch = NULL;
	for(auto batch = batches.begin(); batch != batches.end(); batch++){
		Serial.print("Try to draw batch "); Serial.print(batch->ID()); Serial.print("\n");
		batch->Draw(last_batch);
		last_batch = &(*batch);
	}
}

void BatchPass::Batch::Accept(NodeVisitor& v){
	Serial.print("Batch.Accept Error? \n");
}

void BatchPass::Batch::Draw(Batch* last_batch){
	if(last_batch == NULL){
		GD.Begin(prim);
		GD.ColorRGB(color.rgb());
		// finish these
	}
	else{
		if(prim != last_batch->prim) GD.Begin(prim);
		if(color != last_batch->color) GD.ColorRGB(color.rgb());
		// finish these
	}	
	
	Serial.print("Batch.Draw "); Serial.print(nodes.size()); Serial.print("\n");
	std::for_each(nodes.begin(), nodes.end(), [](NPtr n){n->Draw();});	
}

bool BatchPass::Batch::operator==(const Batch& rhs){
	return prim == rhs.prim && ((Attributes)(*this) == (Attributes&)(rhs));
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