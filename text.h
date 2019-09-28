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

/**
 * Render text using the currently loaded font and currently set font size.
 * Rendering starts at coordinates (x, y), z is always 0.
 * The pixel coordinates that the FreeType2 library uses are scaled by (sx, sy).
 */
void render_text(const char *text, float x, float y, float sx, float sy);
void render_text_Z(const char *text, float x, float y, float z, float sx, float sy);
void render_text2(atlas * a) ;
void render_text3(atlas * a) ;
void render_text4(atlas * a) ;
void init_background(const char *text, float x, float y);
void init_color(GLfloat r,GLfloat g,GLfloat b);
void init_color2();
void init_color3();
void init_color4(GLfloat r,GLfloat g,GLfloat b);
void init_color5(vertex3D color, std::vector<vertex3D> &whd);
void init_cube(const char *text, GLfloat x, GLfloat y, GLfloat z);
void init_vectors();
//~ void init_vectors2(std::vector<vertex3D> &vertex_3D);
void init_vectors2();
void init_vectors3(std::vector<vertex3D> &vectors_boxes);
void init_box();
void init_struct();
int init_resources_test();
int init_program_test();
std::vector<vertex3D> create_vector3D(std::vector<vertex3D> &vector_text_whd, vertex3D &first_vector3D);
std::vector<vertex3D> create_vector3D2(std::vector<vertex3D> &vector_text_whd, vertex3D &first_vector3D, vertex3D &offset);
void create_vector3D3(std::vector<vertex3D> &vector3D, std::vector<vertex3D> &vector_text_whd, vertex3D &first_vector3D, vertex3D &offset);
void create_vector3D4(std::vector<vertex3D> &vector3D,std::vector<vertex3D> &v_origin, std::vector<vertex3D> &vector_text_whd);
void create_coord_vector_text4(const char *text, atlas * a, vertex3D origin, vertex2D &window_coord_scale);
void create_coord_vector_text3(const char *text, atlas * a, float x, float y, float z, float sx, float sy);
void create_coord_vector_text2(const char *text, atlas * a, float x, float y, float z, float sx, float sy);
void create_coord_vector_text(const char *text, atlas * a, float x, float y, float z, float sx, float sy);
void create_coord_vector(const char *text, atlas * a, float x, float y, float sx, float sy);
//~ void displayV3D(std::vector<vertex3D> &vector_3D);
void displayV3D(std::vector<vertex3D> &vector_3D);
void testing_vectorshw();
//~ void init_cube(const char *text);
//~ void init_text(const char *text);
void init_text(const char *text, float x, float y, float z, float h_padding, float v_padding);
void init_text_Entity(E &my_entity,float x, float y, float z, float h_padding, float v_padding);
void init_text_Entity2(E &my_entity,vertex3D &origin, vertex3D &offset, vertex2D &padding);
void init_text_Entity3(E &my_entity,vertex3D &origin, std::vector<vertex3D> &offset, vertex2D &padding, vertex3D color);
void init_text_coordinates(GLfloat x, GLfloat y, GLfloat z);
void init_testing();
//~ void draw_square();
void draw_cube();
void draw_box();
void draw_box2();
void textDisplay();
//~ vertex2D text_frame(std::vector<vertex3D> &vector_input);
void text_frame(std::vector<vertex3D> &vector_input);
void text_frame2(std::vector<vertex3D> &vector_input, uint &nb_indices2);
void text_frame3(std::vector<vertex3D> &vector_input, std::vector<uint> &nb_glyphs, vertex3D &offset);
void onIdle();
void free_resources();
void free_resources_test();

void glm_vector(std::vector<vertex3D> &vector_text_whd, vertex3D &first_vector3D);


#endif /*text.h*/
