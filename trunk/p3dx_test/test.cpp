#include "alcor/act/p3dx_gateway.h"
#include <iostream>
#include <boost/timer.hpp>

using namespace all::act;

int main()
{
	p3dx_gateway	p3dx;
	p3dx.serial_connect();
	ArPose			startPose,endPose;
	double			timeElapsed;
	boost::timer	crono;

	getchar();

	p3dx.set_vel(400.0);

	getchar();

	startPose = p3dx.get_odometry();
	crono.restart();

	getchar();

	endPose = p3dx.get_odometry();
	timeElapsed = crono.elapsed();
	p3dx.stop();
	std::cout << std::endl << "distanza percorsa: "<< endPose.findDistanceTo(startPose) << "   " << "velocita: "<< endPose.findDistanceTo(startPose)/timeElapsed<< std::endl;

	//p3dx.move(1000.0);
	//getchar();

	//temp = p3dx.get_odometry();
	//std::cout << std::endl << temp.getX() << "   "<<temp.getY()<<"   "<<temp.getTh()<<std::endl;

	getchar();
}