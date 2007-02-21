#include <conio.h>
#include "alcor/splam/SlamThread.h"
//---------------------------------------------------------------------------
int main()
{
	SlamThread * slam;
	slam = new SlamThread;
	slam->runAsync();
	slam->StartServer();
	while(getche() != 'q')
	{
		std::cout << "Premere 'q' per uscire"<<std::endl;
	}
	delete slam;
	return 0;
}