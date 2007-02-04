#include "alcor/slam/slamworker.h"
#include "PmapWrap.h"

#include "SlamData.h"
#include "alcor/slam/ScanData.h"

SlamWorker::SlamWorker(char* name){
	sWrap= new PmapWrap(name);
}

SlamWorker::~SlamWorker(){
	sWrap->~PmapWrap();
}

pose2_t SlamWorker::GetPosition(){
	return sWrap->GetPosition();
}

void SlamWorker::FillMapData(SlamData* data){
	sWrap->FillMapData(data);
}

void SlamWorker::Process(ScanData* scan){
	sWrap->Process(scan);
}

void SlamWorker::SaveMap(const char* filename){
	sWrap->SalvaMappa(filename);
}

int SlamWorker::NRange(){
	return sWrap->GetNRange();
}

size_t	SlamWorker::GetSize(){
	return sWrap->GetSize();
}

size2_t	SlamWorker::GetCurrentCoord(){
	return sWrap->GetCurrentCoord();
}

bool	SlamWorker::InMap(int a, int b){
	return sWrap->InMap(a,b);
}

pose2_t	SlamWorker::GetPositionOf(size2_t temp){
	return sWrap->GetPositionOf(temp);
}

bool	SlamWorker::IsFirstScan(){
	return sWrap->IsFirstScan();
}

pose2_t	SlamWorker::GetLastLodoPose(){
	return	sWrap->GetLastLodoPose();
}

size2_t SlamWorker::GetCoordOf(pose2_t temp){
	return	sWrap->GetCoordOf(temp);
}