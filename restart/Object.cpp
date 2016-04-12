#include <fstream>
#include "Vec2.hpp"
#include "Vec3.hpp"
#include "Face.hpp"
#include "Object.hpp"

Object::Object(std::string filepath, int& textureCount) {
   std::ifstream file;
   file.open(filepath + "object.obj");

   std::string line;
   while(getline(file, line)) {
      if(line.compare(0, 2, "vn")) normals.push_back(new Vec3(line));
      else if(line.compare(0, 2, "vt")) textures.push_back(new Vec2(line));
      else if(line.compare(0, 1, "v")) vertices.push_back(new Vec3(line));
      else if(line.compare(0, 1, "f")) faces.push_back(new Face(line));
   }
   uniqueMap();
}

Object::~Object() {
   for(Vec2* texture : textures) delete texture;
   for(Vec3* vertex : vertices) delete vertex;
   for(Vec3* normal : normals) delete normal;
   for(Vec3* tangent : tangents) delete tangent;
   for(Vec3* bitangent : bitangents) delete bitangent;
}
