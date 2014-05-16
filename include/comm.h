#include "boost\asio.hpp"
#include "cinder\app\AppNative.h"
#include "nodecontroller.h"

using namespace boost::asio;
class comm {
public:
	comm();
	//void init();
	static comm * get() {return instance;}
	~comm() {
		ci::app::AppNative::get()->console()<<"comm desctrut!!"<<std::endl;
	}
	void setupNodes(nodecontroller &controller);
	void getNodes(nodecontroller::NS &nodeset,int scale,int x,int y);
	inline bool isConnected() {return isconnected;};
private:
	static comm *instance;
	socket_base::bytes_readable cmd_bytes_readable;
	streambuf buf;
	std::iostream ss;
	io_service ios;
	ip::tcp::endpoint remote;
	ip::tcp::socket sock;
	bool isconnected;
};
	