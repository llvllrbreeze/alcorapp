//--------------------------------------------------------------------++
#include "mex.h"
//--------------------------------------------------------------------++
#include "alcor/matlab/opencv_matlab_utils.hpp"
//--------------------------------------------------------------------++
#include <boost/function.hpp>
#include <map>
#include <boost/assign/list_inserter.hpp>
//#include "highgui.h"
//--------------------------------------------------------------------++
using namespace all;
//--------------------------------------------------------------------++
#define MX_CALL_FUNCTION(_funcname_) extern void _funcname_ \
  ( int nlhs, mxArray *plhs[], int nrhs, const mxArray  *prhs[]); 
////--------------------------------------------------------------------++
MX_CALL_FUNCTION(cv_create)
MX_CALL_FUNCTION(cv_laplace)
MX_CALL_FUNCTION(cv_pyrdown2)
MX_CALL_FUNCTION(cv_resize)
//--------------------------------------------------------------------++
/// The "real" mex function.
/// Basically 'routes' the control to a specific external routine.
//--------------------------------------------------------------------++
typedef boost::function<void (int nlhs, 
                        mxArray* plhs[], 
                        int nrhs, 
                        const mxArray* prhs[])> func_t;

typedef std::map<int,func_t> function_table_t;

static		function_table_t*	p_function_table	= 0;
//--------------------------------------------------------------------++
//--------------------------------------------------------------------++
static	int	myStaticDataInitialized	= 0;
static  std::size_t myFuncTableSize	= 0;
//--------------------------------------------------------------------++
void exitFcn() 
{
	if(myStaticDataInitialized==0)
	{
		printf("exitFun Invoked ...Cleaning\n");
		if(p_function_table !=  0)
		{
      printf("deleting function table\n");
			//delete p_function_table;
      printf("deleted function table!!!!\n");
			p_function_table = 0;
		}	
		myStaticDataInitialized	= 1;
		myFuncTableSize			= 0;
	}

}
//--------------------------------------------------------------------++
static void init_function_table()
{
	if(myStaticDataInitialized == 0)
	{
		//
		p_function_table = new function_table_t;

    boost::assign::insert(*p_function_table) 
      (0, &cv_create)//self.CREATE = 0;
      (1, &cv_laplace)//self.LAPLACE_5 = 1;
      (2, &cv_pyrdown2)//self.PYRDOWN = 2;
      (3, &cv_resize)//self.RESIZE = 4
      ;
    //
		myStaticDataInitialized = 1;
		myFuncTableSize = static_cast<int> (p_function_table->size());

		mexMakeMemoryPersistent(&myStaticDataInitialized);
		mexMakeMemoryPersistent(&myFuncTableSize);
		mexMakeMemoryPersistent(p_function_table);

		mexAtExit(exitFcn);
	}
};
//--------------------------------------------------------------------++
#define COMMAND_IN prhs[0]
//--------------------------------------------------------------------++
void mexFunction( int nlhs, mxArray* plhs[], int nrhs, const mxArray
*prhs[] )
{
	if(nrhs >0)
	{
		if(myStaticDataInitialized == 0)
		{
			printf("Initializing  mex file\n");
			init_function_table();
		}
		
		int cmd = static_cast<int>(mxGetScalar(COMMAND_IN));
    
		if( cmd >= 0 && cmd < myFuncTableSize )
			{
				//Ok we are in the range...
				//func_t func = function_table->at(cmd);
        func_t func = p_function_table->operator [](cmd);
				//Check ... you'll never know...
				if(func)
					// N.B. the -1 and +1
					//That's why we have already read 
					//from the std arguments arrays
					func(nlhs, plhs, nrhs-1, prhs+1);
			}
		else
	     	mexErrMsgTxt("Something is bad .. null function reference.\n");		
    }
    else
    {
    	printf("Specify Command ID please.\n");
    }
}
////--------------------------------------------------------------------++
//
//void mexFunction( int nlhs, mxArray* plhs[], int nrhs, const mxArray
//*prhs[] )
//{
//  if(nrhs>0)
//  {
//    //printf("passing image to mxarray_to_iplimage\n");
//
//    //matlab::mxcv::iplimage_ptr p_image = matlab::mxcv::mxarray_to_iplimage<core::uint8_t>(prhs[0]);
//    IplImage* p_image   = matlab::mxcv::mxarray_to_iplimage<core::uint8_t>(prhs[0]);
//
//    //
//    IplImage* p_laplace_img16 = cvCreateImage(cvSize(p_image->width,p_image->height),IPL_DEPTH_16S,1);
//
//    //
//    IplImage* p_laplace_img8  = cvCloneImage(p_image);
//
//    //
//    printf("Apply Sobel Operator\n");
//    cvLaplace( p_image, p_laplace_img16, 5);
//
//    //
//    printf("Convert Scale\n");
//    cvConvertScale( p_laplace_img16, p_laplace_img8, 1, 0 );
//
//    //printf("Show Image\n");
//    //cvNamedWindow("Originale");
//    //cvShowImage("Originale", p_image);
//    //cvNamedWindow("laplace");
//    //cvShowImage("laplace", p_laplace_img8);
//
//
//    if(nlhs > 0)
//    {
//      //
//      plhs[0] = matlab::mxcv::iplimage_to_mxarray<core::int16_t>(p_laplace_img16);
//    }
//    //all::core::BOOST_SLEEP(1000);
//    //cvDestroyWindow("boooo");
//    //all::core::BOOST_SLEEP(100);
//
//    //matlab::mxcv::release_iplimage(p_image);
//    matlab::mxcv::release_iplimage(p_laplace_img16);
//    matlab::mxcv::release_iplimage(p_laplace_img8);
//  }
//
//}

