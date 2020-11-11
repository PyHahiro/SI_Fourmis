#define STB_IMAGE_IMPLEMENTATION
#define M_PI           3.14159265358979323846
#include <windows.h>
#include <GL/freeglut.h>
#include <cstdio>
#include <cstdlib>
#include <stdlib.h>
#include "stb_image.h"
#include <iostream>

/*class Point*/
class Point {
public:
    //coordonn�es x, y et z du point
    double x;
    double y;
    double z;
    // couleur r, v et b du point
    float r;
    float g;
    float b;
};
//Tableau pour stocker les sommets du cube et leur couleur
Point pCube[8] = {
    {-0.5,-0.5, 0.5,1.0,0.0,0.0},
    { 0.5, -0.5, 0.5,0.0,1.0,0.0},
    { 0.5, -0.5, -0.5,0.0,0.0,1.0},
    { -0.5, -0.5, -0.5,1.0,1.0,1.0},
    { -0.5,0.5, 0.5,1.0,0.0,0.0},
    { 0.5, 0.5, 0.5,0.0,1.0,0.0},
    { 0.5, 0.5, -0.5,0.0,0.0,1.0},
    { -0.5, 0.5, -0.5,1.0,1.0,1.0} };

//Tableau pour stocker les indices des sommets par face pour le cube
int fCube[6][4] = {
  {0,3,2,1},
  {0,1,5,4},
  {1,2,6,5},
  {2,3,7,6},
  {0,4,7,3},
  {4,5,6,7} };

char presseSouris,presseClavier, haut, bas, droite, gauche;
int anglex, angley, x, y, xold, yold;
double cam[3];
GLUquadricObj* pObj;
const GLfloat LightPos1[4] = { 0.0f, 10.0f, 0.0f, 1.0f };
const GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
const GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
int LightPos2[4] = { 0, -10, 0 ,1 };
int width;
int height;
unsigned char* Image;


/* Prototype des fonctions */
void affichage();
void clavier(unsigned char touche, int x, int y);
void reshape(int x, int y);
void idle();
void vSpecial(int v, int x, int y); 
void mouse(int bouton, int etat, int x, int y);
void motion(int x, int y);
void drawRepere();
void drawCorps();
void drawPatte1();
void drawPattesDevant(float x, float y, float z);
void drawPattesMilieu(float x, float y, float z);
void drawPattesArriere(float x, float y, float z);
void drawCorpsTronc();
void drawCorpsTorse();
void drawTete();
void drawCou();
void drawMandibules();
void Mandibule();
void drawOeil(float posX,float posY,float posZ);
void drawAntenne();
void mandibuleFace(float y);
void primitiveCylinder(int n, float high, float largeur);
unsigned char* loadJpegImage(const char* fichier, int* width, int* height);


int main(int argc, char** argv)
{
    cam[0] = 0; cam[1] = 1; cam[2] = -5;
    /* initialisation de glut et creation
       de la fenetre */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Fourmis");

    unsigned char* Image = loadJpegImage("Cuir.jpg", &width, &height);

    pObj = gluNewQuadric();

    /* Initialisation d'OpenGL */
    /*
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    //Lumi�re
    /*glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, LightPos1);*/

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1, 0.1, 100.0);
    gluLookAt(cam[0], cam[1], cam[2], 0, 0, 0, 0, 1, 0);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);
    glPointSize(2.0);
    glEnable(GL_DEPTH_TEST);


    /* Parametre de texture */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
        GL_RGB, GL_UNSIGNED_BYTE, Image);

    /* enregistrement des fonctions de rappel */
    glutDisplayFunc(affichage);
    glutKeyboardFunc(clavier);
    glutSpecialFunc(vSpecial);

    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    /* Entree dans la boucle principale glut */
    /* Entree dans la boucle principale glut */
    glutMainLoop();
    return 0;
}


void affichage()
{
    int i, j;
    /* effacement de l'image avec la couleur de fond */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);

    glMatrixMode(GL_MODELVIEW);
    /* Rotation */
    glLoadIdentity();
    glRotatef(angley, 1.0, 0.0, 0.0);
    glRotatef(anglex, 0.0, 1.0, 0.0); 
    glColor3d(255, 255, 255);
    drawMandibules();
    drawPattesDevant(30, 0.6, -1);
    drawPattesMilieu(90.0f, 1.09f, 0.35f);
    drawPattesArriere(150.0f, 0.8f, 1.5f);
  
    drawCorps();  

    drawCorpsTronc();

    drawCorpsTorse();

    drawTete();

    drawCou();

    drawOeil(0.25,0.85,-1.22);

    drawOeil(-0.25,0.85,-1.22);
    glColor3d(255, 255, 255);
    drawAntenne();

    drawRepere();
    glFlush();
    //glutPostRedisplay();
    //On echange les buffers
    glutSwapBuffers();
}

//Drawing
void drawCorpsTronc()
{
    glPushMatrix();
    glTranslated(0, 0.5, -0.1);
    glRotated(90, 1, 0, 0);
    glColor3f(1, 1, 1);
    glClearColor(0, 0, 0, 1);
    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    primitiveCylinder(6, 1, 0.28);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void primitiveCylinder(int n, float high, float largeur)
{
    for (int i = 0; i < n; i++)
    {
        if (i < 1)
        {
            glScalef(1.0, 1.0, 1.0);
            glBegin(GL_POLYGON);
            glTexCoord2f(0.0, 1);
            glVertex3f((largeur / 2) * cos((i * M_PI) / (n / 2)), 0, (largeur / 2) * sin((i * M_PI) / (n / 2)));
            glTexCoord2f(1.0, 1.0);
            glVertex3f((largeur / 2) * cos((i + 1) * M_PI / (n / 2)), 0, (largeur / 2) * sin((i + 1) * M_PI / (n / 2)));
            glTexCoord2f(1, 0 );
            glVertex3f((largeur / 2) * cos((i + 1) * M_PI / (n / 2)), high, (largeur / 2) * sin((i + 1) * M_PI / (n / 2)));
            glTexCoord2f(0.0, 0.0);
            glVertex3f((largeur / 2) * cos((i * M_PI) / (n / 2)), high, (largeur / 2) * sin(i * M_PI / (n / 2)));
            glEnd();
        }
        else
        {
            glScalef(1.0, 1.0, 1.0);
            glBegin(GL_POLYGON);
            glTexCoord2f((i-1)/(n-1), 1.0);
            glVertex3f((largeur / 2) * cos((i * M_PI) / (n / 2)), 0, (largeur / 2) * sin((i * M_PI) / (n / 2)));
            glTexCoord2f((i) / (n - 1), 1.0);
            glVertex3f((largeur / 2) * cos((i + 1) * M_PI / (n / 2)), 0, (largeur / 2) * sin((i + 1) * M_PI / (n / 2)));
            glTexCoord2f((i) / (n - 1), 0.0);
            glVertex3f((largeur / 2) * cos((i + 1) * M_PI / (n / 2)), high, (largeur / 2) * sin((i + 1) * M_PI / (n / 2)));
            glTexCoord2f((i - 1) / (n - 1), 0.0);
            glVertex3f((largeur / 2) * cos((i * M_PI) / (n / 2)), high, (largeur / 2) * sin(i * M_PI / (n / 2)));
            glEnd();
        }
    }
}
void drawMandibules()
{
    glPushMatrix();
    glColor3f(165, 42, 42);
    glTranslated(0.1, 0.65, -1.4);
    glScalef(0.2, 0.2, 0.4);
    glRotatef(180, 0, 0, 1);
    Mandibule();
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(-0.1, 0.65, -1.4);
    glScalef(0.2, 0.2, 0.4);
    Mandibule();
    glPopMatrix();
    glColor3d(255, 255, 255);
}
void mandibuleFace(float y)
{
    int n = 40;
    /*
   * PARTIE INFERIEURE
   */
    glBegin(GL_POLYGON);
    /* Main*/
    glVertex3f(-0.4f, y, 0.0f);
    glVertex3f(-0.3f, y, 0.4f); //bas
    glVertex3f(-0.1f, y, 0.4f); //bas
    glVertex3f(-0.05f, y, 0.0f); //bas
    /* Doigts*/
    glVertex3f(0.05f, y, -0.05f);
    glVertex3f(0.0f, y, -0.15f);
    glVertex3f(0.1f, y, -0.2f);
    glVertex3f(0.05f, y, -0.3f);
    glVertex3f(0.15f, y, -0.35f);
    glVertex3f(0.15f, y, -0.4f);
    /* Dos de la main */
    for (int i = n / 4; i > 0; i--)
    {
        float angle = 2 * M_PI * i / n;
        glVertex3f(-cos(angle) * 0.4, y, -sin(angle) * 0.4);
    }
    glEnd();
}
void Mandibule()
{
    int n = 40;
    mandibuleFace(0.0);
    mandibuleFace(0.2);
    /*
    * PARTIE CENTRALE
    */
    glBegin(GL_QUADS);
    glVertex3f(-0.4f, 0.0f, 0.0f);
    glVertex3f(-0.4f, 0.2f, 0.0f);
    glVertex3f(-0.3f, 0.2f, 0.4f);
    glVertex3f(-0.3f, 0.0f, 0.4f); //bas
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(-0.1f, 0.0f, 0.4f); //bas
    glVertex3f(-0.1f, 0.2f, 0.4f);
    glVertex3f(-0.05f, 0.2f, 0.0f);
    glVertex3f(-0.05f, 0.0f, 0.0f); //bas
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(-0.3f, 0.0f, 0.4f); //bas
    glVertex3f(-0.3f, 0.2f, 0.4f);
    glVertex3f(-0.1f, 0.2f, 0.4f);
    glVertex3f(-0.1f, 0.0f, 0.4f); //bas
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(-0.05f, 0.0f, 0.0f);
    glVertex3f(-0.05f, 0.2f, 0.0f);
    glVertex3f(0.05f, 0.2f, -0.05f);
    glVertex3f(0.05f, 0.0f, -0.05f);
    glEnd();
    /* Doigts */
    glBegin(GL_QUADS);
    glVertex3f(0.05f, 0.0f, -0.05f);
    glVertex3f(0.05f, 0.2f, -0.05f);
    glVertex3f(0.0f, 0.2f, -0.15f);
    glVertex3f(0.0f, 0.0f, -0.15f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(0.0f, 0.0f, -0.15f);
    glVertex3f(0.0f, 0.2f, -0.15f);
    glVertex3f(0.1f, 0.2f, -0.2f);
    glVertex3f(0.1f, 0.0f, -0.2f);
    glEnd();

    glBegin(GL_QUADS);

    glVertex3f(0.1f, 0.0f, -0.2f);
    glVertex3f(0.1f, 0.2f, -0.2f);
    glVertex3f(0.05f, 0.2f, -0.3f);
    glVertex3f(0.05f, 0.0f, -0.3f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(0.05f, 0.0f, -0.3f);
    glVertex3f(0.05f, 0.2f, -0.3f);
    glVertex3f(0.15f, 0.2f, -0.35f);
    glVertex3f(0.15f, 0.0f, -0.35f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(0.15f, 0.0f, -0.35f);
    glVertex3f(0.15f, 0.2f, -0.35f);
    glVertex3f(0.15f, 0.2f, -0.4f);
    glVertex3f(0.15f, 0.0f, -0.4f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(0.15f, 0.0f, -0.4f);
    glVertex3f(0.15f, 0.2f, -0.4f);
    glVertex3f(-cos(2 * M_PI * 10 / n) * 0.4, 0.2, -sin(2 * M_PI * 10 / n) * 0.4);
    glVertex3f(-cos(2 * M_PI * 10 / n) * 0.4, 0.0, -sin(2 * M_PI * 10 / n) * 0.4);
    glEnd();

    glBegin(GL_POLYGON);
    for (int i = n / 4; i > 0; i--)
    {
        float angle = 2 * M_PI * i / n;
        float angle2 = 2 * M_PI * (i-1) / n;
        glBegin(GL_QUADS);
        glVertex3f(-cos(angle) * 0.4, 0.0, -sin(angle) * 0.4);
        glVertex3f(-cos(angle) * 0.4, 0.2, -sin(angle) * 0.4);
        glVertex3f(-cos(angle2) * 0.4, 0.2, -sin(angle2) * 0.4);
        glVertex3f(-cos(angle2) * 0.4, 0.0, -sin(angle2) * 0.4);

    }glEnd();
}

void drawCorps()
{
    glPushMatrix();
    glTranslated(0, 0.5, 1);
    glRotated(-15, 1, 0, 0);
    glColor3d(255, 0, 255);
    glScalef(1.0f, 1.0f, 1.75f);
    glutSolidSphere(0.35, 30, 30);
    glPopMatrix();
}

void drawCorpsTorse()
{
    glPushMatrix();
    glTranslated(0, 0.5, -0.25);
    glRotated(0, 0, 0, 0);
    glColor3d(255, 255, 0);
    glScalef(0.65f, 0.65f, 1.15f);
    glutSolidSphere(0.35, 30, 30);
    glPopMatrix();
}

void drawTete()
{
    glPushMatrix();
    glTranslated(0, 0.65, -1);
    glRotated(0, 0, 0, 0);
    glColor3d(0, 25, 0);
    glScalef(1.0f, 0.65f, 1.0f);
    glutSolidSphere(0.45, 30, 30);
    glPopMatrix();
}

void drawOeil(float posX,float posY,float posZ)
{
    glPushMatrix();
    glTranslated(posX, posY, posZ);
    glRotated(0, 0, 0, 0);
    glColor3d(0, 0, 0);
    glutSolidSphere(0.05, 30, 30);
    glPopMatrix();
}

void drawCou()
{
    glPushMatrix();
    glTranslated(0, 0.65, -1.15);
    glRotated(0, 0, 0, 0);
    glColor3d(0, 0, 1);
    gluCylinder(pObj, 0.05, 0.05, 0.75, 30, 30);
    glPopMatrix();
}

void drawAntenne()
{
    /*
    Antenne gauche
    */

    glPushMatrix();

    glTranslated(-0.5, 1.1, -1.1);
    glPushMatrix();
    glRotated(120, 0, 1, 0);
    glRotated(30, 1, 0, 0);
    gluCylinder(pObj, 0.02, 0.02, 0.5,30,30);
    glPopMatrix();

    glPushMatrix();
    glRotated(170, 0, 1, 0);
    glScaled(0.75, 0.75, 0.75);
    gluCylinder(pObj, 0.02, 0.02, 0.8, 30, 30);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.095, 0.005, -0.56);
    glRotated(-15, 1, 0, 0);
    glRotated(170, 0, 1, 0);
    glScaled(0.5, 0.5, 0.5);
    gluCylinder(pObj, 0.02, 0.02, 0.8, 30, 30);
    glPopMatrix();

    glPopMatrix();

    /*
    Antenne droite
    */
    glPushMatrix();

    glTranslated(0.5, 1.1, -1.1);
    glPushMatrix();
    glRotated(-120, 0, 1, 0);
    glRotated(30, 1, 0, 0);
    gluCylinder(pObj, 0.02, 0.02, 0.5, 30, 30);
    glPopMatrix();

    glPushMatrix();
    glRotated(-170, 0, 1, 0);
    glScaled(0.75, 0.75, 0.75);
    gluCylinder(pObj, 0.02, 0.02, 0.8, 30, 30);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.1,0.005,-0.53);
    glRotated(-5, 1, 0, 0);
    glRotated(-170, 0, 1, 0);
    glScaled(0.5, 0.5, 0.5);
    gluCylinder(pObj, 0.02, 0.02, 0.8, 30, 30);
    glPopMatrix();

    glPopMatrix();
}


void drawRepere()
{
    //axe x en rouge
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0, 0, 0.0);
    glVertex3f(1, 0, 0.0);
    glEnd();
    //axe des y en vert
    glBegin(GL_LINES);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0, 0, 0.0);
    glVertex3f(0, 1, 0.0);
    glEnd();
    //axe des z en bleu
    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0, 0, 0.0);
    glVertex3f(0, 0, 1.0);
    glEnd();
}

void drawPattesDevant(float angle, float ecart, float z)
{
    /*
   Patte devant gauche
   */
    float x = angle; //Angle des pattes
    float e = ecart; //Ecartement des pattes
    float depZ = z; //Deplacer en z les pates

    glPushMatrix();
    glTranslated(-e, 0, depZ);
    //Pied1
    glPushMatrix();
    glRotated(x, 0, 1, 0);
    glTranslated(0, 0.03, 0.11);
    glRotated(-38, 1, 0, 0);

    glScalef(0.3f, 0.1f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.15, 35, 1);
    glPopMatrix();

    //Main1
    glPushMatrix();
    glRotated(x, 0, 1, 0);
    glTranslatef(0, 0.12, 0.23);
    glRotated(-55, 1, 0, 0);

    glScalef(0.3f, 0.1f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.4, 35, 1);
    glPopMatrix();

    //Tibia1
    glPushMatrix();
    glRotated(x, 0, 1, 0);
    glTranslatef(0, 0.45, 0.46);
    glRotated(-60, 1, 0, 0);

    glScalef(0.3f, 0.2f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.4, 35, 1);
    glPopMatrix();


    //Cuisse1
    glPushMatrix();
    glRotated(x, 0, 1, 0);
    glTranslatef(0, 0.8, 0.65);
    glRotated(60, 1, 0, 0);

    glScalef(0.3f, 0.2f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.3, 35, 1);
    glPopMatrix();

    glPopMatrix();

    /*
    Patte milieu droite
    */


    glPushMatrix();
    glTranslated(e, 0, depZ);

    //Pied2
    glPushMatrix();
    glRotated(-x, 0, 1, 0);
    glTranslated(0, 0.03, 0.11);
    glRotated(-38, 1, 0, 0);

    glScalef(0.3f, 0.1f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.15, 35, 1);
    glPopMatrix();

    //Main2
    glPushMatrix();
    glRotated(-x, 0, 1, 0);
    glTranslatef(0, 0.12, 0.23);
    glRotated(-55, 1, 0, 0);

    glScalef(0.3f, 0.1f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.4, 35, 1);
    glPopMatrix();

    //Tibia2
    glPushMatrix();
    glRotated(-x, 0, 1, 0);
    glTranslatef(0, 0.45, 0.46);
    glRotated(-60, 1, 0, 0);

    glScalef(0.3f, 0.2f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.4, 35, 1);
    glPopMatrix();


    //Cuisse2
    glPushMatrix();
    glRotated(-x, 0, 1, 0);
    glTranslatef(0, 0.8, 0.65);
    glRotated(60, 1, 0, 0);

    glScalef(0.3f, 0.2f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.3, 35, 1);
    glPopMatrix();
    glPopMatrix();
}
void drawPattesMilieu(float angle, float ecart, float z)
{
    /*
    Patte milieu gauche
    */
    float x = angle; //Angle des pattes
    float e = ecart; //Ecartement des pattes
    float depZ = z; //Deplacer en z les pates

    glPushMatrix();
    glTranslated(-e, 0, depZ);
    //Pied1
    glPushMatrix();
    glRotated(x, 0, 1, 0);
    glTranslated(0, 0.03, 0.11);
    glRotated(-38, 1, 0, 0);

    glScalef(0.3f, 0.1f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.15, 35, 1);
    glPopMatrix();

    //Main1
    glPushMatrix();
    glRotated(x, 0, 1, 0);
    glTranslatef(0, 0.12, 0.23);
    glRotated(-55, 1, 0, 0);

    glScalef(0.3f, 0.1f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.4, 35, 1);
    glPopMatrix();

    //Tibia1
    glPushMatrix();
    glRotated(x, 0, 1, 0);
    glTranslatef(0, 0.45, 0.46);
    glRotated(-60, 1, 0, 0);

    glScalef(0.3f, 0.2f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.4, 35, 1);
    glPopMatrix();


    //Cuisse1
    glPushMatrix();
    glRotated(x, 0, 1, 0);
    glTranslatef(0, 0.78, 0.65);
    glRotated(30, 1, 0, 0);

    glScalef(0.3f, 0.2f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.4, 35, 1);
    glPopMatrix();

    glPopMatrix();


    /*
    Patte milieu droite
    */


    glPushMatrix();
    glTranslated(e, 0, depZ);

    //Pied2
    glPushMatrix();
    glRotated(-x, 0, 1, 0);
    glTranslated(0, 0.02, 0.11);
    glRotated(-40, 1, 0, 0);

    glScalef(0.3f, 0.1f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.15, 35, 1);
    glPopMatrix();

    //Main2
    glPushMatrix();
    glRotated(-x, 0, 1, 0);
    glTranslatef(0, 0.12, 0.23);
    glRotated(-55, 1, 0, 0);

    glScalef(0.3f, 0.1f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.4, 35, 1);
    glPopMatrix();

    //Tibia2
    glPushMatrix();
    glRotated(-x, 0, 1, 0);
    glTranslatef(0, 0.45, 0.46);
    glRotated(-60, 1, 0, 0);

    glScalef(0.3f, 0.2f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.4, 35, 1);
    glPopMatrix();


    //Cuisse2
    glPushMatrix();
    glRotated(-x, 0, 1, 0);
    glTranslatef(0, 0.78, 0.65);
    glRotated(30, 1, 0, 0);

    glScalef(0.3f, 0.2f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.4, 35, 1);
    glPopMatrix();

    glPopMatrix();
}
void drawPattesArriere(float angle, float ecart, float z)
{
    /*
    Patte arriere gauche
    */
    float x = angle; //Angle des pattes
    float e = ecart; //Ecartement des pattes
    float depZ = z; //Deplacer en z les pates

    glPushMatrix();
    glTranslated(-e, 0, depZ);
    //Pied1
    glPushMatrix();
    glRotated(x, 0, 1, 0);
    glRotated(-30, 1, 0, 0);

    glScalef(0.3f, 0.1f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.2, 35, 1);
    glPopMatrix();

    //Main1
    glPushMatrix();
    glRotated(x, 0, 1, 0);
    glTranslatef(0, 0.08, 0.15);
    glRotated(-50, 1, 0, 0);

    glScalef(0.3f, 0.1f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.5, 35, 1);
    glPopMatrix();

    //Tibia1
    glPushMatrix();
    glRotated(x, 0, 1, 0);
    glTranslatef(0, 0.45, 0.46);
    glRotated(-60, 1, 0, 0);

    glScalef(0.3f, 0.2f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.5, 35, 1);
    glPopMatrix();


    //Cuisse1
    glPushMatrix();
    glRotated(x, 0, 1, 0);
    glTranslatef(0, 0.88, 0.7);
    glRotated(40, 1, 0, 0);

    glScalef(0.3f, 0.2f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.5, 35, 1);
    glPopMatrix();

    glPopMatrix();

    /*
    Pied arriere droite 
    */
    glPushMatrix();
    glTranslated(e,0,depZ);

    //Pied2
    glPushMatrix();
    glRotated(-x, 0, 1, 0);
    glRotated(-30, 1, 0, 0);

    glScalef(0.3f, 0.1f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.18, 35, 1);
    glPopMatrix();

    //Main1
    glPushMatrix();
    glRotated(-x, 0, 1, 0);
    glTranslatef(0, 0.08, 0.15);
    glRotated(-50, 1, 0, 0);

    glScalef(0.3f, 0.1f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.5, 35, 1);
    glPopMatrix();

    //Tibia2
    glPushMatrix();
    glRotated(-x, 0, 1, 0);
    glTranslatef(0, 0.45, 0.46);
    glRotated(-60, 1, 0, 0);

    glScalef(0.3f, 0.2f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.5, 35, 1);
    glPopMatrix();

    //Cuisse2
    glPushMatrix();
    glRotated(-x, 0, 1, 0);
    glTranslatef(0, 0.88, 0.7);
    glRotated(40, 1, 0, 0);

    glScalef(0.3f, 0.2f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.5, 35, 1);
    glPopMatrix();
    //
    glPopMatrix();
}


//KEYS
void vSpecial(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_UP:
        {
            angley = (angley - 5) % 360;
            break;
        }

        case GLUT_KEY_DOWN:
        {
            angley = (angley + 5) % 360;
            break;
        }

        case GLUT_KEY_LEFT:
        {
            anglex = (anglex + 5) % 360;
            break;
        }

        case GLUT_KEY_RIGHT:
        {
            anglex = (anglex - 5) %360;
            break;
        }
        default: break;
    }
    glutPostRedisplay();
}


void clavier(unsigned char touche, int x, int y)
{
    switch (touche)
    {
    case 'p': /* affichage du carre plein */
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glutPostRedisplay();
        break;
    case 'f': /* affichage en mode fil de fer */
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glutPostRedisplay();
        break;
    case 's': /* Affichage en mode sommets seuls */
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        glutPostRedisplay();
        break;
    case 'd':
        glEnable(GL_DEPTH_TEST);
        glutPostRedisplay();
        break;
    case 'D':
        glDisable(GL_DEPTH_TEST);
        glutPostRedisplay();
        break;
    case 'z':
        cam[0] = cam[0]-0.1;
        cam[2] = cam[2]-0.1;
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0, 1, 0.1, 100.0);
        gluLookAt(cam[0], cam[1], cam[2], 0, 0, 0, 0, 1, 0);
        glutPostRedisplay();
        break;
    case 'Z':
        cam[0] = cam[0]+0.1;
        cam[2] = cam[2]+0.1;
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0, 1, 0.1, 100.0);
        gluLookAt(cam[0], cam[1], cam[2], 0, 0, 0, 0, 1, 0);
        glutPostRedisplay();
        break;
        break;
    case 'q': /*la touche 'q' permet de quitter le programme */
        exit(0);

    }
}

void reshape(int x, int y)
{
    if (x < y)
        glViewport(0, (y - x) / 2, x, x);
    else
        glViewport((x - y) / 2, 0, y, y);
}

void mouse(int button, int state, int x, int y)
{
    /* si on appuie sur le bouton gauche */
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        presseSouris = 1; /* le booleen presse passe a 1 (vrai) */
        xold = x; /* on sauvegarde la position de la souris */
        yold = y;
    }
    /* si on relache le bouton gauche */
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        presseSouris = 0; /* le booleen presse passe a 0 (faux) */
}

void motion(int x, int y)
{
    if (presseSouris) /* si le bouton gauche est presse */
    {
        /* on modifie les angles de rotation de l'objet
       en fonction de la position actuelle de la souris et de la derniere
       position sauvegardee */
        anglex = anglex + (x - xold);
        angley = angley + (y - yold);
        glutPostRedisplay(); /* on demande un rafraichissement de l'affichage */
    }

    xold = x; /* sauvegarde des valeurs courante de le position de la souris */
    yold = y;
}

unsigned char* loadJpegImage(const char* fichier, int* width, int* height)
{
    int bpp;
    unsigned char* image = stbi_load(fichier, width, height, &bpp, 3);
    if (image == nullptr)
    {
        std::cout << "Erreur, impossible de charger l'image " << fichier << std::endl;
    }
    else
    {
        std::cout << "Texture chargee: " << fichier << std::endl;
    }

    return image;
}
