#include <SPI.h>
#include <GD2.h>
#include <vector>
#include <algorithm>
#include <string>
#include <list>
#include <map>
#include "mlib.h"


class ptype{
	static int frame_cnt;
public:
	ptype(){frame_cnt = 0;}
	void IncFrame(){frame_cnt++;}
};

class pint : public ptype{
	int i;
public:	
	pint(int _i){i = _i;}
	virtual operator int() const {return i;}
};



class NodeVisitor;

struct DrawState{
	TMat2		tmat;
	Primitive 	prim;
	int			color;
};




class Node{
public:
			Node();
	virtual	~Node()=0;
	virtual	void 		Draw(DrawState& state)=0;
	virtual int			ID(){return id;}
	virtual void		Accept(NodeVisitor& v)=0;

private:
	int			id;
	static int  next_id;
};


class GroupNode : public Node{
public:
	GroupNode();
	~GroupNode()=0;
	virtual	void		Draw(DrawState& state);
	virtual void		Accept(NodeVisitor& v);

private:	
	std::list<Node*> 	nodes;
};

class Transform : public GroupNode{

};


class Line : public Node{
public:	
	Line(float x0, float y0, float x1, float y1);
	~Line(){}
	virtual	void 		Draw(DrawState& state);	
	virtual void		Accept(NodeVisitor& v);
private:
	V2 start, end;	
};



int ModelToScreen_X(float x);
int ModelToScreen_Y(float y);










//// visitor 

class NodeVisitor{
public:
	virtual void Visit_GroupNode(GroupNode& g){;}
};

