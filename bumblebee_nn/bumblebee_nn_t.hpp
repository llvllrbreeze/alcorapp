#ifndef ann_t_HPP_INCLUDED
#define ann_t_HPP_INCLUDED
//-------------------------------------------------------------------
#include <boost/shared_ptr.hpp>
#include <alcor.extern/ANN/ANN.h>					// ANN declarations
#include "alcor/core/core.h"
//-------------------------------------------------------------------
namespace all {  namespace core {
//-------------------------------------------------------------------
///
class ann_t
{
public:
  ///
  ann_t();

  ///
  

private:
  //
  boost::shared_ptr<ANNkd_tree>		
    kdTree;
};
//-------------------------------------------------------------------
//all::core
//-------------------------------------------------------------------
inline ann_t::ann_t()
{
}


//-------------------------------------------------------------------
//inline ann_t::ann_t()
//{
//}
//-------------------------------------------------------------------
#endif //ann_t_HPP_INCLUDED