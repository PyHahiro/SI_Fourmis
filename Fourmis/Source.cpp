
/*
* Pour plus d'informations, vous devriez avoir en joint le rapport de projet en version PDF.
* pour tout soucis ou spécification qui n'apparaissent pas dans les commentaires ou dans le rapport
* merci de nous recontacter à l'adresse mail suivante Jonathan.martinmaestre71@gmail.com
*
* code écrit par :
* JOULAIN Matthieu &
* MARTIN MAESTRE Jonathan
*/
#define STB_IMAGE_IMPLEMENTATION
#define M_PI           3.14159265358979323846
#include <windows.h>
#include <GL/freeglut.h>
#include <cstdio>
#include <cstdlib>
#include <stdlib.h>
#include "stb_image.h"
#include <iostream>
/*
* Déclaration des nombreuses variables utiles
* 
* Variables pour gérer les entrés claviers et souris, modifier la caméra en fonction
*/

char presseSouris,presseClavier, haut, bas, droite, gauche; 
int anglex, angley, x, y, xold, yold;

/*
* Variables utile à l'animation (optimisation possible)
*/
float mandibuleANIM = 10, Avant_xG = 10, Avant_xD = 80, Milieu_xG = 0, Milieu_xD = 0, Arriere_xG = 70, Arriere_xD = 0, Antenne = 0, Fessier = 0,DG = 0;
boolean DGB = true ,Fesse = true,Antennes = true,Avant = false,Milieu = false,Arriere = false,trigo = true,pattes = false;

/*
 * Variables caméra & quadratic suivi des variables d'environnements pour la lumière
 */
double cam[3];
GLUquadricObj* pObj;
const GLfloat LightPos1[4] = { 0.0f, 10.0f, 0.0f, 1.0f };
const GLfloat light_ambient[] = { 0.5f, 0.2f, 0.2f, 1.0f };
const GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 0.5f, 1.0f, 1.0f, 1.0f };
const GLfloat light_direction[] = { 0.0f, 0.0f, 0.0f};
const GLfloat lightPos2[4] = { 0.0f, -10.0f, 0.0f ,1.0f };
const GLfloat light_diffuse2[] = { 0.0f , 0.5f, 0.0f, 1.0 };

/*
* Variables Textures & images
*/

GLuint textureIds[2];
int width[2];
int height[2];

unsigned char* Image;
unsigned char* Image2;


/* Prototype des fonctions */
void affichage();
void clavier(unsigned char touche, int x, int y);
void reshape(int x, int y);
void vSpecial(int v, int x, int y); 
void mouse(int bouton, int etat, int x, int y);
void motion(int x, int y);
void drawRepere();
void drawCorps();
void drawPattesDevant();
void drawPattesMilieu();
void drawPattesArriere();
void drawCorpsTronc(int n);
void drawCorpsTorse();
void drawTete();
void drawCou();
void drawMandibules(float animation);
void Mandibule();
void drawOeil(float posX,float posY,float posZ);
void drawAntenne();
void mandibuleFace(float y);
void primitiveCylinder(int n, float high, float largeur);
unsigned char* loadJpegImage(const char* fichier, int* width, int* height);
void Anim();


int main(int argc, char** argv)
{
    /*
    * Initialisation des fonctions GLUT nécéssaire
    */
    cam[0] = 0; cam[1] = 1; cam[2] = -5; //Caméra
    /* initialisation de glut et creation
       de la fenetre */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Fourmis");

    //Images
    unsigned char* Image = loadJpegImage("Carbon.jpg", &width[0], &height[0]);
    unsigned char* Image2 = loadJpegImage("blacktexture.jpg", &width[1], &height[1]);
    
    //Quadratic init
    pObj = gluNewQuadric();

    /* Initialisation d'OpenGL */
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    //Lumi�re1
    glLightfv(GL_LIGHT0, GL_POSITION, LightPos1);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);

    //lumiere2
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse2);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos2);
    
    //Caméra init
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1, 0.1, 100.0);
    gluLookAt(cam[0], cam[1], cam[2], 0, 0, 0, 0, 1, 0);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);
    glPointSize(2.0);


    /* Parametre de texture */
    glGenTextures(2, textureIds);

    glBindTexture(GL_TEXTURE_2D, textureIds[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width[0], height[0], 0,
        GL_RGB, GL_UNSIGNED_BYTE, Image);
    
    glBindTexture(GL_TEXTURE_2D, textureIds[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width[1], height[1], 0,
        GL_RGB, GL_UNSIGNED_BYTE, Image2);
    

    /* enregistrement des fonctions de rappel */
    glutDisplayFunc(affichage);
    glutKeyboardFunc(clavier);
    glutSpecialFunc(vSpecial); //Spécial uniquement pour les flèches

    glutReshapeFunc(reshape);

    //Iddle => tourne en arrière plan, animation
    glutIdleFunc(Anim);
    
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    /* Entree dans la boucle principale glut */
    /* Entree dans la boucle principale glut */
    glutMainLoop();
    return 0;
}


void Anim()
{
    /*
    * Fonction qui tournera tout le temps, pour l'animation
    * partie automatique
    */
    if (mandibuleANIM > 20 || mandibuleANIM < 0) //Mandibules
    {
        trigo = !trigo;
    }

    if (trigo)
    {
        mandibuleANIM += 0.2;
    }
    else
    {
        mandibuleANIM -= 0.2;
    }

    if (Antenne > 5 || Antenne < -5) //Antenne
        Antennes = !Antennes;

    if (Antennes)
        Antenne += 0.1;
    else
        Antenne -= 0.1;


    if (pattes) //Pattes n'a pas été modifié mais c'est une vérification, partie non automatique
    {
        if (Fessier > 10 || Fessier < 0) //Les fesses (assez transparent!)
            Fesse = !Fesse;
        if (Fesse)
            Fessier += 0.05;
        else
            Fessier -= 0.05;

        if (DG > 10 || DG < -10) //Les movements droites gauche de la tête et du coup
            DGB = !DGB;
        if (DGB)
            DG += 0.05;
        else
            DG -= 0.05;
         
        if (Avant_xG > 80 || Avant_xG < 10) //Pattes avant
            Avant = !Avant;
        if (Avant)
        {
            Avant_xG += 0.2;
            Avant_xD -= 0.2;
        }
        else
        {
            Avant_xG -= 0.2;
            Avant_xD += 0.2;
        }

        if (Milieu_xG > 20 || Milieu_xG < -20) //Pattes du milieu
            Milieu = !Milieu;
        if (Milieu)
        {
            Milieu_xG += 0.12;
            Milieu_xD -= 0.12;
        }
        else
        {
            Milieu_xG -= 0.12;
            Milieu_xD += 0.12;
        }

        if (Arriere_xG > 70 || Arriere_xG < 0) //Pattes de derrieres
            Arriere  = !Arriere;
        if (Arriere)
        {
            Arriere_xG += 0.2;
            Arriere_xD -= 0.2;
        }
        else
        {
            Arriere_xG -= 0.2;
            Arriere_xD += 0.2;
        }

    }
    glutPostRedisplay();
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
    

    //Patte AVANT droite
    glPushMatrix();

    glTranslatef(0.2, 0, -0.3);
    glRotatef(-Avant_xD, 0, 1, 0);
    glTranslatef(0.4, 0, -0.7);

    glRotatef(-30, 0, 1, 0);
    drawPattesDevant();
    glPopMatrix();

    //Patte AVANT gauche
    glPushMatrix();

    glTranslatef(-0.2, 0, -0.3);
    glRotatef(Avant_xG, 0, 1, 0);
    glTranslatef(-0.4, 0, -0.70);

    glRotatef(30, 0, 1, 0);
    drawPattesDevant();
    glPopMatrix();

    //Patte MILIEU droite
    glPushMatrix();
    glTranslatef(0.09, 0, 0.3);
    glRotatef(Milieu_xD, 0, 1, 0);
    glTranslatef(1, 0, 0);

    glRotatef(-90, 0, 1, 0);
    drawPattesMilieu();
    glPopMatrix();

    // Patte MILIEU gauche
    glPushMatrix();
    glTranslatef(-0.09, 0, 0.3);
    glRotatef(Milieu_xD, 0, 1, 0);
    glTranslatef(-1, 0, 0);

    glRotatef(90, 0, 1, 0);
    drawPattesMilieu();
    glPopMatrix();


    // Patte DERRIERE gauche
    glPushMatrix();
    
    glTranslatef(-0.2, 0, 0.7);

    glRotatef(150-Arriere_xG, 0, 1, 0);
    glTranslatef(0, 0, -1.1);

    drawPattesArriere();
    glPopMatrix();

    // Patte DERRIERE droite
    glPushMatrix();
    
    glTranslatef(0.2, 0, 0.7);
    glRotatef(-150+Arriere_xD, 0, 1, 0);
    glTranslatef(0, 0, -1.1);

    drawPattesArriere();
    glPopMatrix();
  
    // Corps
    drawCorps();  

    // Tronc (Primitive)
    drawCorpsTronc(8);
    /*
    * On a implémenté un peu d'animation ici (pour faire bouger à droite gauche), le coup et la tête
    */
    glPushMatrix();
    glRotatef(DG, 0, 1, 0);

    drawCorpsTorse();
    glPushMatrix();
    glRotatef(DG, 0, 1, 0);
    drawTete();

    drawCou();

    drawOeil(0.25,0.85,-1.22);

    drawOeil(-0.25,0.85,-1.22);

    drawAntenne();
    //Mandibules (Primitive)
    drawMandibules(mandibuleANIM);
    glPopMatrix();
    glPopMatrix();

    //drawRepere();
    glFlush();
    //glutPostRedisplay();
    //On echange les buffers
    glutSwapBuffers();
}

//Drawing
void drawCorpsTronc(int n)
{
    /*
    * Fonction ou sont appliqué les deux types de textures, plaqué et enroulé.
    */
    glPushMatrix();
    glTranslated(0, 0.5, -0.1);
    glRotated(90, 1, 0, 0);
    glColor3f(1, 1, 1);
    glClearColor(0, 0, 0, 1);
    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_REPLACE);
    primitiveCylinder(n, 1, 0.28);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void primitiveCylinder(int n, float high, float largeur)
{
   /*
   * Fonction primitive d'un cylindre
   * n => nombre de côté
   * high => hauteur
   * largeur => largeur
   */
    for (int i = 0; i < n; i++)
    {
        if (i == 5)
        {
            glBindTexture(GL_TEXTURE_2D, textureIds[1]);
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
            glBindTexture(GL_TEXTURE_2D, textureIds[0]);
            glScalef(1.0, 1.0, 1.0);
            glBegin(GL_POLYGON);
            glTexCoord2f((float)i/n, 1.0);
            glVertex3f((largeur / 2) * cos((i * M_PI) / (n / 2)), 0, (largeur / 2) * sin((i * M_PI) / (n / 2)));
            glTexCoord2f((float)(i+1) / n, 1.0);
            glVertex3f((largeur / 2) * cos((i + 1) * M_PI / (n / 2)), 0, (largeur / 2) * sin((i + 1) * M_PI / (n / 2)));
            glTexCoord2f((float)(i+1) / n, 0.0);
            glVertex3f((largeur / 2) * cos((i + 1) * M_PI / (n / 2)), high, (largeur / 2) * sin((i + 1) * M_PI / (n / 2)));
            glTexCoord2f((float)i / n, 0.0);
            glVertex3f((largeur / 2) * cos((i * M_PI) / (n / 2)), high, (largeur / 2) * sin(i * M_PI / (n / 2)));
            glEnd();
        }
    }
}

void drawMandibules(float anim)
{
    /* 
     * Modification et animation des mandibules
     */
    glPushMatrix();
    glColor3f(1.0f, 0, 0);
    glPushMatrix();
    glTranslatef(0.1, 0.68, -1.4);
    glScalef(0.2, 0.2, 0.4);
    glRotatef(anim, 0, 1, 0);
    glRotatef(180, 0, 0, 1);
    Mandibule();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-0.1, 0.645, -1.4);
    glScalef(0.2, 0.2, 0.4);
    glRotatef(-anim, 0, 1, 0);
    Mandibule();
    glPopMatrix();

    glPopMatrix();
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
    * Création des pièces une par une
    * Possibilité d'optimisé ?
    */
    glBegin(GL_QUADS);
    glVertex3f(-0.4f, 0.0f, 0.0f);
    glVertex3f(-0.4f, 0.2f, 0.0f);
    glVertex3f(-0.3f, 0.2f, 0.4f);
    glVertex3f(-0.3f, 0.0f, 0.4f); 
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(-0.1f, 0.0f, 0.4f); 
    glVertex3f(-0.1f, 0.2f, 0.4f);
    glVertex3f(-0.05f, 0.2f, 0.0f);
    glVertex3f(-0.05f, 0.0f, 0.0f); 
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(-0.3f, 0.0f, 0.4f);
    glVertex3f(-0.3f, 0.2f, 0.4f);
    glVertex3f(-0.1f, 0.2f, 0.4f);
    glVertex3f(-0.1f, 0.0f, 0.4f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(-0.05f, 0.0f, 0.0f);
    glVertex3f(-0.05f, 0.2f, 0.0f);
    glVertex3f(0.05f, 0.2f, -0.05f);
    glVertex3f(0.05f, 0.0f, -0.05f);
    glEnd();
    /* 
     * Doigts
     * J'ai séparé la mandibule en 3 parties
     * le tronc : partie presque carré
     * les doigts : partie en forme d'escalier (pince)
     * dessus de la main : partie ronde de la mandibule
     */
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
    /* 
     * Construction et animation des fesses de la fourmis (sphere du fond vers les pattes arrières)
     */
    glPushMatrix();
    glRotatef(-Fessier, 1, 0, 0);
    glTranslatef(0, 0.5, 1);

    glRotatef(-15, 1, 0, 0);
    glColor3d(255, 0, 255);
    glScalef(1.0f, 1.0f, 1.75f);
    glutSolidSphere(0.35, 30, 30);
    glPopMatrix();
}

void drawCorpsTorse()
{
    /*
    * Construction sans animation du corps "torse" de la fourmis, entre le coup et notre primitive de cylindre
    */
    glPushMatrix();
    glTranslatef(0, 0.5, -0.25);
    glRotatef(0, 0, 0, 0);
    glColor3d(255, 255, 0);
    glScalef(0.65f, 0.65f, 1.15f);
    glutSolidSphere(0.35, 30, 30);
    glPopMatrix();
}

void drawTete()
{
    /*
    * Construction de la tête
    */
    glPushMatrix();
    glTranslatef(0, 0.65, -1);
    glRotatef(0, 0, 0, 0);
    glColor3f(255, 0, 0);
    glScalef(1.0f, 0.65f, 1.0f);
    glutSolidSphere(0.45, 30, 30);
    glPopMatrix();
}

void drawOeil(float posX,float posY,float posZ)
{
    /*
    * Construction des yeux
    */
    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glRotatef(0, 0, 0, 0);
    glColor3f(255, 0, 0);
    glutSolidSphere(0.05, 30, 30);
    glPopMatrix();
}

void drawCou()
{
    /*
    * Construction du coup
    */
    glPushMatrix();
    glTranslatef(0, 0.65, -1.15);
    glRotatef(0, 0, 0, 0);
    glColor3f(255, 0, 0);
    gluCylinder(pObj, 0.05, 0.05, 0.75, 30, 30);
    glPopMatrix();
}

void drawAntenne()
{
    /*
    Antenne gauche
    Code optimisable (pour évité les répétitions)
    */

    glPushMatrix();
    glTranslatef(-0.5, 1.05, -1.1);

    glPushMatrix();
    glRotatef(120, 0, 1, 0);

    glRotatef(30, 1, 0, 0);
    gluCylinder(pObj, 0.02, 0.02, 0.5,30,30);
    glPopMatrix();


    glPushMatrix();
    glRotatef(Antenne, 1, 0, 0);
    glRotatef(170, 0, 1, 0);

    glScalef(0.75, 0.75, 0.75);
    gluCylinder(pObj, 0.02, 0.02, 0.8, 30, 30);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.095, 0.005+ (Antenne/100), -0.56);
    glRotatef(-15, 1, 0, 0);
    glRotatef(170, 0, 1, 0);
    glScaled(0.5, 0.5, 0.5);
    gluCylinder(pObj, 0.02, 0.02, 0.8, 30, 30);
    glPopMatrix();

    glPopMatrix();

    /*
    Antenne droite
    */
    glPushMatrix();

    glTranslatef(0.5, 1.05, -1.1);
    glPushMatrix();
    glRotatef(-120, 0, 1, 0);
    glRotatef(30, 1, 0, 0);
    gluCylinder(pObj, 0.02, 0.02, 0.5, 30, 30);
    glPopMatrix();

    glPushMatrix();
    glRotatef(Antenne, 1, 0, 0);
    glRotatef(-170, 0, 1, 0);
    glScaled(0.75, 0.75, 0.75);
    gluCylinder(pObj, 0.02, 0.02, 0.8, 30, 30);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.1,0.005+ (Antenne / 100),-0.53);
    glRotatef(-5 , 1, 0, 0);
    glRotatef(-170, 0, 1, 0);
    glScaled(0.5, 0.5, 0.5);
    gluCylinder(pObj, 0.02, 0.02, 0.8, 30, 30);
    glPopMatrix();

    glPopMatrix();
}


void drawRepere()
{
    /*
    * On le garde car il a été très utile, merci a lui !
    */
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

void drawPattesDevant()
{
    /*
    * Les pattes sont une succession de 4 cylindres applati et de boules au bout de chacun de ses cylindres
    * 3 fonctions différentes ont été utilisé car par soucis du détail, aucune de nos pattes ne fait la même taille
    * celles de devant étant les plus petites, celles de derriere les plus grandes
    */

    glPushMatrix();

    /*Cuisse1
    * Séparer en 4 partie
    * Cuisse: partie sur la fourmis, la moins aligné par rapport aux 3 autres cylindres
    * Tibia: Partie la plus grande et plus fine que la cuisse
    * main : petite partie juste avant le cylindre des doigts et plus fine que le tibia
    * doigt : partie encore plus petit et plus fine que la main
    */
    glPushMatrix();
    glTranslatef(0, 0.8, 0.65);
    glRotatef(60, 1, 0, 0);

    glPushMatrix();
    glScalef(0.3f, 0.2f, 0.1f);
    gluSphere(pObj, 0.1, 30, 30);
    glPopMatrix();

    glScalef(0.3f, 0.2f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.3, 35, 1);
    glPopMatrix();

    glPushMatrix();
    //Tibia1
    glPushMatrix();
    glTranslatef(0, 0.45, 0.46);
    glRotatef(-60, 1, 0, 0);

    glPushMatrix();
    glScalef(0.3f, 0.2f, 0.1f);
    gluSphere(pObj, 0.1, 30, 30);
    glPopMatrix();

    glScalef(0.3f, 0.2f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.4, 35, 1);
    glPopMatrix();
    //Main1
    glPushMatrix();
    glTranslatef(0, 0.12, 0.23);
    glRotatef(-55, 1, 0, 0);

    glScalef(0.3f, 0.1f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.4, 35, 1);
    glPopMatrix();
    //Pied1
    glPushMatrix();
    glTranslatef(0, 0.03, 0.11);
    glRotatef(-38, 1, 0, 0);

    glPushMatrix();
    glScalef(0.3f, 0.1f, 0.1f);
    gluSphere(pObj, 0.1, 30, 30);
    glPopMatrix();

    glScalef(0.3f, 0.1f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.15, 35, 1);
    glPopMatrix();
    glPopMatrix();
    
    glPopMatrix();

   
}
void drawPattesMilieu()
{
    /*
    Patte milieu
    même chose que les pattes de devant mais avec des modifications au niveau des valeurs
    de taille, glTranslate glRotate
    */

    glPushMatrix();
    //Pied1
    glPushMatrix();
    glTranslatef(0, 0.03, 0.11);
    glRotatef(-38, 1, 0, 0);

    glPushMatrix();
    glScalef(0.3f, 0.1f, 0.1f);
    gluSphere(pObj, 0.1, 30, 30);
    glPopMatrix();

    glScalef(0.3f, 0.1f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.15, 35, 1);
    glPopMatrix();

    //Main1
    glPushMatrix();
    glTranslatef(0, 0.12, 0.23);
    glRotatef(-55, 1, 0, 0);

    glScalef(0.3f, 0.1f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.4, 35, 1);
    glPopMatrix();

    //Tibia1
    glPushMatrix();
    glTranslatef(0, 0.43, 0.46);
    glRotatef(-60, 1, 0, 0);

    glPushMatrix();
    glScalef(0.3f, 0.2f, 0.1f);
    gluSphere(pObj, 0.1, 30, 30);
    glPopMatrix();

    glScalef(0.3f, 0.2f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.4, 35, 1);
    glPopMatrix();


    //Cuisse1
    glPushMatrix();
    glTranslatef(0, 0.78, 0.65);
    glRotatef(30, 1, 0, 0);

    glPushMatrix();
    glScalef(0.3f, 0.2f, 0.1f);
    gluSphere(pObj, 0.1, 30, 30);
    glPopMatrix();

    glScalef(0.3f, 0.2f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.4, 35, 1);
    glPopMatrix();

    glPopMatrix();
}
void drawPattesArriere()
{
    /*
    même chose que les pattes de devant mais avec des modifications au niveau des valeurs
    de taille, glTranslate glRotate
    */

    glPushMatrix();
    //Pied1
    glPushMatrix();
    glRotatef(-30, 1, 0, 0);

    glPushMatrix();
    glScalef(0.3f, 0.1f, 0.1f);
    gluSphere(pObj, 0.1, 30, 30);
    glPopMatrix();

    glScalef(0.3f, 0.1f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.2, 35, 1);
    glPopMatrix();

    //Main1
    glPushMatrix();
    glTranslatef(0, 0.08, 0.15);
    glRotatef(-50, 1, 0, 0);

    glScalef(0.3f, 0.1f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.5, 35, 1);
    glPopMatrix();

    //Tibia1
    glPushMatrix();
    glTranslatef(0, 0.44, 0.46);
    glRotatef(-60, 1, 0, 0);

    glPushMatrix();
    glScalef(0.3f, 0.2f, 0.1f);
    gluSphere(pObj, 0.1, 30, 30);
    glPopMatrix();

    glScalef(0.3f, 0.2f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.5, 35, 1);
    glPopMatrix();

    //Cuisse1
    glPushMatrix();
    glTranslatef(0, 0.88, 0.7);
    glRotatef(40, 1, 0, 0);

    glPushMatrix();
    glScalef(0.3f, 0.2f, 0.1f);
    gluSphere(pObj, 0.1, 30, 30);
    glPopMatrix();

    glScalef(0.3f, 0.2f, 1.0f);
    gluCylinder(pObj, 0.1, 0.1, 0.5, 35, 1);
    glPopMatrix();

    glPopMatrix();
}


//KEYS
void vSpecial(int key, int x, int y)
{
    /*
    * Les signes spéciaux ici les flèches du clavier
    * on modifie l'angle de vue de la caméra en fonction de la touche
    */
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
    /*
    * Ici on a repris les fonctions vues en TP et rajouté des évennements pour l'appuie sur la touche
    * - z & Z (zoom de caméra)
    * - a (Animation)
    */

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
    /*
    * On a décidé de faire en sorte que z zoom et Z dezoom, car, c'était mieux pour nous
    */
    case 'Z': //Bornes de la caméra max, on modifie les valeurs
        if (cam[0] <= 0.1 && cam[0] >= -0.1)
            cam[2] = cam[2] - 0.1;
        else if (cam[2] <= 0.1 && cam[2] >= -0.1)
            cam[0] = cam[0] - 0.1;
        else
        {
            cam[0] = cam[0] - 0.1;
            cam[2] = cam[2] - 0.1;
        }
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0, 1, 0.1, 100.0);
        gluLookAt(cam[0], cam[1], cam[2], 0, 0, 0, 0, 1, 0);
        glutPostRedisplay();
        break;
    case 'z':
        if(cam[0] <= 0.1 && cam[0] >= -0.1)
            cam[2] = cam[2] + 0.1;
        else if (cam[2] <= 0.1 && cam[2] >= -0.1)
            cam[0] = cam[0] + 0.1;
        else
        {
            cam[0] = cam[0] + 0.1;
            cam[2] = cam[2] + 0.1;
        }
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0, 1, 0.1, 100.0);
        gluLookAt(cam[0], cam[1], cam[2], 0, 0, 0, 0, 1, 0);
        glutPostRedisplay();
        break;
        break;
    case 'a':
        //Active/désactive le boolean qui gère l'animation quand la fourmis "marche"
        pattes = !pattes;
        printf("test");
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

