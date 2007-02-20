#include "alcor.extern\Compression/Codec/LZW.h"
#include "alcor.extern\Compression/Codec/RLE.h"
#include "alcor.extern\Compression/Generic.h"
#include <iostream>
using namespace std;

int main()
{
	long slen, tlen;
	BYTE *target;

	char *temp;

	char vect[1000];
	for(int i=0;i<1000;i++)
		vect[i]=0;
	vect[7] = 3;
	vect[77] = -5;

	TGeneric<CLZW> *lz = new TGeneric<CLZW>;
	tlen = lz->GetMaxEncoded(1000);
	target = new BYTE[tlen+1];
	lz->Encode(target, tlen, (BYTE*)vect, 1000);

	cout << "lunghezza della sorgente: 1000"<<endl;
	cout << "lunghezza dopo la compressione: "<< tlen<<endl;

	delete[] target;

	TGeneric<CRLE> *punt = new TGeneric<CRLE>;
	tlen = punt->GetMaxEncoded(1000);
	target = new BYTE[tlen+1];
	punt->Encode(target, tlen, (BYTE*)vect, 1000);

	cout << "lunghezza della sorgente: 1000"<<endl;
	cout << "lunghezza dopo la compressione: "<< tlen<<endl;

	temp = new char[1010];

	punt->Decode((BYTE*)temp,slen,target,tlen);
	
	cout << "lunghezza della decodifica: "<<slen<<endl;
	cout << "carattere 7: "<<(int)temp[7]<<endl;
	cout << "carattere 77: "<<(int)temp[77]<<endl;
}