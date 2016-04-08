#include <GL/gl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilities.h"

const size_t LINE_SIZE = 512;
obj* objects[256] = { NULL };

int prefix(const char* pre, const char* str) {
   return strncmp(pre, str, strlen(pre)) == 0;
}

void parseVec2(char* line, vec2* dest, int index) {
   sscanf(line, "%*s %f %f", &dest[index][0], &dest[index][1]);
}

void parseVec3(char* line, vec3* dest, int index) {
   sscanf(line, "%*s %f %f %f", &dest[index][0], &dest[index][1], &dest[index][2]);
}

void parseFace(char* line, face* dest, int index) {
   sscanf(line, "%*s %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
      &dest[index][0][0], &dest[index][0][1], &dest[index][0][2],
      &dest[index][1][0], &dest[index][1][1], &dest[index][1][2],
      &dest[index][2][0], &dest[index][2][1], &dest[index][2][2],
      &dest[index][3][0], &dest[index][3][1], &dest[index][3][2]);
}

int nextIndex() {
   int index = 0;
   while(objects[index++]);
   return --index;
}

int objLoad(char* filePath) {
   FILE* file = fopen(filePath, "r");
   char* line = (char*)calloc(LINE_SIZE, sizeof(char));
   char* token = NULL, * delim = " \n\t";

   int vertexCount = 0, textureCount = 0, normalCount = 0, faceCount = 0;
   while(fgets(line, LINE_SIZE, file) != NULL) {
      if(prefix("vt", line)) ++textureCount;
      else if(prefix("vn", line)) ++normalCount;
      else if(prefix("v", line))  ++vertexCount;
      else if(prefix("f", line))  ++faceCount;
   }
   rewind(file);

   obj* object = (obj*)calloc(1, sizeof(obj));
   object->faceCount = faceCount;
   object->vertices = (vec3*)calloc(vertexCount,sizeof(vec3));
   object->normals = (vec3*)calloc(normalCount, sizeof(vec3));
   object->tangents = (vec3*)calloc(vertexCount, sizeof(vec3));
   object->bitangents = (vec3*)calloc(vertexCount, sizeof(vec3));
   object->textures = (vec2*)calloc(textureCount, sizeof(vec2));
   object->faces = (face*)calloc(faceCount, sizeof(face));

   int vertexTally = 0, normalTally = 0, tangentTally = 0,
       bitangentTally = 0, textureTally = 0, faceTally = 0;

   while(fgets(line, LINE_SIZE, file) != NULL) {
      if(prefix("vt", line)) parseVec2(line, object->textures, tangentTally++);
      else if(prefix("vn", line)) parseVec3(line, object->normals, normalTally++);
      else if(prefix("vx", line)) parseVec3(line, object->tangents, tangentTally++);
      else if(prefix("vy", line)) parseVec3(line, object->bitangents, bitangentTally++);
      else if(prefix("v", line)) parseVec3(line, object->vertices, vertexTally++);
      else if(prefix("f", line)) parseFace(line, object->faces, faceTally++);
   }

   free(line);
   fclose(file);

   int index = nextIndex();
   objects[index] = object;
   return index;
}

void objUnload() {
   for(int i = 0; i < 256; ++i) {
      if(!objects[i]) continue;
      free(objects[i]->vertices);
      free(objects[i]->normals);
      free(objects[i]->textures);
      free(objects[i]->bitangents);
      free(objects[i]->tangents);
      free(objects[i]->faces);
      free(objects[i]);
   }
}

obj* getObject(int index) { return objects[index]; }
