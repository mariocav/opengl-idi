#include <stdio.h>

#define BOLDBLACK "\033[1m\033[30m"
#define RESET "\033[0m"

#if defined(__APPLE__)
    #include <OpenGL/OpenGL.h>
    #include <GLUT/GLUT.h>
#else
    #include <GL/gl.h>
    #include <GL/freeglut.h>
#endif


void refrescaPantalla(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glutWireSphere(0.5,40,40);
    glutSolidTeapot(0.5);
    glutSwapBuffers();
}

void reubicaVP(int width, int height) {
    if(width>height) {
        glViewport((width-height)/2, 0, height, height);
    }else if(height>width){
        glViewport(0, (height-width)/2, width, width);
    }
}

void initGlut(int argc, const char * argv[]) {
    glutInit(&argc, (char **)argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("IDI: Practiques OpenGL");
    glutDisplayFunc(refrescaPantalla);
    glutReshapeFunc(reubicaVP);
    initGL();
    glutMainLoop();
}

void initGL() {
    glClearColor(0,0,0,1);
}

int main(int argc, const char * argv[]) {
    initGlut(argc, argv);
    return 0;
}