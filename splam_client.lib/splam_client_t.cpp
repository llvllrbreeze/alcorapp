#include "splam_client_t.h"
#include "alcor.apps/slam/SlamClient.h"

//---------------------------------------------------------------------------
splam_client_t::splam_client_t()
{
  splam_.reset(new SlamClient);
  splam_->runAsync(); 
}
//---------------------------------------------------------------------------

void splam_client_t::get_slam_data(SlamData& data)
{
  splam_->FillSlamData(data);
}

//---------------------------------------------------------------------------splam_
