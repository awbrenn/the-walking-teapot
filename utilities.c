#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void cpy(vec3 src, vec3 dest) {
   memycpy(dest, src, sizeof(vec3));
}

void add(vec3 lhs, vec3 rhs, vec3 dest) {
   dest[0] = lhs[0] + rhs[0];
   dest[1] = lhs[1] + rhs[1];
   dest[2] = lhs[2] + rhs[2];
}

void sub(vec3 lhs, vec3 rhs, vec3 dest) {
   dest[0] = lhs[0] - rhs[0];
   dest[1] = lhs[1] - rhs[1];
   dest[2] = lhs[2] - rhs[2];
}

void sub2(vec2 lhs, vec2 rhs, vec2 dest) {
   dest[0] = lhs[0] - rhs[0];
   dest[1] = lhs[1] - rhs[1];
}

void mul(vec3 lhs, vec3 rhs, vec3 dest) {
   dest[0] = lhs[0] * rhs[0];
   dest[1] = lhs[1] * rhs[1];
   dest[2] = lhs[2] * rhs[2];
}

float len(vec3 lhs) {
   return sqrt(dot(lhs, lhs));
}

float dot(vec3 lhs, vec3 rhs) {
   return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2];
}

void scale(vec3 lhs, float factor, vec3 dest) { 
   dest[0] = lhs[0] * factor;
   dest[1] = lhs[1] * factor;
   dest[2] = lhs[2] * factor;
}

void unit(vec3 lhs, vec3 dest) {
   scale(lhs, len(lhs), dest);
}

void cross(vec3 lhs, vec3 rhs, vec3 dest) {
   vec3 temp = { dest[0], dest[1], dest[2] };

   temp[0] = lhs[1]*rhs[2] - lhs[2]*rhs[1]; 
   temp[1] = lhs[2]*rhs[0] - lhs[0]*rhs[2];
   temp[2] = lhs[0]*rhs[1] - lhs[1]*rhs[0];

   dest[0] = temp[0];
   dest[1] = temp[1];
   dest[2] = temp[2];
}

void dieWithError(char* message) {
   fprintf(stderr, "ERROR: %s\n", message);
   exit(-1);
}
