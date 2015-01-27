void pinta_terra() {
    glBegin(GL_QUADS);
    glVertex3f(-5,0,-5);
    glVertex3f(-5,0,5);
    glVertex3f(5,0,5);
    glVertex3f(5,0,-5);
}

void pinta_escena() {
    pinta_terra();
    glPushMatrix();
    glRotate(90,1,0,0);
    glTranslatef(-(xmin+xmax)/2, (-(ymin+ymax)/2), (-(zmin+zmax)/2));
    // capsa centrada l'origen de coordenades i amb el nas mirant a l'eix de les Y

    glPushMatrix();
    float alcada_actual = (ymax-ymin);
    float escala = 1/alcada_actual;
    glEscalef(0,0,escala) // com hem girat la imatge, l'alçada correspon a l'eix de les Z
    ymax = ymax*escala;
    ymin = ymin*escala;
    glTranslatef(-2.5, 0, -ymin+2.5);
    pinta_model();
    glPopMatrix();

    glPushMatrix();
    float alcada_actual = (ymax-ymin);
    float escala = 2/alcada_actual;
    glEscalef(0,0,escala) // com hem girat la imatge, l'alçada correspon a l'eix de les Z
    ymax = ymax*escala;
    ymin = ymin*escala;
    glTranslatef(2.5, 0, -ymin-2.5);
    pinta_model();
    glPopMatrix();

    glPopMatrix();
}