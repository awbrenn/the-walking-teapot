#include <string>
#include <vector>
class Vec2;
class Vec3;
class Face;

class Object {
   public:
      Object(std::string filepath, int& textureCount);
      ~Object();

   private:
      Object(); // Explicitly disallowed
      Object(const Object& src); // Explicitly disallowed
      Object& operator=(const Object& rhs); // Explicitly disallowed

      std::vector<Vec2*> textures;
      std::vector<Vec3*> vertices;
      std::vector<Vec3*> normals;
      std::vector<Vec3*> tangents;
      std::vector<Vec3*> bitangents;
      std::vector<Vec3*> faces;

      void uniqueMap();
};
