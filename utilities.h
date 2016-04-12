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

void cpy(vec3 src, vec3 dest);
void add(vec3 lhs, vec3 rhs, vec3 dest);
void sub(vec3 lhs, vec3 rhs, vec3 dest);
void sub2(vec2 lhs, vec2 rhs, vec2 dest);
void mul(vec3 lhs, vec3 rhs, vec3 dest);
float dot(vec3 lhs, vec3 rhs);
float len(vec3 lhs);
void scale(vec3 lhs, float factor, vec3 dest);
void unit(vec3 lhs, vec3 dest);
void crosss(vec3 lhs, vec3 rhs, vec3 dest);

void dieWithError(char* message);

#endif
