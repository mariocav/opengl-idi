#include <stdio.h>
#include "utils.h"

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

int xActual = 0;
int yActual = 0;

void rotaNinot(int x, int y) {

    float w = (float)glutGet(GLUT_WINDOW_WIDTH);
    float h = (float)glutGet(GLUT_WINDOW_HEIGHT);

    float xDiferencia = x-xActual;
    float yDiferencia = y-yActual;

    float xAbsolutaDiferencia = abs(xDiferencia);
    float yAbsolutaDiferencia = abs(yDiferencia);

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

void escalaNinot(int x, int y) {
    float w = (float)glutGet(GLUT_WINDOW_WIDTH);
    float h = (float)glutGet(GLUT_WINDOW_HEIGHT);

    float xDiferencia = x-xActual;

    float xAbsolutaDiferencia = abs(xDiferencia);

    if(xDiferencia<0 && xAbsolutaDiferencia>3) {
        glScalef(0.8, 0.8, 0.8);
    }else if(xAbsolutaDiferencia>3) {
        glScalef(1.2, 1.2, 1.2);
    }

    xActual = x;

    glutPostRedisplay();
}

void helper(int button, int state, int x, int y) {
    if(button == GLUT_LEFT_BUTTON) {
        yActual = y;
        xActual = x;
    }
}

void mostraAjuda() {
    printf("\n-+-+-+-+-+-+-+-[AJUDA]-+-+-+-+-+-+-+-\n"BOLDBLACK"ESC"RESET":\t\t\tsurt del programa.\n"BOLDBLACK"H"RESET":\t\t\tmostra l'ajuda del programa.");
    printf("\n"BOLDBLACK"R"RESET":\t\t\tcanvia l'estat de l'aplicació (mode rotació).");
    printf("\n"BOLDBLACK"E"RESET":\t\t\tcanvia l'estat de l'aplicació (mode scalat).");
    printf("\n"BOLDBLACK"N"RESET":\t\t\tcanvia l'estat de l'aplicació (mode normal, només visualització).");
    printf("\n"BOLDBLACK"BOTÓ ESQUERRA (escala)"RESET":");
    printf("\tmantenir-lo premut i desplaçar-lo horitzontalment aplica un escalat sobre l'objecte (cap a l'esquerra, el disminueix; cap a la dreta, l'augmenta de tamany).");
    printf("\n"BOLDBLACK"BOTÓ ESQUERRA (rota)"RESET":");
    printf("\tmantenir-lo i desplaçar-lo horitzontalment i/o verticalment aplica una rotació de l'objecte (horitzontalment, sobre l'eix de les Y; verticalment, sobre l'eix de les X). Així, amb un cop de mouse cap a l'esquerra o cap avall, el rota en el sentit anti-horari; cap a la dreta o cap amunt, en sentit horari).\n");
    printf("-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
}

void selecciona(unsigned char key, int x, int y) {
    if(key=='h') {
        mostraAjuda();
    }else if(key=='e') {
        glutMotionFunc(escalaNinot);
    }else if(key=='r') {
        glutMotionFunc(rotaNinot);
    }else if(key=='n') {
        glutMotionFunc(NULL);
    }else if(key==27) {
        exit(0);
    }
}

void refrescaPantalla(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    dibuixaEixos();
    dibuixaNinot();
    glutSwapBuffers();
}

void initGlut(int argc, const char * argv[]) {
    glutInit(&argc, (char **)argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("IDI: Practiques OpenGL");
    glutDisplayFunc(refrescaPantalla);
    glutReshapeFunc(reubicaVP);
    glutMotionFunc(rotaNinot);
    glutMouseFunc(helper);
    glutKeyboardFunc(selecciona);
    initGL();
    glutMainLoop();
}

int main(int argc, const char * argv[]) {
    initGlut(argc, argv);
}