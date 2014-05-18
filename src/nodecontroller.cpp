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

void nodecontroller::draw()
{
	auto it = nodeset.begin();
	for( ;it != nodeset.end(); it++)
		it->second.draw();
	gl::popMatrices();
}

void nodecontroller::update()
{
	//�����Ȳ�ʹ�ö�ʱ������ǿ��ÿ��ϵͳupdateʱ���������Ҫ����
	//�Ժ�ʵ�ֶ�ʱ��������ÿ�ζ�Ҫ��������
	eye *e = eye::get();
	ci::Vec2f p = ci::Vec2f(max(0.0, e->pos.x - e->pos.z*sqrt(2.0)/2),
							max(0.0, e->pos.y - e->pos.z*sqrt(2.0)/2));
	int xth, yth;
	getxy(e->scale, p, xth, yth);
	comm *io = comm::get();
	/*XXX ̫�ֱ������������������delicate������
	*/
	nodeset.clear();
	io->getNodes(this->nodeset,static_cast<int>(ceil(e->scale)),xth,yth);
	
}