#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT  600

#define TIMER_PERIOD    16 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532	   // Degree to Radian conversion

#define MAX_FIRE 1000
#define SINGULAR 1
#define DOUBLE   2

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false, flame = 0;
int  winWidth, winHeight, step, stepc = 0, tool = 0, fc = 0, f; // current Window width and height

typedef struct {
    double r, g, b;
}color_t;
typedef struct {
    double x, y;
}point_t;
typedef struct {
    point_t pos;      //Player's position on Y-Axis
    double angle;        //Player's angle
}player_t;
typedef struct {
    point_t pos;
    color_t color;
    double radius;
    bool hit;
    double angle;
}ufo_t;
typedef struct {
    double x;
    bool hit;
}enemy2_t;
typedef struct {
    point_t pos;
    float angle;
    bool active;
}fire_t;

player_t p = { {0,0}, 0 }, e = { {150,150}, 0 };
ufo_t ufo = { {-150, 150}, {}, 20, false, 0 };
enemy2_t ship = { -240, false };
fire_t fr[MAX_FIRE];
point_t c;
bool spacebar = false,
extraComs = false;
int fire_rate = 0,
mode = SINGULAR,
FIRERATE = 45,
difficultyU = 3,
difficultyS = 7,
fSpeed = 7;


void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

void vertex(point_t P, point_t Tr, double angle) {
    float xp = (P.x * cos(angle) - P.y * sin(angle)) + Tr.x;
    float yp = (P.x * sin(angle) + P.y * cos(angle)) + Tr.y;
    glVertex2f(xp, yp);
}

point_t vertexcore(point_t P, point_t Tr, double angle) {
    float xp = (P.x * cos(angle) - P.y * sin(angle)) + Tr.x;
    float yp = (P.x * sin(angle) + P.y * cos(angle)) + Tr.y;
    point_t pos;
    pos.x = xp;
    pos.y = yp;
    return(pos);
}



void drawPlayer(int s, int t) {
    float angle = p.angle * D2R;
    glColor3f(0.7, 0, 0);
    glBegin(GL_POLYGON);
    vertex({ -5, -14 }, p.pos, angle);
    vertex({ 5, -14 }, p.pos, angle);
    vertex({ 5, 14 }, p.pos, angle);
    vertex({ -5, 14 }, p.pos, angle);
    glEnd();

    glColor3f(0.9, 0.9, 0.4);
    circle((vertexcore({ 0,0 }, p.pos, angle)).x,
        (vertexcore({ 0,0 }, p.pos, angle).y), 6);

    glColor3f(0, 0, 0);
    circle(vertexcore({ 3.5,3 }, p.pos, angle).x,
        vertexcore({ 3.5,3 }, p.pos, angle).y, 1);
    circle(vertexcore({ 3.5,-3 }, p.pos, angle).x,
        vertexcore({ 3.5,-3 }, p.pos, angle).y, 1);


    switch (s)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        break;
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13: { glColor3f(0, 0, 0);
        glBegin(GL_POLYGON);
        vertex({ 5, -10 }, p.pos, angle);
        vertex({ 10, -10 }, p.pos, angle);
        vertex({ 10, -2 }, p.pos, angle);
        vertex({ 5, -2 }, p.pos, angle);
        glEnd();
        glBegin(GL_POLYGON);
        vertex({ -5, 10 }, p.pos, angle);
        vertex({ -10, 10 }, p.pos, angle);
        vertex({ -10, 2 }, p.pos, angle);
        vertex({ -5, 2 }, p.pos, angle);
        glEnd(); }
           break;
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
        if (down != 1)
        {
            glColor3f(0, 0, 0);
            glBegin(GL_POLYGON);
            vertex({ 5, -10 }, p.pos, angle);
            vertex({ 15, -10 }, p.pos, angle);
            vertex({ 15, -2 }, p.pos, angle);
            vertex({ 5, -2 }, p.pos, angle);
            glEnd();
            glBegin(GL_POLYGON);
            vertex({ -5, 10 }, p.pos, angle);
            vertex({ -15, 10 }, p.pos, angle);
            vertex({ -15, 2 }, p.pos, angle);
            vertex({ -5, 2 }, p.pos, angle);
            glEnd();

            {if (tool != 1)
            {
                glColor3f(0.7, 0, 0);
                glBegin(GL_POLYGON);
                vertex({ -5, -14 }, p.pos, angle);
                vertex({ -10, -14 }, p.pos, angle);
                vertex({ -10, -8 }, p.pos, angle);
                vertex({ -5, -8 }, p.pos, angle);
                glEnd();
                glColor3f(0.9, 0.9, 0.4);
                glBegin(GL_POLYGON);
                vertex({ -10, -14 }, p.pos, angle);
                vertex({ -13, -14 }, p.pos, angle);
                vertex({ -13, -8 }, p.pos, angle);
                vertex({ -10, -8 }, p.pos, angle);
                glEnd();

            }

            glColor3f(0.7, 0, 0);
            glBegin(GL_POLYGON);
            vertex({ 5, 14 }, p.pos, angle);
            vertex({ 10, 14 }, p.pos, angle);
            vertex({ 10, 8 }, p.pos, angle);
            vertex({ 5, 8 }, p.pos, angle);
            glEnd();
            glColor3f(0.9, 0.9, 0.4);
            glBegin(GL_POLYGON);
            vertex({ 10, 14 }, p.pos, angle);
            vertex({ 13, 14 }, p.pos, angle);
            vertex({ 13, 8 }, p.pos, angle);
            vertex({ 10, 8 }, p.pos, angle);
            glEnd(); }

        }
        break;
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27: { glColor3f(0, 0, 0);
        glBegin(GL_POLYGON);
        vertex({ -5, -10 }, p.pos, angle);
        vertex({ -10, -10 }, p.pos, angle);
        vertex({ -10, -2 }, p.pos, angle);
        vertex({ -5, -2 }, p.pos, angle);
        glEnd();
        glBegin(GL_POLYGON);
        vertex({ 5, 10 }, p.pos, angle);
        vertex({ 10, 10 }, p.pos, angle);
        vertex({ 10, 2 }, p.pos, angle);
        vertex({ 5, 2 }, p.pos, angle);
        glEnd(); }
           break;
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
    case 33:
    case 34:
        if (down != 1)
        {
            glColor3f(0, 0, 0);
            glBegin(GL_POLYGON);
            vertex({ -5, -10 }, p.pos, angle);
            vertex({ -15, -10 }, p.pos, angle);
            vertex({ -15, -2 }, p.pos, angle);
            vertex({ -5, -2 }, p.pos, angle);
            glEnd();
            glBegin(GL_POLYGON);
            vertex({ 5, 10 }, p.pos, angle);
            vertex({ 15, 10 }, p.pos, angle);
            vertex({ 15, 2 }, p.pos, angle);
            vertex({ 5, 2 }, p.pos, angle);
            glEnd();

            {glColor3f(0.7, 0, 0);
            glBegin(GL_POLYGON);
            vertex({ 5, -14 }, p.pos, angle);
            vertex({ 10, -14 }, p.pos, angle);
            vertex({ 10, -8 }, p.pos, angle);
            vertex({ 5, -8 }, p.pos, angle);
            glEnd();
            glColor3f(0.9, 0.9, 0.4);
            glBegin(GL_POLYGON);
            vertex({ 10, -14 }, p.pos, angle);
            vertex({ 13, -14 }, p.pos, angle);
            vertex({ 13, -8 }, p.pos, angle);
            vertex({ 10, -8 }, p.pos, angle);
            glEnd();
            glColor3f(0.7, 0, 0);

            glBegin(GL_POLYGON);
            vertex({ -5, 14 }, p.pos, angle);
            vertex({ -10, 14 }, p.pos, angle);
            vertex({ -10, 8 }, p.pos, angle);
            vertex({ -5, 8 }, p.pos, angle);
            glEnd();
            glColor3f(0.9, 0.9, 0.4);
            glBegin(GL_POLYGON);
            vertex({ -10, 14 }, p.pos, angle);
            vertex({ -13, 14 }, p.pos, angle);
            vertex({ -13, 8 }, p.pos, angle);
            vertex({ -10, 8 }, p.pos, angle);
            glEnd(); }

        }
        break;

    }

    switch (t)
    {
    case 0:
        break;
    case 1:
        glColor3f(0.7, 0, 0);
        glBegin(GL_POLYGON);
        vertex({ 5, -14 }, p.pos, angle);
        vertex({ 13, -14 }, p.pos, angle);
        vertex({ 13, -8 }, p.pos, angle);
        vertex({ 5, -8 }, p.pos, angle);
        glEnd();
        glColor3f(0.9, 0.9, 0.4);
        glBegin(GL_POLYGON);
        vertex({ 13, -14 }, p.pos, angle);
        vertex({ 17, -14 }, p.pos, angle);
        vertex({ 17, -8 }, p.pos, angle);
        vertex({ 13, -8 }, p.pos, angle);
        glEnd();
        glColor3f(0, 0, 0);
        glBegin(GL_POLYGON);
        vertex({ 17, -13 }, p.pos, angle);
        vertex({ 25, -13 }, p.pos, angle);
        vertex({ 25, -9 }, p.pos, angle);
        vertex({ 17, -9 }, p.pos, angle);
        glEnd();
        break;
    case 2:
        break;
    case 3:
        break;
    }
}


void drawFires() {
    for (int i = 0; i < MAX_FIRE; i++) {
        if (fr[i].active && mode == SINGULAR) {
            glColor3f(1, 1, 0);
            circle(fr[i].pos.x, fr[i].pos.y, 1);
        }
        else if (fr[i].active && mode == DOUBLE) {
            glColor3ub(255, 72, 72);
            circle(fr[i].pos.x - 20, fr[i].pos.y, 4);
            circle(fr[i].pos.x + 20, fr[i].pos.y, 4);
        }
    }
}

bool FR(int rate)
{
    switch (rate % FIRERATE)
    {
    case 1:
    case 2:
    case 3:
    case 4:
        return 1;
        break;

    default:
        return(0);
        break;
    }
}

int findAvailableFire() {
    for (int i = 0; i < MAX_FIRE; i++) {
        if (fr[i].active == false) return i;
    }
    return -1;
}


/*bool UFOCollision(fire_t fire, ufo_t ufo) {
    double dx = c.x - fire.pos.x;
    double dy = c.y - fire.pos.y;
    double d = sqrt(dx * dx + dy * dy);
    return d <= ufo.radius + 7;
}*/

/*bool shipCollision(fire_t fire, enemy2_t ship) {
    if (fire.pos.x <= ship.x + 90 && fire.pos.x >= ship.x &&
        fire.pos.y >= 307.5 && fire.pos.y <= 330)
        return true;
    else
        return false;
}*/

//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(0.5, 0.5, 0.5, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    // drawBG();
    drawFires();
    drawPlayer(step, tool);

    float angle = p.angle * D2R;
    switch (tool)
    {
    case 1:
        if (FR(fc) == 1)
        {
            glColor4f(1, 1, 0, 0.8);
            glBegin(GL_POLYGON);
            vertex({ 27, -15 }, p.pos, angle);
            vertex({ 27, -7 }, p.pos, angle);
            vertex({ 34, -11 }, p.pos, angle);
            glEnd();
        }
        break;
    }





    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    switch (key) {
    case ' ':
        spacebar = true;
        flame = 1;
        break;
    case 'x':
    case 'X': extraComs = !extraComs; break;
    case 'f':
    case 'F': if (mode == SINGULAR) mode++;
            else mode--; break;
    case 'w':
    case 'W': if (FIRERATE >= 1) FIRERATE--; break;
    case 's':
    case 'S':
        p.angle += 180;//if (FIRERATE <= 60) FIRERATE++; break;
    case 'o':
    case 'O': if (fSpeed <= 30) fSpeed += 2;  break;
    case 'p':
    case 'P': if (fSpeed > 2) fSpeed -= 2;
        break;
    case '1':
        tool = 0;
        break;
    case '2':
        tool = 1;

    }

    // to refresh the window it calls display() function
    //glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    if (key == ' ')
    {
        spacebar = false;
        flame = 0;
    }
    // to refresh the window it calls display() function
    //glutPostRedisplay();
}


void onSpecialKeyDown(int key, int x, int y) {
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
    case GLUT_KEY_F1:
        if (ship.hit == true) ship.x = -240;
        ufo.hit = false; ship.hit = false;
        break;
    }

    // to refresh the window it calls display() function
    //glutPostRedisplay();
}


void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }

    // to refresh the window it calls display() function
    //glutPostRedisplay();
}


void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}


void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.

    if (left)
        p.angle += 5;


    if (right)
        p.angle -= 5;


    if (up)
    {
        p.pos.x += 2.5 * cos(p.angle * D2R);
        p.pos.y += 2.5 * sin(p.angle * D2R);
        step = stepc % 35;
        stepc++;
    }
    else if (down)
    {
        p.pos.x -= 1.25 * cos(p.angle * D2R);
        p.pos.y -= 1.25 * sin(p.angle * D2R);
        step = stepc % 35;
        stepc++;
    }
    else
        step = 0;

    if (flame)
        fc++;
    else
        fc = 0;


    if (spacebar && fire_rate == 0 && tool == 1) {
        int availFire = findAvailableFire();
        if (availFire != -1) {

            fr[availFire].pos = vertexcore({ 25, -11 }, p.pos, p.angle * D2R);
            fr[availFire].angle = p.angle;
            fr[availFire].active = true;
            fire_rate = FIRERATE;
        }
    }

    if (fire_rate > 0) fire_rate--;

    for (int i = 0; i < MAX_FIRE; i++) {
        if (fr[i].active) {
            fr[i].pos.x += fSpeed * cos(fr[i].angle * D2R);
            fr[i].pos.y += fSpeed * sin(fr[i].angle * D2R);

            if (fr[i].pos.x > winWidth / 2 || fr[i].pos.x < -winWidth / 2 || fr[i].pos.y > winHeight / 2 || fr[i].pos.y < -winHeight / 2)
                fr[i].active = false;

        }
    }

    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(250, 120);
    glutCreateWindow("korkunç bişi");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}