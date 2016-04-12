#include "Vec3.hpp"
#include <sstream>
#include <cmath>

Vec3::Vec3() :
   vals{ 0.0f, 0.0f, 0.0f }
{}

Vec3::Vec3(GLfloat x, GLfloat y, GLfloat z) :
   vals{ x, y, z }
{}

Vec3::Vec3(const std::string& input) :
   vals
{
   std::istringstream iss(input);
   iss >> vals[0] >> vals[1] >> vals[2];
}

Vec3::Vec3(const Vec3& src) :
   vals{ src[0], src[1], src[2] }
{}

Vec3& Vec3::operator=(const Vec3& rhs) {
   vals[0] = rhs[0];
   vals[1] = rhs[1];
   vals[2] = rhs[2];
   return *this;
}

GLfloat Vec3::operator [](int i) const { return vals[i]; }
GLfloat& Vec3::operator [](int i) { return vals[i]; }

Vec3 Vec3::operator+(const Vec3& rhs) const { return Vec3(vals[0]+rhs[0], vals[1]+rhs[1], vals[2]+rhs[2]); }
Vec3& Vec3::operator+=(const Vec3& rhs) { return  *this = *this + rhs; }

Vec3 Vec3::operator-(const Vec3& rhs) const { return Vec3(vals[0]-rhs[0], vals[1]-rhs[1], vals[2]-rhs[2]); }
Vec3& Vec3::operator-=(const Vec3& rhs) { return *this = *this + rhs; }

Vec3 Vec3::operator*(const Vec3& rhs) const { return Vec3(vals[0]*rhs[0], vals[1]*rhs[1], vals[2]*rhs[2]); }
Vec3& Vec3::operator*=(const Vec3& rhs) { return *this = *this * rhs; }
Vec3& Vec3::operator*=(GLfloat factor) { return operator*=(Vec3(factor, factor, factor)); }

Vec3 Vec3::operator/(const Vec3& rhs) const { return Vec3(vals[0]/rhs[0], vals[1]/rhs[1], vals[2]/rhs[2]); }
Vec3& Vec3::operator/=(const Vec3& rhs) { return *this = *this / rhs; }

GLfloat Vec3::dot(const Vec3& rhs) const { return vals[0]*rhs[0] + vals[1]*rhs[1] + vals[2]*rhs[2]; }
Vec3 Vec3::cross(const Vec3& rhs) const {
   Vec3 toReturn;
   toReturn[0] = vals[1]*rhs[2] - vals[2]*rhs[1];
   toReturn[1] = vals[2]*rhs[0] - vals[0]*rhs[2];
   toReturn[2] = vals[0]*rhs[1] - vals[1]*rhs[0];
   return toReturn;
}
GLfloat Vec3::length() const { return sqrt(dot(*this)); }

std::ostream& Vec3::operator<<(std::ostream& output) {
   return output << "{ " << vals[0] << ", " << vals[1] << ", " << vals[2] << " }";
}
std::istream& Vec3::operator>>(std::istream& input) {
   return input >> vals[0] >> vals[1] >> vals[2];
}
