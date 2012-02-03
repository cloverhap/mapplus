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

#ifdef DEBUG
//#define DEBUG_VALUE  // function values
//#define DEBUG_POS  // position values
//#define DEBUG_FUNC  // function count
//#define DEBUG_KEY // key press
#endif

#define PI 3.14
#define MAX_DB_LINE_LENGTH 1024
#define MAX_CHARS 100
#define MAX_AREAS 50
#define MAX_OBJECTS 100

// enum for direction
#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4

// enum for keyboard layout
#define KB_QWERTY 1
#define KB_DVORAK 2

// enum for game mode
#define SYSTEM_MODE 1  // Game engine loading, etc?
#define TITLE_MODE 2   // Title, Game over, etc
#define GAME_MODE 3    // 3D Drawing
#define MENU_MODE 4    // Game menu
#define FLAT_MODE 5    // 2D Drawing
#define DEBUG_MODE 6   // Use console or special debugging keys?
#define DIAG_MODE 7    // Dialogue interaction

#include "GL/freeglut.h"

#include "object.h"  // object-related class definitions
#include <string>

using namespace::std;

// global variables
extern GLdouble eye_pos[3];
extern GLdouble center_pos[3];
extern GLdouble up_pos[3];
extern GLdouble speed;            // moving speed
extern GLdouble angular_speed;    // rotating speed

// TODO: research data-driven/component-system design
extern unit * units[MAX_CHARS];   // list of characters (logic will need to be changed eventually to allow dynamic char creation)
extern area * areas[MAX_AREAS];   // same as above
extern item * items[MAX_OBJECTS]; // ditto

extern GLint game_mode;
extern string diag_message;       // Message to send to dialogue box
extern GLboolean goal_reached;

extern GLint kb_layout;  // Keyboard layout (QWERTY or dvorak)
extern GLint screen_size_x;
extern GLint screen_size_y;


// global functions
GLint GLmax(GLint, GLint);
void load_mode(GLint);

// global functions (from main.cpp)
void exit_glut(const char *exit_message);
void init();

#endif
