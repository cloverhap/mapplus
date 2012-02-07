#ifndef _GL_WRAPPER__H_
#define _GL_WRAPPER__H_

#include "global.h"

// structures
/*typedef struct {
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER;
*/


/// functions

// Translate x, y, z planes by the given values
void translate_eye(GLdouble, GLdouble, GLdouble);
// Rotate x, y, z planes by the given degrees (in radions) wrt eye
void rotate_eye(GLdouble, GLdouble);
void draw_cube(GLenum, GLdouble);

/// utility functions
void get_rel_axis(GLdouble[], GLdouble[], GLdouble[]);

#endif
