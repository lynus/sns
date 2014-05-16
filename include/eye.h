#pragma once
#include "cinder/Vector.h"
#include "cinder/Camera.h"
using namespace ci;

class eye {
public:
	static eye * get(){return instance;};
	eye(){instance=this;};
	void init(Vec3f _pos,float ratio);
	void setClose(float z_delta);
	void update();
	void setPos(int dir, float delta);
	enum{
		VERTICAL	=0,
		HORIZONAL	=1
	};
	Vec3f pos;
	float scale;
	bool need_update;
	static eye *instance;
};
