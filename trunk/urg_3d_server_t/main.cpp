#define WIN32_LEAN_AND_MEAN

#include "urg_gl_server_t.hpp"


#include <glutm/main.h>


void test() {
	printf("ok");
	getchar();
	printf("3");
	getchar();
}


bool GlutMain(const std::vector<std::string> &arg) {

	urg_gl_server_t* main = new urg_gl_server_t(320, 240);
	
	boost::shared_ptr<boost::thread> glt_thread;

	glt_thread.reset(new boost::thread(&test));

	main->open();

	return true;
}