#include "egl.h"


//// PValue ///////////////////////////////////////////////////

std::list<PEnv*> PValue::scope;



//// Node /////////////////////////////////////////////////////

int Node::next_id = 0;

Node::Node(){
	id = next_id++;
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
	return std::shared_ptr<Line>(new Line(x0, y0, x1, y1));
}



void Line::Draw(DrawState& state){
	if(state.prim != LINES) GD.Begin(LINES);
	V2 s = state.tmat * Start();
	V2 e = state.tmat * End();	
	GD.Vertex2f(ModelToScreen_X(s.x), ModelToScreen_Y(s.y));
	GD.Vertex2f(ModelToScreen_X(e.x), ModelToScreen_Y(e.y));
}	

void Line::Accept(NodeVisitor& v){
	v.Visit_Line(*this);
}	


//// Rectangle ////////////////////////////////////////////////////


Rectangle::Rectangle(float width, float height){;}




//// Group ////////////////////////////////////////////////////

Group::Group(){

}

std::shared_ptr<Group> Group::Make(NPtr n){
	auto g = std::shared_ptr<Group>(new Group());
	if(n) g->Add(n);
	return g;
}

Group::~Group(){
	std::for_each(nodes.begin(), nodes.end(), [](NPtr n){n.reset();});
}

void Group::Draw(DrawState& state){
	std::for_each(nodes.begin(), nodes.end(), [&state](NPtr n){n->Draw(state);});
}

void Group::Accept(NodeVisitor& v){
	v.Visit_Group(*this);
	std::for_each(nodes.begin(), nodes.end(), [&v](NPtr n){n->Accept(v);});
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
	if(n) t->Add(n);
	return t;  
}

void Transform::Draw(DrawState& state){
	state.tmat *= tmat;
	Group::Draw(state);
}

void Transform::Accept(NodeVisitor& v){
	v.Visit_Transform(*this);
	Group::Accept(v);
}


//// Parametric ////////////////////////////////////////////////////

Parametric::Parametric(){

}

Parametric::~Parametric(){

}

std::shared_ptr<Parametric> Parametric::Make(NPtr n){
	auto p = std::shared_ptr<Parametric>(new Parametric());
	if(n) p->Add(n);
	return p;
}

void Parametric::Draw(DrawState& state){
	PValue::PushScope(&env);
	Group::Draw(state);
	PValue::PopScope();
}

void Parametric::Accept(NodeVisitor& v){
	v.Visit_Parametric(*this);
	Group::Accept(v);
}

Parametric&	Parametric::Set(const std::string& p, int v){
	env.PInts[Key(p)] = v;
	return *this;
}

Parametric&	Parametric::Set(const std::string& p, float v){
	env.PFloats[Key(p)] = v;
	return *this;
}

Parametric&	Parametric::Set(const std::string& p, std::string& v){
	env.PStrings[Key(p)] = v;
	return *this;
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










int ModelToScreen_X(float x){return (int)(((x * 16) + 0.5) + (GD.w * 8));}
int ModelToScreen_Y(float y){return (int)(((-y * 16) + 0.5) + (GD.h * 8));}