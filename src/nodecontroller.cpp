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
	if (id == nodecontroller::get()->selected_node_id) 
		gl::color(1.0f,1.0f,1.0f);
	if (id == -2)
		gl::color(1.0f,0.5f,0.5f);
	else 	
		gl::color(1.0f*weight,0.0f,1.0f*(1-weight));
		
	int win_width = config::get()->win_width;
	int win_height = config::get()->win_height;
	float radius = (float)500/(nodecontroller::get()->getRange());
	radius = constrain(radius,0.00001f,0.0005f);
	/*ci::Rectf r = ci::Rectf( (pos.x - radius)*win_width, (pos.y-radius)*win_height,
							(pos.x+radius)*win_width, (pos.y+radius)*win_height);
	gl::drawSolidRect(r);*/
	glRectf((pos.x - radius)*win_height, (pos.y-radius)*win_height,
							(pos.x+radius)*win_height, (pos.y+radius)*win_height);
}

void nodecontroller::drawEdge()
{
	gl::color(1.0f,1.0f,1.0f,0.8);
	auto it = edgeset.begin();
	node n1,n2;
	//glEnable(GL_LINE_STIPPLE);
	glLineStipple(3,0x5555);
	for ( ; it != edgeset.end(); it++) {
		n1 = nodeset[it->first.first];
		n2 = nodeset[it->first.second];
		gl::drawLine(n1.pos*CONFIG(win_height),n2.pos*CONFIG(win_height));
	}
	//glDisable(GL_LINE_STIPPLE);
}
void nodecontroller::draw()
{
	glMatrixMode(GL_PROJECTION);
    //glPushMatrix();
    glLoadIdentity();
	float ex = (eye::get()->pos.x)*CONFIG(win_width);
	float ey = (eye::get()->pos.y)*CONFIG(win_height);
	float scale_x = CONFIG(win_width)/2/pow(2,eye::get()->scale);
	float scale_y = CONFIG(win_height)/2/pow(2,eye::get()->scale);
	//草泥马！！！！！！！！！！
	//glOrtho(ex-x_scale,ex+x_scale,ey+scale_7,ey-y_scale,-1.0f,1.0f);
	ci::CameraOrtho cam( ex-scale_x,ex+scale_x,ey+scale_y,ey-scale_y, -1.0, 1.0 );
	ci::gl::setMatrices( cam );
	drawEdge();
	auto it = nodeset.begin();
	for( ;it != nodeset.end(); it++)
		it->second.draw();
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
	int xth, yth,_range;
	getxy(e->scale, p, xth, yth);
	/*XXX 太粗暴的做法，等下想想更delicate的做法
	*/
	//nodeset.clear();
	if (e->scale <= 0.0f)
		io->getNodes(this->nodeset,0,xth,yth,_range);
	else
		io->getNodes(this->nodeset,static_cast<int>(ceil(e->scale)),xth,yth,_range);
	if (last_range == -1)
		last_range = _range;
	else if(_range != last_range) {
		e->scale += 1;
		last_range = _range;
	}
}

void nodecontroller::getSelectNode(float mx,float my)
{
	// find the closest node and close enough 
	auto sit = nodeset.end();
	float mindis = 1.0f;
	float _dis;
	Vec2i win = app::AppNative::get()->getWindowSize();
	mx /= win.x;
	my /= win.y;
	for (auto it = nodeset.begin(); it != nodeset.end(); it++) 
		if ( (_dis=sqrt( pow((mx-(it->second.pos.x)),2)+ pow((my-(it->second.pos.y)),2) )) <mindis) {
			mindis = _dis;
			sit = it;
		}
	if (sit != nodeset.end() && mindis < 0.005)
		selected_node_id = sit->second.id;
	node n;
	n.set(-2,Vec2f(mx/CONFIG(win_width),my/CONFIG(win_height)),3);
	addnode(n);
}

void nodecontroller::updateSelectedNode(float x,float y) 
{
	if (selected_node_id != -1) {
		 nodeset[selected_node_id].pos.x = x/CONFIG(win_width);
		 nodeset[selected_node_id].pos.y = y/CONFIG(win_height);
	}
}
