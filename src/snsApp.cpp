
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
nodecontroller *nodecontroller::instance=NULL;

class snsApp : public AppNative {
  public:
	  snsApp():AppNative(),io(),myconfig(),controller(){isfullscreen=false;};
	  void prepareSettings( Settings *settings );
	void setup();
	void mouseDown( MouseEvent event );	
	void mouseUp( MouseEvent event );
	void mouseWheel( MouseEvent event );
	void mouseDrag( MouseEvent event);
	void keyDown( KeyEvent event);
	void update();
	void resize();
	void draw();
	void reset() {io.reset();};
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
	panelUI->addParam("Zoom", &myeye.scale);
	panelUI->addParam("Eye Location", &myeye.pos,"",true);
	panelUI->addButton("Reset",std::bind(&snsApp::reset,this));
}

void snsApp::resize()
{
	Vec2i win_size =  getWindowSize();
	CONFIG(win_width) = win_size.x;
	CONFIG(win_height) = win_size.y;
}
void snsApp::mouseDown( MouseEvent event )
{
	if ( event.isLeft()) {
		float mouse_x = (float)event.getX();
		float mouse_y = (float)event.getY();
		//mouse's position need to be convert back to node's space 
		//in order to perform selection detection
		myeye.convertMouse(mouse_x,mouse_y);
		controller.getSelectNode(mouse_x,mouse_y);
		if (controller.selected_node_id != -1)
			controller.stop();
	}
}
void snsApp::mouseUp( MouseEvent event ) 
{ 
	if(controller.selected_node_id != -1) {
		controller.start();
		// comm should set selected_node_id to -1 after update
	}
}
void snsApp::mouseWheel( MouseEvent event)
{
	float  wheel = event.getWheelIncrement();
	myeye.setClose(wheel/10);
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
		myeye.setPos(eye::VERTICAL, -0.1f);
		break;
	case 'S':
	case 's':
		myeye.setPos(eye::VERTICAL, +0.1f);
		break;
	case 'A':
	case 'a':
		myeye.setPos(eye::HORIZONAL, -0.1f);
		break;
	case 'd':
	case 'D':
		myeye.setPos(eye::HORIZONAL, +0.1f);
		break;
	case 'F':
	case 'f':
		AppNative::get()->setFullScreen(isfullscreen = !isfullscreen);
		break;
	case 'R':
	case 'r':
		myeye.reset();
		break;
	case 'x':
		io.reset();
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
		case KeyEvent::KEY_SPACE:
			if (controller.selected_node_id != -1)
				controller.swi();
		default:
			break;
	}
}

void snsApp::mouseDrag( MouseEvent event) 
{
	console()<<"enter mouse Drag\n"<<event.getX()<<"  "<<event.getY()<<std::endl;
	if (controller.selected_node_id != -1) {
		float x = event.getX();
		float y = event.getY();
		myeye.convertMouse(x,y);
		controller.updateSelectedNode( x,y );
	}
	
}
void snsApp::update()
{
	//eye has updated by dealing with user input
	controller.update();
	myeye.update();
}

void snsApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0.5f, 0.5f, 0.5f ) ); 
	controller.draw();
	panelUI->draw();
}

CINDER_APP_NATIVE( snsApp, RendererGl )
