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
	float weight;
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
	nodecontroller() { last_range = -1; instance = this; selected_node =-1;};
	static nodecontroller * get(){return instance;};
	int getRange() { return last_range;};
	inline void addnode(const node &n) {nodeset[n.id]=n; };
	void addnodes(int nr_node);
	inline void addedge(const nodepair &pair) { edgeset[pair]=true;}
	void randomset();
	void draw();
	void drawEdge();
	void update();
	void needUpdateNow();
	void getSelectNode(float mx,float my);
	int selected_node;
	void swi(){ is_stoped = !is_stoped;};
	inline bool is_start() { return !is_stoped;};
	inline void start() {is_stoped = false;};
private:
	static nodecontroller *instance;
	bool is_stoped;
	int last_range;
};

