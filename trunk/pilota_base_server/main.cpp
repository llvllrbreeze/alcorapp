#define WIN32_LEAN_AND_MEAN
#include "pilota_base_server.hpp"


int main()
{
	pilota_base_server pilota;
	getchar();
	pilota.cancel();
	getchar();
	return 0;
}