
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "nodecontroller.h"
#include "eye.h"
#include "cinder\CinderMath.h"
#include "comm.h"
#include "config.h"
using namespace ci;
using namespace ci::app;
using namespace std;

comm * comm::instance=NULL;
eye * eye::instance=NULL;
config * config::instance=NULL;
class snsApp : public AppNative {
  public:
	  snsApp():AppNative(),io(),myconfig(){};
	  void prepareSettings( Settings *settings );
	void setup();
	void mouseDown( MouseEvent event );	
	void keyDown( KeyEvent event);
	void update();
	void draw();
	config myconfig;
	nodecontroller controller;
	eye myeye;
	comm io;	
};


void snsApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize(myconfig.win_width,myconfig.win_height);
	settings->setFrameRate(myconfig.frame_rate );
	settings->setFullScreen(0);
}

void snsApp::setup()
{
	if (io.isConnected() != true)
		app::AppNative::get()->quit();
	myeye.init(Vec3f(0.5f,0.5f,sqrt(2.0f)/2), getWindowAspectRatio());
	io.setupNodes(controller);
}

void snsApp::mouseDown( MouseEvent event )
{
}

void snsApp::keyDown( KeyEvent event)
{
	switch (event.getChar()) {
	case 'q':
		this->quit();
		break;
	case 'w':
		myeye.setClose(0.1f);
		break;
	case 's':
		myeye.setClose(-0.1f);
		break;
	case 'f':
		AppNative::get()->setFullScreen(0);
		break;
	default:
		break;
	}

	switch (int t=event.getCode()) {
		case KeyEvent::KEY_LEFT:
			myeye.setPos(eye::HORIZONAL, -0.1f);
			break;
		case KeyEvent::KEY_RIGHT:
			myeye.setPos(eye::HORIZONAL, +0.1f);
			break;
		case KeyEvent::KEY_UP:
			myeye.setPos(eye::VERTICAL, -0.1f);
			break;
		case KeyEvent::KEY_DOWN:
			myeye.setPos(eye::VERTICAL, +0.1f);
			break;
		default:
			break;
	}
}

void snsApp::update()
{
	//eye has updated by dealing with user input
	myeye.update();
	controller.update();
}

void snsApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0.5f, 0.5f, 0.5f ) ); 
	controller.draw();
}

CINDER_APP_NATIVE( snsApp, RendererGl )
