#pragma once
#include "cinder/Vector.h"
#include "cinder/Color.h"
#include <map>
#include <utility>
#include "eye.h"
typedef std::pair<int,int> nodepair;
class node {
public:
	ci::Vec2f pos;
	int id;
	int num;
	float radius;
	void draw();
	inline void set(int _id, ci::Vec2f _pos, float r) {
		id= _id;
		pos= _pos;
		radius = r;
	};
};

class nodecontroller {
	std::map<int,node> nodeset;
	std::map<nodepair,bool> edgeset;
	bool needImmUpdate;
public:
	typedef std::map<int,node> NS;
	nodecontroller(int nr_node);
	nodecontroller() {};
	inline void addnode(const node &n) {nodeset[n.id]=n; };
	void addnodes(int nr_node);
	inline void addedge(const nodepair &pair) { edgeset[pair]=true;}
	void randomset();
	void draw();
	void update();
	void needUpdateNow();
};

