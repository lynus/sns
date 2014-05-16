#include "cinder\Camera.h"
#include "cinder\Vector.h"
#include "cinder\app\AppNative.h"
#include <cmath>
#include <ostream>
#include "eye.h"
#include "cinder\CinderMath.h"

using namespace ci;

#define min(x,y) (x)<=(y)?(x):(y)
#define max(x,y) (x)>=(y)?(x):(y)
static void getxy(float _scale, Vec2f p, int &x, int &y)
{
	int scale = (int)ceil(_scale);
	//空间编号从零开始
	x =(int)( p.x*pow(2.0,scale));
	y =(int) (p.y*pow(2.0,scale));
}
void eye::init(Vec3f _pos,float ratio) 
{
	pos = _pos;
	scale= -log(sqrt(2.0)*pos.z)/log(2.0);
	cam.setPerspective(70.0f, ratio,-1.0f,1.0f);
	need_update = true;

}

void eye::setClose(float z_delta)
{
	pos.z += z_delta;
	float t = sqrt(2.0)* pos.z;
	float new_scale = -log(t)/log(2.0);
	scale = new_scale;
	need_update = true;
}

void eye::update()
{
	if (need_update == false)
		return;
	//upleft position of the viewport
	ci::Vec2i reso = ci::app::AppNative::get()->getWindowSize();
	float realheight = pos.z*reso.y;
	ci::Vec3f realpos(pos.x*reso.x, pos.y*reso.y,realheight);
	cam.lookAt(realpos,Vec3f(pos.x*reso.x,pos.y*reso.y,0.0),Vec3f::yAxis());
	gl::setModelView(cam);
	need_update = false;
}

void eye::setPos(int dir, float delta)
{
	if (dir == VERTICAL) {
		pos.y += delta;
		if (scale >= 0.0f)
			pos.y = constrain( pos.y,static_cast<float>(pow(2,-scale)/2),1.0f-static_cast<float>(pow(2,-scale)/2));
		else 
			pos.y = constrain(pos.y,1.0f-static_cast<float>(pow(2,-scale)/2),static_cast<float>(pow(2,-scale)/2));
	}
	if (dir == HORIZONAL) {
		pos.x += delta;
		if (scale >= 0.0f)
			pos.x = constrain(pos.x,static_cast<float>(pow(2,-scale)/2),1.0f-static_cast<float>(pow(2,-scale)/2));
		else
			pos.x = constrain(pos.x,1.0f-static_cast<float>(pow(2,-scale)/2),static_cast<float>(pow(2,-scale)/2));
	}
	need_update = true;
}