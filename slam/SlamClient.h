#ifndef	SLAMSERVER_H
#define	SLAMSERVER_H
//---------------------------------------------------------------------------
#include "Aria.h"
#include "ArNetworking.h"
#include "alcor.apps/slam/slamdatanet.h"
#include "alcor/deprecated/i_connection_handler.h"
#include "alcor/core/iniwrapper.h"
//---------------------------------------------------------------------------
#pragma comment (lib,"coder.lib")
//---------------------------------------------------------------------------
class	SlamClient	:public alcor::interfaces::i_connection_handler
{
public:	//typedefs
	typedef ArFunctor1C<SlamClient, ArNetPacket*>	Functor;

public:	//ctor, dtor, copy ctor, copy assign
	SlamClient(char* inifile = "config/slam.ini");
	~SlamClient();

public:	//SERVICES!!!!
	/**	il metodo FillSlamData prende in imput una reference all oggetto da riempire e copia
		in esso il contenuto dell'oggetto privato slamData_. 
		prendendo il parametro per reference, gli utilizzatori del Client non devono preoccuparsi
		della allocazione e deallocazione del proprio oggetto SlamData. ci pensa il metodo :D*/
	void			FillSlamData(SlamData&);
	bool			IsValid();
	void			SetVictimPosition(DoublePair);
	DoublePair		GetVictimPosition();
	ArTypes::Byte	GetVictimCount(){return victimCount_;}

private:	//callbacks per la ricezione dei dati
	Functor		MapDataReceiveCB;
	Functor		OthersDataReceiveCB;
	Functor		VictimDataReceiveCB;
	void		MapDataReceive(ArNetPacket*);
	void		OthersDataReceive(ArNetPacket*);
	void		VictimDataReceive(ArNetPacket*);
	ArNetPacket	victimPack;

private:	//callback su "avvenuta connessione" e "disconnessione" dal server
	virtual void register_to();
	virtual void lost_connection();

private:	//data
	SlamData		slamData_;
	SlamDataNet		*slamDataNet_;
	ArMutex			slamDataMutex_;
	DoublePair		victims_;
	ArTypes::Byte	victimCount_;

private:
	iniWrapper		ini_;
};
//---------------------------------------------------------------------------
#endif