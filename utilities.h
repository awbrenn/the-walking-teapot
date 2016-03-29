#ifndef UTILITIES
#define UTILITIES

typedef struct vec2 {
   GLfloat x, y;
} vec2;

typedef struct vec3 {
   GLfloat x, y, z;
} vec3;

typedef struct face {
   vec3 a, b, c, d;
} face;

void dieWithError(char* message);

#endif
