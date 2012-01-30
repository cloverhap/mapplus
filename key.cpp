#include <iostream>
#include "key.h"

/***
 * Key press callback
 ****/
void process_key(unsigned char key, int x, int y)
{
#ifdef DEBUG_KEY
    cout << "Pressed " << key << endl;
#endif
    if (kb_layout == KB_QWERTY) {
        switch (key) {
            case 27: // esc
                exit_glut("User quit the game.");
                break;
            case 'd':
                // move right
                translate_eye(speed,0.0,0.0);
                break;
            case 'a':
                // move left
                translate_eye(-speed,0.0,0.0);
                break;
            case 'w':
                // move forward
                translate_eye(0.0,0.0,-speed);
                break;
            case 's':
                // move back
                translate_eye(0.0,0.0,speed);
                break;
            case 'e':
                // rotate right
                rotate_eye(-angular_speed,0);
                break;
            case 'q':
                // rotate left
                rotate_eye(angular_speed,0);
                break;
            case 'W':
                // rotate up
                rotate_eye(0,angular_speed);
                break;
            case 'S':
                // rotate down
                rotate_eye(0,-angular_speed);
                break;
            default: break;
        }
    }
    else if (kb_layout == KB_DVORAK) {
        switch (key) {
            case 'q':
            case 27: // esc
                exit_glut("User quit the game.");
                break;
            case 'e':
                // move right
                translate_eye(speed,0.0,0.0);
                break;
            case 'a':
                // move left
                translate_eye(-speed,0.0,0.0);
                break;
            case ',':
                // move forward
                translate_eye(0.0,0.0,-speed);
                break;
            case 'o':
                // move back
                translate_eye(0.0,0.0,speed);
                break;
            case '.':
                // rotate right
                rotate_eye(-angular_speed,0);
                break;
            case '\'':
                // rotate left
                rotate_eye(angular_speed,0);
                break;
            case '<':
                // rotate up
                rotate_eye(0,angular_speed);
                break;
            case 'O':
                // rotate down
                rotate_eye(0,-angular_speed);
                break;
            case '+':
                // speed up
                speed += .25;
                angular_speed += PI/12.0;
                break;
            case '-':
                // slow down
                speed -= .1;
                angular_speed -= PI/24.0;
                if (speed < 0 || angular_speed < 0) {
                    speed = 0;
                    angular_speed = 0;
                }
                break;
            default: break;
        }
    }

    glutPostRedisplay();
}

/***
 * Special Key press callback
 ****/
void process_special_key(int key, int x, int y)
{
#ifdef DEBUG_KEY
    cout << "Pressed a special key" << endl;
#endif
    switch (key) {
        case GLUT_KEY_RIGHT:
            // move right
            translate_eye(speed,0.0,0.0);
            break;
        case GLUT_KEY_LEFT:
            // move left
            translate_eye(-speed,0.0,0.0);
            break;
        case GLUT_KEY_UP:
            // move up
            translate_eye(0.0,0.0,-speed);
            break;
        case GLUT_KEY_DOWN:
            // move down
            translate_eye(0.0,0.0,speed);
            break;
        case GLUT_KEY_F2:
            // TODO: set each Fn key to diff configurations of shapes/positions
            init();
            break;
        case GLUT_KEY_F12:
            if (kb_layout == KB_QWERTY) kb_layout = KB_DVORAK;
            else kb_layout = KB_QWERTY;
            break;
        default: break;
    }

    glutPostRedisplay();
}

/***
 * Mouse Action callback
 ****/
void process_mouse(int button, int state, int x, int y)
{
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN) {  // mouse clicked
            }
            break;
        default: break;
    }
}
