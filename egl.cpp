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

//// Line /////////////////////////////////////////////////////

Line::Line(float x0, float y0, float x1, float y1): start(V2(x0, y0)), end(V2(x1, y1)){

}



void Line::Draw(DrawState& state){
	if(state.prim != LINES) GD.Begin(LINES);
	V2 s = state.tmat * start;
	V2 e = state.tmat * end;
	GD.Vertex2f(ModelToScreen_X(s.x), ModelToScreen_Y(s.y));
	GD.Vertex2f(ModelToScreen_X(e.x), ModelToScreen_Y(e.y));
}	

void Line::Accept(NodeVisitor& v){

}	

//// Group ////////////////////////////////////////////////////

GroupNode::GroupNode(){

}

GroupNode::~GroupNode(){
	std::for_each(nodes.begin(), nodes.end(), [](Node* n){delete n;});
}

void GroupNode::Draw(DrawState& state){
	std::for_each(nodes.begin(), nodes.end(), [&state](Node* n){n->Draw(state);});
}

void GroupNode::Accept(NodeVisitor& v){
	v.Visit_GroupNode(*this);
	std::for_each(nodes.begin(), nodes.end(), [&v](Node* n){n->Accept(v);});
}








int ModelToScreen_X(float x){return (int)(((x * 16) + 0.5) + (GD.w * 8));}
int ModelToScreen_Y(float y){return (int)(((-y * 16) + 0.5) + (GD.h * 8));}