#include <iostream>
#include "alcor/splam/splam_thread.h"
//---------------------------------------------------------------------------
using namespace all::splam;
//---------------------------------------------------------------------------
int main()
{
	char esc='0';
	splam_thread splam;
	splam.run();
	splam.start_server();
	while(esc != 'q')
	{
		std::cin >> esc;
		std::cout << "Premere 'q' per uscire"<<std::endl;
	}
	return 0;
}