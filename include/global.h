#ifndef _GLOBAL__H_
#define _GLOBAL__H_

#ifndef max
#define max(a,b) ( ((a) > (b)) ? (a) : (b) )
#endif
#ifndef min
#define min(a,b) ( ((a) < (b)) ? (a) : (b) )
#endif

// #pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#define GLUT_DISABLE_ATEXIT_HACK
#define FREEGLUT_STATIC
#define DEBUG_VALUE  // function values
#define DEBUG_POS  // position values
//#define DEBUG_FUNC  // function count
#define DEBUG_KEY // key press

#define PI 3.14
#define MAX_DB_LINE_LENGTH 1024
#define MAX_CHARS 100
#define MAX_AREAS 50
#define MAX_OBJECTS 100

// enum for direction
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

// enum for keyboard layout
#define KB_QWERTY 0
#define KB_DVORAK 1

#include "GL/freeglut.h"

#include "object.h"  // object-related class definitions

using namespace::std;

// global variables
extern GLdouble eye_pos[3];
extern GLdouble center_pos[3];
extern GLdouble up_pos[3];
extern GLdouble speed;            // moving speed
extern GLdouble angular_speed;    // rotating speed
extern unit * units[MAX_CHARS];   // list of characters (logic will need to be changed eventually to allow dynamic char creation)
extern area * areas[MAX_AREAS];   // same as above
extern item * items[MAX_OBJECTS]; // ditto
extern GLint kb_layout;  // Keyboard layout

// global functions
GLint GLmax(GLint, GLint);

// global functions (from main.cpp)
void exit_glut(const char *exit_message);
void init();

#endif
