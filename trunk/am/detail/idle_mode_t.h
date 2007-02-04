//##########################################################
class idle_mode_t : public unit_state_t
{
public:
  idle_mode_t(){};

};
//##########################################################
///
class listen_t : public idle_mode_t
{
	//ListenState
public:
	listen_t():idle_mode_t(){};

public:
	virtual void invoke(attentive_search_t& hsm){};

public:
	 // //// TAKE_SNAP_SHOT
  //virtual void onSTARTSEARCH(attentive_search_t& hsm)
  //{
  //  hsm.p3dx->set_vel(50);
  //  hsm.change_state(attentive_loop::Instance());
  //};

public:
	virtual void print(){cout << "Idle::Listen" << endl;};
};
//##########################################################