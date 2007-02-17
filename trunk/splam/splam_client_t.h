#ifndef splam_client_t_H_INCLUDED
#define splam_client_t_H_INCLUDED
//---------------------------------------------------------------------------
#pragma comment (lib, "splam_client_t.lib")
//---------------------------------------------------------------------------
#include "alcor.apps\slam\SlamData.h"
#include <boost\shared_ptr.hpp>
//---------------------------------------------------------------------------
class SlamClient;
//---------------------------------------------------------------------------
struct splam_client_t
{
public:
  splam_client_t(); 

public:
  void get_slam_data(SlamData&);


private:
  boost::shared_ptr<SlamClient> splam_;

};
//---------------------------------------------------------------------------

#endif //splam_client_t_H_INCLUDED