#include <GL/gl.h>

class Vec2 {
   public:
      Vec2();
      Vec2(GLfloat x, GLfloat y);
      Vec2(const std::string& input);
      Vec2(const Vec2& src);
      Vec2& operator=(const Vec2& rhs);

      GLfloat operator[](int i) const;
      GLfloat& operator[](int i);

      Vec2 operator+(const Vec2& rhs) const;
      Vec2& operator+=(const Vec2& rhs);

      Vec2 operator-(const Vec2& rhs) const;
      Vec2& operator-=(const Vec2& rhs);

      Vec2 operator*(const Vec2& rhs) const;
      Vec2& operator*=(const Vec2& rhs);
      Vec2& operator*=(GLfloat factor);

      Vec2 operator/(const Vec2& rhs) const;
      Vec2& operator/=(const Vec2& rhs);

      GLfloat dot(const Vec2& rhs) const;
      GLfloat length() const;

   private:
      GLfloat vals[2];
};


