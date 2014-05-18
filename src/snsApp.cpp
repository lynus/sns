
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/params/Params.h"
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
	  snsApp():AppNative(),io(),myconfig(){isfullscreen=false;};
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
	params::InterfaceGlRef panelUI;
	bool isfullscreen;
};


void snsApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize(myconfig.win_width,myconfig.win_height);
	settings->setFrameRate(myconfig.frame_rate );
	settings->setFullScreen(isfullscreen);
}

void snsApp::setup()
{
	if (io.isConnected() != true)
		app::AppNative::get()->quit();
	myeye.init(Vec3f(0.5f,0.5f,sqrt(2.0f)/2), getWindowAspectRatio());
	io.setupNodes(controller);
	controller.start();
	panelUI =  params::InterfaceGl::create( "network visualizer", Vec2i( 200, 160 ) );
	panelUI->addParam("Zooms", &myeye.scale);
	panelUI->addParam("eye location", &myeye.pos,"",true);
}

void snsApp::mouseDown( MouseEvent event )
{
}

void snsApp::keyDown( KeyEvent event)
{
	switch (event.getChar()) {
	case 'q':
	case 'Q':
		this->quit();
		break;
	case 'W':
	case 'w':
		myeye.setClose(0.1f);
		break;
	case 'S':
	case 's':
		myeye.setClose(-0.1f);
		break;
	case 'F':
	case 'f':
		AppNative::get()->setFullScreen(isfullscreen = !isfullscreen);
		break;
	case 'R':
	case 'r':
		myeye.reset();
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
		case KeyEvent::KEY_SPACE:
			controller.swi();
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
	panelUI->draw();
}

CINDER_APP_NATIVE( snsApp, RendererGl )
