#include "simple_gaze_logger_t.h"


int main ()
{
	using namespace uor;
	simple_gaze_logger_t logger;

	logger.mainloop();

	getchar();

	logger.cancel();

	getchar();

	return 0;
}