/*

File PmapWrap.h è la classe nascosta di SlamWorker che si occupa di convertire e di calcolare
le varie mappe, di acquisire le scansioni del laser e la posizione odometrica.
*/
#ifndef PmapWrap_H
#define PmapWrap_H

#include "alcor/extern/pmap/lodo.h"
#include "alcor/extern/pmap/omap.h"
#include "alcor/extern/pmap/pmap.h"
#include "alcor/extern/pmap/slap.h"
#include "alcor/slam/SlamData.h"
#include "alcor/slam/ScanData.h"
#include "alcor/types/defs.h"
using namespace std;

class PmapWrap{
public:
			PmapWrap(char* name);
			~PmapWrap();
	void	FillMapData(SlamData* data); ///< Il puntatore è già allocato, questa funzione pensa a riempire la struttura con i suoi dati
	pose2_t	GetPosition(); ///< restituisce la posizione corrispondente al campione migliore
	void	Process(ScanData* scan); ///< contiene l'algoritmo per il particle filter
	void	SalvaMappa(const char* filename); ///< questa funzione si occupa si realizzare la occupancy grid
	int		GetNRange();
	size_t	GetSize();
	size2_t	GetCurrentCoord();
	size2_t GetCoordOf(pose2_t);	//da COORDINATE ASSOLUTE (qualsiasi) a CELLA
	bool	InMap(int,int);
	pose2_t	GetPositionOf(size2_t);
	bool	IsFirstScan(){return (lodo->scan_count == 0);}
	pose2_t	GetLastLodoPose(){return current_lodoPose;}
private:
	omap_t* omap;
	pmap_t* pmap;
	lodo_t* lodo;
	int n_range;
	double *raggi;
	pose2_t corrpose;
	pose2_t	offset_laser_pose;
	pose2_t current_lodoPose;
};

#endif
