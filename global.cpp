#include "global.h"
#include "gl_wrapper.h"
#include <iostream>
#include <stdio.h>
#include <math.h>

// global variables
GLdouble eye_pos[3];
GLdouble center_pos[3];
GLdouble up_pos[3];
GLdouble speed;          // moving speed
GLdouble angular_speed;  // rotating speed
unit* units[MAX_CHARS];  // list of characters (logic will need to be changed eventually to allow dynamic char creation)
area* areas[MAX_AREAS];  // same as above
item* items[MAX_ITEMS];  // ditto
BMP title_bg;            // title background
BMP title_bg_original;   // title background in original size
GLuint textures[1];       // custom texture IDs
string diag_message;     // Message to send to dialogue box
GLint game_mode;         // Current stage of the program
GLboolean goal_reached;

GLint kb_layout;  // Keyboard layout
GLint screen_size_x;
GLint screen_size_y;


GLint GLmax(GLint a, GLint b)
{
#ifdef DEBUG_FUNC
    static int max_count;
    max_count++;
    printf("Rotate count: %i\n", max_count);
#endif
    return a > b ? a : b;
}

void load_mode(GLint mode) {
    game_mode = mode;
    switch (mode) {
    case GAME_MODE:
        glClearColor(1.0,1.0,1.0,0.0);
        break;
    case TITLE_MODE:
        glClearColor(0.0,0.0,0.0,0.0);
        if (!title_bg.image) {
            load_bitmap((GLbyte*)"system\\Flickr-_The Digital Story_-3740187815_3fd65b938d_o.bmp", &title_bg_original);
            if (title_bg_original.image) {
                GLint w = min(screen_size_x,title_bg_original.width);
                GLint h = min(screen_size_y,title_bg_original.height);
                if ((title_bg.image = (GLubyte*)malloc(w*h*3*sizeof(GLubyte))) == 0) {
#ifdef DEBUG
                    cout << "Error allocating memory." << endl;
#endif
                } else if (gluScaleImage(GL_RGB,title_bg_original.width,title_bg_original.height,GL_UNSIGNED_BYTE,title_bg_original.image,
                                         w,h,GL_UNSIGNED_BYTE,title_bg.image)) {
#ifdef DEBUG
                    cout << "Error scaling title background image." << endl;
#endif
                } else {
                    title_bg.width = w;
                    title_bg.height = h;
                }
            }
#ifdef DEBUG
            else cout << "Title background image could not be loaded." << endl;
#endif
        }
        break;
    default:
        break;
    }
    return;
}

unsigned short read_short_int(FILE *f)
{
    unsigned short a,b;
    fread((char*)&a,2,1,f);
    b =  a & (0x00ff) << 8;
    b |= a & (0xff00) >> 8;
    return b;
}
unsigned long read_long_int(FILE *f)
{
    unsigned long a,b;
    fread((char*)&a,4,1,f);
    b =  a & (0x000000ff) << 24;
    b |= a & (0x0000ff00) << 8;
    b |= a & (0x00ff0000) >> 8;
    b |= a & (0xff000000) >> 24;
    return b;
}

/***
 * Load Bitmap image from path
 * @post Returns 0 on success, 1 on failure
 *       Loads the .bmp file into b (BMP) on success
 * WARNING/BUG: A windows only implementation because of
 *              BITMAPFILEHEADER and BITMAPINFOHEADER and the
 *              assumed format of the bitmap
 ****/
GLint load_bitmap(GLbyte* path, BMP* b) {
    FILE* in;
    BITMAPFILEHEADER header;
    BITMAPINFOHEADER info;

    // open file
    in = fopen((const char*)path,"rb");
    if (in == 0) {
#ifdef DEBUG
        printf("Cannot open %s\n",path);
#endif
        return 1;
    }

    // Read in the bitmap file header
    fread(&header, sizeof(BITMAPFILEHEADER), 1, in);

    // Verify that this is a bmp file
    if ((header.bfType & 0xff) != 'B' || (header.bfType & 0xff00) >> 8 != 'M') //" != 0x4D42)
    {
        fclose(in);
        return 1;
    }

    // Read in the bitmap info header
	fread(&info, sizeof(BITMAPINFOHEADER), 1, in);

	if (info.biClrUsed != 0 && info.biClrUsed > 0) {
        // if there's a colour table, it's too confusing - return with error
        return 1;
	} else {
        switch (info.biBitCount) {
        case 1:  // monochrome (1 bit per pixel)
        case 4:  // 16 colors (4 bits per pixel)
        case 8:  // 256 colors (byte per pixel)
            // too confusing - return with error
            return 1;
            break;
        default: // uses all colours
            break;
        }
	}

    // Check the compression
	if (info.biCompression != BI_RGB) {
	    // not the basic Windows bitmap format, too confusing - return with error
        return 1;
	}

	if ((b->image = (GLubyte*)malloc(info.biSizeImage)) == 0) {
        fclose(in);
#ifdef DEBUG
        printf("Error allocating memory.");
#endif
        return 1;
	}

    // read bitmap
    b->width = info.biWidth;
    b->height = info.biHeight;

    fseek(in,header.bfOffBits,SEEK_SET);
    fread((char*)b->image,info.biSizeImage,1,in);
    //for(int i = (b->height-1)*b->width; i >= 0; i -= b->width)
    //    for(int x = 0; x < b->width; x++)
    //        b->image[(GLuint)i+x] = (GLubyte)fgetc(in);

	if (!b->image) {
        fclose(in);
        return 1;
	}

    // Reorder BGR to RGB, assumes 3 bytes per pixel (info.biBitCount is 24)
	GLubyte tmp;
    for (GLuint i = 0; i < info.biSizeImage; i += 3) {
        tmp = b->image[i];
        b->image[i] = b->image[i + 2];
        b->image[i + 2] = tmp;
    }

	fclose(in);
	return 0;
}

/***
 * Load image and save as texture
 * @post returns 0 on success, 1 on failure
 ****/
GLint load_texture(GLbyte* path, GLint id) {
    BMP loaded;
    BMP scaled;

    if (load_bitmap(path, &loaded)) {
        cout << "Failed to load texture from " << path << endl;
        return 1;
    }

    scaled.width = pow(2,(int)log2(loaded.width)) + 2;
    scaled.height = pow(2,(int)log2(loaded.height)) + 2;
    if (!(scaled.image = (GLubyte*)malloc(scaled.width*scaled.height*3*sizeof(GLubyte)))) {
        cout << "Error allocating memory in load_texture." << endl;
        free(loaded.image);
        return 1;
    }
    if (gluScaleImage(GL_RGB,loaded.width,loaded.height,GL_UNSIGNED_BYTE,loaded.image,
                      scaled.width,scaled.height,GL_UNSIGNED_BYTE,scaled.image)) {
        cout << "Error scaling texture." << endl;
        return 1;
    }

    // http://www.gamedev.net/page/resources/_/technical/opengl/opengl-texture-mapping-an-introduction-r947
    glBindTexture(GL_TEXTURE_2D, id);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, scaled.width, scaled.height, 0, GL_RGB, GL_UNSIGNED_BYTE, scaled.image);

    return 0;
}
