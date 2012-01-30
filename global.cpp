#include "global.h"

// global variables
GLdouble eye_pos[3];
GLdouble center_pos[3];
GLdouble up_pos[3];
GLdouble speed;           // moving speed
GLdouble angular_speed;   // rotating speed
unit * units[MAX_CHARS];  // list of characters (logic will need to be changed eventually to allow dynamic char creation)
area * areas[MAX_AREAS];  // same as above
item * items[MAX_OBJECTS]; // ditto
GLint kb_layout;  // Keyboard layout

GLint GLmax(GLint a, GLint b)
{
#ifdef DEBUG_FUNC
    static int max_count;
    max_count++;
    printf("Rotate count: %i\n", max_count);
#endif
    return a > b ? a : b;
}
