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

    glPushMatrix();

    glLoadIdentity();

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

    glPopMatrix();

}


void refrescaPantalla(void) {
	glClear(GL_COLOR_BUFFER_BIT);
    dibuixaEixos();
    //glLoadIdentity();
    //glRotated(45, 0, 1, 0);

    //glRotated(45, 0, 0, 1);

    //glScalef(0.6, 1, 1);
    //glTranslatef(0.60, 0, 0);


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

char eixActual = 'x';

void escalaObjectes(char operacio) {

   	if(operacio=='+') {
   		if(eixActual=='x') {
   			glScalef(1.2, 1, 1);
   		}else if(eixActual=='y') {
   			glScalef(1, 1.2, 1);
   		}else if(eixActual=='z') {
   			glScalef(1, 1, 1.2);
   		}
   	}else if(operacio=='-'){
   		if(eixActual=='x') {
   			glScalef(0.8, 1, 1);
   		}else if(eixActual=='y') {
   			glScalef(1, 0.8, 1);
   		}else if(eixActual=='z') {
   			glScalef(1, 1, 0.8);
   		}
   	}

    glutPostRedisplay();
}

void mostraAjuda() {
	printf("prèmer x,y o z per canviar l'eix de coordenades on s'aplicarà l'escalat\n");
	printf("prèmer + o - per aplicar un escalat de +20%% o -20%% sobre l'objecte, en l'eix de coordenades seleccionat\n");
	printf("ESC per sortir de l'aplicació");
}

void selecciona(unsigned char key, int x, int y) {
    if(key=='h') {
        mostraAjuda();
    }else if(key=='x' || key=='y' || key=='z') {
        eixActual = key;
    }else if(key=='+' || key=='-') {
       	escalaObjectes(key);
    }else if(key==27) {
        exit(0);
    }
}

void initGlut(int argc, const char * argv[]) {
    glutInit(&argc, (char **)argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("IDI: Practiques OpenGL");
    glutDisplayFunc(refrescaPantalla);
    glutReshapeFunc(reubicaVP);
    glutKeyboardFunc(selecciona);
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