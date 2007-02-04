#ifndef SCANDATA_H
#define SCANDATA_H
//------------------------------------------------------------------------------------------------
//#include <vector>
#include <time.h>
#include "alcor.apps/slam/slap.h"
#include "alcor/core/core.h"
//------------------------------------------------------------------------------------------------
/** Questa struttura dati di un singolo scan e che verrà utilizzata dallo Slam per processare uno 
scan...
I campi dati corrispondono:
1) La posizione odometrica in cui si trova il robot nel momento in cui viene eseguita la scansione
2)Il vettore con i dati del laser (IN MILLIMETRI)
3)Il tempo in cui viene effettuata la lettura*/
typedef	std::vector<int>			    IntVect;
typedef	std::vector<int>::iterator	IntVectIt;

struct ScanData{
	//Laser
  all::core::IntVect			ranges;		    ///< vettore con i dati del laser
	double			start_angle;	///< angolo di partenza della scansione
	double			angle_step;		///< incremento angolare da Step a Step

	//Robot
	pose2_t			pos_odometrica; ///< posizione odometrica corrente

	//Tempo
	time_t			tempo;			///< istante di tempo in cui i dati vengono acquisiti
};
//------------------------------------------------------------------------------------------------
#endif