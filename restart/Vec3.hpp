#include <iostream>
#include <GL/gl.h>

class Vec3 {
   public:
      Vec3();
      Vec3(GLfloat x, GLfloat y, GLfloat z);
      Vec3(const std::string& input);
      Vec3(const Vec3& src);
      Vec3& operator=(const Vec3& rhs);

      GLfloat operator[](int i) const;
      GLfloat& operator[](int i);

      Vec3 operator+(const Vec3& rhs) const;
      Vec3& operator+=(const Vec3& rhs);

      Vec3 operator-(const Vec3& rhs) const;
      Vec3& operator-=(const Vec3& rhs);

      Vec3 operator*(const Vec3& rhs) const;
      Vec3& operator*=(const Vec3& rhs);
      Vec3& operator*=(GLfloat factor);

      Vec3 operator/(const Vec3& rhs) const;
      Vec3& operator/=(const Vec3& rhs);

      GLfloat dot(const Vec3& rhs) const;
      Vec3 cross(const Vec3& rhs) const;
      GLfloat length() const;

      std::ostream& operator<<(std::ostream& output);
      std::istream& operator>>(std::istream& input);

   private:
      GLfloat vals[3];
};


