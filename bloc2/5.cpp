#include <stdio.h>
#include <float.h> // DBL_MAX
#include "utils.h"
#include "model.h"


typedef struct {
    double minX;
    double maxX;
    double minY;
    double maxY;
    double minZ;
    double maxZ;
} Capsa;

Capsa c;
Model m;

int xActual = 0;
int yActual = 0;

float xLego = 0.00;
float yLego = 0.00;

float xLegoDespl = 0.00;
float yLegoDespl = 0.00;

bool mouLegoman = false;

void carregaModel(const char *s) {
    m.load(s);
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

void obteCapsaContenidora() {
    c.minX = c.minY = c.minZ = DBL_MAX;
    c.maxX = c.maxY = c.maxZ = -DBL_MAX;
    for (int i = 0; i < m.vertices().size(); ++i) {
        const Vertex &vertex = m.vertices()[i];
        if(i%3==0) {
            c.minX = (vertex < c.minX)? vertex : c.minX;
            c.maxX = (vertex > c.maxX)? vertex : c.maxX;
        }else if(i%3==1) {
            c.minY = (vertex < c.minY)? vertex : c.minY;
            c.maxY = (vertex > c.maxY)? vertex : c.maxY;
        }else if(i%3==2) {
            c.minZ = (vertex < c.minZ)? vertex : c.minZ;
            c.maxZ = (vertex > c.maxZ)? vertex : c.maxZ;
        }
    }
}

void distanciaTrasllat(double d[]) {
    double x = -c.minX-(c.maxX-c.minX)/2;
    double y = -c.minY-(c.maxY-c.minY)/2;
    double z = -c.minZ-(c.maxZ-c.minZ)/2;

    d[0] = x;
    d[1] = y;
    d[2] = z;
}

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

void dibuixaLegoman() {
    glPushMatrix();
    double d[3]; distanciaTrasllat(d);

    double f = ((c.maxY+d[1])-(c.minY+d[1]))/0.5;

    glTranslated(
        (-(c.minX+d[0])/f)-0.75+xLegoDespl,
        (-(c.minY+d[1])/f)-0.4+yLegoDespl,
        (-(c.maxZ+d[2])/f)+0.75
    );
    glScaled(1/f, 1/f, 1/f);

    glTranslated(d[0],d[1],d[2]);
    dibuixaModel();
    glPopMatrix();
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

void traslladaLego(int x, int y) {

    double w = (float)glutGet(GLUT_WINDOW_WIDTH);
    double h = (float)glutGet(GLUT_WINDOW_HEIGHT);

    double xDiferencia = x-xLego;
    double yDiferencia = y-yLego;

    double xAbsolutaDiferencia = abs(xDiferencia);
    double yAbsolutaDiferencia = abs(yDiferencia);

    if(xDiferencia<0 && xAbsolutaDiferencia>5) {
        xLegoDespl += -0.05;
    }else if(xAbsolutaDiferencia>5) {
        xLegoDespl += 0.05;
    }

    if(yDiferencia<0 && yAbsolutaDiferencia>5) {
        yLegoDespl += 0.05;
    }else if(yAbsolutaDiferencia>5){
        yLegoDespl += -0.05;
    }

    yLego = y;
    xLego = x;

    glutPostRedisplay();
}

void escalaEscena(int x, int y) {
    double w = (float)glutGet(GLUT_WINDOW_WIDTH);
    double h = (float)glutGet(GLUT_WINDOW_HEIGHT);

    double xDiferencia = x-xActual;

    double xAbsolutaDiferencia = abs(xDiferencia);

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
        if(mouLegoman) {
            yLego = y;
            xLego = x;
        }else{
            yActual = y;
            xActual = x;
        }
    }
}

void mostraAjuda() {
    printf("\n-+-+-+-+-+-+-+-[AJUDA]-+-+-+-+-+-+-+-\n"BOLDBLACK"ESC"RESET":\t\t\tsurt del programa.\n"BOLDBLACK"H"RESET":\t\t\tmostra l'ajuda del programa.");
    printf("\n"BOLDBLACK"R"RESET":\t\t\tcanvia l'estat de l'aplicació (mode rotació).");
    printf("\n"BOLDBLACK"E"RESET":\t\t\tcanvia l'estat de l'aplicació (mode escalat).");
    printf("\n"BOLDBLACK"N"RESET":\t\t\tcanvia l'estat de l'aplicació (mode normal, només visualització).");
    printf("\n"BOLDBLACK"BOTÓ ESQUERRA (escala)"RESET":");
    printf("\tmantenir-lo premut i desplaçar-lo horitzontalment aplica un escalat sobre l'escena (cap a l'esquerra, el disminueix; cap a la dreta, l'augmenta de tamany).");
    printf("\n"BOLDBLACK"BOTÓ ESQUERRA (rota)"RESET":");
    printf("\tmantenir-lo i desplaçar-lo horitzontalment i/o verticalment aplica una rotació sobre l'escena (horitzontalment, sobre l'eix de les Y; verticalment, sobre l'eix de les X). Així, amb un cop de mouse cap a l'esquerra o cap avall, el rota en el sentit anti-horari; cap a la dreta o cap amunt, en sentit horari).");
    printf("\n"BOLDBLACK"V"RESET":\t\t\tcanvia entre els modes GL_LINE i GL_FILL");
    printf("\n"BOLDBLACK"B"RESET":\t\t\t(des)activa el flag GL_DEPTH_TEST\n");
    printf("\n"BOLDBLACK"C"RESET":\t\t\tcanvia l'estat de l'aplicació (mode lego).");
    printf("\n"BOLDBLACK"BOTÓ ESQUERRA (lego)"RESET":");
    printf("\tmantenir-lo i desplaçar-lo horitzontalment i/o verticalment aplica una traslació sobre l'objecte legoman (horitzontalment, sobre l'eix de les X; verticalment, sobre l'eix de les Y).\n");
    printf("-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
}

int polygonMode = GL_FILL;

void selecciona(unsigned char key, int x, int y) {
    mouLegoman = false;
    if(key=='h') {
        mostraAjuda();
    }else if(key=='e') {
        glutMotionFunc(escalaEscena);
    }else if(key=='r') {
        glutMotionFunc(rotaEscena);
    }else if(key=='n') {
        glutMotionFunc(NULL);
    }else if(key==27) {
        exit(0);
    }else if(key=='v') {
        polygonMode = (polygonMode==GL_FILL) ? GL_LINE : GL_FILL;
        glutPostRedisplay();
    }else if(key=='b') {
        if(glIsEnabled(GL_DEPTH_TEST))
            glDisable(GL_DEPTH_TEST);
        else
            glEnable(GL_DEPTH_TEST);
        glutPostRedisplay();
    }else if(key=='c') {
        mouLegoman = true;
        glutMotionFunc(traslladaLego);
    }
}

void refrescaPantalla(void) {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK,polygonMode);
    dibuixaEixos();
    dibuixaTerra();
    dibuixaNinot();
    dibuixaLegoman();
    glutSwapBuffers();
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
    char s[] = "./models/legoman.obj";
    /*if(!argv[1]) {
        printf("Usage: ./4 [RUTA_OBJECTE]");
        exit(1);
    }
    carregaModel(argv[1]);*/
    carregaModel(s);
    obteCapsaContenidora();
    initGlut(argc, argv);
}