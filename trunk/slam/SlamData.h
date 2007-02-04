#ifndef SLAMDATA_H
#define	SLAMDATA_H
//---------------------------------------------------------------------------
#include <vector>
#include "alcor.apps/slam/scandata.h"
//---------------------------------------------------------------------------
typedef	signed char					MapValue;
typedef std::vector<MapValue>		MapPointVect;
typedef MapPointVect::iterator		MapPointVectIt;
//---------------------------------------------------------------------------
/**	questi i dati che lo slam offre ai client... ovviamente non tutti i client chiederanno tutti i dati...
	le coordinate sulla Mappa sono organizzate come segue:
	1) lo (0,0) è al CENTRO della mappa e l'asse X è l'asse orizzontale
	2) le dimensioni della Mappa sono (in CELLE): mapXdimension_*mapYdimension_
	3) la risoluzione di ogni singola cella è (in METRI): mapResolution_ */
typedef	std::vector<pose2_t>	PoseVect;
typedef PoseVect::iterator		PoseVectIt;

struct	SlamData
{
	// Mappa
	MapPointVect		cells_;
	int					mapXdimension_;	///<dimensione in CELLE dell'asse X della mappa
	int					mapYdimension_;	///<dimensione in CELLE dell'asse Y della mappa
	double				mapResolution_;	///<risoluzione in METRI della mappa

	// Percorso del Robot
	PoseVect			path_;

	// Ultimo Scan del Laser
	ScanData			scan_;
	
	// Goal 
	PoseVect			goalPath_;	///<Percorso Dalla posizione corrente al goal_
	pose2_t				goal_;		///<Goal Definitivo del Robot
	pose2_t				goalTemp_;	///<Goal "vicino" al Robot (bastone e carota!)
};
//---------------------------------------------------------------------------
#endif