#include <SPI.h>
#include <GD2.h>
#include <vector>
#include <algorithm>
#include <string>
#include <list>
#include <map>
#include <memory>
#include "mlib.h"


union value {		
		int   vi;		
	    float vf;
	    std::string* vs;
	};

class Key{
	int	h;	
public:
	Key(const std::string& s);	
	Key(value v){h = v.vi;}
	bool operator<(const Key& rhs)const{return h < rhs.h;}
	operator value(){value v = {h}; return v;}
};

class PEnv{
public:
	std::map< Key, int > PInts;
	std::map< Key, float > PFloats;
	std::map< Key, std::string > PStrings;
};

class PValue{
protected:	
	static std::list<PEnv*> scope;	
	
public:
	virtual ~PValue() = 0;
	static void 	PushScope(PEnv* s){scope.push_front(s);}
	static void		PopScope();
};

class PInt : public PValue{
	value x;
	bool is_param;
public:
	PInt(int v){x.vi = v; is_param = false;}
	PInt(const std::string& s){x = Key(s); is_param = true;}
	operator 	int(){return is_param ? scope.front()->PInts[Key(x)] : x.vi;}	
};

class PFloat : public PValue{
	value x;
	bool is_param;
public:
	PFloat(float v){x.vf = v; is_param = false;}
	PFloat(const char* s){x = Key(s); is_param = true;}
	operator 	float(){return is_param ? scope.front()->PFloats[Key(x)] : x.vf;}	
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

typedef std::shared_ptr<Node> NPtr;


class Group : public Node{
public:
	Group();
	~Group();
	static std::shared_ptr<Group>	Make(NPtr n = NULL);		
	virtual	void					Draw(DrawState& state);
	virtual void					Accept(NodeVisitor& v);
	Group&							Add(NPtr n);
	void							Remove(NPtr n);
	NPtr							Find(int _id);

protected:	
	std::list<NPtr> nodes;
};


class Transform : public Group{
public:
	Transform();
	~Transform();
	static std::shared_ptr<Transform> 	Make(TMat2& m, NPtr n = NULL);
	virtual	void						Draw(DrawState& state);
	virtual void						Accept(NodeVisitor& v);
	TMat2&								Matrix(){return tmat;}

protected:
	TMat2 tmat;
};


class Parametric : public Group{
	public:
	Parametric();
	~Parametric();
	static std::shared_ptr<Parametric> 	Make(NPtr n = NULL);
	virtual	void						Draw(DrawState& state);
	virtual void						Accept(NodeVisitor& v);
	Parametric&							Set(const std::string& p, int v);
	Parametric&							Set(const std::string& p, float v);
	Parametric&							Set(const std::string& p, std::string& v);

protected:
	PEnv env;
};


class Line : public Node{
public:	
	Line(PFloat x0, PFloat y0, PFloat x1, PFloat y1);
	~Line(){}
	static std::shared_ptr<Line>    Make(PFloat x0, PFloat y0, PFloat x1, PFloat y1);
	virtual	void 					Draw(DrawState& state);	
	virtual void					Accept(NodeVisitor& v);
	V2								Start(){return V2(x0, y0);}
	V2								End(){return V2(x1, y1);}
protected:
	PFloat x0, y0, x1, y1;	
};

class Rectangle : public Group{
public:
	Rectangle(float width, float height);

private:	

};



int ModelToScreen_X(float x);
int ModelToScreen_Y(float y);










//// visitor 

class NodeVisitor{
public:
	virtual void Visit_Group(Group& g){;}
	virtual void Visit_Transform(Transform& t){;}
	virtual void Visit_Parametric(Parametric& p){;}
	virtual void Visit_Line(Line& l){;}
};

