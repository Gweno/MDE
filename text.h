//-----------text.h-------------
#ifndef TEXT_H
#define TEXT_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>

#include "E.h"

extern GLfloat r,g,b;

struct vertex3D {
    GLfloat x;
    GLfloat y;
    GLfloat z;
};

struct vertex2D {
    GLfloat x;
    GLfloat y;
};

struct atlas;

bool init_font(uint uFontSize,const char *uFontFilename);
bool init_program();
void render_text(atlas * a) ;
void init_color(vertex3D color, std::vector<vertex3D> &whd);
void init_vectors(std::vector<vertex3D> &vectors_boxes);
void create_vector(std::vector<vertex3D> &vector3D,std::vector<vertex3D> &v_origin, std::vector<vertex3D> &vector_text_whd);
void create_coord_vector_text(const char *text, atlas * a, vertex3D origin, vertex2D &window_coord_scale);
void displayV3D(std::vector<vertex3D> &vector_3D);
void init_text_Entity(E &my_entity,vertex3D &origin, std::vector<vertex3D> &offset, vertex2D &padding, vertex3D color);
void init_text_coordinates(GLfloat x, GLfloat y, GLfloat z);
void textDisplay();
void text_frame(std::vector<vertex3D> &vector_input, std::vector<uint> &nb_glyphs, vertex3D &offset);
void onIdle();
void free_resources();
void draw_box();

void process_v_E_data(std::vector<GLdata> & v_E_data,std::vector<vertex3D> & v_offset);

#endif /*text.h*/
