#include "alcor/gui/glfw_win_t.hpp"

class glfw_win_test : public all::core::glfw_win_t {

protected:

	void _gl_init_cb() {
		glEnable(GL_DEPTH);
	}

	void _draw_cb() {

		//view setting
		glfw_win_t::_draw_cb();

		glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
  
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		glBegin(GL_LINES);
			glColor3f(1.0, 0, 0);
			glVertex3f(0,0,0);
			glVertex3f(100,0,0);
			glColor3f(0, 1, 0);
			glVertex3f(0,0,0);
			glVertex3f(0,100,0);
			glColor3f(0, 0, 1);
			glVertex3f(0,0,0);
			glVertex3f(0,0,100);
		glEnd();

	}

};


int main() {
	
	glfw_win_test window;

	window.run_async();

	printf("ok\n");

	getchar();


}