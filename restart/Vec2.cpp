#include "Vec2.hpp"
#include <cmath>
#include <sstream>

Vec2::Vec2() :
   vals{ 0.0f, 0.0f }
{}

Vec2::Vec2(GLfloat x, GLfloat y) :
   vals{ x, y }
{}

Vec2::Vec2(const std::string& input) :
   vals()
{
   std::istringstream iss(input);
   iss >> vals[0] >> vals[1];
}

Vec2::Vec2(const Vec2& src) :
   vals{ src[0], src[1] }
{}

Vec2& Vec2::operator=(const Vec2& rhs) {
   vals[0] = rhs[0];
   vals[1] = rhs[1];
   return *this;
}

GLfloat Vec2::operator [](int i) const { return vals[i]; }
GLfloat& Vec2::operator [](int i) { return vals[i]; }

Vec2 Vec2::operator+(const Vec2& rhs) const { return Vec2(vals[0]+rhs[0], vals[1]+rhs[1]); }
Vec2& Vec2::operator+=(const Vec2& rhs) { return  *this = *this + rhs; }

Vec2 Vec2::operator-(const Vec2& rhs) const { return Vec2(vals[0]-rhs[0], vals[1]-rhs[1]); }
Vec2& Vec2::operator-=(const Vec2& rhs) { return *this = *this + rhs; }

Vec2 Vec2::operator*(const Vec2& rhs) const { return Vec2(vals[0]*rhs[0], vals[1]*rhs[1]); }
Vec2& Vec2::operator*=(const Vec2& rhs) { return *this = *this * rhs; }
Vec2& Vec2::operator*=(GLfloat factor) { return operator*=(Vec2(factor, factor)); }

Vec2 Vec2::operator/(const Vec2& rhs) const { return Vec2(vals[0]/rhs[0], vals[1]/rhs[1]); }
Vec2& Vec2::operator/=(const Vec2& rhs) { return *this = *this / rhs; }

GLfloat Vec2::dot(const Vec2& rhs) const { return vals[0]*rhs[0] + vals[1]*rhs[1]; }
GLfloat Vec2::length() const { return sqrt(dot(*this)); }
