//##########################################################
struct debug_mode_t : public unit_state_t
{
  debug_mode_t(){};
    ////IDLE
  virtual void onIDLE(attentive_search_t& machine)
  {
    //machine.reset_devices();
    //machine.p3dx->stop_robot();

    machine.change_state(idle_debug::Instance());
  };
};
//##########################################################
struct idle_debug_t : public debug_mode_t
{
  idle_debug_t(){m_tick = 500;};
///Override this method if you want your state to do something.
  void invoke(all::am::attentive_search_t& machine)
{
  machine.change_state(show_images::Instance());
}

};
//##########################################################
struct show_images_t : public debug_mode_t
{
///Override this method if you want your state to do something.
  void invoke(all::am::attentive_search_t& hsm)
{
	//
  hsm.grab_data();  
  //
  hsm.push_data();


#ifdef USEPANTILT
  ///
  hsm.call_script("am_debug(rgb, xyz, MappaX, doropose, pantilt, elapsed)"); 
#else
  ///
  hsm.call_script("am_debug(rgb, xyz, MappaX, doropose, [0,0], elapsed)"); 
#endif
  //
  hsm.change_state(idle_debug::Instance());
}
};
//##########################################################

//##########################################################
