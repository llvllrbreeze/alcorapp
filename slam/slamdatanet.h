#ifndef SLAMDATANET_H
#define	SLAMDATANET_H
//---------------------------------------------------------------------------
#include "Aria.h"
#include "Arnetworking.h"
#include "alcor.apps/slam/SlamData.h"
#include "alcor/core/compression/Codec/RLE.h"
#include "alcor/core/compression/Generic.h"
//---------------------------------------------------------------------------
typedef	std::pair<double,double>				DoublePair;
//---------------------------------------------------------------------------
/**	La classe SlamDataNet è nota al Server ed al Client dello SLAM per trasmettere lo SlamData via TCP
	offre i metodi di Serializzazione import e pack ed un header di controllo.

	L'header AL MOMENTO indica semplicemente se il dato è valido o no (0 = non valido... heart beat)
	
	Lo SlamData DEVE ESSERE ISTANZIATO DALL'ESTERNO... per cui non servono distruttori.
	*/
class	SlamDataNet
{
public:
	enum
	{
		broken_data	=	0,
		valid_data	=	1,
	};

public:
	SlamDataNet(size_t);
	~SlamDataNet();

public:
	SlamData*				data_;
	ArTypes::Byte			header_;
	TGeneric<CRLE>			*runLenght_;
	BYTE					*target_;
	long					target_lenght;



public:
	void	importMap(ArNetPacket*);
	void	packMap(ArNetPacket*);
	void	importOthers(ArNetPacket*);
	void	packOthers(ArNetPacket*);
};
//---------------------------------------------------------------------------
#endif