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



//// Group ////////////////////////////////////////////////////

Group::Group(){

}

Group::~Group(){
	std::for_each(nodes.begin(), nodes.end(), [](Node* n){delete n;});
}

void Group::Update(){
	std::for_each(nodes.begin(), nodes.end(), [](Node* n){n->Update();});
}

void Group::Accept(NodeVisitor& v){
	v.Visit_Group(*this);
	std::for_each(nodes.begin(), nodes.end(), [&v](Node* n){n->Accept(v);});
}