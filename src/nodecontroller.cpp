#include "nodecontroller.h"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"
#include "eye.h"
#include "comm.h"
#include "config.h"
#include "cinder\app\AppNative.h"
#include <map>

#define min(x,y) (x)<=(y)?(x):(y)
#define max(x,y) (x)>=(y)?(x):(y)
static void getxy(float _scale, Vec2f p, int &x, int &y)
{
	int scale = (int)ceil(_scale);
	//subspace is indexed from zero
	x =(int)( p.x*pow(2.0,scale));
	y =(int) (p.y*pow(2.0,scale));
}
void nodecontroller::addnodes(int nr_node)
{
	node n;
	ci::Vec2f pos;
	for (int i=0;i<nr_node; i++) {
		pos.x = ci::Rand::randFloat(1.0f);
		pos.y = ci::Rand::randFloat(1.0f);
		n.set(i,pos,1);
		addnode(n);
	}
}

void nodecontroller::randomset()
{
	std::map<int,node>::iterator it=nodeset.begin();
	for( ;it != nodeset.end(); it++) {
		it->second.pos.x = ci::Rand::randFloat(1.0);
		it->second.pos.y = ci::Rand::randFloat(1.0);
	}
}
		
void node::draw()
{
	if (id != -1)
		gl::color(1.0f,1.0f,1.0f);
	else if(id == -1)
		gl::color(1.0f,0.0f,0.0f);
	else 
		gl::color(0.0f,1.0f,0.0f);
	int win_width = config::get()->win_width;
	int win_height = config::get()->win_height;
	float radius = 5/pow(2,eye::get()->scale);
	if (eye::get()->scale<3.3) {
		ci::gl::drawSolidCircle( ci::Vec2f(win_width*pos.x,win_height*pos.y),radius);
	}else {
		ci::Rectf r = ci::Rectf( pos.x*win_width-radius, pos.y*win_height-radius,
							pos.x*win_width+radius, pos.y*win_height+radius);
		gl::drawSolidRect(r);
	}
}

void nodecontroller::drawEdge()
{
	auto it = edgeset.begin();
	node n1,n2;
	for ( ; it != edgeset.end(); it++) {
		n1 = nodeset[it->first.first];
		n2 = nodeset[it->first.second];
		gl::drawLine(n1.pos*CONFIG(win_width),n2.pos*CONFIG(win_height));
	}
}
void nodecontroller::draw()
{
	auto it = nodeset.begin();
	for( ;it != nodeset.end(); it++)
		it->second.draw();
	drawEdge();
	gl::popMatrices();
}

void nodecontroller::update()
{
	//现在先不使用定时器，先强制每次系统update时候，向服务器要数据
	//以后实现定时器，不需每次都要更新数据
	if (is_stoped)
		return;
	eye *e = eye::get();
	comm *io = comm::get();
	ci::Vec2f p = ci::Vec2f(max(0.0, e->pos.x - e->pos.z*sqrt(2.0)/2),
							max(0.0, e->pos.y - e->pos.z*sqrt(2.0)/2));
	int xth, yth;
	getxy(e->scale, p, xth, yth);
	/*XXX 太粗暴的做法，等下想想更delicate的做法
	*/
	nodeset.clear();
	if (e->scale <= 0.0f)
		io->getNodes(this->nodeset,0,xth,yth);
	else
		io->getNodes(this->nodeset,static_cast<int>(ceil(e->scale)),xth,yth);
	
}

void nodecontroller::getSelectNode(float mx,float my)
{
	// find the closest node and close enough 
	auto sit = nodeset.end();
	float dis = 1.0f;
	float _dis;
	for (auto it = nodeset.begin(); it != nodeset.end(); it++) 
		if ( (_dis=sqrt( pow((mx-(it->second.pos.x)),2)+ pow((my-(it->second.pos.y)),2) )) <dis) {
			dis = _dis;
			sit = it;
		}
	if (sit != nodeset.end() && dis < 0.005)
		sit->second.id = -1;
	node n;
	n.set(-2,Vec2f(mx,my),3);
	//addnode(n);
}