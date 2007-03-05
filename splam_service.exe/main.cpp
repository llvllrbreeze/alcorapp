#include <iostream>
#include <conio.h>
#include "alcor/splam/splam_thread.h"
//---------------------------------------------------------------------------
using namespace all::splam;
//---------------------------------------------------------------------------
int main()
{
	splam_thread splam;
	splam.run();
	splam.start_server();
	while(getche() != 'q')
	{
		std::cout << "Premere 'q' per uscire"<<std::endl;
	}
	return 0;
}