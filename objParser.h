#ifndef OBJ_PARSER
#define OBJ_PARSER
#include "utilities.h"

int objLoad(char* filename);
void objUnload();
obj* getObject(int index);

#endif
