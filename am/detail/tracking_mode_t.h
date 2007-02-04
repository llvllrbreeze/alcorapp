//##########################################################
struct tracking_mode_t : public unit_state_t
{
//public:
  tracking_mode_t(): 
    centro_c(320)
    ,centro_r(240)
    ,theta_rob(0)
    ,theta_cam(0)
    ,theta_target(0)
    ,current_pan(0)
    ,current_tilt(0)

  {};

  ////IDLE
  virtual void onIDLE(attentive_search_t& machine)
  {
    machine.change_state(idle_track::Instance());
  };

  //
  int centro_c;
  int centro_r;

  //Sono aggiornate ad ogni tick
  //Sono assolute ma si tiene conto solo del delta ...
  double theta_rob;
  double theta_cam;
  double theta_target;
  double current_pan;
  double current_tilt;

};
//##########################################################
struct idle_track_t : public tracking_mode_t
{
  idle_track_t(){m_tick=250;};
///Override this method if you want your state to do something.
  void invoke(all::am::attentive_search_t& hsm)
{
  //skip
}
};
//##########################################################
///pre
struct init_track_t : public tracking_mode_t
{
  //init_track_t(){m_tick=250;};
///Override this method if you want your state to do something.
  void invoke(all::am::attentive_search_t& machine)
{
  //skip
  //machine.pre_tracking_ini();
  double  centro_r = machine. workspace->get_scalar_double("centro_r");
  double  centro_c = machine.workspace->get_scalar_double("centro_c");
  //
  machine.pt_move_to_screen_coord(centro_r, centro_c, 1.5);

  //INIT TRACKING ...
  //TODO:Aggiorna i setpoint
  float pan, tilt;
  machine.ptu->get_current_pantilt(pan,tilt);

  //double pan_error = machine.pinhole.delta_pan_from_pixel(centro_c) ;
  //Desired Heading
  double theta_rob = machine.p3dx->get_odometry().getTh().deg();

  tracking_loop::Instance().theta_target =  
            theta_rob + pan;

  tracking_loop::Instance().current_pan =
            pan;

  tracking_loop::Instance().current_tilt =
            tilt;

  tracking_loop::Instance().centro_r = 240;
  tracking_loop::Instance().centro_c = 320;

  //START TRACKING LOOP
  machine.change_state(tracking_loop::Instance());
}

};
//##########################################################
///
struct tracking_setup_t : public tracking_mode_t
{
///Override this method if you want your state to do something.
  void invoke(all::am::attentive_search_t& hsm)
{
  hsm.ptu->set_pantilt(0,0,1.0);
  //
  printf("Tracking setup ....  grab\n");
  //
  hsm.grab_tracking_data();
  //
  hsm.push_tracking_data();

  //
  hsm.call_function("[centro_r centro_c coerente] = fh_setup(rgb)");

  hsm.change_state(idle_track::Instance());

  printf("Done Setup\n");

}

};
//##########################################################
///
struct tracking_loop_t : public tracking_mode_t
{
    tracking_loop_t()
    {
      m_tick= 10;
    };

///Override this method if you want your state to do something meningful.
void invoke(all::am::attentive_search_t& hsm)
{
  boost::timer toc;
  
  //CORRECT
  boost::timer tic;
  //-------------------------------------------------------------------------
  //***CURRENT ROBOT HEADING***
  theta_rob    = hsm.p3dx->get_odometry().getTh().deg();
  double delta_target =  theta_target-theta_rob  ;
  //
  //TILT la navigazione è planare ... tiene conto solo del precedente centro_r
  float tilt_error = hsm.pinhole.delta_tilt_from_pixel(static_cast<double>(centro_r));
  //
  float pan_setpoint  =   delta_target; 
  float tilt_setpoint =   current_tilt - tilt_error;

  tic.restart();
  hsm.ptu->set_pantilt(pan_setpoint, tilt_setpoint, 1.0);
  printf("PTU correct %.2f\n",tic.elapsed());

  current_pan = pan_setpoint;
  //
  //printf("\nCORRECTION DEBUG\n");  
  //printf("Theta ROB %.2f\n",    theta_rob);
  ////printf("Pan %.2f\n",   pan);
  //printf("Delta TARGET %.2f\n",    delta_target);
  //printf("Theta TARGET %.2f\n", theta_target);
  //printf("pan_setpoint %.2f\n", pan_setpoint);

  //Dovrebbe stare al centro adesso ... teoricamente
  centro_r = 240;
  centro_c = 320;
  //-------------------------------------------------------------------------
  //***GRAB***
  //printf("\n**TRACKING LOOP ... grab**\n");
  tic.restart();
  hsm.grab_tracking_data();

  hsm.push_tracking_data();  
  printf("Grab: %.2f\n",tic.elapsed());
  //-------------------------------------------------------------------------
  ////***TRACK***
  //printf("call_function\n");
  tic.restart();
  hsm.call_function("[centro_r centro_c]= fh_track_and_show(rgb, [240, 320])");
  printf("Track Routine %.2f\n",tic.elapsed());
  //-------------------------------------------------------------------------
  //***GATHER***
  centro_r = static_cast<int> (hsm.workspace->get_scalar_double("centro_r") );
  centro_c = static_cast<int> (hsm.workspace->get_scalar_double("centro_c") );

  //printf("Nuovo Target Row :%d Col: %d\n",  centro_r,centro_c );

  if(centro_c > 0)//che non sia nullo ....
  {
  //TODO:Aggiorna i setpoint
   //hsm.ptu->update_pantilt();
   //hsm.ptu->get_current_pantilt(pan,tilt);
   //pan = pan_setpoint;
   double pan_delta = hsm.pinhole.delta_pan_from_pixel(centro_c);

  //Desired Heading
  //theta_rob     =  hsm.p3dx->get_odometry().getTh().deg();



  //
  //printf("\n*\ntheta_rob: %.2f\n pan: %.2f\n delta_centro: %.2f\n", theta_rob, pan, pan_delta);
  //printf("theta_target: %.2f\n\n", theta_target);

  //hsm.workspace->put_scalar("theta_robot",  theta_rob*constants<double>::deg_to_rad());
  //hsm.workspace->put_scalar("theta_target", theta_target*constants<double>::deg_to_rad());
  //hsm.workspace->put_scalar("theta_cam", (theta_rob + pan)*constants<double>::deg_to_rad());
  ////
  //hsm.call_function("drawTrackingStatus(theta_robot, theta_cam, theta_target)");

  //-------------------------------------------------------------------------
  //***ROBOT SET POINT***
    //Desired Heading
  theta_rob             =   hsm.p3dx->get_odometry().getTh().deg();  
  theta_target          =   theta_rob + current_pan + pan_delta;
  double delta_heading  =   theta_target - theta_rob;

  if(fabs(delta_heading) > 5)//ops .. una soglia :D
    hsm.p3dx->set_delta_heading(math::angle(delta_heading, math::deg_tag));

  }//centro_c>0
  else
  {
    //TODO: skip??
    centro_c = 320;
    centro_r = 240;
  }

  printf("\nTOTAL %.2f\n",toc.elapsed());
  
}

};