#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glx.h>
#include <GL/glext.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>

#include "objParser.h"

#define EYEDX 0.08

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

unsigned int setShaders() {
   char *vs, *fs;
   GLuint v, f, p;

   v = glCreateShader(GL_VERTEX_SHADER);
   f = glCreateShader(GL_FRAGMENT_SHADER);
   
   vs = readShaderProgram("shader.vert");
   fs = readShaderProgram("shader.frag");

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
   glUseProgram(p);

   return(p);
}

void doViewVolume() {
}

void drawStuff() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glFlush();
}

void initOpenGL(int argc, char** argv) {
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_ACCUM);
   glutInitWindowSize(1280, 720);
   glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - 1280)/2, (glutGet(GLUT_SCREEN_HEIGHT) - 720)/2);
   glutCreateWindow("The Walking Teapot");

   glClearColor(0.0, 0.0, 0.0, 0.0);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_MULTISAMPLE_ARB);

   doViewVolume();
   setShaders();
}

void go() {
   drawStuff();
}

void getOut(unsigned char key, int x, int y) {
   switch(key) {
      case 'q':
         objUnload();
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

   return 0;
}
