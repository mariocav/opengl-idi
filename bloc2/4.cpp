#include <stdio.h>
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

void carregaModel(const char *s) {
    m.load(s);
}

void dibuixaModel() {
    // Read our .obj file
    const std::vector<Face> f = m.faces();
    const std::vector<Vertex> v = m.vertices();
    for (int i = 0; i < f.size(); ++i) {
        glBegin(GL_TRIANGLES);
            glColor3f(
                Materials[f[i].mat].diffuse[0],
                Materials[f[i].mat].diffuse[1],
                Materials[f[i].mat].diffuse[2]
            );
            glVertex3dv(&m.vertices()[f[i].v[0]]);
            glVertex3dv(&m.vertices()[f[i].v[1]]);
            glVertex3dv(&m.vertices()[f[i].v[2]]);
        glEnd();
    }
}

void obteCapsaContenidora() {
    for (int i = 0; i < m.vertices().size(); i+=3) {
        const Vertex &xVertex = m.vertices()[i];
        c.minX = (xVertex < c.minX)? xVertex : c.minX;
        c.maxX = (xVertex > c.maxX)? xVertex : c.maxX;
        const Vertex &yVertex = m.vertices()[i+1];
        c.minY = (yVertex < c.minY)? yVertex : c.minY;
        c.maxY = (yVertex > c.maxY)? yVertex : c.maxY;
        const Vertex &zVertex = m.vertices()[i+2];
        c.minZ = (zVertex < c.minZ)? zVertex : c.minZ;
        c.maxZ = (zVertex > c.maxZ)? zVertex : c.maxZ;
    }
}

void distanciaTrasllat(double d[]) {
    // cuánto debemos desplazar el objeto para situarlo en el origen de coordenadas?
    double x = -c.minX-(c.maxX-c.minX)/2;
    double y = -c.minY-(c.maxY-c.minY)/2;
    double z = -c.minZ-(c.maxZ-c.minZ)/2;

    d[0] = x;
    d[1] = y;
    d[2] = z;
}

double factorReduccio(double &f) {
    // cuánto debemos reducir el objeto para que quepa en pantalla?
    f = 1.00f;

    if(c.maxX > 1.0f && c.maxX > f) f = c.maxX;
    if(c.minX < -1.0f && abs(c.minX) > f) f = abs(c.minX);

    if(c.maxY > 1.0f && c.maxY > f) f = c.maxY;
    if(c.minY < -1.0f && abs(c.minY) > f) f = abs(c.minY);

    if(c.maxZ > 1.0f && c.maxZ > f) f = c.maxZ;
    if(c.minZ < -1.0f && abs(c.minZ) > f) f = abs(c.minZ);

    return f;

}

void dibuixaNinot() {

    double d[3]; distanciaTrasllat(d);

    c.maxX += d[0]; c.minX += d[0];
    c.maxY += d[1]; c.minY += d[1];
    c.maxZ += d[2]; c.minZ += d[2];

    double f; factorReduccio(f);
    if(f>1.00) {
        glScaled(1/f, 1/f, 1/f);

        c.maxX /= f;
        c.minX /= f;
        c.maxY /= f;
        c.minY /= f;
        c.maxZ /= f;
        c.minZ /= f;

    }
    glTranslated(d[0],d[1],d[2]);

    dibuixaModel();
}

void rotaNinot(int x, int y) {

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

void escalaNinot(int x, int y) {
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
    printf("\tmantenir-lo i desplaçar-lo horitzontalment i/o verticalment aplica una rotació de l'objecte (horitzontalment, sobre l'eix de les Y; verticalment, sobre l'eix de les X). Així, amb un cop de mouse cap a l'esquerra o cap avall, el rota en el sentit anti-horari; cap a la dreta o cap amunt, en sentit horari).");
    printf("\n"BOLDBLACK"V"RESET":\t\t\tcanvia entre els modes GL_LINE i GL_FILL");
    printf("\n"BOLDBLACK"B"RESET":\t\t\t(des)activa el flag GL_DEPTH_TEST\n");
    printf("-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
}

int polygonMode = GL_FILL;

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
    }else if(key=='v') {
        polygonMode = (polygonMode==GL_FILL) ? GL_LINE : GL_FILL;
        glutPostRedisplay();
    }else if(key=='b') {
        if(glIsEnabled(GL_DEPTH_TEST))
            glDisable(GL_DEPTH_TEST);
        else
            glEnable(GL_DEPTH_TEST);
        glutPostRedisplay();
    }
}

void refrescaPantalla(void) {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK,polygonMode);
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
    //char s[] = "./models/HomerProves.obj";
    if(!argv[1]) {
        printf("Usage: ./4 [RUTA_OBJECTE]");
        exit(1);
    }
    carregaModel(argv[1]);
    obteCapsaContenidora();
    initGlut(argc, argv);
}