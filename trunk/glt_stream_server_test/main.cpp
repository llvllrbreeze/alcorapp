#define WIN32_LEAN_AND_MEAN

#include <alcor/gui/glt_stream_server_t.hpp>
#include <glutm/main.h>


void test() {
	printf("ok");
	getchar();
	printf("3");
	getchar();
}

void draw_data() {
	
	glClearColor( 0.88f, 0.88f, 0.9f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	///X
    glBegin(GL_LINES);
    glColor3f(0.0f,1.0f,0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(100.0f, 0.0f, 0.0f);
    glEnd();

    ///Y
    glBegin(GL_LINES);
    glColor3f(1.0f,0.0f,0.1f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 100.0f, 0.0f);
    glEnd();

    ///Z
    glBegin(GL_LINES);
    glColor3f(0.0f,0.0f,1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 100.0f);
    glEnd();

}

bool GlutMain(const std::vector<std::string> &arg) {

	glt_stream_server_t* main = new glt_stream_server_t(640, 480);
	
	boost::shared_ptr<boost::thread> glt_thread;

	glt_thread.reset(new boost::thread(&test));

	main->draw_data = &draw_data;

	main->open();

	return true;
}

