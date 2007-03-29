#include <alcor/sense/urg_3d_scanner_t.hpp>
#include <alcor.extern/Cimg/Cimg.h>
#include <boost/numeric/ublas/vector.hpp>
#include <fstream>


int main() {


	all::act::PololuServoController pololu;
	all::sense::urg_laser_t urg;

	urg.connect();
	pololu.connect();

	all::sense::urg_3d_scanner_t scanner(&urg, &pololu, 0);

	getchar();

	scanner.reset(all::math::angle(-90, all::math::deg_tag), all::math::angle(90, all::math::deg_tag), all::math::angle(-40, all::math::deg_tag),all::math::angle(40, all::math::deg_tag), 80);
	all::sense::urg_pcd_ptr m_pcd;

	//pololu.servoOn(2);
	getchar();

	m_pcd = scanner.do_scan();

	//cimg_library::CImg <float> scan_image;

	//scan_image.assign(m_pcd->z_buffer_ptr, m_pcd->ncols, m_pcd->nrows, 1, 1);

	
	//std::fstream out_file;
	//out_file.open("points.txt", std::fstream::out);

	//for (int i = 0; i < m_pcd->nrows * m_pcd->ncols; i++) {
	//	out_file << m_pcd->x_buffer_ptr[i] << ' ' << m_pcd->y_buffer_ptr[i] << ' ' << m_pcd->z_buffer_ptr[i] << std::endl;
	//}


	//scan_image.display();

	cimg_library::CImg <short> polar_image;
	polar_image.assign(m_pcd->polar_buffer.get(), m_pcd->ncols, m_pcd->nrows, 1, 1);
	polar_image.display();

	getchar();
	
}