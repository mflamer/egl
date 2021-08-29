#include "egl.h"


//// PValue ///////////////////////////////////////////////////

int ptype::frame_cnt = 0;



//// Node /////////////////////////////////////////////////////

int Node::next_id = 0;

Node::Node(){
	id = next_id++;
}

Node::~Node(){

}

std::shared_ptr<Node> Node::Find(int _id){
	if(_id == id) return std::shared_ptr<Node>(this);
	return NULL;
}

//// Line /////////////////////////////////////////////////////

Line::Line(float x0, float y0, float x1, float y1): start(V2(x0, y0)), end(V2(x1, y1)){

}

std::shared_ptr<Line> Line::Make(float x0, float y0, float x1, float y1){
	return std::shared_ptr<Line>(new Line(x0, y0, x1, y1));
}



void Line::Draw(DrawState& state){
	if(state.prim != LINES) GD.Begin(LINES);
	V2 s = state.tmat * start;
	V2 e = state.tmat * end;	
	Serial.print(end.x); Serial.print("\n"); Serial.print(end.y); Serial.print("\n");
	Serial.print(e.x); Serial.print("\n"); Serial.print(e.y); Serial.print("\n");
	GD.Vertex2f(ModelToScreen_X(s.x), ModelToScreen_Y(s.y));
	GD.Vertex2f(ModelToScreen_X(e.x), ModelToScreen_Y(e.y));
}	

void Line::Accept(NodeVisitor& v){
	v.Visit_Line(*this);
}	

//// Group ////////////////////////////////////////////////////

Group::Group(){

}

std::shared_ptr<Group> Group::Make(std::shared_ptr<Node> n){
	auto g = std::shared_ptr<Group>(new Group());
	g->Add(n);
	return g;
}

Group::~Group(){
	std::for_each(nodes.begin(), nodes.end(), [](std::shared_ptr<Node> n){n.reset();});
}

void Group::Draw(DrawState& state){
	std::for_each(nodes.begin(), nodes.end(), [&state](std::shared_ptr<Node> n){n->Draw(state);});
}

void Group::Accept(NodeVisitor& v){
	v.Visit_Group(*this);
	std::for_each(nodes.begin(), nodes.end(), [&v](std::shared_ptr<Node> n){n->Accept(v);});
}

Group& Group::Add(std::shared_ptr<Node> n){
	if(n) nodes.push_front(n);
	return *this;
}

void Group::Remove(std::shared_ptr<Node> n){

}

std::shared_ptr<Node> Group::Find(int _id){
	if(_id == id) return std::shared_ptr<Node>(this);	
	std::shared_ptr<Node> found = NULL;
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

std::shared_ptr<Transform> Transform::Make(TMat2& m, std::shared_ptr<Node> n){
	auto t = std::shared_ptr<Transform>(new Transform());
	t->Matrix() = m;
	t->Add(n);
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




















int ModelToScreen_X(float x){return (int)(((x * 16) + 0.5) + (GD.w * 8));}
int ModelToScreen_Y(float y){return (int)(((-y * 16) + 0.5) + (GD.h * 8));}