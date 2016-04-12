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

void uniqueMap(int vertexCount, int objectIndex) {
   obj* object = objects[objectIndex];
   int* tangentCount = (int*)calloc(vertexCount, sizeof(int));
   int* bitangentCount = (int*)calloc(vertexCount, sizeof(int));

   int i, j;
   for(i = 0; i < object->faceCount; ++i) {
      vec3 tangent1, bitangent1;

      vec3 pos1; cpy(object->vertices[object->faces[i][0][0]], pos1);
      vec3 pos2; cpy(object->vertices[object->faces[i][0][1]], pos2);
      vec3 pos3; cpy(object->vertices[object->faces[i][0][2]], pos3);

      vec2 uv1; cpy(object->textures[object->faces[i][1][0]], uv1);
      vec2 uv2; cpy(object->textures[object->faces[i][1][1]], uv2);
      vec2 uv3; cpy(object->textures[object->faces[i][1][2]], uv3);

      vec3 edge1; sub(pos2, pos1, edge1);
      vec3 edge2; sub(pos3, pos1, edge2);
      vec2 deltaUV1; sub(uv2, uv1, deltaUV1);
      vec2 deltaUV2; sub(uv3, uv1, deltaUV2);

      GLfloat f = 1.0f / (deltaUV1[0] * deltaUV2[1] - deltaUV2[0] * deltaUV1[1]);

      tangent1[0] = f * (deltaUV2[1] * edge1[0] - deltaUV1[1] * edge2[0]);
      tangent1[1] = f * (deltaUV2[1] * edge1[1] - deltaUV1[1] * edge2[1]);
      tangent1[2] = f * (deltaUV2[1] * edge1[2] - deltaUV1[1] * edge2[2]);
      unit(tangent1, tangent1);      

      bitangent1[0] = f * (-deltaUV2[0] * edge1[0] + deltaUV1[0] * edge2[0]);
      bitangent1[1] = f * (-deltaUV2[0] * edge1[1] + deltaUV1[0] * edge2[1]);
      bitangent1[2] = f * (-deltaUV2[0] * edge1[2] + deltaUV1[0] * edge2[2]);
      unit(bitangent1, bitangent1);

      for(int i = 0; i < 3; ++i) {
         add(object->tangents[object->faces[i][0][j]], tangent1, object->tangents[objects->faces[i][0][j]]); 
         add(object->bitangents[object->faces[i][0][j]], bitangent1, object->tangents[objects->faces[i][0][j]]); 
         ++tangentCount[object->faces[i][0][j]];
         ++bitangentCount[object->faces[i][0][j]];
      }
   }

   for(i = 0; i < object->faceCount; ++i) {
      for(j = 0; j < 4; ++j) {
         scale(object->tangents[object->faces[i][0][j]],
               1.0f / tangentCount[object->faces[i][0][j]],
               object->tangents[object->faces[i][0][j]]);
         scale(object->bitangents[object->faces[i][0][j]],
               1.0f / bitangentCount[object->faces[i][0][j]],
               object->bitangents[object->faces[i][0][j]]);
      }
   }
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

   int vertexTally = 0, normalTally = 0, textureTally = 0, faceTally = 0;

   while(fgets(line, LINE_SIZE, file) != NULL) {
      else if(prefix("vt", line)) parseVec2(line, object->textures, textureTally++);
      else if(prefix("vn", line)) parseVec3(line, object->normals, normalTally++);
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
