#include <stdio.h>
#include "utils.h"

float rotacioActual = 10.00;

void dibuixaTriangles() {
    glPushMatrix();
    glTranslatef(-0.5, 0, 0);
    glRotatef(rotacioActual, 0, 1, 0);
    glutWireTeapot(0.25f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.5, 0, 0);
    glRotatef(180-rotacioActual, 0, 1, 0);
    glutWireTeapot(0.25f);
    glPopMatrix();
}

int xActual = 0;

void rotaTriangles(int x, int y) {

    float w = (float)glutGet(GLUT_WINDOW_WIDTH);

    float xDiferencia = x-xActual;

    float xAbsolutaDiferencia = abs(xDiferencia);

    if(xDiferencia<0 && xAbsolutaDiferencia>5) {
        rotacioActual -= w/360*2;
    }else if(xAbsolutaDiferencia>5){
        rotacioActual += w/360*2;
    }

    xActual = x;

    glutPostRedisplay();
}

void helper(int button, int state, int x, int y) {
    if(button == GLUT_LEFT_BUTTON) {
        xActual = x;
    }
}

void refrescaPantalla(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    dibuixaEixos();
    dibuixaTriangles();
    glutSwapBuffers();
}

void initGlut(int argc, const char * argv[]) {
    glutInit(&argc, (char **)argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("IDI: Practiques OpenGL");
    glutDisplayFunc(refrescaPantalla);
    glutReshapeFunc(reubicaVP);
    glutMotionFunc(rotaTriangles);
    glutMouseFunc(helper);
    initGL();
    glutMainLoop();
}

int main(int argc, const char * argv[]) {
    initGlut(argc, argv);
}