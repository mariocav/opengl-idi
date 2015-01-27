/*

*/

#if defined(__APPLE__)
    #include <OpenGL/OpenGL.h>
    #include <GLUT/GLUT.h>
#else
    #include <GL/gl.h>
    #include <GL/freeglut.h>
#endif

#include "model.h"
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

float xLegoDespl = 0.00;
float yLegoDespl = 0.00;

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
Model m;

void carregaModel(const char *s) {
    m.load(s);
}

Capsa cl;

void dibuixaModel() {
    const std::vector<Face> faces = m.faces();
    const std::vector<Vertex> vertexs = m.vertices();
    for (int i = 0; i < faces.size(); ++i) {
        glBegin(GL_TRIANGLES);
        glColor3f(
            Materials[faces[i].mat].diffuse[0],
            Materials[faces[i].mat].diffuse[1],
            Materials[faces[i].mat].diffuse[2]
        );
        glVertex3dv(&m.vertices()[faces[i].v[0]]);
        glVertex3dv(&m.vertices()[faces[i].v[1]]);
        glVertex3dv(&m.vertices()[faces[i].v[2]]);
        glEnd();
    }
}

void dibuixaTerra() {
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(-0.75,-0.4,-0.75);
    glVertex3f(-0.75,-0.4,0.75);
    glVertex3f(0.75,-0.4,0.75);
    glVertex3f(0.75,-0.4,-0.75);
    glEnd();
    glPopMatrix();
}

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
#define PERSPECTIVA 1
int camera = PERSPECTIVA;

void canviaTipusCamera(int tipusCamera) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(tipusCamera == AXONOMETRICA) {
        if(widthFinestra>=heightFinestra) {
            double ra = (double)widthFinestra/heightFinestra;
            glOrtho(-e.radi*ra, e.radi*ra, -e.radi, e.radi, -e.radi, 4*e.radi);
        }else if(widthFinestra<heightFinestra) {
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


void distanciaTrasllat(double d[]) {
    double x = -cl.minX-(cl.maxX-cl.minX)/2;
    double y = -cl.minY-(cl.maxY-cl.minY)/2;
    double z = -cl.minZ-(cl.maxZ-cl.minZ)/2;

    d[0] = x;
    d[1] = y;
    d[2] = z;
}

void dibuixaLegoman() {
    glPushMatrix();
    double d[3]; distanciaTrasllat(d);

    double f = ((cl.maxY+d[1])-(cl.minY+d[1]))/0.5;

    glTranslated(
        (-(cl.minX+d[0])/f)-0.75+xLegoDespl,
        (-(cl.minY+d[1])/f)-0.4+yLegoDespl,
        (-(cl.maxZ+d[2])/f)+0.75
    );
    glScaled(1/f, 1/f, 1/f);

    glTranslated(d[0],d[1],d[2]);
    dibuixaModel();
    glPopMatrix();
}

void dibuixaNinot() {
    glPushMatrix();
    glColor3f(0.2,0.5,0.7);
    glutWireSphere(0.4, 20, 20);
    glTranslatef(0, 0.6, 0);
    glutWireSphere(0.2, 20, 20);
    glTranslatef(0.1, 0, 0);
    glRotated(90, 0, 1, 0);
    glutWireCone(0.1, 0.2, 20, 20);
    glPopMatrix();
}

void calculaCapsaEscena() {
    c.minX = -0.75;
    c.maxX = 0.75;
    c.minY = -0.4;
    c.maxY = 0.8;
    c.minZ = -0.75;
    c.maxZ = 0.75;
}

void calculaCapsaLegoman() {
    cl.minX = cl.minY = cl.minZ = DBL_MAX;
    cl.maxX = cl.maxY = cl.maxZ = -DBL_MAX;
    for (int i = 0; i < m.vertices().size(); ++i) {
        const Vertex &vertex = m.vertices()[i];
        if(i%3==0) {
            cl.minX = (vertex < cl.minX)? vertex : cl.minX;
            cl.maxX = (vertex > cl.maxX)? vertex : cl.maxX;
        }else if(i%3==1) {
            cl.minY = (vertex < cl.minY)? vertex : cl.minY;
            cl.maxY = (vertex > cl.maxY)? vertex : cl.maxY;
        }else if(i%3==2) {
            cl.minZ = (vertex < cl.minZ)? vertex : cl.minZ;
            cl.maxZ = (vertex > cl.maxZ)? vertex : cl.maxZ;
        }
    }
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

void helper(int button, int state, int x, int y) {
    if(button == GLUT_LEFT_BUTTON) {

    }
}

void mostraAjuda() {
    printf("\n-+-+-+-+-+-+-+-[AJUDA]-+-+-+-+-+-+-+-\n"BOLDBLACK"ESC"RESET":\t\t\tsurt del programa.\n"BOLDBLACK"H"RESET":\t\t\tmostra l'ajuda del programa.");
    printf("\n"BOLDBLACK"R"RESET":\t\t\treseteja l'aplicació al seu estat inicial (càmera perspectiva).");
    printf("\n"BOLDBLACK"BOTÓ ESQUERRA"RESET":");
    printf("\t\tmantenir-lo i desplaçar-lo horitzontalment i/o verticalment modifica els angles d'Euler.");
    printf("\n"BOLDBLACK"P"RESET":\t\t\tcanvia a càmera perspectiva (axonomètrica).");
    printf("\n-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
}

int polygonMode = GL_FILL;

void selecciona(unsigned char key, int x, int y) {
    if(key=='h') {
        mostraAjuda();
    }else if(key=='p') {
        camera = !camera;
        canviaTipusCamera(camera);
        glutPostRedisplay();
    }else if(key=='r') {
        camera = PERSPECTIVA;
        angleX = 45;
        angleY = 45;
        angleZ = 0;
        canviaTipusCamera(camera);
        inicialitzaPosicioCamera();
        glutPostRedisplay();
    }else if(key==27) {
        exit(0);
    }
}

void refrescaPantalla(void) {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK,polygonMode);
    dibuixaEixos();
    dibuixaTerra();
    dibuixaNinot();
    dibuixaLegoman();
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
    glutMotionFunc(anglesEuler);
    glutMouseFunc(helper);
    glutKeyboardFunc(selecciona);
    glEnable(GL_DEPTH_TEST);
    initGL();
    glutMainLoop();
}

int main(int argc, const char * argv[]) {
    char s[] = "./models/legoman.obj";
    carregaModel(s);
    calculaCapsaLegoman();
    calculaCapsaEscena();
    calculaEsfera();
    initGlut(argc, argv);
}