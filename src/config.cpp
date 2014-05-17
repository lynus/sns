#include "config.h"
#include <string>
#include <map>
#include <fstream>
#include <cstring>
using namespace std;
static string & trim(string &s) 
{
	 if (s.empty()) {  
        return s;  
	 }  
    s.erase(0,s.find_first_not_of(" "));  
    s.erase(s.find_last_not_of(" ") + 1);  
    return s;  
}  
config::config(){
	string line,key,value;
	map<string,string> option;
	char buf[256],t[32];
	int pos;
	instance = this;
	string fname = "config.ini";
	ifstream fconfig(fname.c_str());
	if (!fconfig.is_open())
		goto nofile;
	while(!fconfig.eof()) {
		fconfig.getline(buf,256);
		line = string(buf);
		pos = line.find('=');
		key= trim(line.substr(0,pos));
		if (key[0] == '#')
			continue;
		value = trim(line.substr(pos+1));
		option[key]=value;
	}
	ip = option[string("ip")];
	win_width = atoi(option[string("win_width")].c_str());
	win_height = atoi(option[string("win_height")].c_str());
	frame_rate = static_cast<float>( atoi(option[string("frame_rate")].c_str()));
	return;
	
nofile:
	ip = "127.0.0.1";
	win_width = 800;
	win_height = 800;
	frame_rate = 3.0;
	return;
}
