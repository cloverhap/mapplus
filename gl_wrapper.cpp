#include "gl_wrapper.h"
#include "controller.h"
#include <math.h>
#include <stdio.h>

/* global variables */
extern GLdouble eye_pos[3];
extern GLdouble center_pos[3];
extern GLdouble up_pos[3];

/**
 * Note: OpenGL uses Right-handed coordinate system, with
 * x going right, y going up, and z going towards the screen (backwards)
 * Right-handed means use the right hand to visualize cross-products
 * http://en.wikipedia.org/wiki/File:Right_hand_rule_cross_product.svg
 **/

/* Utility Functions */
/***
 * dot_product
 * @pre arrays of size 3
 ****/
GLdouble dot_product(GLdouble a[], GLdouble b[]) {
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

/***
 * cross_product
 * @pre arrays of size 3
 ****/
GLdouble* cross_product(GLdouble a[], GLdouble b[], GLdouble result[]) {
    result[0] = a[1]*b[2] - a[2]*b[1];
    result[1] = a[2]*b[0] - a[0]*b[2];
    result[2] = a[0]*b[1] - a[1]*b[0];
    return result;
}

/***
 * get_norm
 * A norm, here, is the length of the vector
 * @pre n size of array v
 ****/
GLdouble get_norm(GLdouble v[],GLint n) {
    int i;
    int len = n;//sizeof(v)/sizeof(GLdouble);
    GLdouble norm = 0;
    for (i = 0; i < len; i++) {
        norm += pow(v[i],2);
    }
    return sqrt(norm);
}

/***
 * Normalize
 * @pre arrays of size 3
 ****/
GLdouble* normalize(GLdouble v[], GLdouble result[]) {
    GLdouble norm = sqrt(pow(v[0],2) + pow(v[1],2) + pow(v[2],2));
    if (!norm) return result;  // norm is zero
    result[0] = v[0]/norm;
    result[1] = v[1]/norm;
    result[2] = v[2]/norm;
    result[0] = (result[0] == 0) ? 0 : result[0];
    result[1] = (result[1] == 0) ? 0 : result[1];
    result[2] = (result[2] == 0) ? 0 : result[2];
    return result;
}

/***
 * Get the relative xyz axis to the player's direction
 * @pre arrays of size 3
 ****/
void get_rel_axis(GLdouble x[], GLdouble y[], GLdouble z[]) {
    GLdouble forward_vector[3] = {center_pos[0] - eye_pos[0],
                                  center_pos[1] - eye_pos[1],
                                  center_pos[2] - eye_pos[2]};

    // Calculate vectors that for the player's relative axis
    // x axis is to the right, y is up, and z reaches behind
    x = cross_product(forward_vector,up_pos,x);
    y[0] = up_pos[0];
    y[1] = up_pos[1];
    y[2] = up_pos[2];
    z[0] = -forward_vector[0];
    z[1] = -forward_vector[1];
    z[2] = -forward_vector[2];
    // Normalize the vectors
    x = normalize(x,x);
    y = normalize(y,y);
    z = normalize(z,z);

#ifdef DEBUG_VALUE
    printf("x-axis vector: [%f %f %f]\ny-axis vector: [%f %f %f]\nz-axis vector: [%f %f %f]\n",
           x[0],x[1],x[2],
           y[0],y[1],y[2],
           z[0],z[1],z[2]);
#endif
    return;
}

/***
 * Print positional values
 ****/
void print_pos() {
#ifdef DEBUG_POS
    printf("Camera: %f %f %f\n", eye_pos[0], eye_pos[1], eye_pos[2]);
    printf("Center: %f %f %f\n", center_pos[0], center_pos[1], center_pos[2]);
    printf("Up:     %f %f %f\n", up_pos[0], up_pos[1], up_pos[2]);
#endif
}


/* Public Functions */

/***
 * Translating
 * This is a relative translation, from the p.o.v. of the camera
 ****/
void translate_eye(GLdouble x, GLdouble y, GLdouble z) {
#ifdef DEBUG_FUNC
    static int translate_eye_count;
    translate_eye_count++;
    printf("Translate count: %i\n", translate_eye_count);
#endif

#ifdef DEBUG_VALUE
    printf("Relative translation by: %f %f %f\n", x, y, z);
#endif

    // Calculate the player's directional axis in terms of the reference axis
    GLdouble x_axis[3] = {0,0,0};
    GLdouble y_axis[3] = {0,0,0};
    GLdouble z_axis[3] = {0,0,0};
    get_rel_axis(x_axis, y_axis, z_axis);

    // multiply by how fast the player's moving to get the movement vector
    GLdouble x_move = x*x_axis[0] + y*y_axis[0] + z*z_axis[0];
    GLdouble y_move = x*x_axis[1] + y*y_axis[1] + z*z_axis[1];
    GLdouble z_move = x*x_axis[2] + y*y_axis[2] + z*z_axis[2];

    // note the movements of the view
    if (x || y || z) {
        eye_pos[0] += x_move;
        eye_pos[1] += y_move;
        eye_pos[2] += z_move;
        units[0]->position[0] = eye_pos[0];
        units[0]->position[1] = eye_pos[1];
        units[0]->position[2] = eye_pos[2];
        center_pos[0] += x_move;
        center_pos[1] += y_move;
        center_pos[2] += z_move;
        up_pos[0] = y_axis[0];
        up_pos[1] = y_axis[1];
        up_pos[2] = y_axis[2];

        // Check if unit is out of bounds of map
       // printf("Debug: %d\n",areas[units[0]->map_id]->id);
        GLdouble dist_to_end[3] = {eye_pos[0] - areas[units[0]->map_id - 1]->dimension[0],
                                   eye_pos[1] - areas[units[0]->map_id - 1]->dimension[1],
                                   eye_pos[2] - areas[units[0]->map_id - 1]->dimension[2]};
        for (int i = 0; i < 3; i++) {
            if (eye_pos[i] < 0) {
                center_pos[i] += -eye_pos[i];
                //up_pos[i] += -eye_pos[i];
                eye_pos[i] = 0;  // += -eye_pos[i];
                units[0]->position[i] = eye_pos[i];
            } else if (dist_to_end[i] > 0) {
                center_pos[i] -= dist_to_end[i];
                //up_pos[i] -= dist_to_end[i];
                eye_pos[i] -= dist_to_end[i];
                units[0]->position[i] = eye_pos[i];
            }
        }
    }

#ifdef DEBUG_VALUE
    printf("Translation vector: [%f %f %f]\n", x, y, z);
    printf("Char %d Position: %d %d %d\n",1,(int)units[0]->position[0],(int)units[0]->position[1],(int)units[0]->position[2]);
    printf("Object %d Position: %d %d %d\n",1,(int)items[0]->position[0],(int)items[0]->position[1],(int)items[0]->position[2]);
#endif
    print_pos();

    // Apply changes
    glPushAttrib(GL_MATRIX_MODE);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye_pos[0],eye_pos[1],eye_pos[2],
              center_pos[0],center_pos[1],center_pos[2],
              up_pos[0],up_pos[1],up_pos[2]);
    glPopAttrib();
}

/***
 * Rotating
 * This is a relative rotation, from the p.o.v. of the camera
 * TODO: use quaternion representation (This is a big task)
 ****/
void rotate_eye(GLdouble h, GLdouble v) {
#ifdef DEBUG_FUNC
    static int rotate_eye_count;
    rotate_eye_count++;
    printf("Rotate count: %i\n", rotate_eye_count);
#endif
#ifdef DEBUG_VALUE
    printf("Rotate by: %lf %lf\n", h, v);
#endif

    // Calculate the player's directional axis in terms of the reference axis
    GLdouble x_axis[3] = {0,0,0};
    GLdouble y_axis[3] = {0,0,0};
    GLdouble z_axis[3] = {0,0,0};
    get_rel_axis(x_axis, y_axis, z_axis);

    // Calculate the new center point relative to the eye
    GLdouble movement[3] = {0,0,0};
    // 1. Apply horizontal rotation:
    //    Relative direction vector is (-sin(h)*x_axis, 0, -cos(h)*z_axis)
    if (h) {
        movement[0] = -sin(h)*x_axis[0] - cos(h)*z_axis[0];
        movement[1] = -sin(h)*x_axis[1] - cos(h)*z_axis[1];
        movement[2] = -sin(h)*x_axis[2] - cos(h)*z_axis[2];
        //printf("Movement after horizontal rotation: [%lf %lf %lf]\n",movement[0],movement[1],movement[2]);
    }
    // 2. Apply vertical rotation:
    //    Relative direction vector is (0, sin(v)*y_axis, -cos(v)*z_axis)
    if (v) {
        movement[0] = sin(v)*y_axis[0] - cos(v)*z_axis[0];
        movement[1] = sin(v)*y_axis[1] - cos(v)*z_axis[1];
        movement[2] = sin(v)*y_axis[2] - cos(v)*z_axis[2];
        //printf("After vertical: [%lf %lf %lf]\n",movement[0],movement[1],movement[2]);
    }
    // 3. Normalize the result and scale it to previous eye to center distance
    //    This step isn't needed, but for sake of clarity in debugging purposes only
    normalize(movement,movement);
    GLdouble f[3] = {center_pos[0] - eye_pos[0], center_pos[1] - eye_pos[1], center_pos[2] - eye_pos[2]};  // the vector pointing forward
    GLdouble scale = get_norm(f,3);
    //printf("Movement norm before scaling: %lf\n",get_norm(movement,3));
    movement[0] *= scale;
    movement[1] *= scale;
    movement[2] *= scale;
    //printf("Movement norm after scaling: %lf\n",get_norm(movement,3));
    // These steps lead to an approximate rotation

    // Note the movement of the view
    if (h || v) { // as long as there is movement
        center_pos[0] = eye_pos[0] + movement[0];
        center_pos[1] = eye_pos[1] + movement[1];
        center_pos[2] = eye_pos[2] + movement[2];
    }
    // Note the movement of up
    if (v) {
        up_pos[0] = cos(v)*y_axis[0] + sin(v)*z_axis[0];
        up_pos[1] = cos(v)*y_axis[1] + sin(v)*z_axis[1];
        up_pos[2] = cos(v)*y_axis[2] + sin(v)*z_axis[2];
        normalize(up_pos,up_pos);
    }

#ifdef DEBUG_VALUE
    printf("New center relative to the eye: [%lf %lf %lf]\n",movement[0],movement[1],movement[2]);
    //printf("Angle, rotation vector: %f, %f %f %f\n", rangle, rvector[0], rvector[1], rvector[2]);
#endif
    print_pos();

    // Apply changes
    glPushAttrib(GL_MATRIX_MODE);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye_pos[0],eye_pos[1],eye_pos[2],
              center_pos[0],center_pos[1],center_pos[2],
              up_pos[0],up_pos[1],up_pos[2]);
    glPopAttrib();
}

/***
 * Generate a random 32*32 stipple pattern
 * TODO: make it random
 ****/
static GLubyte * getRandomStipple() {
    /*static GLubyte randomly[] = {
      0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
      0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
      0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
      0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
      0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
      0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
      0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
      0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
      0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
      0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
      0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
      0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
      0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
      0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
      0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
      0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55
    };*/
    /* stipple pattern explained:
        the pattern starts from the first byte at the lower left,
        then goes right, then keeps going right, then goes up.
        4 bytes make one column, and there are 32 rows.
        The bytes are ordered with the most significant bit on the left
        (big endian).
        Hopefully that made sense, if else visit
        http://www.glprogramming.com/red/chapter02.html
        (stipple pattern first appears in ch.2)
        Careful: bit ordering can be changed by calling glPixelStore*()
    */
    static GLubyte randomly[] = {
      0xB0, 0x00, 0x00, 0x03, // row 1 (from the bottom)
      0x60, 0x00, 0x00, 0x06,
      0x30, 0x00, 0x00, 0x0B,
      0x18, 0x00, 0x00, 0x18,
      0x0B, 0x00, 0x00, 0x30, // row 5
      0x06, 0x00, 0x00, 0x60,
      0x03, 0x00, 0x00, 0xB0,
      0x01, 0x80, 0x01, 0x80,
      0x00, 0xB0, 0x03, 0x00,
      0x00, 0x60, 0x06, 0x00, // row 10
      0x00, 0x30, 0x0B, 0x00,
      0x00, 0x18, 0x18, 0x00,
      0x00, 0x0B, 0x30, 0x00,
      0x00, 0x06, 0x60, 0x00,
      0x00, 0x03, 0xB0, 0x00,
      0x00, 0x01, 0x80, 0x00, // row 16
      0x00, 0x01, 0x80, 0x00,
      0x00, 0x03, 0xB0, 0x00,
      0x00, 0x06, 0x60, 0x00,
      0x00, 0x0B, 0x30, 0x00, // row 20
      0x00, 0x18, 0x18, 0x00,
      0x00, 0x30, 0x0B, 0x00,
      0x00, 0x60, 0x06, 0x00,
      0x00, 0xB0, 0x03, 0x00, // row 24
      0x01, 0x80, 0x01, 0x80,
      0x03, 0x00, 0x00, 0xB0,
      0x06, 0x00, 0x00, 0x60,
      0x0B, 0x00, 0x00, 0x30, // row 28
      0x18, 0x00, 0x00, 0x18,
      0x30, 0x00, 0x00, 0x0B,
      0x60, 0x00, 0x00, 0x06,
      0xB0, 0x00, 0x00, 0x03  // top row (row 32)
    };

    return randomly;
}

/***
 * Draw a cube with the given size,
 * mode is GL_FILL = use stipple pattern on faces
 * mode is GL_LINE(GL_SOLID) = use single colour on faces
 ****/
void draw_cube(GLenum mode, GLdouble size) {
    size /= 2;
    glPushAttrib(GL_POLYGON_BIT);  // save current polygon state

    if (mode != 0) glPolygonMode(GL_FRONT_AND_BACK, mode);
    if (mode == GL_FILL) {
        glEnable(GL_POLYGON_STIPPLE);
        glPolygonStipple(getRandomStipple());
    }
    glBegin(GL_QUADS);
        glVertex3f(-size,-size,size);  // Side 1 (front)
        glVertex3f(size,-size,size);
        glVertex3f(size,size,size);
        glVertex3f(-size,size,size);
        glVertex3f(-size,-size,size);  // Side 2
        glVertex3f(-size,size,size);
        glVertex3f(-size,size,-size);
        glVertex3f(-size,-size,-size);
        glVertex3f(-size,size,size);   // Side 3
        glVertex3f(size,size,size);
        glVertex3f(size,size,-size);
        glVertex3f(-size,size,-size);
        glVertex3f(size,size,size);    // Side 4
        glVertex3f(size,-size,size);
        glVertex3f(size,-size,-size);
        glVertex3f(size,size,-size);
        glVertex3f(size,-size,size);   // Side 5
        glVertex3f(-size,-size,size);
        glVertex3f(-size,-size,-size);
        glVertex3f(size,-size,-size);
        glVertex3f(-size,-size,-size); // Side 6 (back)
        glVertex3f(-size,size,-size);
        glVertex3f(size,size,-size);
        glVertex3f(size,-size,-size);
    glEnd();
    glDisable(GL_POLYGON_STIPPLE);

    glPopAttrib();
}

