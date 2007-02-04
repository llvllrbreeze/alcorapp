
  /// Base class for all states of the Unit state machine. Default handlers are
  /// defined for all the events. Base state handler will be invoked if a class does not 
  /// override these handlers.
  struct unit_state_t
  {

    unit_state_t(): m_tick(100)
    {

    };

	  ///Override this method if you want your state to do something.
	  virtual void invoke(attentive_search_t& ){};

///////////////////////////////////////////////////////////////

	  //// 
    virtual void onSTARTSEARCH(attentive_search_t& hsm)
    {
      //hsm.p3dx->set_vel(50);
      //TODO:  qualcosa del tipo 'hsm.pre_start_search'
      hsm.change_state(attentive_loop::Instance());
    };
    ////RESET
    virtual void onRESET(attentive_search_t& machine)
    {
      machine.reset_devices();
      machine.change_state(listen::Instance());
    };
    ////DEBUG
    virtual void onDEBUG(attentive_search_t& machine)
    {
#ifdef USEPANTIL
      machine.ptu.setPanTilt(0,0);
#endif
      machine.change_state(show_images::Instance());
    };

    ////IDLE di default è un reset ...
    virtual void onIDLE(attentive_search_t& machine)
    {
      machine.reset_devices();
      machine.change_state(listen::Instance());
    };

  //public:
	  ///Tick period in msec.
	  int		get_tick() const {return m_tick;}
	  ///Tick period in msec.
	  void	set_tick(int tick){m_tick=tick;}

  //private:
	  ///Intra Loop Period
	  int m_tick;
	  };

//#endif //unit_state_H_INCLUDED