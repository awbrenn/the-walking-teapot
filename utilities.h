#ifndef UTILITIES
#define UTILITIES
#include <GL/gl.h>

typedef GLfloat vec2[2];
typedef GLfloat vec3[3];
typedef int face[4][3];
typedef struct obj {
   int faceCount;
   face* faces;

   vec2* textures;
   vec3* vertices;
   vec3* normals;
   vec3* tangents;
   vec3* bitangents;
} obj;

void dieWithError(char* message);

#endif
