#ifndef pilota_server_base_HPP_INCLUDED
#define pilota_server_base_HPP_INCLUDED
//-----------------------------------------------
#include "alcor/act/p3_gateway.h"
#include "alcor/act/p3_server_t.h"
#include "alcor/act/directed_perception_ptu_server_t.h"
#include "alcor/sense/bumblebee_driver_t.h"
#include "alcor/matlab/matlab_mx_utils.hpp"
//-----------------------------------------------
#include <boost/thread/mutex.hpp> 
//-----------------------------------------------
#include "alcor/core/memory_stream_source_t.hpp"
#include <alcor/core/stream_server_t.hpp>
//-----------------------------------------------
//#define _USE_MATLAB_
//-----------------------------------------------
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#pragma comment (lib,"AriaStatic.lib")
#pragma comment (lib,"ArNetworkingStatic.lib")
//-----------------------------------------------
#pragma comment (lib,"p3_gateway.lib")
#pragma comment (lib,"p3_client_server.lib")
//-----------------------------------------------
#pragma  comment (lib, "data_streaming.lib")
//-----------------------------------------------
#pragma comment (lib, "jpeg_codec.lib")
//-----------------------------------------------
#pragma comment (lib,"bumblebee_driver_t.lib")
//-----------------------------------------------
#if defined(_USE_MATLAB_)
#pragma comment (lib, "libmat.lib")
#pragma comment (lib, "libmx.lib")
#endif
//-----------------------------------------------
#endif
//-----------------------------------------------
#include <boost/lexical_cast.hpp>
//-----------------------------------------------
#if defined(_USE_MATLAB_)
#include "mat.h"
#endif
//-----------------------------------------------
#define ARIA_STATIC
//-----------------------------------------------
using namespace all;
//-----------------------------------------------
class pilota_base_server
{
public:
  ///
  pilota_base_server();

    ///
  void cancel() {running_ = false;};

  ///
  void take_picture();

  //IMAGE STREAM
  all::core::memory_stream_source_sptr_t  stream_source_ptr;
  all::core::stream_server_t*             stream_server_ptr; 

private:
  ///
  void threadloop();

  //ptu
  act::directed_perception_sptr ptu;
  act::directed_perception_ptu_server_sptr  ptuserver;

  //p3
  act::p3_server_ptr_t  p3_server_;
  act::p3_gateway_ptr_t p3;
  ///Bumblebee
  sense::bumblebee_sptr bee;

  static int nshot;
  ///
  boost::shared_ptr<boost::thread> thisthread;
  ///
  volatile bool running_;
  ///
  boost::mutex mutex;

};

//-----------------------------------------------
int pilota_base_server::nshot;
//-----------------------------------------------
inline pilota_base_server::pilota_base_server()
{
  //ptu gateway
  act::directed_perception_sptr ptu (new act::directed_perception_ptu_t);
  ptu->open("config/dpptu_conf.ini");
  //ptu server
  printf("ptuserver\n");
  ptuserver.reset(new act::directed_perception_ptu_server_t("config/ptu_server.ini"));
  printf("ptuserver.assign_ptu\n");
  ptuserver->assign_ptu(ptu);
  ptuserver->run_async();

  //p3 gateway
  std::string configname   = "config/p3at_conf.ini";
  p3.reset(new act::p3_gateway());
  p3->open(configname);

  //p3 server
  p3_server_.reset(new act::p3_server_t(configname.c_str()));
  p3_server_->set_gateway_ptr(p3);
  p3_server_->run_async();

  //bee
    //bumblebee
  bee.reset(new sense::bumblebee_driver_t());
  bee->open("config/bumblebeeA.ini");

  //callback
  p3_server_->set_on_trigger_cb(boost::bind(&pilota_base_server::take_picture, this) );

  
  //Streaming
  stream_source_ptr.reset(new all::core::memory_stream_source_t( bee->nrows(), bee->ncols() ) );
  stream_server_ptr = new all::core::stream_server_t(stream_source_ptr,"config/bumblebee_stream_server.ini");
  stream_server_ptr->run_async();

  thisthread.reset( 
      new boost::thread
      (
          boost::bind(&pilota_base_server::threadloop, this) 
      )
    );  

}
//-----------------------------------------------
inline void pilota_base_server::threadloop()
{
  printf("Starting loop\n");
  running_ = true;
  while (running_)
  { 
    {
      boost::mutex::scoped_lock lock (mutex);
      if(bee->grab())
      {
        core::uint8_sarr rgb = bee->get_color_buffer(core::right_img);
        stream_source_ptr->update_image(rgb);
        BOOST_SLEEP(100);
        boost::thread::yield();
      }
    }
  }
}
//-----------------------------------------------
inline void pilota_base_server::take_picture()
{
  #if defined(_USE_MATLAB_)

  boost::mutex::scoped_lock lock (mutex);
  //grab() and save mat file
  if (bee->grab())
  {
    printf("Images grabbed\n");
    //
    std::string namebase = "cameralog_";//.mat";
    //
    MATFile *pmat = 0;
    //
    namebase += boost::lexical_cast<std::string>(nshot);
    namebase += ".mat";
    //
    pmat = matOpen(namebase.c_str(), "w");
  
    //
    core::uint8_sarr  rgb    = bee->get_color_buffer(core::right_img);
    core::single_sarr depth  = bee->get_depth_buffer();
    
    //-----------------
    mxArray* mx_rgb = 
      matlab::buffer2array<core::uint8_t>::create_from_planar(rgb.get()
                                                      ,matlab::row_major
                                                      ,bee->nrows()
                                                      ,bee->ncols());
    //-----------------
    mxArray* mx_depth  = 
      all::matlab::buffer2array<  core::single_t  >::create_from_planar(
       depth.get()
      ,matlab::row_major
      ,bee->nrows()
      ,bee->ncols());

    matPutVariable(pmat, "rgb",mx_rgb);
    matPutVariable(pmat, "xyz",mx_depth);
    
    //  
    matClose(pmat);    
    nshot++;
    printf("Saved: %d mat\n\n", nshot);

  }
#endif
}

//-----------------------------------------------
#endif //pilota_server_base_HPP_INCLUDED