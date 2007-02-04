//##########################################################
struct search_mode_t : public unit_state_t
{
//public:
  search_mode_t(){};

  ////IDLE
  void onIDLE(attentive_search_t& hsm)
  {
    //override?
    hsm.pt_control_loop_sptr->enable(false);
    hsm.pt_control_loop_sptr->clear_polar_reference();

    hsm.reset_devices();
    //nav_loop::Instance().target_dir = 0;
    hsm.change_state(idle_search::Instance());
  };

  void onRESET(attentive_search_t& hsm)
  {
    hsm.pt_control_loop_sptr->enable(false);
    hsm.pt_control_loop_sptr->clear_polar_reference();
    //
    unit_state_t::onRESET(hsm);
  };

};
//##########################################################
struct idle_search_t : public search_mode_t
{
  idle_search_t(){m_tick=250;};
///Override this method if you want your state to do something.
  void invoke(all::am::attentive_search_t& hsm)
{
  hsm.grab_data();
  hsm.push_data();
  hsm.call_script("am_debug(rgb, xyz, MappaCur, doropose, [0,0])");
}
};
//##########################################################
struct nav_loop_t : public search_mode_t
{
  nav_loop_t(){};//:target_dir (0){};
  ///Absolute target direction.
  //double target_dir;
  ///
  void invoke(all::am::attentive_search_t& hsm)
  {
    hsm.grab_data();
    hsm.push_data();
    hsm.call_script("am_debug(rgb, xyz, MappaCur, doropose, pantilt,elapsed)");
  } 

};
//##########################################################
///
struct attentive_loop_t : public search_mode_t
{  
  ///ctor
  attentive_loop_t(){};

///Override this method if you want your state to do something.
  void invoke(all::am::attentive_search_t& hsm)
{
  //
  printf("attentive_loop ... grab\n");
  //
  hsm.grab_data();
  hsm.push_data();

  hsm.call_script("[deltarobot target] = am_attention_loop(rgb, xyz,MappaCur, doropose, pantilt)");
  hsm.call_script("am_debug(rgb, xyz, MappaCur, doropose, pantilt,elapsed)");

  double deltarobot = hsm.workspace->get_scalar_double("deltarobot");
  
  double theta_rob    = core::dconstants::rad_to_deg(hsm.splam_data.path_.back().rot);
  //nav_loop::Instance().target_dir = theta_rob + deltarobot;
  hsm.pt_control_loop_sptr->set_polar_reference(math::deg_tag, theta_rob + deltarobot);

  hsm.doro_delta_heading(deltarobot);

  hsm.pt_control_loop_sptr->enable(true);

  hsm.change_state(nav_loop::Instance());
}

};
