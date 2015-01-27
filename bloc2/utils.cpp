#include "utils.h"

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

void reubicaVP(int width, int height) {
    if(width>height) {
        glViewport((width-height)/2, 0, height, height);
    }else if(height>width){
        glViewport(0, (height-width)/2, width, width);
    }
}

/* Models */

void initGL() {
    glClearColor(0,0,0,1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1,1,-1,1,-1,1);
    glMatrixMode(GL_MODELVIEW);
}