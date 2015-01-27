/*

Apartado 9.2
Cambiar entre cámaras en 3a y 1a persona.

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

int angleX = 0;
int angleY = 0;
int angleZ = 0;

float xPatricio = 0;
float zPatricio = 0;

int anglePatricio = -90;
float velPatricio = 0.25;

double zNear;
double zFar;

bool terceraPersona = true;

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
    glVertex3f(-5,0,-5);
    glVertex3f(-5,0,5);
    glVertex3f(5,0,5);
    glVertex3f(5,0,-5);
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
    if(terceraPersona) {
        glTranslated(0, 0, -2*e.radi);
        glRotated(angleZ, 0, 0, 1);
        glRotated(angleX, 1, 0, 0);
        glRotated(-angleY, 0, 1, 0);
        //glTranslated(-VRP.x, -VRP.y, -VRP.z); // el ninot está en origen, no hace falta.
    }else{
        gluLookAt(
            xPatricio+0.1, 1.5, zPatricio+0.1,
            xPatricio+0.1+sin((anglePatricio*3.14)/180), 1.5, zPatricio + cos((anglePatricio*3.14)/180),
            0, 1, 0
        );
    }
}

#define AXONOMETRICA 0
#define PERSPECTIVA 1
int camera = PERSPECTIVA;

double radians_per_grau = 3.14/180;
double graus_per_radian = 180/3.14;
double fov;

Esfera zoomOrtho;
void canviaTipusCamera(int tipusCamera) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(tipusCamera == AXONOMETRICA) {
        if(widthFinestra>=heightFinestra) {
            double ra = (double)widthFinestra/heightFinestra;
            glOrtho(-zoomOrtho.radi*ra, zoomOrtho.radi*ra, -zoomOrtho.radi, zoomOrtho.radi, -e.radi, 4*e.radi);
        }else if(widthFinestra<heightFinestra) {
            double ra = (double)heightFinestra/widthFinestra;
            glOrtho(-zoomOrtho.radi, zoomOrtho.radi, -zoomOrtho.radi*ra, zoomOrtho.radi*ra, -e.radi, 4*e.radi);
        }
    }else{
        if(widthFinestra>=heightFinestra) {
            double ra = (double)widthFinestra/heightFinestra;
            gluPerspective(fov, ra, zNear, zFar);
        }else{
            double ra = (double)widthFinestra/heightFinestra;
            double nouFov = 2*(atan(tan(double(fov*radians_per_grau)/2)/ra))*graus_per_radian;
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

double d[3];

float factorEscalado(float maximaDimension) {
    float x = (cl.maxX+d[0])-(cl.minX+d[0]);
    float y = (cl.maxY+d[1])-(cl.minY+d[1]);
    float z = (cl.maxZ+d[2])-(cl.minZ+d[2]);

    float max;

    if(x>y && x>z) {
        max = x;
    }else if(y>x && y>z) {
        max = y;
    }else{
        max = z;
    }

    return max/maximaDimension;

}

void dibuixaPatricios() {
    glPushMatrix();
    distanciaTrasllat(d);

    float alturaDeseada = 1;

    double f = factorEscalado(alturaDeseada);

    glTranslated(
        (-(cl.minX+d[0])/f+xPatricio),
        (-(cl.minY+d[1])/f),
        (-(cl.maxZ+d[2])/f+zPatricio)
    );

    glRotatef(anglePatricio, 0, 1, 0);


    glScaled(1/f, 1/f, 1/f);

    glTranslated(d[0],d[1],d[2]);
    dibuixaModel();
    glPopMatrix();

    // 2o patricio

    glPushMatrix();

    alturaDeseada = 1.5;

    f = factorEscalado(alturaDeseada);

    glTranslatef(
        (-(cl.minX+d[0])/f)+2.5,
        (-(cl.minY+d[1])/f),
        (-(cl.maxZ+d[2])/f)+2.5
    );

    glScaled(1/f, 1/f, 1/f);

    glTranslatef(d[0],d[1],d[2]);
    dibuixaModel();
    glPopMatrix();
}

void dibuixaParets() {
    glPushMatrix();
    glTranslatef(0,0.75,-4.9);
    glScalef(10, 1.5, 0.2);
    glutSolidCube(1);
    glPopMatrix();

    // 2a pared

    glPushMatrix();
    glTranslatef(1.5, 0.75, 2.5);
    glScalef(0.2,1.5,4);
    glutSolidCube(1);
    glPopMatrix();
}

void dibuixaNinots() {
    glPushMatrix();
    glTranslatef(2.5, 0.4, -2.5);
    glColor3f(0.2,0.5,0.7);
    glutWireSphere(0.4, 20, 20);
    glTranslatef(0, 0.6, 0);
    glutWireSphere(0.2, 20, 20);
    glTranslatef(0.1, 0, 0);
    glRotated(90, 0, 1, 0);
    glutWireCone(0.1, 0.2, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-2.5, 0.4, 2.5);
    glColor3f(0.2,0.3,0.7);
    glutWireSphere(0.4, 20, 20);
    glTranslatef(0, 0.6, 0);
    glutWireSphere(0.2, 20, 20);
    glTranslatef(0.1, 0, 0);
    glRotated(90, 0, 1, 0);
    glutWireCone(0.1, 0.2, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-2.5, 0.4, -2.5);
    glColor3f(0.1,0.5,0.5);
    glutWireSphere(0.4, 20, 20);
    glTranslatef(0, 0.6, 0);
    glutWireSphere(0.2, 20, 20);
    glTranslatef(0.1, 0, 0);
    glRotated(90, 0, 1, 0);
    glutWireCone(0.1, 0.2, 20, 20);
    glPopMatrix();
}

void calculaCapsaEscena() {
    c.minX = -5;
    c.maxX = 5;
    c.minY = 0;
    c.maxY = 1.5;
    c.minZ = -5;
    c.maxZ = 5;
}

void calculaCapsaPatricio() {
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

    zoomOrtho.diametre = e.diametre;
    zoomOrtho.radi = e.diametre / 2.0;
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

void zoom(int x, int y) {

    double w = (float)glutGet(GLUT_WINDOW_WIDTH);
    double h = (float)glutGet(GLUT_WINDOW_HEIGHT);

    double yDiferencia = y-yActual;

    double yAbsolutaDiferencia = abs(yDiferencia);

    if(yDiferencia<0 && yAbsolutaDiferencia>3) {
        if(camera==AXONOMETRICA)
            zoomOrtho.radi *= 1.05;
        else
            fov *= 1.05;
    }else if(yAbsolutaDiferencia>3){
        if(camera==AXONOMETRICA)
            zoomOrtho.radi *= 0.95;
        else
            fov *= 0.95;
    }

    if(camera==AXONOMETRICA) {
        if(zoomOrtho.radi>4*e.radi)
            zoomOrtho.radi = 4*e.radi;
    }else {
        if(fov>120)
            fov = 120;
    }

    yActual = y;

    canviaTipusCamera(camera);

    glutPostRedisplay();
}

void helper(int button, int state, int x, int y) {
    if(button == GLUT_RIGHT_BUTTON) {
        if(state == GLUT_UP) {
            //glutMotionFunc(anglesEuler);
        }else{
            yActual = y;
            glutMotionFunc(zoom);
        }
    }
}

void mostraAjuda() {
    printf("\n-+-+-+-+-+-+-+-[AJUDA]-+-+-+-+-+-+-+-\n"BOLDBLACK"ESC"RESET":\t\t\tsurt del programa.\n"BOLDBLACK"H"RESET":\t\t\tmostra l'ajuda del programa.");
    printf("\n"BOLDBLACK"R"RESET":\t\t\treseteja l'aplicació al seu estat inicial (càmera perspectiva).");
    printf("\n"BOLDBLACK"BOTÓ ESQUERRA"RESET":");
    printf("\t\tmantenir-lo i desplaçar-lo horitzontalment i/o verticalment modifica els angles d'Euler.");
    printf("\n"BOLDBLACK"P"RESET":\t\t\tcanvia a càmera perspectiva (axonomètrica).");
    printf("\n"BOLDBLACK"BOTÓ DRET"RESET":");
    printf("\t\tmantenir-lo i desplaçar-lo verticalment fa zoom de la càmera activa.");
    printf("\n"BOLDBLACK"V"RESET":\t\t\t(no) pinta les parets de l'escena.");
    printf("\n"BOLDBLACK"W,S,A,D"RESET":\t\tmou (W,S) i rota (A,D) el Patricio central.");
    printf("\n"BOLDBLACK"Z,X"RESET":\t\t\tincrementa/decrementa la velocitat de moviment del Patricio central.");
    printf("\n"BOLDBLACK"C"RESET":\t\t\tcanvia el mode de càmera a tercera (primera) persona.");
    printf("\n-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
}

int polygonMode = GL_FILL;

bool pintaParets = true;

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
    }else if(key=='v') {
        pintaParets = !pintaParets;
        glutPostRedisplay();
    }else if(key==27) {
        exit(0);
    }else if(key=='w') {
        xPatricio += sin((anglePatricio*3.14)/180)*velPatricio;
        zPatricio += cos((anglePatricio*3.14)/180)*velPatricio;
        inicialitzaPosicioCamera();
        glutPostRedisplay();
    }else if(key=='s') {
        zPatricio -= cos((anglePatricio*3.14)/180)*velPatricio;
        xPatricio -= sin((anglePatricio*3.14)/180)*velPatricio;
        inicialitzaPosicioCamera();
        glutPostRedisplay();
    }else if(key=='a') {
        anglePatricio += 5;
        inicialitzaPosicioCamera();
        glutPostRedisplay();
    }else if(key=='d') {
        anglePatricio -= 5;
        inicialitzaPosicioCamera();
        glutPostRedisplay();
    }else if(key=='z') {
        velPatricio += 0.25;
    }else if(key=='x') {
        velPatricio -= 0.25;
        if(velPatricio<=0)
            velPatricio = 0;
    }else if(key=='c') {
        terceraPersona = !terceraPersona;
        if(terceraPersona) {
            zNear = 2*e.radi-e.radi;
        }else{
            zNear = 0.2;
        }
        canviaTipusCamera(camera);
        inicialitzaPosicioCamera();
        glutPostRedisplay();
    }
}

void refrescaPantalla(void) {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK,polygonMode);
    dibuixaEixos();
    dibuixaTerra();
    dibuixaNinots();
    dibuixaPatricios();
    if(pintaParets)
        dibuixaParets();
    //dibuixaEsfera();
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
    char s[] = "./models/Patricio.obj";
    carregaModel(s);
    calculaCapsaPatricio();
    calculaCapsaEscena();
    calculaEsfera();
    fov = 2*(asin(e.radi/(2*e.radi)))*graus_per_radian;
    zNear = 2*e.radi-e.radi;
    zFar = 2*e.radi+e.radi;
    initGlut(argc, argv);
}