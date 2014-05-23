#include "cinder\Camera.h"
#include "cinder\Vector.h"
#include "cinder\app\AppNative.h"
#include <cmath>
#include <ostream>
#include "eye.h"
#include "cinder\CinderMath.h"
#include "config.h"

using namespace ci;

#define min(x,y) (x)<=(y)?(x):(y)
#define max(x,y) (x)>=(y)?(x):(y)

Matrix44f modelview;
Matrix44f projection;
Area viewport;
Vec3f unproject(const Vec3f &pt)
{
 /* find the inverse modelview-projection-matrix */
 Matrix44f a = projection * modelview;
 a.invert(0.0f);

 /* transform to normalized coordinates in the range [-1, 1] */
 Vec4f in;
 in.x = (pt.x - viewport.getX1())/viewport.getWidth()*2.0f-1.0f;
 in.y = (pt.y - viewport.getY1())/viewport.getHeight()*2.0f-1.0f;
 in.z = 2.0f * pt.z - 1.0f;
 in.w = 1.0f;

 /* find the object's coordinates */
 Vec4f out = a * in;
 if(out.w != 0.0f) out.w = 1.0f / out.w;

 /* calculate output */
 Vec3f result;
 result.x = out.x * out.w;
 result.y = out.y * out.w;
 result.z = out.z * out.w;
 
 return result;
}

Vec3f screenToWorld(float x, float y, float z)
{ 
 Vec3f p = Vec3f(x, y, z);

 /* adjust y (0,0 is lowerleft corner in OpenGL) */ 
 p.y = (viewport.getHeight() - p.y);

 /* near plane intersection */
 p.z = 0.0f;
 Vec3f p0 = unproject(p);

 /* far plane intersection */
 p.z = 1.0f;
 Vec3f p1 = unproject(p);

 /* find (x, y) coordinates */
 float t = (z - p0.z) / (p1.z - p0.z);
 p.x = (p0.x + t * (p1.x - p0.x));
 p.y = (p0.y + t * (p1.y - p0.y));
 p.z = z;
 return p;
}


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
	scale= 0;
	need_update = true;
}

void eye::reset()
{
	pos = ci::Vec3f(0.5f,0.5f,0.0f);
	scale = 0.0f;
	pos.z = pow(2,-scale-0.5);
	need_update = true;
}
void eye::setClose(float z_delta)
{
	/*pos.z += z_delta;
	float t = sqrt(2.0)* pos.z;
	float new_scale = -log(t)/log(2.0);
	scale = new_scale;*/
	scale +=z_delta;
	need_update = true;
}

void eye::update()
{
	if (need_update == false)
		return;
	//glOrtho(0,config::get()->win_width,0,config::get()->win_height,0.0f,1.0f);
}

void eye::setPos(int dir, float delta)
{
	if (dir == VERTICAL) {
		pos.y += delta/scale;
		if (scale >= 0.0f)
			pos.y = constrain( pos.y,static_cast<float>(pow(2,-scale)/2),1.0f-static_cast<float>(pow(2,-scale)/2));
		else 
			pos.y = constrain(pos.y,1.0f-static_cast<float>(pow(2,-scale)/2),static_cast<float>(pow(2,-scale)/2));
	}
	if (dir == HORIZONAL) {
		pos.x += delta/scale;
		if (scale >= 0.0f)
			pos.x = constrain(pos.x,static_cast<float>(pow(2,-scale)/2),1.0f-static_cast<float>(pow(2,-scale)/2));
		else
			pos.x = constrain(pos.x,1.0f-static_cast<float>(pow(2,-scale)/2),static_cast<float>(pow(2,-scale)/2));
	}
	need_update = true;
}

void eye::convertMouse(float &mx, float &my)
{
	modelview = gl::getModelView();
	projection = gl::getProjection();
	viewport = gl::getViewport();
	Vec3f p = screenToWorld(mx, my);
	mx = p.x;
	my = p.y;	
}