#include "alcor/core/slam_log_reader_t.h"

int main(int argc, char* argv[]){
	std::string nomefile;
	if(argc > 1)
		nomefile = argv[1];
	else
		nomefile.assign("general_data_log.bin");
	slam_data_log_t loggedData;
	//loggedData.laserData.
	slam_log_reader_t logger(loggedData,nomefile);


	//std::vector <short int> vettore_prova;
	//std::cout << "La dimensione del vettore prova non riservato è : " << sizeof(vettore_prova) << std::endl;
	//vettore_prova.reserve(512);
	//std::cout << "La dimensione del vettore prova dopo il reserve è : " << sizeof(vettore_prova) << std::endl;
	//std::cout << "La size del vettore prova dopo il reserve è : " << vettore_prova.size() << std::endl;

	//vettore_prova.resize(512);
	//std::cout << "La dimensione del vettore prova dopo il resize è : " << sizeof(vettore_prova) << std::endl;
	//std::cout << "La size del vettore prova dopo il push back è : " << vettore_prova.size() << std::endl;
	//for(int i=0;i<512;i++){
	//	vettore_prova.push_back(1);
	//}
	//std::cout << "La dimensione del vettore prova dopo il push back è : " << vettore_prova.size() * sizeof(vettore_prova) << std::endl;
	while(getchar() != 'q')
		logger.next_sample();
	return 1;
}