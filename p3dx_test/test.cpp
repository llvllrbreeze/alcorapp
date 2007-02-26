#include "alcor/act/p3dx_gateway.h"
#include <iostream>

using namespace all::act;

int main()
{
	p3dx_gateway	p3dx;
	p3dx.serial_connect();
	getchar();

	ArPose temp;
	
	//p3dx.move(1000.0);
	//getchar();

	//temp = p3dx.get_odometry();
	//std::cout << std::endl << temp.getX() << "   "<<temp.getY()<<"   "<<temp.getTh()<<std::endl;
	p3dx.set_heading(90.0);
	getchar();

	temp = p3dx.get_odometry();
	std::cout << std::endl << temp.getX() << "   "<<temp.getY()<<"   "<<temp.getTh()<<std::endl;
	getchar()
}