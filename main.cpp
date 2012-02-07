#include "global.h"
#include "gl_wrapper.h"
#include "key.h"
#include "database.h"
#include <iostream>
#include <math.h>

/***
 * Draw and Render
 ****/
static void draw_units() {
    for (int i = 0; units[i] != 0; i++) {
        unit *p1 = units[i];
        glPushMatrix();
        glTranslatef(p1->position[0],p1->position[1],p1->position[2]);
        glRotatef(0,0.0,0.0,1.0);
        glScalef(1.5,1.5,1.5);
        GLfloat clear_color[4];
        glGetFloatv(GL_COLOR_CLEAR_VALUE,clear_color);  // Colour it the background color (trying to make the unit invisible)
        glColor3fv(clear_color);
        draw_cube(GL_FILL,0.5);
        glPopMatrix();
    }
}

static void draw_items() {
    for (int i = 0; items[i] != 0; i++) {
        item *p1 = items[i];
        glPushMatrix();
        glTranslatef(p1->position[0],p1->position[1],p1->position[2]);
        glRotatef(0,0.0,0.0,1.0);
        glScalef(p1->dimension[0],p1->dimension[1],p1->dimension[2]);
        glColor3f(0.0,0.0,1.0);
        if (i == 0) glColor3f(1.0,0.0,0.0);
        draw_cube(GL_LINE,0.5);
        glPopMatrix();
    }
}

static void draw_diag() {
    game_mode = DIAG_MODE;
    // Make a rectangular dialogue area
    glBegin(GL_LINE_LOOP);
        glColor3f(0.0, 0.0, 0.0);
        glVertex2f(2.0, screen_size_y - screen_size_y/4.0 - 2.0);
        glVertex2f(screen_size_x - 2.0, screen_size_y - screen_size_y/4.0 - 2.0);
        glVertex2f(screen_size_x - 2.0, screen_size_y - 2.0);
        glVertex2f(2.0, screen_size_y - 2.0);
    glEnd();

    // Draw text at screen coordinates (100, 120), where (0, 0) is the top-left of the
    // screen in an 18-point Helvetica font
    glColor4f(0.5, 0.0, 0.8, 1.0);
    glRasterPos2f(7.0, screen_size_y - screen_size_y/4.0 + 12.0 - 2.0);  // RasterPos saves text colour
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)diag_message.c_str());
    //glPushMatrix();
    //glTranslatef(7.0, screen_size_y - screen_size_y/4.0 + 12.0 - 2.0, 0);
    //glutStrokeString(GLUT_STROKE_ROMAN, (const unsigned char*)diag_message.c_str());
    //glPopMatrix();
}

static void draw_title() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, screen_size_x, screen_size_y, 0.0, -1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);

    // Draw background for title page
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_CULL_FACE);
    glClear(GL_DEPTH_BUFFER_BIT);  // Clears depth buffer
    if (title_bg.image) {
        glRasterPos2f(0,screen_size_y - 1);
        glDrawPixels(title_bg.width,title_bg.height,GL_RGB,GL_UNSIGNED_BYTE,title_bg.image);
    }
    glPopMatrix();

    // Draw text on screen at (text_pos[0], text_pos[1]) where top-left is (0,0)
    // in an 18-point Helvetica font
    // TODO: dynamically load text here and center it
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_CULL_FACE);
    glClear(GL_DEPTH_BUFFER_BIT);  // Clears depth buffer
    GLfloat text_pos[2] = {screen_size_x/6.0, screen_size_y/2.0};
    GLint cur_line = 0;
    GLfloat line_size = 24;
    glColor3f(0.8, 0.9, 0.0);
    glRasterPos2f(text_pos[0],text_pos[1] + cur_line*line_size);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const GLubyte*)"Let's start with a simple goal!");
    cur_line++;
    glRasterPos2f(text_pos[0],text_pos[1] + cur_line*line_size);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const GLubyte*)"Goal: Walk toward the red rectangle and touch it.");
    cur_line++;
    glRasterPos2f(text_pos[0],text_pos[1] + cur_line*line_size);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const GLubyte*)"Keys: ");
    cur_line++;
    glRasterPos2f(text_pos[0],text_pos[1] + cur_line*line_size);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const GLubyte*)"WASD/Arrow keys: move left/right/forward/backward");
    cur_line++;
    glRasterPos2f(text_pos[0],text_pos[1] + cur_line*line_size);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const GLubyte*)"Q/E: strafe left/right");
    cur_line++;
    glRasterPos2f(text_pos[0],text_pos[1] + cur_line*line_size);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const GLubyte*)"Esc: Quit game");
    cur_line++;
    glRasterPos2f(text_pos[0],text_pos[1] + cur_line*line_size);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const GLubyte*)"");
    cur_line++;
    glRasterPos2f(text_pos[0],text_pos[1] + cur_line*line_size);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const GLubyte*)"Enter to continue.");
    cur_line++;
    glPopMatrix();

    // Making sure we can render 3D again
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

static void draw_HUD() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, screen_size_x, screen_size_y, 0.0, -1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glLoadIdentity();
    glDisable(GL_CULL_FACE);
    glClear(GL_DEPTH_BUFFER_BIT);  // Clears depth buffer

    if (diag_message != "")
        draw_diag();
    else if (game_mode == DIAG_MODE && diag_message == "")
        game_mode = GAME_MODE;

    // Making sure we can render 3D again
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

static void render_scene()
{
    glMatrixMode(GL_MODELVIEW);
    // clear all pixels to clear colour
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (game_mode == GAME_MODE || game_mode == DIAG_MODE) {
        // draw things
        draw_units();
        draw_items();

        // draw 2D HUD
        draw_HUD();
    } else if (game_mode == TITLE_MODE) {
        draw_title();
    }

    glFlush();  // draw graphics in buffer immediately
    glutSwapBuffers();  // switch buffers for double buffering
}

/***
 * Window Resize callback
 ****/
static void resize_scene(int w, int h)
{
    if (h == 0)
        h = 1;  // DON'T divide by zero - please?
    screen_size_x = w;
    screen_size_y = h;
    GLfloat ratio = 1.0 * w / h;
    glViewport(0,0,w,h);  // set the viewport to be the entire window

    // reset the coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // set the correct perspective
    gluPerspective(45,ratio,1,1000);
    //glFrustum(-ratio,ratio,-1.0,1.0,2.0,100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // perspective transformation
    // LookAt(eyeX,eyeY,eyeZ,             eye point
    //        centerX,centerY,centerZ,    reference point
    //        upX,upY,upZ)                "up" vector direction
    gluLookAt(eye_pos[0],eye_pos[1],eye_pos[2],
              center_pos[0],center_pos[1],center_pos[2],
              up_pos[0],up_pos[1],up_pos[2]);

    if (game_mode == TITLE_MODE) {
        // Change size of the title background
        // TODO: How to make resizing the title background not lose so much fidelity and be fast...?
        BMP tmp;
        tmp.width = w;
        tmp.height = h;
        if ((tmp.image = (GLubyte*)malloc(w*h*3*sizeof(GLubyte))) == 0) {
#ifdef DEBUG
            cout << "Error allocating memory." << endl;
#endif
        } else if (gluScaleImage(GL_RGB,title_bg.width,title_bg.height,GL_UNSIGNED_BYTE,title_bg.image,
                                 w,h,GL_UNSIGNED_BYTE,tmp.image)) {
#ifdef DEBUG
            cout << "Error scaling title background image." << endl;
#endif
        } else {
            delete title_bg.image;
            title_bg.image = tmp.image;
            title_bg.width = w;
            title_bg.height = h;
        }
    }

    glutPostRedisplay();
}

/***
 * Idle Function callback
 ****/
static void update_master() {
    int i;
    for (i = 0; units[i]; i++) {
        units[i]->update();
    }
    for (i = 0; areas[i]; i++) {
        areas[i]->update();
    }
    for (i = 0; items[i]; i++) {
        items[i]->update();
    }
    return;
}

static void check_goal() {
    if (game_mode == GAME_MODE && goal_reached == false &&
        (sqrt(pow(units[0]->position[0] - items[0]->position[0],2) +
              pow(units[0]->position[1] - items[0]->position[1],2) +
              pow(units[0]->position[2] - items[0]->position[2],2)
              )
         < 5)) {
        diag_message = "You have reached the goal!";
        diag_message += "\nPress [Space] or [Enter] to continue.";
        diag_message += "\nYou can always press [Esc] to exit.";
        goal_reached = true;
    }
}

static void idle_event()
{
    update_master();
    check_goal();
    glutPostRedisplay();
}

/***
 * Initialization
 ****/
void init()
{
    // Read Database on initialization
    read_db();

    // OpenGL values
    glClearColor(1.0,1.0,1.0,0.0);
    //glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    /*
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Ortho(left,right,bottom,top,nearClipDepth,farClipDepth)
    glOrtho(-50.0,50.0,-50.0,50.0,-1.0,10.0);
    */

    // set up perspective
    // scene values
    eye_pos[0] = units[0]->position[0];     // eye is here
    eye_pos[1] = units[0]->position[1];
    eye_pos[2] = units[0]->position[2];
    center_pos[0] = eye_pos[0];  // looking at the direction of this point
    center_pos[1] = eye_pos[1];
    center_pos[2] = eye_pos[2] - 5;
    up_pos[0] = 0.0;  // up vector
    up_pos[1] = 1.0;
    up_pos[2] = 0.0;

    glPushAttrib(GL_MATRIX_MODE);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // LookAt(eyeX,eyeY,eyeZ,             eye point(camera)
    //        centerX,centerY,centerZ,    reference point (where to look)
    //        upX,upY,upZ)                "up" vector direction
    gluLookAt(eye_pos[0],eye_pos[1],eye_pos[2],
              center_pos[0],center_pos[1],center_pos[2],
              up_pos[0],up_pos[1],up_pos[2]);
    glPopAttrib();

    speed = 0.5;
    angular_speed = (1.0/12.0)*PI;
    title_bg.image = 0;
    title_bg.width = screen_size_x;
    title_bg.height = screen_size_y;
    diag_message = "";
    goal_reached = false;

    kb_layout = KB_QWERTY;
    load_mode(TITLE_MODE);
}

/***
 * exit_glut
 * Takes care of cleanup and other exiting procedures as necessary
 * and exits the main loop
 ****/
void exit_glut(const char* exit_message) {
    int i;
    for (i = 0; units[i] != NULL; i++) {
        delete units[i];
    }
    for (i = 0; areas[i] != NULL; i++) {
        delete areas[i];
    }
    for (i = 0; items[i] != NULL; i++) {
        delete items[i];
    }

    delete title_bg.image;

    cout << exit_message << endl;
#ifndef DEBUG
    // BUG: release build crashes after exit_glut() without an exit() statement
    exit(1);
#endif
    glutLeaveMainLoop();
}

int main(int argc, char *argv[])
{
    // initialize (free)GLUT/OpenGL
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    GLint init_win_size_x = 640, init_win_size_y = 480;
    screen_size_x = init_win_size_x;
    screen_size_y = init_win_size_y;
    glutInitWindowSize(init_win_size_x, init_win_size_y);
    GLint init_win_pos_x = (glutGet(GLUT_SCREEN_WIDTH) - init_win_size_x) / 2;
    GLint init_win_pos_y = (glutGet(GLUT_SCREEN_HEIGHT) - init_win_size_y) / 2;
    cout << "Screen Size: " << glutGet(GLUT_SCREEN_WIDTH) << " " << glutGet(GLUT_SCREEN_HEIGHT) << endl;
    cout << "Window Size: " << init_win_size_x << " " << init_win_size_y << endl;
    cout << "Window Coords: " << init_win_pos_x << " " << init_win_pos_y << endl;
    glutInitWindowPosition(init_win_pos_x, init_win_pos_y);
    GLint window_id = glutCreateWindow("Walking Around");

    init();

    // register callbacks
    glutDisplayFunc(render_scene);
    glutReshapeFunc(resize_scene);
    glutKeyboardFunc(process_key);
    glutSpecialFunc(process_special_key);
    glutMouseFunc(process_mouse);
    glutIdleFunc(idle_event);

    glutMainLoop();
    glutDestroyWindow(window_id);

    //system("PAUSE");
    // TODO: doesn't reach the manually made "Press any key to continue!" ...why?
    char tmp;
    cout << "Press any key to continue!." << endl;
    cin >> tmp;
    return 0;
}

