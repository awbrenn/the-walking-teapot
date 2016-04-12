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

void parseVec2(char* line, vec2* dest) {
   sscanf(line, "%*s %f %f", &dest->x, &dest->y);
}

void parseVec3(char* line, vec3* dest) {
   sscanf(line, "%*s %f %f %f", &dest->x, &dest->y, &dest->z);
}

void parseFace(char* line, face dest) {
   sscanf(line, "%*s %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
      &dest[0][0], &dest[0][1], &dest[0][2],
      &dest[1][0], &dest[1][1], &dest[1][2],
      &dest[2][0], &dest[2][1], &dest[2][2],
      &dest[3][0], &dest[3][1], &dest[3][2]);
}

int nextIndex() {
   int index = 0;
   while(objects[index++]);
   return --index;
}

void addTandB(obj* object, int* vIndices, int* tIndices) {
   vec3 pos1 = object->vertices[vIndices[0] - 1];
   vec3 pos2 = object->vertices[vIndices[1] - 1];
   vec3 pos3 = object->vertices[vIndices[2] - 1];
   
   vec2 uv1 = object->textures[tIndices[0] - 1];
   vec2 uv2 = object->textures[tIndices[1] - 1];
   vec2 uv3 = object->textures[tIndices[2] - 1];

   vec3 edge1 = subV3(pos2, pos1);
   vec3 edge2 = subV3(pos3, pos1);
   vec2 deltaUV1 = subV2(uv2, uv1);
   vec2 deltaUV2 = subV2(uv3, uv1);  

   GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

   vec3 tangent, bitangent;
   tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
   tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
   tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
   tangent = unit(tangent);

   bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
   bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
   bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
   bitangent = unit(bitangent);

   object->tangents[vIndices[0] - 1] = addV3(object->tangents[vIndices[0] - 1], tangent);
   object->bitangents[vIndices[0] - 1] = addV3(object->bitangents[vIndices[0] - 1], bitangent);
   object->tangents[vIndices[1] - 1] = addV3(object->tangents[vIndices[1] - 1], tangent);
   object->bitangents[vIndices[1] - 1] = addV3(object->bitangents[vIndices[1] - 1], bitangent);
   object->tangents[vIndices[2] - 1] = addV3(object->tangents[vIndices[2] - 1], tangent);
   object->bitangents[vIndices[2] - 1] = addV3(object->bitangents[vIndices[2] - 1], bitangent);
}

void uniqueMap(int vertexCount, int objectIndex) {
   obj* object = objects[objectIndex];
   int* count = (int*)calloc(vertexCount, sizeof(int));

   int i, j;
   for(i = 0; i < object->faceCount; ++i) {
       int t1Vertices[3] = {
          object->faces[i][0][0], 
          object->faces[i][1][0], 
          object->faces[i][2][0] 
       };
       int t1Textures[3] = {
          object->faces[i][0][1], 
          object->faces[i][1][1],
          object->faces[i][2][1]
       };
       addTandB(object, t1Vertices, t1Textures);

       int t2Vertices[3] = {
          object->faces[i][0][0], 
          object->faces[i][2][0], 
          object->faces[i][3][0] 
       };
       int t2Textures[3] = {
          object->faces[i][0][1], 
          object->faces[i][2][1],
          object->faces[i][3][1]
       };
       addTandB(object, t2Vertices, t2Textures);

       count[object->faces[i][0][0] - 1] += 2;
       count[object->faces[i][1][0] - 1] += 1;
       count[object->faces[i][2][0] - 1] += 2;
       count[object->faces[i][3][0] - 1] += 1;
   }

   for(i = 0; i < vertexCount; ++i) {
       object->tangents[i] = unit(scale(object->tangents[i], 1.0f / count[i]));
       object->bitangents[i] = unit(scale(object->bitangents[i], 1.0f / count[i]));
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
      if(prefix("vt", line)) parseVec2(line, &object->textures[textureTally++]);
      else if(prefix("vn", line)) parseVec3(line, &object->normals[normalTally++]);
      else if(prefix("v", line)) parseVec3(line, &object->vertices[vertexTally++]);
      else if(prefix("f", line)) parseFace(line, object->faces[faceTally++]);
   }

   free(line);
   fclose(file);

   int index = nextIndex();
   objects[index] = object;
   uniqueMap(vertexCount, index);
   return index;
}

void objUnload() {
    int i;
    for(i = 0; i < 256; ++i) {
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
