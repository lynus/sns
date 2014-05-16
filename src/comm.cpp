#include "boost\asio.hpp"
#include "nodecontroller.h"
#include "comm.h"
#include "cinder\app\AppNative.h"
#include "config.h"
#include <string>
#include <cstdio>
#include <ctime>
using namespace boost::asio;
comm::comm():sock(ios),ss(&buf),cmd_bytes_readable(true){
	remote = ip::tcp::endpoint(ip::address::from_string(config::get()->ip.c_str()),22222);
	try{
		sock.connect(remote);
		sock.io_control(cmd_bytes_readable);
		isconnected = true;
		instance = this;
	}
	catch(std::exception &e) {
		isconnected = false;
	}
}

void comm::setupNodes(nodecontroller &controller)
{
	size_t n;
	int nr_node=0,nr_edge=0;
	char b[128];
	try{
		ss<<"init\n";
		n = sock.write_some(buf.data());
		buf.consume(n);
		n=read(sock,buffer(b,10));
		nr_node = *(int *)b;
		nr_edge = *(int *)(b+5);
		int n1,n2,nr_line=0;
		nodepair p;
		time_t t1 = ::time(NULL);
		for(int i=0; i<nr_edge;i++) {
			n=read(sock,buffer(b,9));
			n1 = *(int *)b;
			n2 = *(int*)(b+4);
			nr_line++;
			assert(n1&&n2);
			p = nodepair(n1,n2);
			controller.addedge(p);
		}
		time_t t2 = ::time(NULL);
		ci::app::AppNative::get()->console()<<t2-t1<<std::endl;
	} catch(std::exception &e){
		ci::app::AppNative::get()->console()<<e.what()<<std::endl;
	  	ci::app::AppNative::get()->quit();
	}
	//controller.addnodes(nr_node);
}

void comm::getNodes(nodecontroller::NS &nodeset,int scale,int x,int y)
{
	size_t n;
	char b[32];
	node node;
	int num,id;
	ss<<x<<','<<y<<','<<scale<<std::endl;
	try{ 
		n = sock.write_some(buf.data());
		buf.consume(n);
		int nRead =0;
		read(sock,buffer(b,4));
		nRead = *(int *)b;
		for (int i=0; i<nRead; i++) {
			read(sock,buffer(b,16));
			id = *(int *)b;
			x = *(int *)(b+4);
			y = *(int *)(b+8);
			num = *(int *)(b+12);
			node.id=id; node.num=num;
			node.pos=ci::Vec2f((float)(x/10000.0), (float)(y/10000.0));
			//notice id is not available 
			nodeset[i]=node;
			
		}
	}catch(std::exception &e) {
		ci::app::AppNative::get()->console()<<e.what()<<std::endl;
		return;
	}    
}