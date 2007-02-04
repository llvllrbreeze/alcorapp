
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
//TRACKING
    virtual void onTRACKING(attentive_search_t& hsm)
    {
      hsm.change_state(init_track::Instance());
    };

    virtual void onTRACKINGSETUP(attentive_search_t& hsm)
    {
      hsm.change_state(tracking_setup::Instance());
    };
///////////////////////////////////////////////////////////////

	  //// 
    virtual void onSTARTSEARCH(attentive_search_t& hsm)
    {
      hsm.change_state(attentive_loop::Instance());
    };

    ////RESET
    virtual void onRESET(attentive_search_t& machine)
    {
      machine.reset_devices();
      machine.change_state(listen::Instance());
    };

    ////RESET
    virtual void onRESETPANTILT(attentive_search_t& machine)
    {
       if(machine.ptu.get())   
       {
         machine.ptu->reset();
       }
       machine.change_state(listen::Instance());
    };

    ////DEBUG
    virtual void onDEBUG(attentive_search_t& machine)
    {

      if(machine.ptu.get())
        machine.ptu->set_pantilt(0,0);

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


//  class OWidget;
//
//class ODerivedWidget : public OWidget;
//
//class OWidgetContainer : public OWidget
//{
//    public:
//
//    void Add( boost::shared_ptr<OWidget>& widget )
//   {
//        mWidgets.pushback(widget);
//   }
//
//    std::vector<boost::shared_ptr<OWidget>>     mWidgets;
//}