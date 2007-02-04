#ifndef slam_data_mx_adapter_t_HPP_INCLUDED
#define slam_data_mx_adapter_t_HPP_INCLUDED
//-------------------------------------------------------------------------++
#include "alcor.apps/slam/SlamData.h"
#include "alcor/matlab/matlab_mx_utils.hpp"
//-------------------------------------------------------------------------++
#include <boost/type_traits/has_trivial_assign.hpp>
#include <cstdlib>
//-------------------------------------------------------------------------++
namespace all { namespace util {
//-------------------------------------------------------------------------++
namespace detail {
//TODO:is this a nice solution?
//-------------------------------------------------------------------------++
template<typename I1, typename I2, bool b>
I2 copy_imp(I1 first, I1 last, I2 out, const boost::integral_constant<bool, b>&)
{
   while(first != last)
   {
      *out = *first;
      ++out;
      ++first;
   }
   return out;
}
//-------------------------------------------------------------------------++
template<typename T>
T* copy_imp(const T* first, const T* last, T* out, const boost::true_type&)
{
   memcpy(out, first, (last-first)*sizeof(T));
   return out+(last-first);
}
//-------------------------------------------------------------------------++
}//namespace detail
//-------------------------------------------------------------------------++
template<typename I1, typename I2>
inline I2 copy(I1 first, I1 last, I2 out)
{
   //
   // We can copy with memcpy if T has a trivial assignment operator,
   // and if the iterator arguments are actually pointers (this last
   // requirement we detect with overload resolution):
   //
   typedef typename std::iterator_traits<I1>::value_type value_type;
   return detail::copy_imp(first, last, out, boost::has_trivial_assign<value_type>());
}
//-------------------------------------------------------------------------++
}}//all::util
//-------------------------------------------------------------------------++
//###########################################################################
struct slam_data_mx_adapter_t
{
  ///MAP
  mxArray* adapt_map(const SlamData& slamdata)
  {

    mxArray* mxresult = 
      all::matlab::buffer2array<all::matlab::traits<typename all::core::int8_t> >::create_from_planar(
      (all::core::int8_ptr)&slamdata.cells_[0]
      ,all::matlab::row_major
      ,slamdata.mapYdimension_
      ,slamdata.mapXdimension_
      ,1);

  ///
  return mxresult ;
  }

  ///POSE
  mxArray* adapt_pose(const SlamData& slamdata)
  {  
  mxArray* mx_pose = mxCreateDoubleMatrix(1,3,mxREAL);
  double* start_pr = mxGetPr(mx_pose);

  if(!slamdata.path_.empty())
  {
    pose2_t lastpose = slamdata.path_.back();
    //
    //double doropose[] = {lastpose.pos.x, lastpose.pos.y, lastpose.rot};
    //mxSetPr(mx_pose,doropose);
    start_pr[0] =lastpose.pos.x;
    start_pr[1] =lastpose.pos.y; 
    start_pr[2] =lastpose.rot;
  }
  else
  {
    printf("Splam error .. data not initialized.\n");
          //
    start_pr[0] = 0;
    start_pr[1] = 0;
    start_pr[2] = 0;   

    
  }    
  return mx_pose ;
  }

};


#endif //slam_data_mx_adapter_t