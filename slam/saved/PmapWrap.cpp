#include "PmapWrap.h"
#include "alcor/extern/iniwrapper/iniwrapper.h"

//debug
#include <fstream>
#include <iostream>
using namespace std;
ofstream loggo("loggo.txt",ios::out);
//debug

/** Questa funzione carica i dati necessari all'allocazione di pmap, omap, lodomap direttamente 
da un file ini perchè sono dati statici che non devono essere modificati nei passaggi successivi
*/



PmapWrap::PmapWrap(char* name){
	iniWrapper* w= new iniWrapper(name);

	/** parametri contenuti nel file ini per allocare le mappe */

	n_range= w->GetInt("laser:num_step",0);
	double r_max= w->GetDouble("laser:raggio_max",0.0);
	double r_res = w->GetDouble("mappa:dim_cella",0.0);
	int n_camp= w->GetInt("mappa:num_camp",0);
	double r_start = w->GetDouble("laser:start_angle",0.0);
	double r_step = w->GetDouble("laser:angle_step", 0.0);
	double larg = w->GetDouble("mappa:larghezza",0.0);
	double alt = w->GetDouble("mappa:altezza",0.0);

	/** Alloca la memoria per omap, pmap, lodo*/

	pmap =pmap_alloc(n_range, r_max, r_start,r_step, n_camp, larg*r_res, alt*r_res, r_res);
	lodo =lodo_alloc(n_range,r_max,r_res,r_start, r_step);
	omap = omap_alloc(n_range,r_max,r_start, r_step, larg*r_res, alt*r_res, r_res);
	raggi=new double[n_range];

	offset_laser_pose.pos.x = w->GetDouble("laser:offset_x",0.0);
	offset_laser_pose.pos.y = w->GetDouble("laser:offset_y",0.0);
	offset_laser_pose.rot = w->GetDouble("laser:offset_th",0.0);
}

/*
Il distruttore della classe pensa a deallocare la memoria di lodo, pmap e omap*/

PmapWrap::~PmapWrap(){
	lodo_free(lodo);
	pmap_free(pmap);
	omap_free(omap);
	delete[] raggi;
}

/*
La funzione GetPosition()restituisce la posizione del campione che in quel momento risulta migliore,
per fare ciò indicizza il puntatore alla struttura dei campioni di pmap sul campione miglire e dopo 
copia la posizione corrispondente nella variabile privata di PmapWrap corrpose.
*/

pose2_t PmapWrap::GetPosition(){
	
	pmap_sample_t* campione=PMAP_GET_SAMPLE(pmap,pmap->best_sample);
	corrpose=campione->pose;
	return corrpose;
}

/*
Tale funzione si occupa di riempire il campo relativo alla mappa della struttura slamdata con
i dati relativi ad omap. Questa funzione può essere utilizzata dopo aver richiamato la funzione
che copia in omap la mappa corrispondente al campione migliore
*/

void PmapWrap::FillMapData(SlamData* data){

#if 1
	/// aggiunte per test
	static int contazzo=0;
	char mappazza[10];
#endif

	pmap_sample_t *sample;
	pmap_scan_t *scann;
	pose2_t pose;

	//Calcolo la mappa del campione migliore 
	sample = PMAP_GET_SAMPLE(pmap, pmap->best_sample);
	
	//Realizzo una occupancy grid con i dati della posizione e della scansione del
	//campione migliore
	omap_clear(omap);
	data->path_.clear();
	data->cells_.resize(omap->grid_sx*omap->grid_sy,0);

	data->path_.reserve(pmap->step_count);
	for (int j = 0; j < pmap->step_count; ++j){
		scann = pmap->scans + j;
		pose = sample->poses[j];
		omap_add(omap, pose, pmap->num_ranges, scann->ranges);
		data->path_.push_back(pose);
	}

	for (int i=0; i< omap->grid_size; ++i){
		data->cells_.at(i)= omap->grid[i];
	}
	data->mapResolution_=omap->grid_res;
	data->mapXdimension_= omap->grid_sx;
	data->mapYdimension_=omap->grid_sy;

#if 1
	//aggiunte per test
	contazzo++;

	if(!(contazzo%10))
	{
		sprintf(mappazza,"mappa%d.png",contazzo);
		SalvaMappa(mappazza);
	}
#endif

}

void PmapWrap::Process(ScanData* scan){
	
	// nel ciclo for eseguo un cast da int a doubledei dati contenuti nella stuct ScanData perchè 
	//la funzione lodo_add_scan ha bisogno di un vettore di double
	for (int i=0;i< n_range;++i){
		raggi[i]= ((double)(scan->ranges.at(i)))/1000.0;
		
	}
	
	// ciclo di aggiornamento della posizione corretta necessaria a pmap per effettuare 
	//l'aggiornamento dei campioni
	current_lodoPose = lodo_add_scan(lodo,scan->pos_odometrica,n_range,raggi);
	//debug
	loggo << current_lodoPose.pos.x<<"   "<<current_lodoPose.pos.y<<"   "<<current_lodoPose.rot<<endl;
	//debug
	pmap_update(pmap, pose2_add(offset_laser_pose, current_lodoPose), n_range, raggi);	
	
}

void PmapWrap::SalvaMappa(const char* filename){

	omap_save_pgm(omap, filename);

	
}
int PmapWrap::GetNRange(){
	return n_range;
}

size_t	PmapWrap::GetSize()
{
	return omap->grid_sx*omap->grid_sy;
}

size2_t	PmapWrap::GetCurrentCoord()
{
	size2_t returnvalue;
	double tempval;
	pose2_t temp = GetPosition();
	tempval = omap->grid_sx/2 + temp.pos.x/omap->grid_res;
	returnvalue.first = (size_t)tempval;
	tempval = omap->grid_sy/2 + temp.pos.y/omap->grid_res;
	returnvalue.second = (size_t)tempval;
	return returnvalue;
}

bool	PmapWrap::InMap(int a, int b){
	if((a<0) || (b<0) || (a>=omap->grid_sx) || (b>=omap->grid_sy))
		return false;
	return true;
}

pose2_t	PmapWrap::GetPositionOf(size2_t temp){
	pose2_t returnvalue;

	returnvalue.pos.x = temp.first;
	returnvalue.pos.x = (returnvalue.pos.x - omap->grid_sx/2)*omap->grid_res;
	returnvalue.pos.y = temp.second;
	returnvalue.pos.y = (returnvalue.pos.y - omap->grid_sy/2)*omap->grid_res;
	returnvalue.rot = 0;

	return returnvalue;
}
