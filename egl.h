#include <SPI.h>
#include <GD2.h>
#include <vector>
#include <algorithm>
#include <string>
#include <list>
#include <map>
#include <memory>
#include "mlib.h"

typedef unsigned char U8;

//// color ////////////////////////////////////////////////////

enum Colors {
	// reds
	lightsalmon				= 0xFFA07A,
 	salmon					= 0xFA8072,	
 	darksalmon				= 0xE9967A,	
 	lightcoral				= 0xF08080,	
 	indianred				= 0xCD5C5C,
 	crimson					= 0xDC143C,	
 	firebrick				= 0xB22222,	
 	red						= 0xFF0000,	
 	darkred					= 0x8B0000,
 	// oranges
 	coral					= 0xFF7F50,
 	tomato					= 0xFF6347,	
 	orangered				= 0xFF4500,
 	gold					= 0xFFD700,	
 	orange					= 0xFFA500,	
 	darkorange				= 0xFF8C00,	
 	// yellows
 	lightyellow				= 0xFFFFE0,	
 	lemonchiffon			= 0xFFFACD,	
 	lightgoldenrodyellow	= 0xFAFAD2,
 	papayawhip				= 0xFFEFD5,
 	moccasin				= 0xFFE4B5,
 	peachpuff				= 0xFFDAB9,
 	palegoldenrod			= 0xEEE8AA,
 	khaki					= 0xF0E68C,
 	darkkhaki				= 0xBDB76B,
 	yellow					= 0xFFFF00,
 	// greens
 	awngreen				= 0x7CFC00,
 	chartreuse				= 0x7FFF00,
 	limegreen				= 0x32CD32,
 	lime					= 0x00FF00,
 	forestgreen				= 0x228B22,
 	green					= 0x008000,	
 	darkgreen				= 0x006400,	
 	greenyellow				= 0xADFF2F,	
 	yellowgreen				= 0x9ACD32,	
 	springgreen				= 0x00FF7F,	
 	mediumspringgreen		= 0x00FA9A,	
 	lightgreen				= 0x90EE90,	
 	palegreen				= 0x98FB98,	
 	darkseagreen			= 0x8FBC8F,	
 	mediumseagreen			= 0x3CB371,	
 	seagreen				= 0x2E8B57,
 	olive					= 0x808000,	
 	darkolivegreen			= 0x556B2F,	
 	olivedrab				= 0x6B8E23,	
 	// cyans
 	lightcyan				= 0xE0FFFF,	
 	cyan					= 0x00FFFF,	
 	aqua					= 0x00FFFF,	
 	aquamarine				= 0x7FFFD4,	
 	mediumaquamarine		= 0x66CDAA,	
 	paleturquoise			= 0xAFEEEE,	
 	turquoise				= 0x40E0D0,	
 	mediumturquoise			= 0x48D1CC,	
 	darkturquoise			= 0x00CED1,	
 	lightseagreen			= 0x20B2AA,	
 	cadetblue				= 0x5F9EA0,	
 	darkcyan				= 0x008B8B,	
 	teal					= 0x008080,	
 	// blues
 	powderblue				= 0xB0E0E6,	
 	lightblue				= 0xADD8E6,	
 	lightskyblue			= 0x87CEFA,	
 	skyblue					= 0x87CEEB,	
 	deepskyblue				= 0x00BFFF,	
 	lightsteelblue			= 0xB0C4DE,	
 	dodgerblue				= 0x1E90FF,	
 	cornflowerblue			= 0x6495ED,	
 	steelblue				= 0x4682B4,	
 	royalblue				= 0x4169E1,	
 	blue					= 0x0000FF,	
 	mediumblue				= 0x0000CD,	
 	darkblue				= 0x00008B,	
 	navy					= 0x000080,	
 	midnightblue			= 0x191970,	
 	mediumslateblue			= 0x7B68EE,	
 	slateblue				= 0x6A5ACD,	
 	darkslateblue			= 0x483D8B,	
 	// purples
 	lavender				= 0xE6E6FA,	
 	thistle					= 0xD8BFD8,	
 	plum					= 0xDDA0DD,	
 	violet					= 0xEE82EE,	
 	orchid					= 0xDA70D6,	
 	fuchsia					= 0xFF00FF,	
 	magenta					= 0xFF00FF,	
 	mediumorchid			= 0xBA55D3,	
 	mediumpurple			= 0x9370DB,	
 	blueviolet				= 0x8A2BE2,	
 	darkviolet				= 0x9400D3,	
 	darkorchid				= 0x9932CC,	
 	darkmagenta				= 0x8B008B,	
 	purple					= 0x800080,	
 	indigo					= 0x4B0082	

};

class Color {
	int rgba;
public:
	Color(){rgba = 0xFF000000;}
	Color(U8 r, U8 g, U8 b, U8 a = 255);
	Color(int c){rgba = c;}
	int	rgb(){return rgba & 0x00FFFFFF;}
	int	a(){return (rgba >> 24) & 0xFF;}
	operator int(){return rgba;}
};


//// layer ////////////////////////////////////////////////////

enum Layers: unsigned char{
	Alert 	= 0xF0,
	HUD		= 0xE0,
	UITop   = 0xD0,
	UIMid	= 0xC0,
	UIBase  = 0xB0,
	Layer7	= 0xA0,
	Layer6	= 0x90,
	Layer5	= 0x80,
	Layer4	= 0x70,
	Layer3	= 0x60,
	Layer2	= 0x50,
	Layer1	= 0x40,
	Layer0	= 0x30,
	BkGrd3	= 0x20,
	BkGrd2	= 0x10,
	BkGrd1	= 0x00
};

class Layer {
	unsigned char layer; 
public:
	Layer(){layer = Layers::Layer1;}
	Layer(Layers l): layer(l){}
	Layer& MoveUP(unsigned char d = 1);
	Layer& MoveDN(unsigned char d = 1);
	Layer& MoveTop();
	Layer& MoveBot();		
	Layer& Set(Layers l, unsigned char sub = 0);
	bool operator==(Layer& rhs){return layer == rhs.layer;}

};


//// parametric environment ///////////////////////////////////


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
	PFloat(int v){x.vf = (float)v; is_param = false;}
	PFloat(float v){x.vf = v; is_param = false;}
	PFloat(const char* s){x = Key(s); is_param = true;}
	operator 	float(){return is_param ? scope.front()->PFloats[Key(x)] : x.vf;}	
};

class PColor : public PValue{
	value x;
	bool is_param;
public:
	PColor(Color v){x.vi = v; is_param = false;}
	PColor(const std::string& s){x = Key(s); is_param = true;}
	operator 	Color(){return Color(is_param ? scope.front()->PInts[Key(x)] : x.vi);}	
};



//// node heirachy ////////////////////////////////////////////

class NodeVisitor;


class Node{
public:
	virtual	~Node()=0;
	virtual	void 					Draw()=0;  
	virtual	void 					Update(){}					
	virtual void					Accept(NodeVisitor& v)=0;
protected:
	Node();

};

typedef std::shared_ptr<Node> NPtr;


class Group : public Node{
public:		
	~Group();
	static std::shared_ptr<Group>	Make(NPtr n = NULL);		
	virtual	void					Draw();
	virtual	void 					Update();	
	virtual void					Accept(NodeVisitor& v);
	Group&							Add(NPtr n);
	void							Clear();
	//void							Remove(NPtr n);
	std::list<NPtr>&				Nodes(){return nodes;}

protected:	
	Group(){}
	std::list<NPtr> nodes;
};


class Transform : public Group{
public:	
	~Transform();
	static std::shared_ptr<Transform> 	Make(TMat2& m, NPtr n = NULL);
	virtual void						Accept(NodeVisitor& v);
	TMat2&								Matrix(){return tmat;}

protected:
	Transform();
	TMat2 tmat;
};

class Attributes : public Group{
public:	
	Attributes();
	Attributes(const Attributes& copy);
	~Attributes();
	static std::shared_ptr<Attributes> 	Make(NPtr n = NULL);
	virtual void						Accept(NodeVisitor& v);
	Attributes&							SetLayer(Layer l);
	Attributes&							SetColor(Color c);
	Attributes&							SetTag(unsigned char t);
	Attributes&							SetLineWidth(uint16_t lw);
	Attributes&							SetPointSize(uint16_t ps);
	Attributes							CopyWithSetAttributes(const Attributes& orig);
	bool								operator==(Attributes& rhs);

protected:

	Layer			layer;
	Color 			color;
	unsigned char 	tag;
	uint16_t 		line_width;
	uint16_t		point_size;

	bool			layer_set;
	bool			color_set;
	bool			tag_set;
	bool			line_width_set;
	bool			point_size_set;
};



class Parametric : public Group{
	public:
	Parametric();
	~Parametric();
	static std::shared_ptr<Parametric> 	Make(NPtr n = NULL);
	virtual	void						Draw();
	virtual void						Accept(NodeVisitor& v);
	PEnv&								Env(){return env;}	
	Parametric&							SetI(const std::string& p, int v);
	Parametric&							SetF(const std::string& p, float v);
	Parametric&							SetS(const std::string& p, std::string& v);

protected:
	PEnv env;
};


class Line : public Node{
public:		
	~Line(){}
	static std::shared_ptr<Line>    Make(PFloat x0, PFloat y0, PFloat x1, PFloat y1);
	virtual	void 					Draw();	
	virtual void					Accept(NodeVisitor& v);
	V2								Start(){return V2(x0, y0);}
	V2								End(){return V2(x1, y1);}
protected:
	Line(PFloat x0, PFloat y0, PFloat x1, PFloat y1);

	PFloat x0, y0, x1, y1;	
};

class Rectangle : public Group{
public:	
	static std::shared_ptr<Rectangle>   Make(PFloat w, PFloat h);	
	virtual	void 						Update();
protected:
	Rectangle(PFloat w, PFloat h);
	PFloat w, h;		

};



//// graph visitor //////////////////////////////////////////// 

class NodeVisitor{
public:
	virtual void Visit_Group(Group& n){;}
	virtual void Visit_Transform(Transform& n){;}
	virtual void Visit_Attributes(Attributes& n){;}
	virtual void Visit_Parametric(Parametric& n){;}
	virtual void Visit_Line(Line& n){;}
	virtual void Visit_Rectangle(Rectangle& n){;}
};




class BatchPass : public NodeVisitor{
public:
	BatchPass();
	void 	Visit_Group(Group& n);
	void 	Visit_Transform(Transform& n);
	void 	Visit_Attributes(Attributes& n);
	void 	Visit_Parametric(Parametric& n);
	void 	Visit_Line(Line& n);
	void 	Visit_Rectangle(Rectangle& n);

	void	Draw();

private:
	void FindBatchOrMakeNewAndAdd(Primitive prim, NPtr n);

	class Batch : public Attributes {
	public:	
		Batch(Attributes& attr, Primitive p) : Attributes(attr), prim(p){}
		~Batch(){}
		void						Accept(NodeVisitor& v);
		void						Draw(Batch* last_batch);
		bool						operator==(const Batch& rhs);

		Primitive prim;
	};

	std::list<Batch> 		batches;
	std::list<TMat2> 		Tstk;
	std::list<Attributes>	Astk;

};



//// utility functions ////////////////////////////////////////

int ModelToScreen_X(float x);
int ModelToScreen_Y(float y);

