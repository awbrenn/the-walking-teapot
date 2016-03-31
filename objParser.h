#ifndef OBJ_PARSER
#define OBJ_PARSER
#include "utilities.h"

void objLoad(char* filename);
void objUnload();

int getFaceCount();
face* getFaces();
GLfloat* getTexture(int index);
GLfloat* getVertex(int index);
GLfloat* getNormal(int index);
GLfloat* getTangent(int index);
GLfloat* getBitangent(int index);

#endif
