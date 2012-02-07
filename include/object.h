#ifndef _OBJECT__H_
#define _OBJECT__H_

#include "GL/freeglut.h"

typedef struct {
    GLubyte* image;
    GLint width;
    GLint height;
} BMP;

class object {
public:
    GLbyte name[256];
    virtual void update() = 0;
//protected:
    GLint id;
};

class unit : public object {
public:
    unit();
    GLbyte tag[64];
    GLdouble position[3];
    GLint map_id;
    GLdouble speed();
    void move();
    virtual void update();
private:
    GLdouble _speed;
};

class area : public object {
public:
    GLdouble dimension[3];
    GLint location;
    virtual void update();
};

class item : public object {
public:
    GLdouble dimension[3];
    GLdouble position[3];
    GLint map_id;
    GLvoid* rest;
    virtual void update();
};

#endif
