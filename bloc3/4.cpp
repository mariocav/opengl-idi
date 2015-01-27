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

int widthFinestra = 600;
int heightFinestra = 600;

int xActual = 0;
int yActual = 0;

int angleX = 45;
int angleY = 45;
int angleZ = 0;

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

void inicialitzaPosicioCamera() {
    glLoadIdentity();
    glTranslated(0, 0, -2*e.radi); // ojo, así el glOrtho falla!!
    glRotated(angleZ, 0, 0, 1);
    glRotated(angleX, 1, 0, 0);
    glRotated(-angleY, 0, 1, 0);
    //glTranslated(-VRP.x, -VRP.y, -VRP.z); // el ninot está en origen, no hace falta.
}

#define AXONOMETRICA 0
int camera = AXONOMETRICA;

void canviaTipusCamera(int tipusCamera) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(tipusCamera == AXONOMETRICA) {
        if(widthFinestra>=heightFinestra) {
            double ra = (double)widthFinestra/heightFinestra;
            glOrtho(-e.radi*ra, e.radi*ra, -e.radi, e.radi, -e.radi, 4*e.radi);
        }else if(widthFinestra<heightFinestra) {
            cout << "A " << endl;
            double ra = (double)heightFinestra/widthFinestra;
            glOrtho(-e.radi, e.radi, -e.radi*ra, e.radi*ra, -e.radi, 4*e.radi);
        }
    }else{
        if(widthFinestra>=heightFinestra) {
            double ra = (double)widthFinestra/heightFinestra;
            double radians_per_grau = 3.14/180;
            double graus_per_radian = 180/3.14;
            double fov = 2*(asin(e.radi/(2*e.radi)))*graus_per_radian;
            double zNear = 2*e.radi-e.radi;
            double zFar = 2*e.radi+e.radi;
            gluPerspective(fov, ra, zNear, zFar);
        }else{
            double ra = (double)widthFinestra/heightFinestra;
            double radians_per_grau = 3.14/180;
            double graus_per_radian = 180/3.14;
            double fov = 2*(asin(e.radi/(2*e.radi)))*graus_per_radian;
            double nouFov = 2*(atan(tan(double(fov*radians_per_grau)/2)/ra))*graus_per_radian;
            double zNear = 2*e.radi-e.radi;
            double zFar = 2*e.radi+e.radi;
            gluPerspective(nouFov, ra, zNear, zFar);
        }

    }
    glMatrixMode(GL_MODELVIEW);
}

void reubicaVP(int width, int height) {
    widthFinestra = width;
    heightFinestra = height;
    glViewport(0, 0, widthFinestra, heightFinestra);
    canviaTipusCamera(camera);
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
    glPushMatrix();
    glColor3f(0.3,0.7,0.23);
    glutWireSphere(e.radi, 20, 20);
    glPopMatrix();
}

void anglesEuler(int x, int y) {

    double w = (float)glutGet(GLUT_WINDOW_WIDTH);
    double h = (float)glutGet(GLUT_WINDOW_HEIGHT);

    double xDiferencia = x-xActual;
    double yDiferencia = y-yActual;

    double xAbsolutaDiferencia = abs(xDiferencia);
    double yAbsolutaDiferencia = abs(yDiferencia);

    if(xDiferencia<0 && xAbsolutaDiferencia>5) {
        angleY -= w/360*4;
    }else if(xAbsolutaDiferencia>5){
        angleY += w/360*4;
    }

    if(yDiferencia<0 && yAbsolutaDiferencia>3) {
        angleX -= w/360*4;
    }else if(yAbsolutaDiferencia>3){
        angleX += w/360*4;
    }

    yActual = y;
    xActual = x;

    inicialitzaPosicioCamera();
    glutPostRedisplay();
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
    printf("\n-+-+-+-+-+-+-+-[AJUDA]-+-+-+-+-+-+-+-\n"BOLDBLACK"ESC"RESET":\t\t\tsurt del programa.\n"BOLDBLACK"H"RESET":\t\t\tmostra l'ajuda del programa.");
    printf("\n"BOLDBLACK"R"RESET":\t\t\tcanvia l'estat de l'aplicació (mode rotació).");
    printf("\n"BOLDBLACK"E"RESET":\t\t\tcanvia l'estat de l'aplicació (mode rotació angles euler).");
    printf("\n"BOLDBLACK"P"RESET":\t\t\tcanvia a càmera perspectiva (axonomètrica).");
    printf("\n-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
}

int polygonMode = GL_FILL;

void selecciona(unsigned char key, int x, int y) {
    if(key=='h') {
        mostraAjuda();
    }else if(key=='r') {
        glutMotionFunc(rotaEscena);
    }else if(key=='e') {
        glutMotionFunc(anglesEuler);
    }else if(key=='p') {
        camera = !camera;
        canviaTipusCamera(camera);
        glutPostRedisplay();
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
    inicialitzaPosicioCamera();
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