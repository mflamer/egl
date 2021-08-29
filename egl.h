#include <SPI.h>
#include <GD2.h>
#include <vector>
#include <algorithm>
#include <string>
#include <list>
#include <map>
#include <memory>
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
	virtual	void 					Draw(DrawState& state)=0;
	virtual int						ID(){return id;}
	virtual	std::shared_ptr<Node>	Find(int _id);
	virtual void					Accept(NodeVisitor& v)=0;

protected:
	int			id;
	static int  next_id;
};


class Group : public Node{
public:
	Group();
	~Group();
	static std::shared_ptr<Group>	Make(std::shared_ptr<Node> n);		
	virtual	void					Draw(DrawState& state);
	virtual void					Accept(NodeVisitor& v);
	Group&							Add(std::shared_ptr<Node> n);
	void							Remove(std::shared_ptr<Node> n);
	std::shared_ptr<Node>			Find(int _id);

protected:	
	
	std::list<std::shared_ptr<Node>> nodes;
};


class Transform : public Group{
public:
	Transform();
	Transform(std::shared_ptr<Node> n);
	~Transform();
	static std::shared_ptr<Transform> Make(TMat2& m, std::shared_ptr<Node> n);
	virtual	void					Draw(DrawState& state);
	virtual void					Accept(NodeVisitor& v);
	TMat2&							Matrix(){return tmat;}

protected:
	TMat2 tmat;
};


class Line : public Node{
public:	
	Line(float x0, float y0, float x1, float y1);
	~Line(){}
	static std::shared_ptr<Line>    Make(float x0, float y0, float x1, float y1);
	virtual	void 					Draw(DrawState& state);	
	virtual void					Accept(NodeVisitor& v);
protected:
	V2 start, end;	
};



int ModelToScreen_X(float x);
int ModelToScreen_Y(float y);










//// visitor 

class NodeVisitor{
public:
	virtual void Visit_Group(Group& g){;}
	virtual void Visit_Transform(Transform& t){;}
	virtual void Visit_Line(Line& l){;}
};

