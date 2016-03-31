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
   // specify size and shape of view volume 
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0,16.0 / 9.0,0.1,20.0);

   // specify position for view volume 
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   vec3 eye = { 2.5, 1.8, 2.0 };
   vec3 view = { 0.0, 0.0, 0.0 };
   vec3 up = { 0.0, 1.0, 0.0 };

   gluLookAt(eye[0], eye[1], eye[2], view[0], view[1], view[2], up[0], up[1], up[2]);
}

void drawStuff() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glBegin(GL_QUADS);
   
   int i, j;
   for(i = 0; i < getFaceCount(); ++i) {
      for(j = 0; j < 4; ++j) {
         int vi = getFaces()[i][j][0];
         int ti = getFaces()[i][j][1];
         int ni = getFaces()[i][j][2];

         glNormal3fv(getNormal(ni));
         glVertex3fv(getVertex(vi));
      }
   }

   glEnd();
   glFlush();
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

void initOpenGL(int argc, char** argv) {
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_ACCUM);
   glutInitWindowSize(1280, 720);
   glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - 1280)/2, (glutGet(GLUT_SCREEN_HEIGHT) - 720)/2);
   glutCreateWindow("The Walking Teapot");

   objLoad("teapot.605.obj");

   glClearColor(0.0, 0.0, 0.0, 0.0);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_MULTISAMPLE_ARB);

   doViewVolume();
   doLights();
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
