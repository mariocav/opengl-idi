#if defined(__APPLE__)
    #include <OpenGL/OpenGL.h>
    #include <GLUT/GLUT.h>
#else
    #include <GL/gl.h>
    #include <GL/freeglut.h>
#endif

#ifndef UTILS

#define UTILS

#define BOLDBLACK "\033[1m\033[30m"
#define RESET "\033[0m"

void dibuixaEixos();

void reubicaVP(int width, int height);

/* Models */

void initGL();

#endif