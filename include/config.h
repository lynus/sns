#include <string>
#define CONFIG(data) (config::get()->data)
class config 
{
public:
	config();
	void setup();
	static config* get(){return instance;};
	std::string ip;
	int win_width;
	int win_height;
	float frame_rate;
	static config *instance;
};