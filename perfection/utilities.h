#ifndef UTILITIES
#define UTILITIES
#include <GL/gl.h>

typedef struct vec2 {
   GLfloat x, y;
} vec2;
typedef struct vec3 {
   GLfloat x, y, z;
} vec3;
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

vec3 addV3(vec3 lhs, vec3 rhs);
vec3 subV3(vec3 lhs, vec3 rhs);
vec3 mulV3(vec3 lhs, vec3 rhs);
vec3 divV3(vec3 lhs, vec3 rhs);

vec2 addV2(vec2 lhs, vec2 rhs);
vec2 subV2(vec2 lhs, vec2 rhs);
vec2 mulV2(vec2 lhs, vec2 rhs);
vec2 divV2(vec2 lhs, vec2 rhs);

float dot(vec3 lhs, vec3 rhs);
vec3 scale(vec3 src, float factor);
vec3 cross(vec3 lhs, vec3 rhs);
vec3 unit(vec3 src);

void dieWithError(char* message);

#endif
