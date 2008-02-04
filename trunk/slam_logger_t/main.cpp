#include "alcor/core/slam_logger_t.h"
#include <alcor/sense/urg_laser_t.hpp>

int main(int argc, char* argv[]){
	std::string nomefile;
	if(argc > 1)
		nomefile = argv[1];
	else
		nomefile.assign("general_data_log.bin");
		
	all::sense::urg_laser_t* m_urg;
	all::sense::urg_scan_data_ptr m_curr_scan;
	slam_data_log_t data_logged;

	m_urg = new all::sense::urg_laser_t();
	std::cout << "Premi un tasto per iniziare l'acquisizione" << std::endl;
	getchar();
	if(m_urg->connect()){
		m_curr_scan = m_urg->do_scan(all::math::angle(-110, all::math::deg_tag), all::math::angle(110, all::math::deg_tag), 1);
		slam_logger_t *logger = new slam_logger_t(nomefile,true,false,false,(int) m_curr_scan->scan_points.size());
		std::cout << "Laser connesso" << std::endl;
		std::cout << "Attendo uno scan" << std::endl;
		for(int i=0;i<100;i++){
			m_curr_scan = m_urg->do_scan(all::math::angle(-110, all::math::deg_tag), all::math::angle(110, all::math::deg_tag), 1);
			std::cout << "La dimensione dello scan è : " << m_curr_scan->scan_points.size() << std::endl;
			data_logged.laserData = m_curr_scan;
			logger->add_data_(data_logged);
		}
		delete(logger);
	}
	getchar();
	return 1;
}