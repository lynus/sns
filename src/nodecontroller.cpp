#include "nodecontroller.h"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"
#include "eye.h"
#include "comm.h"
#include "cinder\app\AppNative.h"
#include <map>

#define min(x,y) (x)<=(y)?(x):(y)
#define max(x,y) (x)>=(y)?(x):(y)
static void getxy(float _scale, Vec2f p, int &x, int &y)
{
	int scale = (int)ceil(_scale);
	//空间编号从零开始
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
	ci::Vec2i reso = ci::app::AppNative::get()->getWindowSize();
	ci::Vec2f v(reso.x*pos.x,reso.y*pos.y);
	ci::gl::drawSolidCircle( ci::Vec2f(reso.x*pos.x,reso.y*pos.y),3);
	//ci::gl::drawSphere(ci::Vec3f(reso.x*pos.x,reso.y*pos.y,0),5);
	//ci::gl::vertex(pos*900);
}

void nodecontroller::draw()
{
	auto it = nodeset.begin();
	for( ;it != nodeset.end(); it++)
		it->second.draw();
	gl::popMatrices();
}

void nodecontroller::update()
{
	//现在先不使用定时器，先强制每次系统update时候，向服务器要数据
	//以后实现定时器，不需每次都要更新数据
	eye *e = eye::get();
	ci::Vec2f p = ci::Vec2f(max(0.0, e->pos.x - e->pos.z*sqrt(2.0)/2),
							max(0.0, e->pos.y - e->pos.z*sqrt(2.0)/2));
	int xth, yth;
	getxy(e->scale, p, xth, yth);
	comm *io = comm::get();
	/* XXX 太粗暴的做法，等下想想更delicate的做法
	*/
	nodeset.clear();
	io->getNodes(this->nodeset,static_cast<int>(ceil(e->scale)),xth,yth);
	
}