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

#include "objParser.h"

#define EYEDX 0.08

unsigned int renderCount = 0;
unsigned int shaders[32];

void loadTexture(char *filename, unsigned int texture) {
   FILE *fopen(), *fptr;
   char buf[512], *parse;
   int im_size, im_width, im_height, max_color;
   unsigned char *texture_bytes; 

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

   glBindTexture(GL_TEXTURE_2D, texture);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,im_width,im_height,0,GL_RGB, 
      GL_UNSIGNED_BYTE,texture_bytes);
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

unsigned int setShaders(char* vert, char* frag) {
   char *vs, *fs;
   GLuint v, f, p;

   v = glCreateShader(GL_VERTEX_SHADER);
   f = glCreateShader(GL_FRAGMENT_SHADER);
   
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

   char log[256];
   int length;
   glGetShaderInfoLog(f, 256, &length, log);
   fprintf(stderr, "%s\n", log);

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

   vec3 eye = { 10.0, 3.0, -3.0 };
   vec3 view = { 0.0, 0.5, 0.0 };
   vec3 up = { 0.0, 1.0, 0.0 };

   gluLookAt(eye[0], eye[1], eye[2], view[0], view[1], view[2], up[0], up[1], up[2]);
}

void drawStuff() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glActiveTexture(GL_TEXTURE1);
   
   glEnable(GL_TEXTURE_2D);
   glBegin(GL_QUADS);
   
   int n, i, j;
   for(n = 0; n < renderCount; ++n) {
      obj* object = getObject(n);
      glUseProgram(shaders[n]);
      
      unsigned int index_tangent = glGetAttribLocation(shaders[n], "tangent");
      unsigned int index_bitangent = glGetAttribLocation(shaders[n], "bitangent");

      for(i = 0; i < object->faceCount; ++i) {
         for(j = 0; j < 4; ++j) {
            int vi = object->faces[i][j][0];
            int ti = object->faces[i][j][1];
            int ni = object->faces[i][j][2];

            glTexCoord2fv(object->textures[ti - 1]);
            glNormal3fv(object->normals[ni - 1]);

//            glVertexAttrib3fv(index_tangent, object->tangents[vi - 1]);
//            glVertexAttrib3fv(index_bitangent, object->bitangents[vi - 1]);
            glVertex3fv(object->vertices[vi - 1]);
         }
      }
   }
   glEnd();
   glDisable(GL_TEXTURE_2D);

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

void setUniformParameter(unsigned int p, char* varName, unsigned int value) {
   int location = glGetUniformLocation(p, varName);
   glUniform1i(location, value);
}

void loadResources(char* folder, int textureStart) {
   char filepath1[256], filepath2[256];

   strcpy(filepath1, folder);
   strcat(filepath1, "/shader.vert");
   strcpy(filepath2, folder);
   strcat(filepath2, "/shader.frag");
   shaders[renderCount] = setShaders(filepath1, filepath2);
   
   strcpy(filepath1, folder);
   strcat(filepath1, "/object.obj");      
   objLoad(filepath1);

   glActiveTexture(GL_TEXTURE0 + textureStart + 0);
   strcpy(filepath1, folder);
   strcat(filepath1, "/diffuse.ppm");
   loadTexture(filepath1, textureStart + 0);
   setUniformParameter(shaders[renderCount], "myDiffuse", textureStart + 0);

   glActiveTexture(GL_TEXTURE0 + textureStart + 1);
   strcpy(filepath1, folder);
   strcat(filepath1, "/normal.ppm");
   loadTexture(filepath1, textureStart + 1);
   setUniformParameter(shaders[renderCount++], "myNormal", textureStart + 1); 
}

void initOpenGL(int argc, char** argv) {
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_ACCUM);
   glutInitWindowSize(1280, 720);
   glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - 1280)/2, (glutGet(GLUT_SCREEN_HEIGHT) - 720)/2);
   glutCreateWindow("The Walking Teapot");

   loadResources("teapot", 0);

   glClearColor(0.0, 0.0, 0.0, 0.0);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_MULTISAMPLE_ARB);

   doViewVolume();
   doLights();
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
