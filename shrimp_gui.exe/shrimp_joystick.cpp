#include "shrimp_joystick.h"


shrimp_joystick::shrimp_joystick() {
	yaxis_to_speed.set_range(axis_max, axis_min, shrimp_min_speed, shrimp_max_speed);
	xaxis_to_steer.set_range(axis_min, axis_max, shrimp_min_steer, shrimp_max_steer);

	m_mti.open("config/mti_driver.ini");

	reset_mti();

}


all::math::angle shrimp_joystick::get_steer() {
	wxPoint joyPos((*this).GetPosition());
	int steer = xaxis_to_steer.x_to_y(joyPos.x);
	return all::math::angle(steer, all::math::deg_tag);
}

int shrimp_joystick::get_speed() {
	wxPoint joyPos((*this).GetPosition());
	int speed = yaxis_to_speed.x_to_y(joyPos.y);
	return speed;
}


void shrimp_joystick::reset_mti() {
	m_mti.reset(all::sense::tags::align_reset);
}

void shrimp_joystick::get_pan_tilt(all::math::angle& pan, all::math::angle& tilt) {
	all::math::rpy_angle_t rpy = m_mti.get_euler();
	pan = rpy.roll;
	tilt = rpy.pitch;
}



