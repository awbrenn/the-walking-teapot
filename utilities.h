#ifndef UTILITIES
#define UTILITIES

typedef struct vec2 {
   GLfloat x, y;
} vec2;

typedef struct vec3 {
   GLfloat x, y, z;
} vec3;

typedef struct face {
   int  a[3], b[3], c[3], d[3];
} face;

void dieWithError(char* message);

#endif
