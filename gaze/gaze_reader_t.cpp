#include "gaze_reader_t.h"
#include "alcor/matlab/matlab_mx_utils.hpp"
//-------------------------------------------------------------------------++
#include "alcor.extern/CImg/CImg.h"
using namespace cimg_library;
//-------------------------------------------------------------------------++
namespace all { namespace gaze {
//-------------------------------------------------------------------------++
  gaze_reader_t::gaze_reader_t():
          current_sample_(0)
        , nsamples_(0)
{
}
//-------------------------------------------------------------------------++
gaze_reader_t::~gaze_reader_t()
{
  if (gazelog_.is_open())
    gazelog_.close();
}
//-------------------------------------------------------------------------++
void gaze_reader_t::load(std::string& binlog)
{
  //
  gazelog_.open(binlog.c_str(), std::ios::in|std::ios::binary);

  printf("\nSession data:\n");
  gazelog_.read((char*)&nsamples_, sizeof(nsamples_));
  printf("-> nsamples: %d\n", nsamples_);

  gazelog_.read((char*)&total_elapsed_, sizeof(total_elapsed_));
  printf("-> total time: %f\n", total_elapsed_);

  gazelog_.read((char*)&eyedims_, sizeof(eyedims_));
  printf("-> eye dims: %d:%d:%d\n", eyedims_.row_, eyedims_.col_, eyedims_.depth_);

  gazelog_.read((char*)&scenedims_, sizeof(scenedims_));
  printf("-> scene dims: %d %d\n", scenedims_.row_, scenedims_.col_);

  printf("\nOffsets:\n");

  gazelog_.read((char*)&elapsed_offset_, sizeof(elapsed_offset_));
  printf("elapsed offset: %d\n", elapsed_offset_);

  gazelog_.read((char*)&eye_offset_, sizeof(eye_offset_));
  printf("eye offset: %d\n", eye_offset_);

  gazelog_.read((char*)&scene_offset_, sizeof(scene_offset_));
  printf("scence offset: %d\n", scene_offset_);

  gazelog_.read((char*)&depth_offset_, sizeof(depth_offset_));
  printf("depth offset: %d\n", depth_offset_);

  gazelog_.read((char*)&rpy_offset_, sizeof(rpy_offset_));
  printf("rpy  offset: %d\n", rpy_offset_);

  //
  header_offset_ =  sizeof(nsamples_) 
                  + sizeof(total_elapsed_)
                  + sizeof(eyedims_)
                  + sizeof(scenedims_)
                  + sizeof(elapsed_offset_)
                  + sizeof(eye_offset_)
                  + sizeof(scene_offset_)
                  + sizeof(depth_offset_)
                  + sizeof(rpy_offset_)
                  ;

  size_t per_sample_offset_ = 
                  elapsed_offset_
                + eye_offset_
                + scene_offset_
                + depth_offset_
                + rpy_offset_;

  printf("header offset: %d\n", header_offset_);
  printf("per sample offset: %d\n", per_sample_offset_);

  allocate_();
}
//-------------------------------------------------------------------------++
void gaze_reader_t::allocate_()
{
  ieye.reset  ( new core::uint8_t  [ eye_offset_ ]  );
  iscene.reset( new core::uint8_t  [ scene_offset_] );
  idepth.reset( new core::single_t [ depth_offset_] );  
}
//-------------------------------------------------------------------------++
void gaze_reader_t::sample_()
{
  //elapsed time .. in seconds
  gazelog_.read((char*)&elapsed_,  elapsed_offset_);

  //eye buffer
  gazelog_.read((char*)&ieye[0],  eye_offset_);

  //rgb scene buffer
  gazelog_.read((char*)&iscene[0],  scene_offset_);

  //xyz depth
  gazelog_.read((char*)&idepth[0],  depth_offset_);

  ///Orientation
  gazelog_.read((char*)&ihead,  rpy_offset_);
}
//-------------------------------------------------------------------------++
void gaze_reader_t::reset()
{
  gazelog_.seekg(header_offset_);
}
//-------------------------------------------------------------------------++
///
void gaze_reader_t::next()
{

}
//-------------------------------------------------------------------------++
///
void gaze_reader_t::play(bool savemat)
{
  reset();

  double last_elapsed = 0;
  unsigned int timeroffset;

  CImgDisplay view (  eyedims_.col_,  eyedims_.row_, "Eye");
  CImg<core::uint8_t> imag;

  CImgDisplay viewscene (  scenedims_.col_,  scenedims_.row_, "SCene");
  CImg<core::uint8_t> imagscene;

  CImgDisplay threedview (  scenedims_.col_,  scenedims_.row_, "3D");
  CImg<core::single_t> threedscene;

  const unsigned char color  [3] = {215,  240,  60};
  const unsigned char blue   [3] = {0,  0,  255};

  ///
  for(size_t i=0 ; i < nsamples_; i++)
  {
    //EXTRACT
    sample_();
    //TIME
    timeroffset = static_cast<unsigned int> ( (elapsed_- last_elapsed) * 1000.0);
    //printf("\nDelta T: %d\n", timeroffset);
    core::BOOST_SLEEP( timeroffset );

    //DRAW
    imag.assign( ieye.get(),  eyedims_.col_, eyedims_.row_, 1, eyedims_.depth_);
    //DRAW
    imagscene.assign( iscene.get(),  scenedims_.col_, scenedims_.row_, 1, scenedims_.depth_);
    //
    imag.draw_text(10,20,  blue, 0, 16, 1, "Elapsed: %.2f", elapsed_);
    imag.draw_text(10,40,  blue, 0, 16, 1, "Roll: %.2f", ihead.roll.deg());
    imag.draw_text(10,60,  blue, 0, 16, 1, "Pitch: %.2f", ihead.pitch.deg());
    imag.draw_text(10,80,  blue, 0, 16, 1, "Yaw: %.2f", ihead.yaw.deg());
    imag.draw_text(10,120, blue, 0, 16, 1, "#: %d", i+1);

    imag.draw_rectangle(1, 1, 120, 150, color, 0.2);
    imag.draw_line(1,1, 120,1, color);
    imag.draw_line(1,1, 0,150, color);
    imag.draw_line(120,1, 120,150, color);
    imag.draw_line(1,150, 120,150, color);

    /////3D
    threedscene.assign(
      (core::single_ptr)idepth.get() + (scenedims_.col_*scenedims_.row_)
         ,  scenedims_.col_
         ,  scenedims_.row_
         ,  1);

    ///----------------------------+
    imag.display(view) ;
    ///----------------------------+
    imagscene.display(viewscene);
    ///----------------------------+
    threedscene.display(threedview);
    ///----------------------------+
    if (savemat)
    {
    ////////////////////////////////////////////////////////////////////////
    //MATLAB --------------------------------------------------------------+
    //
    MATFile *pmat = 0;
    //MATLAB
    std::string namebase = "cameralog_";
    //
    namebase += boost::lexical_cast<std::string>(i+1);
    namebase += ".mat";
    //
    pmat = matOpen(namebase.c_str(), "w");

    //-----------------
    mxArray* mx_rgb = 
      matlab::buffer2array<core::uint8_t>::create_from_planar(iscene.get()
                                                      , matlab::row_major
                                                      , scenedims_.row_
                                                      , scenedims_.col_);
    //-----------------
    mxArray* mx_depth  = 
      all::matlab::buffer2array<  core::single_t  >::create_from_planar(
        idepth.get()
      , matlab::row_major
      , scenedims_.row_
      , scenedims_.col_);

    //add to file
    matPutVariable(pmat, "rgb", mx_rgb);
    matPutVariable(pmat, "xyz", mx_depth);
    
    //  
    matClose(pmat);  

    //
    mxDestroyArray(mx_rgb);
    mxDestroyArray(mx_depth);
    }//savemat -- matlab section code
    ////////////////////////////////////////////////////////////////////////

    //for next loop
    last_elapsed = elapsed_;
    //printf("Roll: %.2f Pitch: %.2f Yaw: %.2f\n"
    //  , ihead.roll.deg(), ihead.pitch.deg(), ihead.yaw.deg());
  }
}
//-------------------------------------------------------------------------++
}}//all::gaze