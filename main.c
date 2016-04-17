#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glx.h>
#include <GL/glext.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>

#include "utilities.h"
#include "objParser.h"

typedef struct imageData {
    unsigned char* bytes;
    unsigned int width;
    unsigned int height;
} imageData;

unsigned int renderCount = 0;
unsigned int shaders[256] = { 0 };
unsigned int diffuse[256] = { 0 };
unsigned int normal[256] = { 0 };
unsigned int specular[256] = { 0 };

void copyCat(char* dest, char* str1, char* str2) {
   strcpy(dest, str1);
   strcat(dest, str2);
}

void doViewVolume() {
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(30.46, 16.0/9.0, 0.1, 25.0);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   vec3 eye = { 9.3, 0.95, -1.1 };
   vec3 view = { 1.0, 0.95, -1.1 };
   vec3 up = { 0.0, 1.0, 0.0 };

   gluLookAt(eye.x, eye.y, eye.z, view.x, view.y, view.z, up.x, up.y, up.z);
}

void doLights() {
float light0_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
float light0_diffuse[] = { 2.0, 2.0, 2.0, 0.0 }; 
float light0_specular[] = { 2.25, 2.25, 2.25, 0.0 }; 
float light0_position[] = { 1.5, 2.0, 2.0, 1.0 };
float light0_direction[] = { -1.5, -2.0, -2.0, 1.0};

// set scene default ambient 
glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light0_ambient); 

// make specular correct for spots 
glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,1); 
glLightfv(GL_LIGHT0,GL_AMBIENT,light0_ambient); 
glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse); 
glLightfv(GL_LIGHT0,GL_SPECULAR,light0_specular); 
glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,1.0); 
glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,180.0); 
glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,1.0); 
glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,0.2); 
glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,0.01); 
glLightfv(GL_LIGHT0,GL_POSITION,light0_position);
glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,light0_direction);
glEnable(GL_LIGHTING);
glEnable(GL_LIGHT0);
}

void loadTexture(char *folder, int type) {
   FILE *fopen(), *fptr;
   char buf[512], *parse;
   int im_size, im_width, im_height, max_color;
   unsigned char *texture_bytes; 

   char filename[256];
   switch(type) {
      case 0: copyCat(filename, folder, "/diffuse.ppm"); break;
      case 1: copyCat(filename, folder, "/normal.ppm"); break;
      case 2: copyCat(filename, folder, "/specular.ppm"); break;
      default: dieWithError("Invalid texture type!"); break;
   }

   fptr=fopen(filename,"r");
   fgets(buf,512,fptr);
   do{
      fgets(buf,512,fptr);
   } while(buf[0]=='#');
   parse = (char *)strtok(buf," \t");
   im_width = atoi(parse);

   parse = (char *)strtok(NULL," \n");
   im_height = atoi(parse);

   fgets(buf,512,fptr);
   parse = (char *)strtok(buf," \n");
   max_color = atoi(parse);

   im_size = im_width*im_height;
   texture_bytes = (unsigned char *)calloc(3,im_size);
   fread(texture_bytes,3,im_size,fptr);
   fclose(fptr);

   unsigned int* storeLocation = NULL;
   switch(type) {
      case 0: storeLocation = &diffuse[renderCount]; break;
      case 1: storeLocation = &normal[renderCount]; break;
      case 2: storeLocation = &specular[renderCount]; break;
   }
   glGenTextures(1, storeLocation);    
   glBindTexture(GL_TEXTURE_2D, *storeLocation); 
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,im_width,im_height,0,GL_RGB,GL_UNSIGNED_BYTE,texture_bytes);
   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
   glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE); 
   free(texture_bytes);
}

char* readShaderProgram(char* filename) {
   FILE *fp;
   char *content = NULL;
   int fd, count;
   fd = open(filename,O_RDONLY);
   count = (int)lseek(fd,0,SEEK_END);
   close(fd);
   content = (char *)calloc(1,(size_t)(count+1));
   fp = fopen(filename,"r");
   count = (int)fread(content,sizeof(char),(size_t)count,fp);
   content[count] = '\0';
   fclose(fp);
   return content;
}

void setShaders(char* folder) {
   char *vs, *fs;
   GLuint v, f, p;

   v = glCreateShader(GL_VERTEX_SHADER);
   f = glCreateShader(GL_FRAGMENT_SHADER);
   
   char vert[256], frag[256];
   copyCat(vert, folder, "/shader.vert");
   copyCat(frag, folder, "/shader.frag");

   vs = readShaderProgram(vert);
   fs = readShaderProgram(frag);

   glShaderSource(v,1,(const char **)&vs,NULL);
   glShaderSource(f,1,(const char **)&fs,NULL);

   free(vs);
   free(fs); 

   glCompileShader(v);
   glCompileShader(f);

   p = glCreateProgram();
   glAttachShader(p,f);
   glAttachShader(p,v);
   glLinkProgram(p);

   shaders[renderCount] = p;
}

void loadObject(char* folder) {
   char filename[256];
   copyCat(filename, folder, "/object.obj");
   objLoad(filename);
}

void loadResources(char* folder) {
   setShaders(folder);   
   int i; for(i = 0; i < 3; ++i) loadTexture(folder, i);   
   loadObject(folder);
   ++renderCount;
}

void initOpenGL(int argc, char** argv) {
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_ACCUM);
   glutInitWindowSize(1280, 720);
   glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - 1280) / 2,
                          (glutGet(GLUT_SCREEN_HEIGHT) - 720) / 2);
   glutCreateWindow("The Walking Teapot");

   loadResources("teapot");
   loadResources("ground");
   loadResources("background");

   glClearColor(0.0, 0.5, 0.0, 0.0);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_MULTISAMPLE_ARB);
   glEnable(GL_TEXTURE_2D);

   doViewVolume();
   doLights();
}

void setUniformParameter(unsigned int p, char* varName, unsigned int value) {
   int location = glGetUniformLocation(p, varName);
   glUniform1i(location, value);
}

void setResources(int n) {
   glUseProgram(shaders[n]);

   setUniformParameter(shaders[n], "myDiffuse", 0);
   setUniformParameter(shaders[n], "myNormal", 1);
   setUniformParameter(shaders[n], "mySpecular", 2);   

   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, diffuse[n]);

   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, normal[n]);
   
   glActiveTexture(GL_TEXTURE2);
   glBindTexture(GL_TEXTURE_2D, specular[n]);
}

void drawStuff() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glActiveTexture(GL_TEXTURE0);

   int n, i, j;
   for(n = 0; n < renderCount; ++n) {
      obj* object = getObject(n);;
      setResources(n);
      
      unsigned int indexTangent = glGetAttribLocation(shaders[n], (const char*)"myTangent");
      unsigned int indexBitangent = glGetAttribLocation(shaders[n], (const char*)"myBitangent");
      printf("%u %u\n", indexTangent, indexBitangent);

      glBegin(GL_TRIANGLES);
      for(i = 0; i < object->faceCount; ++i) {
         for(j = 0; j < 3; ++j) {
            int vi = object->faces[i][j][0];
            int ti = object->faces[i][j][1];
            int ni = object->faces[i][j][2];

            glTexCoord2fv((GLfloat*)&object->textures[ti-1]);
            glNormal3fv((GLfloat*)&object->normals[ni-1]);
            glVertexAttrib3fv(indexTangent, (GLfloat*)&object->tangents[vi - 1]);
            glVertexAttrib3fv(indexBitangent, (GLfloat*)&object->bitangents[vi - 1]);
            glVertex3fv((GLfloat*)&object->vertices[vi-1]);
         }
      }
      glEnd();
   }

   glFlush();
}

void go() {
   drawStuff();
}

void getOut(unsigned char key, int x, int y) {
   switch(key) {
      case 'q':
         exit(1);
      default:
         break;
   }
}

int main(int argc, char** argv) {
   initOpenGL(argc, argv);
   glutDisplayFunc(go);
   glutKeyboardFunc(getOut);
   glutMainLoop();
}
