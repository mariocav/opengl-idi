#include <stdio.h>

#if defined(__APPLE__)
    #include <OpenGL/OpenGL.h>
    #include <GLUT/GLUT.h>
#else
    #include <GL/gl.h>
    #include <GL/freeglut.h>
#endif

void refrescaPantalla(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
    glTranslatef(0,-0.5,0);
    glutSolidSphere(0.5, 16, 16);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0,0.25,0);
    glutSolidSphere(0.25, 16, 16);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.25,0.25,0);
    glRotatef(90,0,1,0);
    glColor3f(0,1,1);
    glutSolidCone(0.05, 0.35, 16, 16);
    glPopMatrix();

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
    glutMainLoop();
}

void initGL() {
    glClearColor(0,0,0,1);
}

int main(int argc, const char * argv[]) {
    initGL();
    initGlut(argc, argv);
    return 0;
}