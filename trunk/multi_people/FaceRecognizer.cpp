#include "FaceRecognizer.h"

FaceRecognizer::FaceRecognizer()
{
	faceImgArr = 0; // array of face images
	personNumTruthMat = 0; // array of person numbers
	nTrainFaces = 0; // the number of training images
	nEigens = 0; // the number of eigenvalues
	pAvgTrainImg = 0; // the average image
	eigenVectArr = 0; // eigenvectors
	eigenValMat = 0; // eigenvalues
	projectedTrainFaceMat = 0; // projected training faces

}

FaceRecognizer::~FaceRecognizer()
{
}

void FaceRecognizer::learnAFace(IplImage * newFface)
{

}

int FaceRecognizer::recognizeAFace(IplImage * probeFace)
{
	CvMat * trainPersonNumMat = 0;  // the person numbers during training
	float * projectedTestFace = 0;	// the projected probe face

	// load the saved training data
	if( !loadTrainingData( &trainPersonNumMat ) ) return -1;

	// project the test images onto the PCA subspace
	projectedTestFace = (float *)cvAlloc( nEigens*sizeof(float) );

	
	int iNearest, nearest, truth;

		// project the test image onto the PCA subspace
		cvEigenDecomposite(
			probeFace,
			nEigens,
			eigenVectArr,
			0, 0,
			pAvgTrainImg,
			projectedTestFace);

		iNearest = findNearestNeighbor(projectedTestFace);
		nearest  = trainPersonNumMat->data.i[iNearest];

		return nearest;	
}

//////////////////////////////////
// learn()
//
void FaceRecognizer::learn()
{
	int i, offset;

	// load training data
	nTrainFaces = loadFaceImgArray("train.txt");
	if( nTrainFaces < 2 )
	{
		fprintf(stderr,
			"Need 2 or more training faces\n"
			"Input file contains only %d\n", nTrainFaces);
		return;
	}

	// do PCA on the training faces
	doPCA();

	// project the training images onto the PCA subspace
	projectedTrainFaceMat = cvCreateMat( nTrainFaces, nEigens, CV_32FC1 );
	offset = projectedTrainFaceMat->step / sizeof(float);
	for(i=0; i<nTrainFaces; i++)
	{
		//int offset = i * nEigens;
		cvEigenDecomposite(
			faceImgArr[i],
			nEigens,
			eigenVectArr,
			0, 0,
			pAvgTrainImg,
			//projectedTrainFaceMat->data.fl + i*nEigens);
			projectedTrainFaceMat->data.fl + i*offset);
	}

	// store the recognition data as an xml file
	storeTrainingData();
}


//////////////////////////////////
// recognize()
//
void FaceRecognizer::recognize()
{
	int i, nTestFaces  = 0;         // the number of test images
	CvMat * trainPersonNumMat = 0;  // the person numbers during training
	float * projectedTestFace = 0;

	// load test images and ground truth for person number
	nTestFaces = loadFaceImgArray("test.txt");
	printf("%d test faces loaded\n", nTestFaces);

	// load the saved training data
	if( !loadTrainingData( &trainPersonNumMat ) ) return;

	// project the test images onto the PCA subspace
	projectedTestFace = (float *)cvAlloc( nEigens*sizeof(float) );
	for(i=0; i<nTestFaces; i++)
	{
		int iNearest, nearest, truth;

		// project the test image onto the PCA subspace
		cvEigenDecomposite(
			faceImgArr[i],
			nEigens,
			eigenVectArr,
			0, 0,
			pAvgTrainImg,
			projectedTestFace);

		iNearest = findNearestNeighbor(projectedTestFace);
		truth    = personNumTruthMat->data.i[i];
		nearest  = trainPersonNumMat->data.i[iNearest];

		printf("nearest = %d, Truth = %d\n", nearest, truth);
	}
}


//////////////////////////////////
// loadTrainingData()
//
int FaceRecognizer::loadTrainingData(CvMat ** pTrainPersonNumMat)
{
	CvFileStorage * fileStorage;
	int i;

	// create a file-storage interface
	fileStorage = cvOpenFileStorage( "facedata.xml", 0, CV_STORAGE_READ );
	if( !fileStorage )
	{
		fprintf(stderr, "Can't open facedata.xml\n");
		return 0;
	}

	nEigens = cvReadIntByName(fileStorage, 0, "nEigens", 0);
	nTrainFaces = cvReadIntByName(fileStorage, 0, "nTrainFaces", 0);
	*pTrainPersonNumMat = (CvMat *)cvReadByName(fileStorage, 0, "trainPersonNumMat", 0);
	eigenValMat  = (CvMat *)cvReadByName(fileStorage, 0, "eigenValMat", 0);
	projectedTrainFaceMat = (CvMat *)cvReadByName(fileStorage, 0, "projectedTrainFaceMat", 0);
	pAvgTrainImg = (IplImage *)cvReadByName(fileStorage, 0, "avgTrainImg", 0);
	eigenVectArr = (IplImage **)cvAlloc(nTrainFaces*sizeof(IplImage *));
	for(i=0; i<nEigens; i++)
	{
		char varname[200];
		sprintf( varname, "eigenVect_%d", i );
		eigenVectArr[i] = (IplImage *)cvReadByName(fileStorage, 0, varname, 0);
	}

	// release the file-storage interface
	cvReleaseFileStorage( &fileStorage );

	return 1;
}


//////////////////////////////////
// storeTrainingData()
//
void FaceRecognizer::storeTrainingData()
{
	CvFileStorage * fileStorage;
	int i;

	// create a file-storage interface
	fileStorage = cvOpenFileStorage( "facedata.xml", 0, CV_STORAGE_WRITE );

	// store all the data
	cvWriteInt( fileStorage, "nEigens", nEigens );
	cvWriteInt( fileStorage, "nTrainFaces", nTrainFaces );
	cvWrite(fileStorage, "trainPersonNumMat", personNumTruthMat, cvAttrList(0,0));
	cvWrite(fileStorage, "eigenValMat", eigenValMat, cvAttrList(0,0));
	cvWrite(fileStorage, "projectedTrainFaceMat", projectedTrainFaceMat, cvAttrList(0,0));
	cvWrite(fileStorage, "avgTrainImg", pAvgTrainImg, cvAttrList(0,0));
	for(i=0; i<nEigens; i++)
	{
		char varname[200];
		sprintf( varname, "eigenVect_%d", i );
		cvWrite(fileStorage, varname, eigenVectArr[i], cvAttrList(0,0));
	}

	// release the file-storage interface
	cvReleaseFileStorage( &fileStorage );
}


//////////////////////////////////
// findNearestNeighbor()
//
int FaceRecognizer::findNearestNeighbor(float * projectedTestFace)
{
	//double leastDistSq = 1e12;
	double leastDistSq = DBL_MAX;
	int i, iTrain, iNearest = 0;

	for(iTrain=0; iTrain<nTrainFaces; iTrain++)
	{
		double distSq=0;

		for(i=0; i<nEigens; i++)
		{
			float d_i =
				projectedTestFace[i] -
				projectedTrainFaceMat->data.fl[iTrain*nEigens + i];
			//distSq += d_i*d_i / eigenValMat->data.fl[i];  // Mahalanobis
			distSq += d_i*d_i; // Euclidean
		}

		if(distSq < leastDistSq)
		{
			leastDistSq = distSq;
			iNearest = iTrain;
		}
	}

	return iNearest;
}


//////////////////////////////////
// doPCA()
//
void FaceRecognizer::doPCA()
{
	int i;
	CvTermCriteria calcLimit;
	CvSize faceImgSize;

	// set the number of eigenvalues to use
	nEigens = nTrainFaces-1;

	// allocate the eigenvector images
	faceImgSize.width  = faceImgArr[0]->width;
	faceImgSize.height = faceImgArr[0]->height;
	eigenVectArr = (IplImage**)cvAlloc(sizeof(IplImage*) * nEigens);
	for(i=0; i<nEigens; i++)
		eigenVectArr[i] = cvCreateImage(faceImgSize, IPL_DEPTH_32F, 1);

	// allocate the eigenvalue array
	eigenValMat = cvCreateMat( 1, nEigens, CV_32FC1 );

	// allocate the averaged image
	pAvgTrainImg = cvCreateImage(faceImgSize, IPL_DEPTH_32F, 1);

	// set the PCA termination criterion
	calcLimit = cvTermCriteria( CV_TERMCRIT_ITER, nEigens, 1);

	// compute average image, eigenvalues, and eigenvectors
	cvCalcEigenObjects(
		nTrainFaces,
		(void*)faceImgArr,
		(void*)eigenVectArr,
		CV_EIGOBJ_NO_CALLBACK,
		0,
		0,
		&calcLimit,
		pAvgTrainImg,
		eigenValMat->data.fl);

	cvNormalize(eigenValMat, eigenValMat, 1, 0, CV_L1, 0);
}


//////////////////////////////////
// loadFaceImgArray()
//
int FaceRecognizer::loadFaceImgArray(char * filename)
{
	FILE * imgListFile = 0;
	char imgFilename[512];
	int iFace, nFaces=0;


	// open the input file
	if( !(imgListFile = fopen(filename, "r")) )
	{
		fprintf(stderr, "Can\'t open file %s\n", filename);
		return 0;
	}

	// count the number of faces
	while( fgets(imgFilename, 512, imgListFile) ) ++nFaces;
	rewind(imgListFile);

	// allocate the face-image array and person number matrix
	faceImgArr        = (IplImage **)cvAlloc( nFaces*sizeof(IplImage *) );
	personNumTruthMat = cvCreateMat( 1, nFaces, CV_32SC1 );

	// store the face images in an array
	for(iFace=0; iFace<nFaces; iFace++)
	{
		// read person number and name of image file
		fscanf(imgListFile,
			"%d %s", personNumTruthMat->data.i+iFace, imgFilename);

		// load the face image
		faceImgArr[iFace] = cvLoadImage(imgFilename, CV_LOAD_IMAGE_GRAYSCALE);

		if( !faceImgArr[iFace] )
		{
			fprintf(stderr, "Can\'t load image from %s\n", imgFilename);
			return 0;
		}
	}

	fclose(imgListFile);

	return nFaces;
}