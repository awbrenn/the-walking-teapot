#include <vector>

class Object;

class Scene {
   public:
      Scene();
      Scene(const Scene& src);
      Scene& operator=(const Scene& rhs);
      ~Scene();

      void addObject(std::string& filepath);

      int objectCount() const;
      Object* operator[](int i);
   private:
      int textureCount;
      std::vector<Object*> objects;
};
