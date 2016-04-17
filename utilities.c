#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

vec3 addV3(vec3 lhs, vec3 rhs) {
   vec3 toReturn = { lhs.x+rhs.x, lhs.y+rhs.y, lhs.z+rhs.z };
   return toReturn;
}
vec3 subV3(vec3 lhs, vec3 rhs) {
   vec3 toReturn = { lhs.x-rhs.x, lhs.y-rhs.y, lhs.z-rhs.z };
   return toReturn;
}
vec3 mulV3(vec3 lhs, vec3 rhs) {
   vec3 toReturn = { lhs.x*rhs.x, lhs.y*rhs.y, lhs.z*rhs.z };
   return toReturn;
}
vec3 divV3(vec3 lhs, vec3 rhs) {
   vec3 toReturn = { lhs.x/rhs.x, lhs.y/rhs.y, lhs.z/rhs.z };
   return toReturn;
}

vec2 addV2(vec2 lhs, vec2 rhs) {
   vec2 toReturn = { lhs.x+rhs.x, lhs.y+rhs.y };
   return toReturn;
}
vec2 subV2(vec2 lhs, vec2 rhs) {
   vec2 toReturn = { lhs.x-rhs.x, lhs.y-rhs.y };
   return toReturn;
}
vec2 mulV2(vec2 lhs, vec2 rhs) {
   vec2 toReturn = { lhs.x*rhs.x, lhs.y*rhs.y };
   return toReturn;
}
vec2 divV2(vec2 lhs, vec2 rhs) {
   vec2 toReturn = { lhs.x/rhs.x, lhs.y/rhs.y };
   return toReturn;
}

float dot(vec3 lhs, vec3 rhs) {
   return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
}
vec3 scale(vec3 src, float factor) {
   vec3 toReturn = { src.x*factor, src.y*factor, src.z*factor };
   return toReturn;
}
vec3 cross(vec3 lhs, vec3 rhs);
vec3 unit(vec3 src) {
   float factor = sqrt(dot(src, src));
   return scale(src, 1.0f/factor);
}

void dieWithError(char* message) {
   fprintf(stderr, "ERROR: %s\n", message);
   exit(-1);
}
