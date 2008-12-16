#include <stdio.h>
#include <string.h>
#include "cv.h"
#include "cvaux.h"
#include "highgui.h"

class FaceRecognizer
{
private:
	//// Global variables
	IplImage ** faceImgArr; // array of face images
	CvMat    *  personNumTruthMat; // array of person numbers
	int nTrainFaces; // the number of training images
	int nEigens; // the number of eigenvalues
	IplImage * pAvgTrainImg; // the average image
	IplImage ** eigenVectArr; // eigenvectors
	CvMat * eigenValMat; // eigenvalues
	CvMat * projectedTrainFaceMat; // projected training faces

public:
	FaceRecognizer();
	~FaceRecognizer();

	void learnAFace(IplImage * newFace);
	int recognizeAFace(IplImage * probeFace);

	//// Function prototypes
	void learn();
	void recognize();
	void doPCA();
	void storeTrainingData();
	int  loadTrainingData(CvMat ** pTrainPersonNumMat);
	int  findNearestNeighbor(float * projectedTestFace);
	int  loadFaceImgArray(char * filename);
};