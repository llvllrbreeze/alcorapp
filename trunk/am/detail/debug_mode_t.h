//##########################################################
struct debug_mode_t : public unit_state_t
{
  debug_mode_t(){};
    ////IDLE
  virtual void onIDLE(attentive_search_t& machine)
  {
    //machine.reset_devices();
    machine.change_state(idle_debug::Instance());
  };
};
//##########################################################
struct idle_debug_t : public debug_mode_t
{
  idle_debug_t(){m_tick = 300;};
///Override this method if you want your state to do something.
  void invoke(all::am::attentive_search_t& machine)
{
  //machine.change_state(show_images::Instance());
}

};
//##########################################################
struct show_images_t : public debug_mode_t
{
  show_images_t(){m_tick = 50;};

///Override this method if you want your state to do something.
  void invoke(all::am::attentive_search_t& hsm)
{
	//
  hsm.grab_data();    
  
#ifdef XODOLOG
  double odo = hsm.p3dx->get_odometry().getTh().deg();
  double loc = all::core::dconstants::rad_to_deg(hsm.splam_data.path_.back().rot);
  hsm.xlog << odo << "  " << loc << endl;
#endif

  //
  hsm.push_data();


//#ifdef USEPANTILT
  ///
  hsm.call_script("am_debug(rgb, xyz, MappaCur, doropose, pantilt, elapsed)"); 


//#else
  ///
  //hsm.call_script("am_debug(rgb, xyz, MappaX, doropose, [0,0], elapsed)"); 
//#endif
  //
  //hsm.change_state(idle_debug::Instance());
}
};
//##########################################################

//##########################################################
