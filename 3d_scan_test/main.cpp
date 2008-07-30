#define WIN32_LEAN_AND_MEAN

#include <alcor/sense/urg_3d_scanner_t.hpp>
#include <alcor.extern/Cimg/Cimg.h>
#include <boost/numeric/ublas/vector.hpp>
#include <fstream>

#include "alcor/gui/glfw_win_t.hpp"

class glfw_win_pcd : public all::core::glfw_win_t {

protected:

	void _gl_init_cb() {
		glEnable(GL_DEPTH);

		glPointSize(2);
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

		glBegin(GL_POINTS);
			for (int i=0; i < pcd.size(); i++) {
				glColor3f(1, 0, pcd[i].z() / 5000);
				glVertex3f(pcd[i].x(), pcd[i].y(), pcd[i].z());
			}
		glEnd();

	}

public:
	std::vector<all::sense::urg_pcd_t::Point_3> pcd;

};

int main() {

	glfw_win_pcd gl_pcd;

	gl_pcd.run_async();

	//gl_pcd.pcd.push_back(all::sense::urg_pcd_t::Point_3(2, 5 , 10));

	//all::act::PololuServoController pololu;
	all::sense::urg_laser_t urg;

	urg.connect();
	//pololu.connect();

	all::sense::urg_3d_scanner_t scanner(&urg, 0);

	getchar();

	scanner.reset(all::math::angle(-90, all::math::deg_tag), all::math::angle(90, all::math::deg_tag), all::math::angle(-40, all::math::deg_tag),all::math::angle(40, all::math::deg_tag), 80);
	all::sense::urg_pcd_ptr m_pcd;

	//pololu.servoOn(2);
	getchar();

	m_pcd = scanner.do_scan();

	//cimg_library::CImg <float> scan_image;

	//scan_image.assign(m_pcd->z_buffer_ptr, m_pcd->ncols, m_pcd->nrows, 1, 1);

	
	std::fstream out_file;
	out_file.open("points.txt", std::fstream::out);

	out_file << m_pcd->nrows << std::endl;
	out_file << m_pcd->ncols << std::endl;

	
	for (int i = 0; i < m_pcd->nrows * m_pcd->ncols; i++) {
		out_file << m_pcd->x_buffer_ptr[i] << ' ' << m_pcd->y_buffer_ptr[i] << ' ' << m_pcd->z_buffer_ptr[i] << std::endl;
	}

	out_file.close();

	//scan_image.display();

	//cimg_library::CImg <short> polar_image;
	//polar_image.assign(m_pcd->polar_buffer.get(), m_pcd->ncols, m_pcd->nrows, 1, 1);
	//polar_image.display();

	//glfw_win_pcd gl_pcd;

	//gl_pcd.run_async();

	gl_pcd.pcd = m_pcd->m_point_cloud;


	getchar();
	
}