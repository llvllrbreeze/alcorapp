/*
Classe che si occupa dello Slam
*/

#ifndef SLAMWORKER_H
#define SLAMWORKER_H

#include "alcor/types/defs.h"
#include "alcor/extern/pmap/slap.h"

class PmapWrap;
struct ScanData;
struct SlamData;

class SlamWorker{
public:
	SlamWorker(char* name = "config/slam.ini");
	~SlamWorker();
	pose2_t GetPosition();
	void	FillMapData(SlamData* data);
	void	Process(ScanData* scan);
	void	SaveMap(const char* filename);
	int		NRange();
	size_t	GetSize();
	size2_t	GetCurrentCoord();	//da COORDINATE ASSOLUTE (correnti) a CELLA
	size2_t GetCoordOf(pose2_t);	//da COORDINATE ASSOLUTE (qualsiasi) a CELLA
	bool	InMap(int,int);
	bool	InMap(size2_t pos){return InMap(pos.first,pos.second);}
	pose2_t	GetPositionOf(size2_t);	// da CELLA a COORDINATE ASSOLUTE
	bool	IsFirstScan();
	pose2_t	GetLastLodoPose();

private:
	PmapWrap* sWrap;

};
#endif