#include <stdio.h>

#if defined(__APPLE__)
    #include <OpenGL/OpenGL.h>
    #include <GLUT/GLUT.h>
#else
    #include <GL/gl.h>
    #include <GL/freeglut.h>
#endif

int alfa = 90;
int beta = 45;

void dibuixaEixos() {
    glPushMatrix();
    glBegin(GL_LINES);
    glColor3f(1,0,0);
    glVertex3f(0,0,0);
    glVertex3f(10,0,0);
    glColor3f(0,1,0);
    glVertex3f(0,0,0);
    glVertex3f(0,10,0);
    glColor3f(0,0,1);
    glVertex3f(0,0,0);
    glVertex3f(0,0,10);
    glEnd();
    glColor3f(1,1,1);
    glPopMatrix();
}

void refrescaPantalla(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    dibuixaEixos();
    glutSolidSphere(5,20,20);
    glPushMatrix();
    glRotatef(-alfa,0,1,0);
    glTranslatef(20,0,0);
    glColor3f(1,0,5);
    glutSolidSphere(3,20,20);
    glRotatef(-beta,0,1,0);
    glTranslatef(5,0,0);
    glColor3f(1,5,0);
    glutSolidSphere(1,20,20);
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
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-50,50,-50,50,0,80);
    gluLookAt(0,40,0,0,0,0,0,0,-1);
    glMatrixMode(GL_MODELVIEW);
    glutDisplayFunc(refrescaPantalla);
    glutReshapeFunc(reubicaVP);
    glutMainLoop();
}

void initGL() {
    glClearColor(0,4,0,1);
}

int main(int argc, const char * argv[]) {
    initGL();
    initGlut(argc, argv);
    return 0;
}