#ifndef urg_gl_server_t_H_INCLUDED
#define urg_gl_server_t_H_INCLUDED

#include <alcor/gui/glt_stream_server_t.hpp>
#include <alcor/sense/urg_3d_scanner_t.hpp>

class urg_gl_server_t : public glt_stream_server_t {

public:
	
	urg_gl_server_t(int, int);

	void draw_scene();

	void do_scan(all::core::client_connection_ptr_t, all::core::net_packet_ptr_t);

private:

	boost::shared_ptr <all::sense::urg_laser_t> m_urg;

	all::sense::urg_3d_scanner_ptr m_scanner;

	all::sense::urg_pcd_ptr m_last_scan;


};

inline urg_gl_server_t::urg_gl_server_t(int w, int h) : glt_stream_server_t(w, h) {
	
	m_urg.reset(new all::sense::urg_laser_t);

	m_urg->connect();

	m_scanner.reset(new all::sense::urg_3d_scanner_t(m_urg.get(), 5));

	m_scanner->reset(all::math::angle(-90, all::math::deg_tag), all::math::angle(90, all::math::deg_tag), all::math::angle(-40, all::math::deg_tag),all::math::angle(40, all::math::deg_tag), 80);

	draw_data = boost::bind(&urg_gl_server_t::draw_scene, this);

	m_stream_server->add_command_handler("scan", boost::bind(&urg_gl_server_t::do_scan, this, _1, _2));

}


inline void urg_gl_server_t::draw_scene() {
	
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

	if (m_last_scan) {
		std::vector<all::sense::urg_pcd_t::Point_3>::iterator point_it = m_last_scan->m_point_cloud.begin();

		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_POINTS);
		for (point_it; point_it!= m_last_scan->m_point_cloud.end(); point_it++) {
			glColor3f(0.0f, 0.0f, -point_it->z()*0.2);
			glVertex3f(point_it->x(), point_it->y(), point_it->z());
		}
		glEnd();
	}

}

inline void urg_gl_server_t::do_scan(all::core::client_connection_ptr_t client, all::core::net_packet_ptr_t packet) {
	m_last_scan = m_scanner->do_scan();
}

#endif