// Trabalho de Computação Gráfica - BF3
// Cena 3D com OpenGL
#include <GLUT/glut.h>
#include <cmath>
#include <cstdlib>

// camera
float camX = 0.0f, camY = 2.0f, camZ = 8.0f;
float camYaw = 0.0f;

// bola
bool animando = false;
float bolaX = -2.0f, bolaZ = 0.0f, bolaRot = 0.0f;
float angulo = 0.0f;

// texturas
GLuint texturaChao, texturaCubo;

void gerarTexturaChao() {
    int tam = 64;
    unsigned char* data = new unsigned char[tam * tam * 3];
    for (int y = 0; y < tam; y++) {
        for (int x = 0; x < tam; x++) {
            int i = (y * tam + x) * 3;
            bool escuro = ((x / 8) % 2) == ((y / 8) % 2);
            data[i] = 0;
            data[i + 1] = escuro ? 60 : 100;
            data[i + 2] = 0;
        }
    }

    glGenTextures(1, &texturaChao);
    glBindTexture(GL_TEXTURE_2D, texturaChao);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tam, tam, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    delete[] data;
}

void gerarTexturaCubo() {
    int size = 64;
    unsigned char* pixels = new unsigned char[size * size * 3];
    for(int y = 0; y < size; y++) {
        for(int x = 0; x < size; x++) {
            int idx = (y * size + x) * 3;
            bool white = ((x/8) % 2) == ((y/8) % 2);
            pixels[idx] = white ? 255 : 30;
            pixels[idx+1] = white ? 255 : 30;
            pixels[idx+2] = white ? 255 : 30;
        }
    }

    glGenTextures(1, &texturaCubo);
    glBindTexture(GL_TEXTURE_2D, texturaCubo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    delete[] pixels;
}

void init() {
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);

    float amb[] = {0.3f, 0.3f, 0.3f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);

    float pos[] = {5.0f, 10.0f, 5.0f, 0.0f};
    float dif[] = {1.0f, 1.0f, 0.9f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);

    gerarTexturaChao();
    gerarTexturaCubo();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);
}

void desenharChao() {
    glBindTexture(GL_TEXTURE_2D, texturaChao);
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);   glVertex3f(-10, 0, -10);
        glTexCoord2f(10, 0);  glVertex3f( 10, 0, -10);
        glTexCoord2f(10, 10); glVertex3f( 10, 0,  10);
        glTexCoord2f(0, 10);  glVertex3f(-10, 0,  10);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void desenharCubo() {
    glBindTexture(GL_TEXTURE_2D, texturaCubo);
    glColor3f(1, 1, 1);
    glPushMatrix();
        glTranslatef(2, 0.5, -1);
        glutSolidCube(1.0f);
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void desenharPiramide() {
    glColor3f(0.9f, 0.7f, 0.2f);
    glPushMatrix();
    glTranslatef(-2, 0, 1);
    glBegin(GL_TRIANGLES);
        glVertex3f(0, 1, 0); glVertex3f(-0.5, 0, 0.5); glVertex3f(0.5, 0, 0.5);
        glVertex3f(0, 1, 0); glVertex3f(0.5, 0, 0.5); glVertex3f(0.5, 0, -0.5);
        glVertex3f(0, 1, 0); glVertex3f(0.5, 0, -0.5); glVertex3f(-0.5, 0, -0.5);
        glVertex3f(0, 1, 0); glVertex3f(-0.5, 0, -0.5); glVertex3f(-0.5, 0, 0.5);
    glEnd();
    glPopMatrix();
}

void desenharBola() {
    glColor3f(1.0f, 0.2f, 0.2f);
    glPushMatrix();
        glTranslatef(bolaX, 0.5f, bolaZ);
        glRotatef(bolaRot, 0, 1, 0);
        glutSolidSphere(0.5, 20, 20);
    glPopMatrix();
}

// sistema solar simplificado
void desenharSistemaSolar() {
    glPushMatrix();
        glTranslatef(0, 1.5, -3);

        // sol
        glColor3f(1.0, 0.9, 0.2);
        glutSolidSphere(0.4, 20, 20);

        // planeta 1 orbitando
        glRotatef(angulo, 0, 1, 0);
        glTranslatef(1.5, 0, 0);
        glColor3f(0.2, 0.5, 1.0);
        glutSolidSphere(0.15, 15, 15);

        // lua do planeta
        glRotatef(angulo * 3, 0, 1, 0);
        glTranslatef(0.3, 0, 0);
        glColor3f(0.8, 0.8, 0.8);
        glutSolidSphere(0.05, 10, 10);
    glPopMatrix();

    // planeta 2
    glPushMatrix();
        glTranslatef(0, 1.5, -3);
        glRotatef(-angulo * 0.6, 0, 1, 0);
        glTranslatef(2.3, 0, 0);
        glColor3f(1.0, 0.3, 0.2);
        glutSolidSphere(0.2, 15, 15);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float lx = sin(camYaw * M_PI / 180.0f);
    float lz = -cos(camYaw * M_PI / 180.0f);
    gluLookAt(camX, camY, camZ,
              camX + lx, camY, camZ + lz,
              0, 1, 0);

    // luz colorida orbitando
    float luzX = 3.0f * cos(angulo * M_PI / 180.0f);
    float luzZ = 3.0f * sin(angulo * M_PI / 180.0f);
    float posLuz[] = {luzX, 3.0f, luzZ, 1.0f};
    float corLuz[] = {0.5f, 0.2f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT1, GL_POSITION, posLuz);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, corLuz);

    desenharChao();
    desenharCubo();
    desenharPiramide();
    desenharBola();
    desenharSistemaSolar();

    glutSwapBuffers();
}

void teclado(unsigned char key, int x, int y) {
    float vel = 0.3f;

    switch (key) {
        case 'w': camX += sin(camYaw * M_PI / 180.0f) * vel;
                  camZ -= cos(camYaw * M_PI / 180.0f) * vel; break;
        case 's': camX -= sin(camYaw * M_PI / 180.0f) * vel;
                  camZ += cos(camYaw * M_PI / 180.0f) * vel; break;
        case 'a': camYaw -= 3.0f; break;
        case 'd': camYaw += 3.0f; break;
        case 'q': camY += vel; break;
        case 'e': camY -= vel; break;
        case ' ': animando = !animando; break;
        case 27: exit(0);
    }
    glutPostRedisplay();
}

void timer(int v) {
    if (animando) {
        bolaZ += 0.05f;
        bolaRot += 10.0f;
        if (bolaZ > 3.0f) {
            bolaZ = -3.0f;
            bolaRot = 0;
        }
    }
    angulo += 1.0f;
    if(angulo > 360) angulo = 0;

    glutTimerFunc(16, timer, 0);
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Trabalho CG - Cena 3D");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(teclado);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}
