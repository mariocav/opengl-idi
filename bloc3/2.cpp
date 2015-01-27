/*

*/

#if defined(__APPLE__)
    #include <OpenGL/OpenGL.h>
    #include <GLUT/GLUT.h>
#else
    #include <GL/gl.h>
    #include <GL/freeglut.h>
#endif

#include <stdio.h>
#include <float.h> // DBL_MAX
#include <cmath>
#include <iostream>
using namespace std;

#define BOLDBLACK "\033[1m\033[30m"
#define RESET "\033[0m"

int xActual = 0;
int yActual = 0;

typedef struct {
    double minX;
    double maxX;
    double minY;
    double maxY;
    double minZ;
    double maxZ;
} Capsa;

typedef struct {
    double diametre;
    double radi;
} Esfera;

Capsa c;
Esfera e;

void dibuixaEixos() {
    glPushMatrix();
    glLoadIdentity();
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

void canviaPosicioCamera() {

}

#define AXONOMETRICA 0

void canviaTipusCamera(int width, int height, int tipusCamera) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(tipusCamera == AXONOMETRICA) {
        if(width>=height) {
            double ra = (double)width/height;
            glOrtho(-e.radi*ra, e.radi*ra, -e.radi, e.radi, -e.radi, 2*e.radi);
        }else if(height>width) {
            double ra = (double)height/width;
            glOrtho(-e.radi, e.radi, -e.radi*ra, e.radi*ra, -e.radi, 2*e.radi);
        }
    }
    glMatrixMode(GL_MODELVIEW);
}

void reubicaVP(int width, int height) {
    glViewport(0, 0, width, height);
    canviaTipusCamera(width, height, AXONOMETRICA);
}

void dibuixaNinot() {
    glPushMatrix();
    glutWireSphere(0.4, 20, 20);
    glTranslatef(0, 0.6, 0);
    glutWireSphere(0.2, 20, 20);
    glTranslatef(0.1, 0, 0);
    glRotated(90, 0, 1, 0);
    glutWireCone(0.1, 0.2, 20, 20);
    glPopMatrix();
}

void calculaCapsa() {
    c.minX = -0.4;
    c.maxX = 0.4;
    c.minY = -0.4;
    c.maxY = 0.8;
    c.minZ = -0.4;
    c.maxZ = 0.4;
}

void calculaEsfera() {
    e.diametre =
        sqrt(
            pow((c.maxX-c.minX), 2.0) +
            pow((c.maxY-c.minY), 2.0) +
            pow((c.maxZ-c.minZ), 2.0)
        );
    e.radi = e.diametre / 2.0;
}

void dibuixaEsfera() {
    glColor3f(0.3,0.7,0.23);
    glutWireSphere(e.radi, 20, 20);
}

void rotaEscena(int x, int y) {

    double w = (float)glutGet(GLUT_WINDOW_WIDTH);
    double h = (float)glutGet(GLUT_WINDOW_HEIGHT);

    double xDiferencia = x-xActual;
    double yDiferencia = y-yActual;

    double xAbsolutaDiferencia = abs(xDiferencia);
    double yAbsolutaDiferencia = abs(yDiferencia);

    if(xDiferencia<0 && xAbsolutaDiferencia>5) {
        glRotatef(w/360*4, 0, 1, 0);
    }else if(xAbsolutaDiferencia>5){
        glRotatef(-w/360*4, 0, 1, 0);
    }

    if(yDiferencia<0 && yAbsolutaDiferencia>3) {
        glRotatef(-h/360*4, 1, 0, 0);
    }else if(yAbsolutaDiferencia>3){
        glRotatef(h/360*4, 1, 0, 0);
    }

    yActual = y;
    xActual = x;

    glutPostRedisplay();
}

void helper(int button, int state, int x, int y) {
    if(button == GLUT_LEFT_BUTTON) {

    }
}

void mostraAjuda() {
    printf("\n-+-+-+-+-+-+-+-[AJUDA]-+-+-+-+-+-+-+-\n"BOLDBLACK"ESC"RESET":\t\t\tsurt del programa.\n"BOLDBLACK"H"RESET":\t\t\tmostra l'ajuda del programa.\n");
    printf("-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
}

int polygonMode = GL_FILL;

void selecciona(unsigned char key, int x, int y) {
    if(key=='h') {
        mostraAjuda();
    }else if(key=='r') {
        glutMotionFunc(rotaEscena);
    }else if(key==27) {
        exit(0);
    }
}

void refrescaPantalla(void) {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK,polygonMode);
    dibuixaEixos();
    dibuixaNinot();
    dibuixaEsfera();
    glutSwapBuffers();
}

void initGL() {
    glClearColor(0,0,0,0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
}

void initGlut(int argc, const char * argv[]) {
    glutInit(&argc, (char **)argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("IDI: Practiques OpenGL");
    glutDisplayFunc(refrescaPantalla);
    glutReshapeFunc(reubicaVP);
    glutMotionFunc(rotaEscena);
    glutMouseFunc(helper);
    glutKeyboardFunc(selecciona);
    initGL();
    glutMainLoop();
}

int main(int argc, const char * argv[]) {
    calculaCapsa();
    calculaEsfera();
    initGlut(argc, argv);
}