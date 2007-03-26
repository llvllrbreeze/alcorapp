#include <iostream>
#include "alcor/splam/splam_thread.h"
//---------------------------------------------------------------------------
using namespace all::splam;
//---------------------------------------------------------------------------
int main()
{
	char esc='0';
	splam_thread splam;
	//esc = getchar();
	splam.run();
	splam.start_server();
	while(esc != 'q')
	{
		esc = getchar();
		std::cout << "Premere 'q' per uscire"<<std::endl;
	}
	return 0;
}