#define STB_IMAGE_IMPLEMENTATION
#define M_PI           3.14159265358979323846
#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include "stb_image.h"

/*class Point*/
class Point {
public:
    //coordonnées x, y et z du point
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
/*const GLfloat LightPos1[4] = { 0.0f, 10.0f, 0.0f, 1.0f };
const GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
const GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
int LightPos2[4] = { 0, -10, 0 ,1 };*/

/* Prototype des fonctions */
void affichage();
void clavier(unsigned char touche, int x, int y);
void reshape(int x, int y);
void idle();
void vSpecial(int v, int x, int y); 
void claviermotion(int x, int y);
void mouse(int bouton, int etat, int x, int y);
void motion(int x, int y);
void drawRepere();
void drawCorps();
void drawPatte1();
void OurCylinder(int precis);

int main(int argc, char** argv)
{
    cam[0] = 5; cam[1] = 1; cam[2] = 5;
    /* initialisation de glut et creation
       de la fenetre */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(500, 500);
    glutCreateWindow("cube");

    /* Initialisation d'OpenGL */
    /*glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    //Lumière
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
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
    // Dessin du cube
    /*
    for (i = 0; i < 6; i++)
    {
        glBegin(GL_POLYGON);
        for (j = 0; j < 4; j++) {
            glColor3f(pCube[fCube[i][j]].r, pCube[fCube[i][j]].g, pCube[fCube[i][j]].b);
            glVertex3f(pCube[fCube[i][j]].x, pCube[fCube[i][j]].y, pCube[fCube[i][j]].z);
        }
        glEnd();
    }*/

    drawRepere();
    drawCorps();    

    glFlush();
    //glutPostRedisplay();
    //On echange les buffers
    glutSwapBuffers();
}

//Drawing

void drawCorps()
{
    glPushMatrix();
    glTranslated(0, 2, -1);
    glRotated(-45, 1, 0, 0);
    glColor3d(255, 255, 255);
    glScalef(1.0f, 1.0f, 1.5f);
    glutSolidSphere(0.5, 30, 30);
    glPopMatrix();
}

void drawPatte1()
{
    /*
    glPushMatrix();
    GLUquadric pied = generateQuadric(glu);
    gluCylinder();
    glPopMatrix();*/

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

void vSpecial(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        angley = angley - 5;
        glutPostRedisplay();
        break;

    case GLUT_KEY_DOWN:
        angley = angley + 5;
        glutPostRedisplay();
        break;

    case GLUT_KEY_LEFT:
        anglex = anglex + 5;
        glutPostRedisplay();
        break;

    case GLUT_KEY_RIGHT:
        anglex = anglex -5;
        glutPostRedisplay();

        break;
    }
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
