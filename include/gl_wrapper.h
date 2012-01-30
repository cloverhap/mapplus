#ifndef _GL_WRAPPER__H_
#define _GL_WRAPPER__H_

#include "global.h"

/// functions

// Translate x, y, z planes by the given values
void translate_eye(GLdouble, GLdouble, GLdouble);
// Rotate x, y, z planes by the given degrees (in radions) wrt eye
void rotate_eye(GLdouble, GLdouble);
void draw_cube(GLenum, GLdouble);

/// utility functions
void get_rel_axis(GLdouble[], GLdouble[], GLdouble[]);

#endif
