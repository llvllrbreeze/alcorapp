#include <iostream>
#include "alcor/math/geom2dtypes.h"
#include "alcor/math/angle.h"
#include "alcor/math/point2d.h"
#include "alcor/math/pose2d.h"
#include "alcor/math/line2d.h"
#include "alcor/math/rect2d.h"
#include "alcor/math/size2d.h"
#include "alcor/math/intersection.h"
#include "alcor/math/distance.h"
//#include "alcor/math/circle2d.h"

using namespace std;
using namespace all::math;

int main()
{
	double x(5);
	double y(12000);

	//angle
	angle q(5.0, deg_tag);
	angle qq(5.0, rad_tag);
	angle qqq = q - qq;
	cout << q << " "<<qq<< " "<<qqq<<endl;
	cout << q.cos() << " "<<qq.sin()<< " "<<qqq.tan()<<endl;
	getchar();

	//point2d
	point2d cc;
	point2d aa(2.0, 3.5);
	point2d bb(y,qqq);
	cout << aa << " "<<bb<< " "<<cc<<endl;
	cout << aa.magnitude() << " "<<bb.orientation()<< " "<<cc.rotate(-qqq)<<endl;
	getchar();

	//pose2d
	pose2d pos1(x,y,q);
	pose2d pos2(aa,qq);
	pose2d pos3(pos1);
	pose2d pos4(bb);
	cout << pos1 << " "<<pos2<< " "<<pos3<< " "<<pos4<<endl;
	cout << pos1.getP() << " "<<pos2.getTh()<< " "<<pos3.move(x)<< " "<<pos4.move(y,qqq)<<endl;
	cout << pos1.rotate(qq) << " "<<pos2.relative(cc)<< " "<<pos3.absolute(bb)<< " "<<pos4.getP()+bb<<endl;
	getchar();

	//line2d

	//size2d
	size2d asd = size2d_max;
	size2d rofl(5,-4);
	cout << asd << "   " <<rofl << "i due  size2d sono: " << ((asd==rofl) ? "uguali" : "diversi") <<endl;


	//rect2d

	//poly2d

	//circle2d

	//distance

	//intersection
	point2dVect temp = intersection(rect2d(aa,cc),rect2d(aa,bb));
	cout << temp.front()<<endl;
	getchar();

	return 0;
}