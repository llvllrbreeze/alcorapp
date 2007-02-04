//##########################################################
struct search_mode_t : public unit_state_t
{
//public:
  search_mode_t(){};

  ////IDLE
  virtual void onIDLE(attentive_search_t& machine)
  {
    //machine.reset_devices();
    machine.p3dx->stop_robot();
    machine.change_state(idle_search::Instance());
  };

};
//##########################################################
struct idle_search_t : public search_mode_t
{
  idle_search_t(){m_tick=250;};
///Override this method if you want your state to do something.
  void invoke(all::am::attentive_search_t& hsm)
{

}
};
//##########################################################
///
struct attentive_loop_t : public search_mode_t
{
///Override this method if you want your state to do something.
  void invoke(all::am::attentive_search_t& hsm)
{
  //
  printf("attentive_loop ... grab\n");
  //
  hsm.grab_data();
  //put images in workspace....
  printf("Push data in WS\n");
  hsm.push_data();

  ///
  printf("Run Saliency\n");
  //hsm.call_script("[am_winx am_winy] = am_process_rgb(rgb)");
  //hsm.call_script("am_display_xyz(xyz)");
//"rgb"
//"xyz"
//"MappaX"
//"doropose"
//"pantilt"
#ifdef USEPANTILT
  hsm.call_script("am_attention_loop(rgb, xyz,MappaX, doropose, pantilt)");
  hsm.call_script("am_debug(rgb, xyz, MappaX, doropose, pantilt)");
#else
  hsm.call_script("am_attention_loop(rgb, xyz,MappaX, doropose, [0,0])");
  hsm.call_script("am_debug(rgb, xyz, MappaX, doropose, [0,0])");
#endif

  //hsm.change_state(idle_search::Instance());

  ////read results ....
  //double row = hsm.workspace->get_scalar_double("am_winx");
  //double col = hsm.workspace->get_scalar_double("am_winy");

  //printf("Winx (row) = %f Winy (col) = %f\n", row, col);
  /////
  ////hsm.call_script("am_show_splam(mappa)");

  ////related actions on doro .... or states ..
  ////ptu?
  //hsm.pt_move_to_screen(row, col);
  //hsm.doro_delta_heading_from_screen(col);

}

};
