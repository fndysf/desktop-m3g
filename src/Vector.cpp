#include "Vector.hpp"
#include "Exception.hpp"
#include <iostream>
#include <cmath>
using namespace std;
using namespace m3g;


Vector:: Vector () : x(0), y(0), z(0), w(1)
{
}

Vector:: Vector (float x_, float y_, float z_, float w_) :
  x(x_), y(y_), z(z_), w(w_)
{
}

Vector:: ~Vector ()
{
}

void Vector:: get (float* xyz) const
{
  if (w == 0) {
    throw ArithmeticException (__FILE__, __func__, "W component is 0.");
  }
  xyz[0] = x/w; 
  xyz[1] = y/w;
  xyz[2] = z/w;
}

float& Vector:: operator[] (int index)
{
  if (index < 0 || index > 3) {
    throw IndexOutOfBoundsException (__FILE__, __func__, "Index is invalid, index=%d.", index);
  }
  return (&x)[index];
}

const float& Vector:: operator[] (int index) const
{
  if (index < 0 || index > 3) {
    throw IndexOutOfBoundsException (__FILE__, __func__, "Index is invalid, index=%d.", index);
  }
  return (&x)[index];
}

void Vector:: normalize ()
{
  float len = sqrtf(x*x+y*y+z*z);
  if (len == 0) {
    throw ArithmeticException (__FILE__, __func__, "Normalized, but length of vector is 0.");
  }
  x /= len;
  y /= len;
  z /= len;
}

float m3g::dot (const Vector& p, const Vector& q)
{
  return sqrtf (p.x*q.x+p.y*q.y+p.z*q.z);
}

Vector m3g::cross (const Vector& p, const Vector& q)
{
  return Vector (p.y*q.z-p.z*q.y, p.z*q.x-p.x*q.z, p.x*q.y-p.y*q.x);
}

Vector operator+ (const Vector& lhs, const Vector& rhs)
{
  if (lhs.w != rhs.w && lhs.w != 0 && rhs.w != 0) {
    throw IllegalArgumentException (__FILE__, __func__, "Add vectors with different w. lhs.w=%f, rhs.w=%f.", lhs.w, rhs.w);
  }

  if (lhs.w == rhs.w) {
    return Vector(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w);
  } else {
    return Vector(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
  }
}

std::ostream& operator<< (std::ostream& out , const m3g::Vector& v)
{
  return out << v.x << ", " << v.y << ", " << v.z << ", " << v.w;
}

