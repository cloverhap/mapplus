#include "database.h"
#include <stdio.h>
#include <iostream>
#include <fstream>

/***
 * Reads the character database
 * @pre The format for the database is:
 *      first non-comment, non-empty line is the heading
 *      ignore comments (//) and empty lines
 ****/
static int read_chars() {
    ifstream in;
    char l[MAX_DB_LINE_LENGTH];  // line being read
    //GLint field_count = 5;  // how many fields there are in a row, TODO: use for flexible db reading
    GLint counter = 0;

    // Try opening the file
    in.open("db\\chars\\chars.txt");
    if (in == 0) {
        exit_glut("Does character database exist?");
        return 1;
    }

    // read db into array
    while (!in.eof()) {
        in.getline(l,(streamsize)MAX_DB_LINE_LENGTH);
#ifdef DEBUG_VALUE
        //printf("%s", l);
#endif
        // Ignore comments and whitespaces
        if (l[0] == '\0' || l[0] == '\n' || l[0] == ' ' ||
            (l[0] == '/' && l[1] == '/') ||
            (l[0] == 'i' && l[1] == 'd'))
        {
            continue;
        }
        unit *p = new unit();
        if (p == 0) cerr << "Memory Allocation error. read_chars()" << endl;

        // Reading attributes
        sscanf(l, "%d %s %s %lf,%lf,%lf %d",
               &(p->id),
               p->name,
               p->tag,
               &(p->position[0]),&(p->position[1]),&(p->position[2]),
               &(p->map_id));
        units[counter++] = p;
#ifdef DEBUG_VALUE
        printf("Character ID: %d Name: %s Tag: %s Position: %d,%d,%d Map ID: %d\n",p->id,p->name,p->tag,(int)p->position[0],(int)p->position[1],(int)p->position[2],p->map_id);
#endif
    }
#ifdef DEBUG_VALUE
    //printf("Character Database parsed.\n");
#endif

    in.close();
    return 0;
}

static int read_maps() {
    ifstream in;
    char l[MAX_DB_LINE_LENGTH];  // line being read
    //GLint field_count = 5;  // how many fields there are in a row, TODO: use for flexible db reading
    GLint counter = 0;

    // Try opening the file
    in.open("db\\maps\\maps.txt");
    if (in == 0) {
        exit_glut("Does map database exist?");
        return 1;
    }

    // read db into array
    while (!in.eof()) {
        in.getline(l,(streamsize)MAX_DB_LINE_LENGTH);
#ifdef DEBUG_VALUE
        //printf("%s", l);
#endif
        // Ignore comments and whitespaces
        if (l[0] == '\0' || l[0] == '\n' || l[0] == ' ' ||
            (l[0] == '/' && l[1] == '/') ||
            (l[0] == 'i' && l[1] == 'd'))
        {
            continue;
        }

        area *a = new area();
        if (a == 0) fputs("Memory Allocation error. read_maps()\n", stderr);

        // Reading attributes
        sscanf(l, "%d %s %lf,%lf,%lf %d",
               &(a->id),
               a->name,
               &(a->dimension[0]),&(a->dimension[1]),&(a->dimension[2]),
               &(a->location));
        areas[counter++] = a;
#ifdef DEBUG_VALUE
        printf("Area ID: %d Name: %s Dimension: %d,%d,%d Location: %d\n",a->id,a->name,(int)a->dimension[0],(int)a->dimension[1],(int)a->dimension[2],a->location);
#endif
    }
#ifdef DEBUG_VALUE
    //printf("Map Database parsed.\n");
#endif

    in.close();
    return 0;
}

static int read_items() {
    ifstream in;
    char l[MAX_DB_LINE_LENGTH];  // line being read
    //GLint field_count = 5;  // how many fields there are in a row, TODO: use for flexible db reading
    GLint counter = 0;

    // Try opening the file
    in.open("db\\items\\items.txt");
    if (in == 0) {
        exit_glut("Does map database exist?");
        return 1;
    }

    // read db into array
    while (!in.eof()) {
        in.getline(l,(streamsize)MAX_DB_LINE_LENGTH);
#ifdef DEBUG_VALUE
        //printf("%s", l);
#endif
        // Ignore comments and whitespaces
        if (l[0] == '\0' || l[0] == '\n' || l[0] == ' ' ||
            (l[0] == '/' && l[1] == '/') ||
            (l[0] == 'i' && l[1] == 'd'))
        {
            continue;
        }

        item *o = new item();
        if (o == 0) fputs("Memory Allocation error. read_items()\n", stderr);

        // Reading attributes
        sscanf(l, "%d %s %lf,%lf,%lf %lf,%lf,%lf %d",
               &(o->id),
               o->name,
               &(o->dimension[0]),&(o->dimension[1]),&(o->dimension[2]),
               &(o->position[0]),&(o->position[1]),&(o->position[2]),
               &(o->map_id));
        items[counter++] = o;
#ifdef DEBUG_VALUE
        printf("Item ID: %d Name: %s Dimension: %d,%d,%d Position: %d,%d,%d Location: %d\n",o->id,o->name,(int)o->dimension[0],(int)o->dimension[1],(int)o->dimension[2],(int)o->position[0],(int)o->position[1],(int)o->position[2],o->map_id);
#endif
    }
#ifdef DEBUG_VALUE
    //printf("Item Database parsed.\n");
#endif

    in.close();
    return 0;
}

void read_db() {
#ifdef DEBUG_VALUE
    //char cur_dir[256];
    //GetCurrentDirectory(256, cur_dir);
    //printf("in read_db(): %s\n", cur_dir);
#endif
    GLint charsRead = read_chars();
    GLint mapsRead = read_maps();
    GLint itemsRead = read_items();

    if (charsRead || mapsRead || itemsRead) {
        exit_glut("Error reading database!");
    }
}
