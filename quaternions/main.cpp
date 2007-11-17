#include "alcor/math/quaternion_t.hpp"

using namespace all;


int main()
{
  math::quat64_t quat;
  quat.assign(32.4,1.0,2.0,3.0);
  quat.normalize();
  ublas::vector<double> axe( quat.axis());
  std::cout << "AXIS" << axe << std::endl;

  math::quat64_t quat2(12,12,23,132);
  ublas::vector<double> vquat(4);
  FILL_UVECT4(vquat,23,11,23,3);
  math::quat64_t quat3(vquat);
  quat3*=10;
  quat3.normalize();
  quat2.normalize();

  //
  math::quat64_t::rotation_matrix_type rot;
  quat2.to_rotation_matrix(rot);

  std::cout << "quaternion: " << quat2.quat() << std::endl;
  std::cout << "rotation  : " << rot << std::endl << std::endl;

  std::cout << "QUAT3 axis: " << quat3.axis() << " scalar " << quat3.scalar()<<  std::endl;
  std::cout <<vquat <<  std::endl;

  math::quat64_t prodo(quat3*quat2);

  math::quat64_t prodo1;
  
  prodo1 = quat3*quat2;

  std::cout << "Compare" <<  std::endl;
  std::cout <<prodo.axis() <<  std::endl;
  std::cout <<prodo1.axis() <<  std::endl;

  math::quat64_t Iq;
  math::quat64_t::vect3_type ax;

  FILL_UVECT3(ax,0,1,0);

  Iq.from_axis_and_angle(ax, math::op64f::d2r(-90));
  
  math::quat64_t::vect3_type v;
  FILL_UVECT3(v,1,0,0);

  std::cout << std::endl<< "Original: " << v << std::endl;
  Iq.rotate(v);
  std::cout << "rotated: " << v << std::endl;

  math::quat64_t::vect3_type vect3;
    math::quat64_t::vect3_type result;
  FILL_UVECT3(vect3, 12,23.2,44.2);
  
  result = prodo*vect3;
  std::cout << "quat*vect: " << result << std::endl;

  getchar();
  return 0;
}