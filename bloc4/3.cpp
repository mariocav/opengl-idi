/*

Apartado 3
Entrega final.

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
bool lighting = true;

bool normalPerCara = false;

bool light0 = false;
bool light1 = true;
bool light2 = false;

int posLight0 = 0;

void dibuixaModel() {
    const std::vector<Face> &faces = m.faces();
    const std::vector<Vertex> &vertexs = m.vertices();
    const std::vector<Normal> &normals = m.normals();
    int indexMaterial = -1;
    for (int i = 0; i < faces.size(); ++i) {
        if(!lighting) {
            glColor4fv(Materials[faces[i].mat].diffuse);
        }else{
            if(faces[i].mat!=indexMaterial) {
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Materials[faces[i].mat].ambient);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Materials[faces[i].mat].diffuse);
                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Materials[faces[i].mat].specular);
                glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Materials[faces[i].mat].shininess);
                indexMaterial = faces[i].mat;
            }
        }
        glBegin(GL_TRIANGLES);

        if(normalPerCara)
            glNormal3dv(faces[i].normalC);

        if(!normalPerCara)
            glNormal3dv(&normals[faces[i].n[0]]);
        glVertex3dv(&vertexs[faces[i].v[0]]);

        if(!normalPerCara)
            glNormal3dv(&normals[faces[i].n[1]]);
        glVertex3dv(&vertexs[faces[i].v[1]]);

        if(!normalPerCara)
            glNormal3dv(&normals[faces[i].n[2]]);
        glVertex3dv(&vertexs[faces[i].v[2]]);

        glEnd();
    }
}

void dibuixaTerra() { // brillant blau (especular)
    glPushMatrix();
    if(!lighting) {
        glColor3f(0,0,3);
    }else{
        float especular[4] = {0, 0, 0.8, 0.8};
        float difuso[4] = {0, 0, 0.3, 0.1};
        float ambiente[4] = {0, 0, 0.3, 0.2};
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambiente);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, difuso);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, especular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64);
    }
    glNormal3f(0, 1, 0);
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
    glLineWidth(4);
    glBegin(GL_LINES);
    if(!lighting) {
        glColor3f(1,0,0);
    }else{
        float ambiente[4] = {1, 0, 0, 1};
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambiente);
    }
    glNormal3f(0, 0, 0); // para que la iluminacion no afecte a los ejes.
    glVertex3f(0,0,0);
    glVertex3f(10,0,0);
    if(!lighting) {
        glColor3f(0,1,0);
    }else{
        float ambiente[4] = {0, 1, 0, 1};
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambiente);
    }
    glNormal3f(0, 0, 0); // para que la iluminacion no afecte a los ejes.
    glVertex3f(0,0,0);
    glVertex3f(0,10,0);
    if(!lighting) {
        glColor3f(0,0,1);
    }else{
        float ambiente[4] = {0, 0, 1, 1};
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambiente);
    }
    glNormal3f(0, 0, 0); // para que la iluminacion no afecte a los ejes.
    glVertex3f(0,0,0);
    glVertex3f(0,0,10);
    glEnd();
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

void ubicaLight2() {
    float especular[4] = {0.4, 0.4, 0.4, 0.4};
    float difuso[4] = {0.4, 0.4, 0.4, 0.4};
    //float ambiente[4] = {0, 0, 0, 0};
    float pos[4] = {xPatricio, 1.5, zPatricio, 1};
    glLightfv(GL_LIGHT2, GL_POSITION, pos);
    //glLightfv(GL_LIGHT2, GL_AMBIENT, ambiente);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, difuso);
    glLightfv(GL_LIGHT2, GL_SPECULAR, especular);
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
    if(!lighting) {
        glColor3f(0, 0.3, 0);
    }else{
        float especular[4] = {0, 0.3, 0, 0.1};
        float difuso[4] = {0, 0.3, 0, 0.4};
        float ambiente[4] = {0, 0.3, 0, 0.2};
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambiente);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, difuso);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, especular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64);
    }
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

    if(!lighting) {
        glColor3f(1,1,1);
    }else{
        float especular[4] = {1, 1, 1, 0.1};
        float difuso[4] = {1, 1, 1, 0.4};
        float ambiente[4] = {1, 1, 1, 0.2};
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambiente);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, difuso);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, especular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64);
    }

    glutSolidSphere(0.4, 20, 20);
    glTranslatef(0, 0.6, 0);
    glutSolidSphere(0.2, 20, 20);
    glTranslatef(0.1, 0, 0);
    glRotated(90, 0, 1, 0);

    if(!lighting) {
        glColor3f(1,0.5,0);
    }else{
        float especular[4] = {1,0.5,0, 0.1};
        float difuso[4] = {1,0.5,0, 0.4};
        float ambiente[4] = {1,0.5,0, 0.2};
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambiente);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, difuso);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, especular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64);
    }

    glutSolidCone(0.1, 0.2, 20, 20);
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
    glutSolidSphere(e.radi, 20, 20);
    glPopMatrix();
}

void anglesEuler(int x, int y) {

    double xDiferencia = x-xActual;
    double yDiferencia = y-yActual;

    double xAbsolutaDiferencia = abs(xDiferencia);
    double yAbsolutaDiferencia = abs(yDiferencia);

    if(xDiferencia<0 && xAbsolutaDiferencia>5) {
        angleY -= 5;
    }else if(xAbsolutaDiferencia>5){
        angleY += 5;
    }

    if(yDiferencia<0 && yAbsolutaDiferencia>5) {
        angleX  -= 5;
    }else if(yAbsolutaDiferencia>5){
        angleX += 5;
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
            glutMotionFunc(anglesEuler);
        }else{
            yActual = y;
            glutMotionFunc(zoom);
        }
    }
}

void mostraAjuda() {
    printf("\n-+-+-+-+-+-+-+-[AJUDA]-+-+-+-+-+-+-+-\n"BOLDBLACK"ESC"RESET":\t\t\tsurt del programa.\n"BOLDBLACK"H"RESET":\t\t\tmostra l'ajuda del programa.");
    printf("\n"BOLDBLACK"R"RESET":\t\t\treseteja l'aplicació (càmera perspectiva i il·luminació al seu estat inicial).");
    printf("\n"BOLDBLACK"BOTÓ ESQUERRA"RESET":");
    printf("\t\tmantenir-lo i desplaçar-lo horitzontalment i/o verticalment modifica els angles d'Euler.");
    printf("\n"BOLDBLACK"P"RESET":\t\t\tcanvia a càmera perspectiva (axonomètrica).");
    printf("\n"BOLDBLACK"BOTÓ DRET"RESET":");
    printf("\t\tmantenir-lo i desplaçar-lo verticalment fa zoom de la càmera activa.");
    printf("\n"BOLDBLACK"V"RESET":\t\t\t(no) pinta les parets de l'escena.");
    printf("\n"BOLDBLACK"W,S,A,D"RESET":\t\tmou (W,S) i rota (A,D) el Patricio central.");
    printf("\n"BOLDBLACK"Z,X"RESET":\t\t\tincrementa/decrementa la velocitat de moviment del Patricio central.");
    printf("\n"BOLDBLACK"C"RESET":\t\t\tcanvia el mode de càmera a tercera (primera) persona.");
    printf("\n"BOLDBLACK"N"RESET":\t\t\tactiva normals per cara (vertex).");
    printf("\n"BOLDBLACK"1"RESET":\t\t\t(des)activa el llum de càmera (light 1).");
    printf("\n"BOLDBLACK"0"RESET":\t\t\t(des)activa el llum groguenc de l'escena (light 0).");
    printf("\n"BOLDBLACK"M"RESET":\t\t\tsitua el llum groguenc de l'escena a la següent cantonada d'aquesta -en ordre cíclic- o al damunt del Patricio no-central si ja ha completat una volta.");
    printf("\n"BOLDBLACK"2"RESET":\t\t\t(des)activa el llum solidari amb el Patricio central (light 2).");
    printf("\n"BOLDBLACK"i"RESET":\t\t\t(des)activa la il·luminació.");
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
        light0 = false;
        light2 = false;
        light1 = true;
        lighting = true;
        posLight0 = 0;
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT2);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT1);
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
    }else if(key=='i') {
        lighting = !lighting;
        if(lighting)
            glEnable(GL_LIGHTING);
        else
            glDisable(GL_LIGHTING);
        glutPostRedisplay();
    }else if(key=='n') {
        normalPerCara = !normalPerCara;
        glutPostRedisplay();
    }else if(key=='1') {
        light1 = !light1;
        if(light1)
            glEnable(GL_LIGHT1);
        else
            glDisable(GL_LIGHT1);
        glutPostRedisplay();
    }else if(key=='0') {
        light0 = !light0;
        if(light0)
            glEnable(GL_LIGHT0);
        else
            glDisable(GL_LIGHT0);
        glutPostRedisplay();
    }else if(key=='m') {
        posLight0 += 1;
        if(posLight0==5)
            posLight0 = 0;
        glutPostRedisplay();
    }else if(key=='2') {
        light2 = !light2;
        if(light2)
            glEnable(GL_LIGHT2);
        else
            glDisable(GL_LIGHT2);
        glutPostRedisplay();
    }

}

void ubicaLight1() {
    glPushMatrix();
    glLoadIdentity();
    float especular[4] = {1, 1, 1, 0.3};
    float difuso[4] = {0.5, 0.5, 0.5, 0.3};
    //float ambiente[4] = {0, 0, 0, 0};
    float pos[4] = {0, 0, 0, 1};
    glLightfv(GL_LIGHT1, GL_POSITION, pos);
    //glLightfv(GL_LIGHT1, GL_AMBIENT, ambiente);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, difuso);
    glLightfv(GL_LIGHT1, GL_SPECULAR, especular);
    glPopMatrix();
}

void ubicaLight0() { // amarilla
    glPushMatrix();
    float especular[4] = {1, 1, 0, 0.3};
    float difuso[4] = {1, 1, 0, 0.3};
    //float ambiente[4] = {0, 0, 0, 0};
    float pos[4] = {0, 0, 0, 1};
    if(posLight0==0) {
        pos[0] = pos[2] = 5;
    }else if(posLight0==1) {
        pos[0] = 5;
        pos[2] = -5;
    }else if(posLight0==2) {
        pos[0] = pos[2] = -5;
    }else if(posLight0==3) {
        pos[0] = -5;
        pos[2] = 5;
    }else if(posLight0==4) {
        pos[0] = pos[2] = 2.5;
    }
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    //glLightfv(GL_LIGHT0, GL_AMBIENT, ambiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, difuso);
    glLightfv(GL_LIGHT0, GL_SPECULAR, especular);
    glPopMatrix();
}

void refrescaPantalla(void) {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK,polygonMode);
    if(light1)
        ubicaLight1();
    if(light0)
        ubicaLight0();
    if(light2)
        ubicaLight2();
    dibuixaEixos();
    dibuixaTerra();
    glPushMatrix();
    glTranslatef(2.5, 0.4, -2.5);
    dibuixaNinots();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-2.5, 0.4, 2.5);
    dibuixaNinots();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-2.5, 0.4, -2.5);
    dibuixaNinots();
    glPopMatrix();
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
    glEnable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    //glEnable(GL_LIGHT2);
    glEnable(GL_NORMALIZE);
    //glShadeModel(GL_FLAT);
    initGL();
    glutMainLoop();
}

int main(int argc, const char * argv[]) {
    char s[] = "./models/Patricio.obj";
    carregaModel(s);
    calculaCapsaPatricio();
    calculaCapsaEscena();
    calculaEsfera();
    time_t t;
    srand((unsigned) time(&t));
    angleX = rand() % 180;
    angleY = rand() % 180;
    angleZ = rand() % 180;
    fov = 2*(asin(e.radi/(2*e.radi)))*graus_per_radian;
    zNear = 2*e.radi-e.radi;
    zFar = 2*e.radi+e.radi;
    initGlut(argc, argv);
}