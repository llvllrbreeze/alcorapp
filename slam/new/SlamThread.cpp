#include "alcor/slam/SlamThread.h"
#include "alcor/tools/conversion.h"
//-----------------------------------------------------------------------------------------------
using namespace std;
//-----------------------------------------------------------------------------------------------
SlamThread::SlamThread( char* name)
	:ini_(name)
	,mappaCB(this, &SlamThread::Mappa)
	,othersCB(this, &SlamThread::Others)
	,victimCB(this,&SlamThread::Victim)
	,goalCB(this, &SlamThread::Goal)
	,worker(name)
	,scanCount_(0)
	,victimCount_(0)
	,victimPosition_(0,0)
{
	//inizializziamo Aria
	AriaStarter::Instance()->Start();

	// creo spazio per i vector di mask e di scan
	int lenghtmask = ini_.GetInt("laser:num_step",0);
	mask.resize(lenghtmask,1);
	currentScan_.ranges.resize(lenghtmask,0);
	slamd.scan_ = currentScan_;
	fill(mask.begin(),mask.end(),1);

	//inizializzazione dei parametri del robot
	odoPresent_ = (ini_.GetInt("robot:odo",1) == 1);

	//inizializzazione della struttura di SlamDataNet
	slamDataNet_ = new SlamDataNet(worker.GetSize());
	slamDataNet_->data_ = &slamd;

	//inizializzazione dei parametri di connessione del laser
	urg_step_start_	= ini_.GetInt("laser:start_step", UrgDriver::default_start);
	urg_step_end_	= ini_.GetInt("laser:end_step", UrgDriver::default_end);
	urg_cc_	= ini_.GetInt("laser:cc", 2);
	laserComPort = ini_.GetInt("laser:comport",0);
	if(laserComPort)
	{
		UrgDriver::Instance()->Connect(laserComPort);
	}else{
		//logFile_.open(ini_.GetStringAsChar("laser:logfile","laserlog.txt"),std::ios::out);
	}

	//inizializzazione della struttura di ValueIteration per il goal metrico
	valueIteration_.Create(ini_.GetInt("mappa:larghezza",0), ini_.GetInt("mappa:altezza",0),ini_.GetDouble("mappa:dim_cella",0.0)*100.0);

	// leggo dal file INI la porta del server
	port_ = ini_.GetInt("server:port",12321);
	robot.runAsync();

	slamd.goal_.pos.x = 0;
	slamd.goal_.pos.y = 0;
	slamd.goal_.rot = 0;
}

SlamThread::~SlamThread()
{
	robot.cancel();
	this->stopRunning();
	this->join();
	delete slamDataNet_;
}
//-----------------------------------------------------------------------------------------------
int SlamThread::ScanDataLock(){return ScanDataMutex.lock();}
int SlamThread::ScanDataUnlock(){return ScanDataMutex.unlock();}
int SlamThread::SlamDataLock(){return SlamDataMutex.lock();}
int SlamThread::SlamDataUnlock(){return SlamDataMutex.unlock();}
int SlamThread::PmapLock(){return Pmap.lock();}
int SlamThread::PmapUnlock(){return Pmap.unlock();}
//-----------------------------------------------------------------------------------------------
void SlamThread::AcquireAll()
{
	//acquisizione del Laser
	if(laserComPort)
	{
		AcqScan();
	}else{
		//AcqScanFromLogFile();
	}

	//Acquisizione dell'odometria
	AcqOdomPose();

	//Acquisizione dell'istante di tempo
	//todo...
}

void SlamThread::AcqScan()
{
	//la struttura scandata non deve essere modificata nel mentre
	ScanDataLock();

	//salvataggio dello scorso Scan
	if(scanCount_ != 0)
		lastScan_ = currentScan_;
	
	//acquisiamo un scan dal laser e riempiamo la relativa porzione di struttura "ScanData": currentScan_.ranges
	short int* scanlaser;
	scanlaser = UrgDriver::Instance()->GetRange(urg_step_start_, urg_step_end_, urg_cc_);
	for (int i=0;i< UrgDriver::Instance()->GetCurrentScanPoints();++i)
	{
		if(scanlaser[i] < 20)
			currentScan_.ranges[i] = 0;
		else
			currentScan_.ranges[i]= static_cast<int>( scanlaser[i]);
	}
	currentScan_.start_angle = UrgDriver::Instance()->GetCurrentStartAngle();
	currentScan_.angle_step = UrgDriver::Instance()->GetCurrentAngolarResolution();

	scanCount_++;
	
	//rilascio struttura scandata
	ScanDataUnlock();
	
}

void SlamThread::AcqOdomPose()
{
	//la struttura scandata non deve essere modificata nel mentre
	ScanDataLock();

	//acquisizione odometria del Robot e riempimento del campo pos_odometrica della struttura scanData
	if((odoPresent_) && (robot.thisState() == alcor::def::eActiveOpen))
	{
		currentScan_.pos_odometrica = ArPose_to_pose2_t(robot.get_odometry());
		currentScan_.pos_odometrica.pos.x/=1000;
		currentScan_.pos_odometrica.pos.y/=1000;
	}else{
		if(scanCount_ < 2)
		{
			currentScan_.pos_odometrica.pos.x = 0;
			currentScan_.pos_odometrica.pos.y = 0;
			currentScan_.pos_odometrica.rot   = 0;
		}else{
			EstimateOdometry();
		}
	}
	//rilascio struttura scandata
	ScanDataUnlock();
}

void	SlamThread::EstimateOdometry()
{
	//obbiettivo: riempire il campo currentScan_.pos_odometrica...
	//todo...
	//currentScan_.pos_odometrica = 
}

//---------------------------------------------------------------------------------------------
/** La funzione filter può essere utilizzata per eliminare parti dinamiche dalla mappa. La funzione 
moltiplica ogni punto del vettore degli scan con il corrispondente punto del vettore della maschera
se il valore contenuto nella maschera è 1 allora il punto rimane sullo scan, se il valore del punto
nella nella maschera è 0, allora il corrispondete punto dello scan viene eliminato.
La precondizione per utilizzare questa funzione è che lo ScanData sia pieno*/

void SlamThread::Filter()
{
	ScanDataLock();
	for (int i=0; i<mask.max_size();++i)
		currentScan_.ranges[i]= mask.at(i)* currentScan_.ranges[i];
	ScanDataUnlock();
}

/**La funzione SetMask setta il vettore della maschera. In ingresso ha un vettore di coppie 
corrispondente ad un vettore di settori angolari da eliminare dalla mappa. Per fare ciò la funzione
pone a 0 i punti all'interno del settore agolare condsiderato.*/

void SlamThread::SetMask(VectPairDouble vect )
{
	VectPairDoubleIT    it;
	int inizio;
	int fine;
	for(it = vect.begin();   it != vect.end();   ++it)
	{
		inizio=UrgDriver::Instance()->AngleToScanStep(it->first);
		fine= UrgDriver::Instance()->AngleToScanStep(it->second);
		if(inizio < fine)
			fill( mask.begin()+inizio,mask.begin()+fine,0);
	} 
}

//------------------------------------------------------------------------------------------------
void SlamThread::Process()
{
	//mentre si processano i dati di scan, nessuno deve modificarli!
	ScanDataLock();

	//processamento dei dati di scan ed aggiornamento di Mappa e Localizzazione
	worker.Process(&currentScan_);
	
	//rilascio della struttura di scan
	ScanDataUnlock();
}

void SlamThread::FillMap()
{
	//durante l'aggiornamento dei dati di output, nessuno deve modificarli
	SlamDataLock();

	//riempimento dei campi path, mappa e dimensione mappa 
	cout << "Posa Odometrica: "<< currentScan_.pos_odometrica.pos.x<<"  "<<currentScan_.pos_odometrica.pos.y<<"  "<<currentScan_.pos_odometrica.rot<<endl;
	worker.FillMapData(&slamd);
	if(slamd.path_.empty())
		cout << "Posa Corretta: 0 0 0"<<endl;
	else
		cout << "Posa Corretta: "<< slamd.path_.back().pos.x<<"  "<<slamd.path_.back().pos.y<<"  "<<slamd.path_.back().rot<<endl;
	cout << "Path del robot: dimensione: "<<slamd.path_.size()<<endl;

	//rimpimento dello scan corrente (necessario il lock!)
	ScanDataLock();
	slamd.scan_ = currentScan_;
	ScanDataUnlock();

	//calcolo del goal, sulla base dei dati già riempiti di SlamData e riempimento della componente GOAL dello SlamData
	PathFinding();
	cout << "Path del Goal del robot: dimensione: "<<slamd.goalPath_.size()<<endl;

	//rilascio della struttura di output
	SlamDataUnlock();
}

void	SlamThread::PathFinding()
{
	size2_t ttt = worker.GetCurrentCoord();
	cout << "CURRENT COORD: X: "<< ttt.first << " Y: "<< ttt.second<<endl;
	//ofstream tempazzo("azzo.txt",ios::out);

	//Calcolo del Goal temporaneo, del GoalPath e del Goal Definitivo
	size2_t sizee = valueIteration_.GetSize();
	int i,j;
	int temp;

	//passo zero: se il goal è fuori della mappa, bisogna riportarlo in mappa
	size2_t coord = worker.GetCoordOf(slamd.goal_);
	cout<<"il goal ha coordinate nella mappa: "<<coord.first<<"  "<<coord.second<<endl;
	if(!worker.InMap(coord))
	{
		slamd.goalTemp_.pos.x = 0;
		slamd.goalTemp_.pos.y = 0;
		slamd.goalTemp_.rot = 0;
		slamd.goalPath_.clear();
		slamd.goalPath_.push_back(slamd.goalTemp_);
		return;
	}

	//passo 0.5 porre ad un valore elevato le celle
	for(i=0;i<sizee.first;++i)
		for(j=0;j<sizee.second;++j)
			valueIteration_.SetXY(i,j,100000000,((int)(slamd.cells_[j*sizee.first+i])+128));

	//primo passaggio, serve per porre a infinito i muri e le zone troppo vicine ai muri
	for(i=0;i<sizee.first;++i)
		for(j=0;j<sizee.second;++j)
			if(slamd.cells_[j*sizee.first+i]>0)
				valueIteration_.SetUntouchArea(i,j,0.30/slamd.mapResolution_);

	//ultima condizione: il robot non può trovarsi in aree inaccessibili
	size2_t temporary = worker.GetCurrentCoord();
	for(i=temporary.first-1;i<=temporary.first+1;++i)
		for(j=temporary.second-1;j<=temporary.second+1;++j)
			if(worker.InMap(i,j))
				valueIteration_.SetXY(i,j,90000000,((int)(slamd.cells_[j*sizee.first+i])+128));

	//secondo passaggio, serve per porre a zero il goal
	valueIteration_.SetXY(coord.first,coord.second,0,0);

	//for(i=0;i<1000;i++)valueIteration_.SetXY(rand()%sizee.first,rand()%sizee.second,0,0);

	////STEP 2: LOOP
	ValueIterationInfo info=valueIteration_.Loop();

	cout << "value iteration fatta! numero step necesssari: "<<info.nStep<<endl;

	//for(i=0;i<sizee.first;++i)
	//{
	//	for(j=0;j<sizee.second;++j)
	//		tempazzo << valueIteration_.GetXY(i,j).value<<" ";
	//	tempazzo << endl;
	//}


	//STEP 3: CALCULATE UNEXPLORED PATH
	ValueIterationPath path;
	slamd.goalPath_.clear();
	if(valueIteration_.GetPath(worker.GetCurrentCoord(), path) == false)
	{
		//if(slamd.path_.empty())
		//{
		//	slamd.goal_.pos.x = 0;
		//	slamd.goal_.pos.y = 0;
		//	slamd.goal_.rot = 0;
		//}else{
		//	slamd.goal_ = slamd.path_.back();
		//}
		//slamd.goalPath_.push_back(slamd.goal_);

	}else{
		ValueIterationPathIt it;
		for(it=path.begin(); it!=path.end(); ++it)
		{
			//tempazzo << it->first << " "<< it->second<< endl;
			slamd.goalPath_.push_back(worker.GetPositionOf(*it));
		}
		//tempazzo<< endl << endl;

		//
		// per il goal definitivo non prendiamo goalPath.back() anche se "dovrebbe" essere
		// quello settato tramite la SetGoal del client... non lo prendiamo per evitare bug...
		//

		//if(!slamd.goalPath_.empty()){
		//	slamd.goal_ = slamd.goalPath_.back();
		//}else{
		//	if(slamd.path_.empty())
		//	{
		//		slamd.goal_.pos.x = 0;
		//		slamd.goal_.pos.y = 0;
		//		slamd.goal_.rot = 0;
		//	}else{
		//		slamd.goal_ = slamd.path_.back();
		//	}
		//	slamd.goalPath_.push_back(slamd.goal_);
		//}
	}
	size_t sizzo = (1.0/slamd.mapResolution_);
	if(slamd.goalPath_.size()<sizzo)
	{
		slamd.goalTemp_ = slamd.goal_;
	}else{
		slamd.goalTemp_ = slamd.goalPath_.at(sizzo-1);
	}
	//cout << "GOAL PATH CALCOLATO!!!!!!!!!!!!!!!"<<endl;
	//cout << "SIZE: "<< path.size()<<endl;
	//size2_t ttt = worker.GetCurrentCoord();
	//cout << "CURRENT COORD: X: "<< ttt.first << " Y: "<< ttt.second<<endl;
	//cout << "CURRENT POSITIONS: X: "<<worker.GetPosition().pos.x << " Y: "<<worker.GetPosition().pos.y<<endl;
	//cout << "CURRENT POSITION RECALCULATED: X: "<< worker.GetPositionOf(ttt).pos.x<< " Y: "<<worker.GetPositionOf(ttt).pos.y<<endl;

}


//void	SlamThread::GoalFinding()
//{
//	size2_t ttt = worker.GetCurrentCoord();
//	cout << "CURRENT COORD: X: "<< ttt.first << " Y: "<< ttt.second<<endl;
//	//ofstream tempazzo("azzo.txt",ios::out);
//
//	//Calcolo del Goal temporaneo, del GoalPath e del Goal Definitivo
//	size2_t sizee = valueIteration_.GetSize();
//	int i,j;
//	int temp;
//	//primo passaggio, serve per porre a zero le zone unknown
//	for(i=0;i<sizee.first;++i)
//	{
//		for(j=0;j<sizee.second;++j)
//		{
//			temp = slamd.cells_[j*sizee.first+i];
//			if(temp == 0)
//				valueIteration_.SetXY(i,j,0,0);
//			else
//				valueIteration_.SetXY(i,j,100000000,(temp+128));
//		}
//	}
//	//secondo passaggio, serve per porre a infinito i muri e le zone troppo vicine ai muri
//	for(i=0;i<sizee.first;++i)
//		for(j=0;j<sizee.second;++j)
//			if(slamd.cells_[j*sizee.first+i]>0)
//				valueIteration_.SetUntouchArea(i,j,0.30/slamd.mapResolution_);
//	//ultima condizione: il robot non può trovarsi in aree inaccessibili
//	size2_t temporary = worker.GetCurrentCoord();
//	for(i=temporary.first-1;i<=temporary.first+1;++i)
//		for(j=temporary.second-1;j<=temporary.second+1;++j)
//			if(worker.InMap(i,j))
//				valueIteration_.SetXY(i,j,100000000,(slamd.cells_[j*sizee.first+i]+128));
//
//	////STEP 2: LOOP
//	ValueIterationInfo info=valueIteration_.Loop();
//
//	//for(i=0;i<sizee.first;++i)
//	//{
//	//	for(j=0;j<sizee.second;++j)
//	//		tempazzo << valueIteration_.GetXY(i,j).value<<" ";
//	//	tempazzo << endl;
//	//}
//
//
//	//STEP 3: CALCULATE UNEXPLORED PATH
//	ValueIterationPath path;
//	slamd.goalPath_.clear();
//	if(valueIteration_.GetPath(worker.GetCurrentCoord(), path) == false)
//	{
//		if(slamd.path_.empty())
//		{
//			slamd.goal_.pos.x = 0;
//			slamd.goal_.pos.y = 0;
//			slamd.goal_.rot = 0;
//		}else{
//			slamd.goal_ = slamd.path_.back();
//		}
//		slamd.goalPath_.push_back(slamd.goal_);
//	}else{
//		ValueIterationPathIt it;
//		for(it=path.begin(); it!=path.end(); ++it)
//		{
//			//tempazzo << it->first << " "<< it->second<< endl;
//			slamd.goalPath_.push_back(worker.GetPositionOf(*it));
//		}
//		//tempazzo<< endl << endl;
//
//		if(!slamd.goalPath_.empty()){
//			slamd.goal_ = slamd.goalPath_.back();
//		}else{
//			if(slamd.path_.empty())
//			{
//				slamd.goal_.pos.x = 0;
//				slamd.goal_.pos.y = 0;
//				slamd.goal_.rot = 0;
//			}else{
//				slamd.goal_ = slamd.path_.back();
//			}
//			slamd.goalPath_.push_back(slamd.goal_);
//		}
//	}
//	size_t sizzo = (1.0/slamd.mapResolution_);
//	if(slamd.goalPath_.size()<sizzo)
//	{
//		slamd.goalTemp_ = slamd.goal_;
//	}else{
//		slamd.goalTemp_ = slamd.goalPath_.at(sizzo-1);
//	}
//	//cout << "GOAL PATH CALCOLATO!!!!!!!!!!!!!!!"<<endl;
//	//cout << "SIZE: "<< path.size()<<endl;
//	//size2_t ttt = worker.GetCurrentCoord();
//	//cout << "CURRENT COORD: X: "<< ttt.first << " Y: "<< ttt.second<<endl;
//	//cout << "CURRENT POSITIONS: X: "<<worker.GetPosition().pos.x << " Y: "<<worker.GetPosition().pos.y<<endl;
//	//cout << "CURRENT POSITION RECALCULATED: X: "<< worker.GetPositionOf(ttt).pos.x<< " Y: "<<worker.GetPositionOf(ttt).pos.y<<endl;
//
//}

//---------------------------------------------------------------------------
void	SlamThread::StartServer()
{
	// open the server
	if (!server_.open(port_))
	{
		printf("Could not open server port: %d\n", port_ );
		throw std::runtime_error("Error in SlamServer::StartServer");
	}

	// add the "SlamData" service
	server_.addData("Mappa", "Mappa from SLAM module", &mappaCB, "none", "none");
	server_.addData("Others", "Other data from SLAM module", &othersCB, "none", "none");
	server_.addData("Victim", "Nuova Vittima Identificata", &victimCB, "none", "none");
	server_.addData("Goal", "Posa obbiettivo", &goalCB, "none", "none");

	//	run the server thread
	server_.runAsync();
}

void	SlamThread::StopServer()
{
	server_.stopRunning();
	server_.join();
	server_.close();
}

void	SlamThread::Mappa(ArServerClient* client, ArNetPacket* clientPack)
{
	//cout << "Mappa Richiesta :D"<<endl;
	// callback vuota, in quanto i dati dello SLAM vanno spediti solo in broadcast
}

void	SlamThread::Others(ArServerClient* client, ArNetPacket* clientPack)
{
	//cout << "Altri Dati Richiesti :D"<<endl;
	// callback vuota, in quanto i dati dello SLAM vanno spediti solo in broadcast
}

void	SlamThread::Victim(ArServerClient* client, ArNetPacket* clientPack)
{
	ArNetPacket pack;
	double tempCoord;
	ArTypes::Byte temp = clientPack->bufToByte();
	if(temp==1)
	{
		cout<<"Qualcuno ha settato il Goal Permanente! ";
		tempCoord = clientPack->bufToDouble();
		cout<<tempCoord<<"   ";
		pack.doubleToBuf(tempCoord);
		tempCoord = clientPack->bufToDouble();
		cout<<tempCoord<<endl;
		pack.doubleToBuf(tempCoord);
		victimCount_++;
		pack.byteToBuf(victimCount_);
		cout<<"ci sono ora "<<(int)victimCount_<<" vittime!!!"<<endl;
		server_.broadcastPacketTcp(&pack,"Victim");
	}

}

void	SlamThread::Goal(ArServerClient* client, ArNetPacket* clientPack)
{
	double tempCoord;
	ArTypes::Byte temp = clientPack->bufToByte();
	if(temp==1)
	{
		cout<<"Qualcuno ha settato il goal...  ";

		slamd.goal_.pos.x = clientPack->bufToDouble();
		cout<<slamd.goal_.pos.x<<"   ";

		slamd.goal_.pos.y = clientPack->bufToDouble();
		cout<<slamd.goal_.pos.y<<endl;
	}
}

void SlamThread::UpdatePoseOnRobot()
{
	//mentre viene fatto l'update della posizione Localizzata sul Robot, lo slam non deve lavorare
	SlamDataLock();

	//update sul robot della posizione corrente localizzata
	ArPose temp = pose2_t_to_ArPose(worker.GetPosition());
	temp.setX(temp.getX()*1000.0);
	temp.setY(temp.getY()*1000.0);
	robot.set_localized(temp);

	//rilascio
	SlamDataUnlock();	
}

void	SlamThread::BroadcastData()
{
	ArNetPacket packetMap;
	ArNetPacket packetOthers;
	slamDataNet_->header_ = SlamDataNet::valid_data;
	slamDataNet_->packMap(&packetMap);
	slamDataNet_->packOthers(&packetOthers);
	server_.broadcastPacketTcp(&packetMap,"Mappa");
	ArUtil::sleep(100);
	server_.broadcastPacketTcp(&packetOthers,"Others");
	
	cout<<"dati spediti in broadcast ;)"<<endl;
}

//------------------------------------------------------------------------------------------------
/**	i compiti dello slam Thread sono divisibili in due categorie: CICLICI e NON.
	i compiti NON ciclici vengono effettuati alla costruzione o alla chiamata di qualche metodo particolare
	i compiti CICLICI vengono effettuati nella RUN del thread

	i compiti NON CICLICI sono:
	1) l'apertura della connessione seriale con il Laser
	2) la connessione con il client del Robot
	3) l'inizializzazione di Aria
	4) l'attivazione del server

	i compiti CICLICI sono:
	1) l'Acquisizione di una scansione Laser (dal Laser o dal LogFile)
	2) l'Acquisizione dell'Odometria del Robot (dal Robot o dal LogFile)
	3) il Processamento tramite SLAM di tali dati
	4) il calcolo del goal metrico
	5) il Dispatching dei dati ai Client
	6) l'Aggiornamento della Posizione Localizzata sul Server del Robot
	*/
void* SlamThread::runThread(void* arg)
{
	while(this->ArASyncTask::getRunning())
	{
		AcquireAll();
		//Filter();
		Process();
		FillMap();
		BroadcastData();
		UpdatePoseOnRobot();
		ArUtil::sleep(1);
		cout << "SLAMTHREAD IS RUNNING....................................." << endl;
	}
	return NULL;
}
