#include "alcor/sense/bumblebee_driver_t.h"

#include <alcor.extern/ANN/ANN.h>					// ANN declarations
#include <boost/timer.hpp>

#pragma comment (lib, "ANN.lib")
#pragma comment(lib, "bumblebee_driver_t.lib")

using namespace all;

int main()
{

  sense::bumblebee_driver_ptr_t cam = 
      sense::bumblebee_driver_t::create();

  cam->open("config/bumblebeeB.ini");
  //
  int dim = 3;

  boost::timer  timer;
  double grab_3d = 0;
  double fillkd = 0;
  getchar();
  timer.restart();
  //
  cam->grab();
  printf("timetograb: %f\n", timer.elapsed());
  //  
  core::single_sarr depth;
  core::single_sarr depth2;
  int nPts = 0;

getchar();
    timer.restart();
  depth2 = 
    cam->get_depth_buffer(core::interleaved_tag);  
  printf("timetograb3d safe: %f\n", timer.elapsed());
    //
  nPts = 
    cam->valid_3d_points();

  cam->grab();

getchar();
  timer.restart();
   depth = 
    cam->get_depth_buffer_sandbox_();  
  printf("timetograb3d sandbox: %f\n", timer.elapsed());
    //
  nPts = 
    cam->valid_3d_points();



  timer.restart();
  //
  int maxPts = 
    cam->nrows()*cam->ncols();
  //
  ANNpointArray dataPts = annAllocPts(maxPts, dim);  

  for(int i = 0, valid = 0; valid < nPts && i < maxPts;i++)
  {
    if(depth[i+2] > 0)
    {
      dataPts[i][0] = depth[i];
      dataPts[i][1] = depth[i+1];
      dataPts[i][2] = depth[i+2];
    }
  }
  
  ANNkd_tree* kdTree = new ANNkd_tree(					// build search structure
			  dataPts,					// the data points
			  nPts,						// number of points
			  dim);	
 
  printf("kdtree ok! numpoints: %d - time: %f\n", nPts, timer.elapsed());


	//ANNidxArray			nnIdx;					// near neighbor indices
	//ANNdistArray		dists;					// near neighbor distances

  getchar();
	ANNpoint			  queryPt;				// query point
  //int k = 15;
  queryPt = annAllocPt(dim);

  queryPt[0] = 0.0;
  queryPt[1] = 0.0;
  queryPt[2] = 1.5;

  ANNdist  sqradius = 0.12;

  timer.restart();

  int numP = kdTree->annkFRSearch(
    queryPt // query point
    ,sqradius // squared radius
    ,0 );// number of near neighbors to return

  //printf("num Points: %d\n", numP);

	ANNidxArray nnidx = new ANNidx[numP];						// allocate near neigh indices
	ANNdistArray dists = new ANNdist[numP];						// allocate near neighbor dists

  kdTree->annkFRSearch(
    queryPt // query point
    ,sqradius // squared radius
    ,numP // number of near neighbors to return
    ,nnidx  // nearest neighbor array (modified)
    ,dists // dist to near neighbors (modified)
    ,0.0); // error bound

  printf("timetosearch: %f ", timer.elapsed());
  printf("- num Points: %d\n", numP);
  getchar();

  if (numP>0)
  {
    for(int i = 0; i<numP; i++)
    {
      printf("ind: %d -", nnidx[i]);
      printf("- dist: %f\n", sqrt(dists[i]));
      printf("NN: x: %f y: %f z: %f \n\n"
        , dataPts[nnidx[i]][0]
        , dataPts[nnidx[i]][1]
        , dataPts[nnidx[i]][2]);

    }
  }
  getchar();
  annClose();

}