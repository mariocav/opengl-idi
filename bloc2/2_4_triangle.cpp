#include <stdio.h>
#include "utils.h"

typedef struct {
    float x;
    float y;
    float z;
} Vertex;

typedef struct {
    Vertex v[3];
} Triangle;

Triangle triangles[2];

void setTrianglesInicials() {
    Vertex v1 = {(float)-0.5, (float)0, (float)0.0};
    Vertex v2 = {(float)0.5, (float)0, (float)0.0};
    Vertex v3 = {(float)0, (float)1, (float)0.0};
    triangles[0].v[0] = triangles[1].v[0] = v1;
    triangles[0].v[1] = triangles[1].v[1] = v2;
    triangles[0].v[2] = triangles[1].v[2] = v3;
}

float rotacioActual = 25.00;

void dibuixaTriangles() {
    int i,j;
    for (i = 0; i < 2; ++i) {
        glPushMatrix();
        //glLoadIdentity();
        //printf("rad: %f\n",rotacioActual);
        if(i==0) {
            glTranslatef(-0.5, 0, 0);
            //printf("roac: %f\n",rotacioActual);
            glRotatef(rotacioActual, 0, 1, 0); // roto
        }else{
            glTranslatef(0.5, 0, 0);
            //printf("roar: %f\n",-rotacioActual);
            glRotatef(180-rotacioActual, 0, 1, 0); // roto
        }

        glTranslatef( // centro en origen de coordenadas
            -(triangles[i].v[0].x+triangles[i].v[1].x+triangles[i].v[2].x)/3,
            -(triangles[i].v[0].y+triangles[i].v[1].y+triangles[i].v[2].y)/3,
            -(triangles[i].v[0].z+triangles[i].v[1].z+triangles[i].v[2].z)/3
        );



        glBegin(GL_TRIANGLES);
        for (j = 0; j < 3; ++j) {
            float x = triangles[i].v[j].x;
            float y = triangles[i].v[j].y;
            float z = triangles[i].v[j].z;

            int m = j%3;
            if(m==0) {
                glColor3f(1,0,0);
            }else if(m==1) {
                glColor3f(0,1,0);
            }else{
                glColor3f(0,0,1);
            }
            glVertex3f(x, y, z);
        }
        glEnd();
        glPopMatrix();
    }

}

int xActual = 0;

void rotaTriangles(int x, int y) {

    float w = (float)glutGet(GLUT_WINDOW_WIDTH);

    float xDiferencia = x-xActual;

    float xAbsolutaDiferencia = abs(xDiferencia);

    if(xDiferencia<0 && xAbsolutaDiferencia>5) {
        rotacioActual -= w/360*2;
    }else if(xAbsolutaDiferencia>5){
        rotacioActual += w/360*2;
    }

    //printf("ra: %f\n",rotacioActual);

    xActual = x;

    glutPostRedisplay();
}

void helper(int button, int state, int x, int y) {
    if(button == GLUT_LEFT_BUTTON) {
        xActual = x;
    }
}

void refrescaPantalla(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    dibuixaEixos();
    dibuixaTriangles();
    glutSwapBuffers();
}

void initGlut(int argc, const char * argv[]) {
    glutInit(&argc, (char **)argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("IDI: Practiques OpenGL");
    glutDisplayFunc(refrescaPantalla);
    glutReshapeFunc(reubicaVP);
    glutMotionFunc(rotaTriangles);
    glutMouseFunc(helper);
    initGL();
    glutMainLoop();
}

int main(int argc, const char * argv[]) {
    setTrianglesInicials();
    initGlut(argc, argv);
}