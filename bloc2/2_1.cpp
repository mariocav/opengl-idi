#include <stdio.h>

#define BOLDBLACK "\033[1m\033[30m"
#define RESET "\033[0m"

#if defined(__APPLE__)
    #include <OpenGL/OpenGL.h>
    #include <GLUT/GLUT.h>
#else
    #include <GL/gl.h>
    #include <GL/freeglut.h>
#endif

void dibuixaEixos() {
    glBegin(GL_LINES);
    glColor3f(1,0,0);
    glVertex3f(0,0,0);
    glVertex3f(1,0,0);
    glColor3f(0,1,0);
    glVertex3f(0,0,0);
    glVertex3f(0,1,0);
    glColor3f(0,0,1);
    glVertex3f(0,0,0);
    glVertex3f(0,0,1);
    glEnd();
    glColor3f(1,1,1);
}


void refrescaPantalla(void) {
	glClear(GL_COLOR_BUFFER_BIT);
    dibuixaEixos();
    //glLoadIdentity();
    //glRotated(45, 0, 1, 0);
    //glRotated(45, 1, 0, 0);
    //glutWireSphere(0.5,40,40);
    glutWireTeapot(0.5);
    glutSwapBuffers();
}

void reubicaVP(int width, int height) {
    if(width>height) {
        glViewport((width-height)/2, 0, height, height);
    }else if(height>width){
        glViewport(0, (height-width)/2, width, width);
    }
}

float xActual = 0;
float yActual = 0;

void rotaObjectes(int x, int y) {
    float w = (float)glutGet(GLUT_WINDOW_WIDTH);
    float h = (float)glutGet(GLUT_WINDOW_HEIGHT);

    float xDiferencia = x-xActual;
    float yDiferencia = y-yActual;

    float xAbsolutaDiferencia = abs(xDiferencia);
    float yAbsolutaDiferencia = abs(yDiferencia);

    if(xDiferencia<0 && xAbsolutaDiferencia>5) {
        glRotatef(-w/360*4, 0, 1, 0);
    }else if(xAbsolutaDiferencia>5){
        glRotatef(w/360*4, 0, 1, 0);
    }

    if(yDiferencia<0 && yAbsolutaDiferencia>5) {
        glRotatef(-h/360*4, 1, 0, 0);
    }else if(yAbsolutaDiferencia>5){
        glRotatef(h/360*4, 1, 0, 0);
    }

    yActual = y;
    xActual = x;

    glutPostRedisplay();
}

void helperRotaObjectes(int button, int state, int x, int y) {
    if(button == GLUT_LEFT_BUTTON) {
        //if(state == GLUT_UP) {
            yActual = y;
            xActual = x;
        //}
    }
}

void initGlut(int argc, const char * argv[]) {
    glutInit(&argc, (char **)argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("IDI: Practiques OpenGL");
    glutDisplayFunc(refrescaPantalla);
    glutReshapeFunc(reubicaVP);
    glutMotionFunc(rotaObjectes);
    glutMouseFunc(helperRotaObjectes);
    initGL();
    glutMainLoop();
}

void initGL() {
    glClearColor(0,0,0,1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1,1,-1,1,-1,1);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, const char * argv[]) {
    initGlut(argc, argv);
}