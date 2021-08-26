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

class Node{
public:
			Node();
	virtual	~Node()=0;
	virtual	void 		Update()=0;
	virtual int			ID(){return id;}
	virtual void		Accept(NodeVisitor& v)=0;

private:
	int			id;
	static int  next_id;
};

class Drawable : public Node{
public:

private:
	static Primitive prim_t;

};





class Group : public Node{
public:
	Group();
	~Group();
	void				Update();
	void				Accept(NodeVisitor& v);

private:	
	std::list<Node*> 	nodes;


};

















//// visitor 

class NodeVisitor{
public:
	virtual void Visit_Group(Group& g){;}
};

