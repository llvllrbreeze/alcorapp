#include "alcor/sense/bumblebee_driver_t.h"

#include <alcor.extern/ANN/ANN.h>					// ANN declarations

#pragma comment (lib, "ANN.lib")
#pragma comment(lib, "bumblebee_driver_t.lib")

using namespace all;

int main()
{

  sense::bumblebee_driver_ptr_t cam = 
      sense::bumblebee_driver_t::create();

  cam->open("config/bumblebeeB.ini");

  getchar();
  //
  cam->grab();

  //
  int maxPts = 
    cam->nrows()*cam->ncols();

  //
  int dim = 3;
  //
  ANNpointArray dataPts = annAllocPts(maxPts, dim);  
  
  //
  core::single_sarr depth = 
    cam->get_depth_buffer(core::interleaved_tag);
  //
  int nPts = 
    cam->valid_3d_points();

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

	ANNpoint			  queryPt;				// query point
	ANNidxArray			nnIdx;					// near neighbor indices
	ANNdistArray		dists;					// near neighbor distances


}