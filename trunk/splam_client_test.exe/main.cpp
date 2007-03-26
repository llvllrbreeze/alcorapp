#include <iostream>
#include "alcor/splam/splam_client.h"
#include "alcor.extern/CImg/CImg.h"
//---------------------------------------------------------------------------
using namespace all::splam;
//---------------------------------------------------------------------------
int main()
{
	char esc='0';
	splam_client	splam;
	splam_data		data;
	while(esc != 'q')
	{
		esc  = getchar();
		splam.lock();
		splam.get_splam_data(data);
		std::cout << "posa corrente: " << data.get_current_position()<< " --- premere 'q' per uscire!" << std::endl;
		if(esc=='d')
			splam.set_display(!splam.get_display());
		splam.unlock();
	}
	return 0;
}