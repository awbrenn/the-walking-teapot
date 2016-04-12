#include "Object.hpp"
#include "Scene.hpp"

Scene::Scene() :
   textureCount(0),
   objects()
{}

Scene::Scene(const Scene& src) :
   textureCount(0),
   objects(src.objects)
{}

Scene& Scene::operator=(const Scene& rhs) {
   textureCount = rhs.textureCount;
   objects = rhs.objects;
}

Scene::~Scene() {
   for(Object* object : objects) delete object;
}

void Scene::addObject(std::string& filepath) {
   objects.push_back(new Object(filepath, textureCount));
}

int Scene::objectCount() const { return objects.size(); }
Object* Scene::operator[](int i) { return objects[i]; }
