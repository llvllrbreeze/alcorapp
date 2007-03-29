#ifndef shrimp_joystick_H_INCLUDED
#define shrimp_joystick_H_INCLUDED

#include "wx/joystick.h"
#include <alcor/math/angle.h>
#include <alcor/core/interval_map_t.hpp>
#include <alcor/sense/MTi_driver_t.h>

class shrimp_joystick : public wxJoystick {

public:

	shrimp_joystick();

	all::math::angle get_steer();
	int get_speed();

	void get_pan_tilt(all::math::angle& pan, all::math::angle& tilt);

	void reset_mti();

	

private:

	//shrimp parameters
	static const int shrimp_max_speed = 450;
	static const int shrimp_min_speed = -450;

	static const int shrimp_max_steer = 90;
	static const int shrimp_min_steer = -90;
	
	//joystick axis values
	static const int axis_min = 0;
	static const int axis_max = 65536;

	all::core::interval_map_t yaxis_to_speed;
	all::core::interval_map_t xaxis_to_steer;

	

	all::sense::MTi_driver_t m_mti;
	

};

#endif