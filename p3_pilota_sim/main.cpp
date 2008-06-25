#include "alcor/act/pilota_sim.hpp"

int main(){
	std::cout << "Pilota simulato start." << std::endl;
	pilota_sim pilota;
	char a;
	fscanf(stdin,"Inserisci un comando : %c",&a);
	while(a != 'q'){
		fscanf(stdin,"%c",&a);
		getchar();
		std::cout << "lettera letta : " << a << std::endl;
		if(a == 'w')	pilota.wandering();
		if(a == 's')	pilota.stop();
		if (a == 'q') break;
		
	}
	pilota.cancel();
	getchar();
	return 0;
}