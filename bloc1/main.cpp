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

int nVertexsIntroduits = 0;
int estatFons = 0;

typedef struct {
    float x;
    float y;
    float z;
} Vertex;

typedef struct {
    Vertex v[3];
} Triangle;

Triangle triangle;

void setTriangleInicial() {
    Vertex v1 = {(float)-1/2, (float)-1/3, (float)0.9};
    Vertex v2 = {(float)1/2, (float)-1/3, (float)0.9};
    Vertex v3 = {(float)0, (float)2/3, (float)0.9};
    triangle.v[0] = v1;
    triangle.v[1] = v2;
    triangle.v[2] = v3;
}

void refrescaPantalla(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    int i;
    for (i = 0; i < 3; ++i) {
        float x = triangle.v[i].x;
        float y = triangle.v[i].y;
        float z = triangle.v[i].z;

        int m = i%3;
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
    glutSwapBuffers();
}

void reubicaVP(int width, int height) {
    if(width>height) {
        glViewport((width-height)/2, 0, height, height);
    }else if(height>width){
        glViewport(0, (height-width)/2, width, width);
    }
}

void cambiaColorFons(int x, int y) {
    float newX = (float)x;
    float newY = (float)y;

    float w = (float)glutGet(GLUT_WINDOW_WIDTH);
    float h = (float)glutGet(GLUT_WINDOW_HEIGHT);

    glClearColor( newX/w, newY/h, (float)1, (float)1);
    glutPostRedisplay();
}

void introdueixVertexs(int button, int state, int x, int y) {
    if(button == GLUT_LEFT_BUTTON) {
        if(state == GLUT_UP) {
            float w = (float)glutGet(GLUT_WINDOW_WIDTH);
            float h = (float)glutGet(GLUT_WINDOW_HEIGHT);

            float newX = (float)x;
            float newY = (float)y;

            if(w>h) {
                if((newX < (w-h)/2) || (newX > w-((w-h)/2))) {
                    return;
                }else{
                    newX = newX - (w-h)/2;
                    w = w - (w-h);
                }
            }else if(h>w) {
                if((newY < (h-w)/2) || (newY > h-((h-w)/2))) {
                    return;
                }else{
                    newY = newY - (h-w)/2;
                    h = h - (h-w);
                }
            }

            float posX = newX/w;
            float posY = newY/h;

            posX = posX - (1-posX);
            posY = -(posY - (1-posY));

            Vertex nouVertex = {posX, posY, 0.00};
            triangle.v[nVertexsIntroduits] = nouVertex;
            ++nVertexsIntroduits;

            if(nVertexsIntroduits==3) {
                glutMouseFunc(NULL);
                glutPostRedisplay();
            }
        }
    }
}

void mostraAjuda(void) {
    printf("\n-+-+-+-+-+-+-+-[AJUDA]-+-+-+-+-+-+-+-\n"BOLDBLACK"ESC"RESET":\tsurt del programa.\n"BOLDBLACK"H"RESET":\tmostra l'ajuda del programa.");
    printf("\n"BOLDBLACK"T"RESET":\tre-defineix la posició dels vèrtexs del triangle, mitjançant clics amb el botó esquerra del mouse.");
    printf("\n"BOLDBLACK"F"RESET":\tactiva/desactiva el canvi de color a partir de l'arrossegament amb el botó esquerra del mouse.\n");
    printf("-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
}

void selecciona(unsigned char key, int x, int y) {
    if(key=='h') {
        mostraAjuda();
    }else if(key=='f') {
        if(estatFons==0) {
            estatFons = 1;
            glutMotionFunc(cambiaColorFons);
            glutMouseFunc(NULL);
        }else{
            glutMotionFunc(NULL);
            estatFons = 0;
        }
    }else if(key=='t') {
        nVertexsIntroduits = 0;
        glutMouseFunc(introdueixVertexs);
        glutMotionFunc(NULL);
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
    glutKeyboardFunc(selecciona);
    glutReshapeFunc(reubicaVP);
    glutMainLoop();
}

void initGL() {
    glClearColor(0,0,0,1);
}

int main(int argc, const char * argv[]) {
    initGL();
    setTriangleInicial();
    initGlut(argc, argv);
    return 0;
}