#include <GL/gl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilities.h"

const size_t LINE_SIZE = 512;
int faceCount    = 0;
vec3* vertexArray = NULL, *normalArray = NULL, *tangentArray = NULL, *bitangentArray = NULL;
vec2* textureArray = NULL;
face* faceArray = NULL;

int prefix(const char* pre, const char* str) {
   return strncmp(pre, str, strlen(pre)) == 0;
}

void parseVec2(char* line, vec2* dest, int index) {
   sscanf(line, "%*s %f %f", &dest[index].x, &dest[index].y);
}

void parseVec3(char* line, vec3* dest, int index) {
   sscanf(line, "%*s %f %f %f", &dest[index].x, &dest[index].y, &dest[index].z);
}

void parseFace(char* line, face* dest, int index) {
   sscanf(line, "%*s %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
      &dest[index].a[0], &dest[index].a[1], &dest[index].a[2],
      &dest[index].b[0], &dest[index].b[1], &dest[index].b[2],
      &dest[index].c[0], &dest[index].c[1], &dest[index].c[2],
      &dest[index].d[0], &dest[index].d[1], &dest[index].d[2]);
}

void objLoad(char* filePath) {
   FILE* file = fopen(filePath, "r");
   char* line = (char*)calloc(LINE_SIZE, sizeof(char));
   char* token = NULL, * delim = " \n\t";

   int vertexCount = 0, textureCount = 0, normalCount = 0;
   while(fgets(line, LINE_SIZE, file) != NULL) {
      if(prefix("vt", line)) ++textureCount;
      else if(prefix("vn", line)) ++normalCount;
      else if(prefix("v", line))  ++vertexCount;
      else if(prefix("f", line))  ++faceCount;
   }
   rewind(file);

   vertexArray = (vec3*)calloc(vertexCount,sizeof(vec3));
   normalArray = (vec3*)calloc(normalCount, sizeof(vec3));
   tangentArray = (vec3*)calloc(vertexCount, sizeof(vec3));
   bitangentArray = (vec3*)calloc(vertexCount, sizeof(vec3));
   textureArray = (vec2*)calloc(textureCount, sizeof(vec2));
   faceArray = (face*)calloc(faceCount, sizeof(face));

   int vertexTally = 0, normalTally = 0, tangentTally = 0,
       bitangentTally = 0, textureTally = 0, faceTally = 0;

   while(fgets(line, LINE_SIZE, file) != NULL) {
      if(prefix("vt", line)) parseVec2(line, textureArray, tangentTally++);
      else if(prefix("vn", line)) parseVec3(line, normalArray, normalTally++);
      else if(prefix("vx", line)) parseVec3(line, tangentArray, tangentTally++);
      else if(prefix("vy", line)) parseVec3(line, bitangentArray, bitangentTally++);
      else if(prefix("v", line)) parseVec3(line, vertexArray, vertexTally++);
      else if(prefix("f", line)) parseFace(line, faceArray, faceTally++);
   }

   free(line);
   fclose(file);
}

void objUnload() {
   free(vertexArray); vertexArray = NULL;
   free(normalArray); normalArray = NULL;
   free(tangentArray); tangentArray = NULL;
   free(bitangentArray); bitangentArray = NULL;
   free(textureArray); textureArray = NULL;
   free(faceArray); faceArray = NULL;
   faceCount = 0;
}
