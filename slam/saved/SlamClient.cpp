#include "alcor/slam/SlamClient.h"
#include <iostream>
using namespace std;
//---------------------------------------------------------------------------
SlamClient::SlamClient(char* inifile)
	:alcor::interfaces::i_connection_handler(inifile)
	,MapDataReceiveCB(this,&SlamClient::MapDataReceive)
	,OthersDataReceiveCB(this,&SlamClient::OthersDataReceive)
	,VictimDataReceiveCB(this,&SlamClient::VictimDataReceive)
	,ini_(inifile)
	,victimCount_(0)
	,victims_(0,0)
{
	//inizializzazione della struttura SlamData
	int lenghtmask = ini_.GetInt("laser:num_step",0);
	slamData_.cells_.resize(lenghtmask,0);
	slamData_.scan_ = slamData_.scan_;

	//inizializzazione di SlamDataNet: associazione della SlamData
	double temp = ini_.GetInt("mappa:larghezza",0) * ini_.GetInt("mappa:altezza",0);
	slamDataNet_ = new SlamDataNet(static_cast<size_t>(temp));
	slamDataNet_->data_= &slamData_;

	ArTypes::Byte tempo=0;
	victimPack.byteToBuf(tempo);
}

SlamClient::~SlamClient()
{
	m_client.requestStop("Mappa");
	m_client.requestStop("Others");
	m_client.requestStop("Victim");
	m_client.remHandler("Mappa",&MapDataReceiveCB);
	m_client.remHandler("Others",&OthersDataReceiveCB);
	m_client.remHandler("Victim",&VictimDataReceiveCB);
	m_client.stopRunning();
	m_client.join();
	stopRunning();
	join();
	delete slamDataNet_;
}

//---------------------------------------------------------------------------
void	SlamClient::register_to()
{
	m_client.addHandler("Mappa",&MapDataReceiveCB);
	m_client.addHandler("Others",&OthersDataReceiveCB);
	m_client.addHandler("Victim",&VictimDataReceiveCB);
	m_client.request("Mappa",200);
	m_client.request("Others",200);
	m_client.request("Victim",200,&victimPack);
}

void	SlamClient::lost_connection()
{
	// cosa fare quando viene persa la connessione?
}

//---------------------------------------------------------------------------
void	SlamClient::MapDataReceive(ArNetPacket* packet)
{
	//Lo slamData_ non deve essere letto mentre viene acquisito.
	slamDataMutex_.lock();

	//Acquisizione dei dati
	slamDataNet_->importMap(packet);

	//cout << "posizione localizzata: "<< slamData_.path_.back().pos.x<< "  "<< slamData_.path_.back().pos.y<< "  "<<slamData_.path_.back().rot<< endl;
	//cout << "goal temporaneo: "<< slamData_.goalTemp_.pos.x<< "  "<< slamData_.goalTemp_.pos.y<< "  "<<slamData_.goalTemp_.rot<< endl;

	//Rilascio del lock
	slamDataMutex_.unlock();
}

//---------------------------------------------------------------------------
void	SlamClient::OthersDataReceive(ArNetPacket* packet)
{
	//Lo slamData_ non deve essere letto mentre viene acquisito.
	slamDataMutex_.lock();

	//Acquisizione dei dati
	slamDataNet_->importOthers(packet);

	//cout << "posizione localizzata: "<< slamData_.path_.back().pos.x<< "  "<< slamData_.path_.back().pos.y<< "  "<<slamData_.path_.back().rot<< endl;
	//cout << "goal temporaneo: "<< slamData_.goalTemp_.pos.x<< "  "<< slamData_.goalTemp_.pos.y<< "  "<<slamData_.goalTemp_.rot<< endl;

	//Rilascio del lock
	slamDataMutex_.unlock();
}

//---------------------------------------------------------------------------
void	SlamClient::VictimDataReceive(ArNetPacket* packet)
{
	victims_.first = packet->bufToDouble();
	victims_.second = packet->bufToDouble();
	victimCount_ = packet->bufToByte();
	cout<<"E' ARRIVATA UNA VITTIMAAAAAAAA!!!!   "<<victims_.first<<"   "<<victims_.second<<endl;
	cout<<"Ci sono ora "<<(int)victimCount_<<" vittime"<<endl;
}

//---------------------------------------------------------------------------
void	SlamClient::FillSlamData(SlamData &target)
{
	//Lo SlamData non deve essere modificato mentre viene letto
	slamDataMutex_.lock();

	//Essendo la struttura SlamData composta da "value"
	target = slamData_;

	//Rilascio del lock
	slamDataMutex_.unlock();
}

bool	SlamClient::IsValid()
{
	bool returnvalue;
	//Lo SlamData non deve essere modificato mentre testiamo la validità dei dati correnti
	slamDataMutex_.lock();

	returnvalue = slamDataNet_->header_ == SlamDataNet::valid_data;

	//Rilascio del lock
	slamDataMutex_.unlock();

	return returnvalue;
}

void		SlamClient::SetVictimPosition(DoublePair pos)
{
	ArTypes::Byte header=1;//1=set, 0=default heartbeat
	ArNetPacket pack;
	pack.byteToBuf(header);
	pack.doubleToBuf(pos.first);
	pack.doubleToBuf(pos.second);
	m_client.requestOnce("Victim",&pack);
	cout<<"Ho appena settato una nuova vittima: "<<pos.first<<"  "<<pos.second<<endl;
}

DoublePair	SlamClient::GetVictimPosition()
{
	cout<<"Ho appena chiesto la posizione della nuova vittima: "<<victims_.first<<"  "<<victims_.second<<endl;
	return victims_;
}
