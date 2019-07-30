//-----------text.h-------------
#ifndef TEXT_H
#define TEXT_H

#include <GL/glew.h>
#include <GL/freeglut.h>

extern GLfloat r,g,b;

bool init_font(uint uFontSize,const char *uFontFilename);
bool init_program();

/**
 * Render text using the currently loaded font and currently set font size.
 * Rendering starts at coordinates (x, y), z is always 0.
 * The pixel coordinates that the FreeType2 library uses are scaled by (sx, sy).
 */
void render_text(const char *text, float x, float y, float sx, float sy);
void init_background(const char *text, float x, float y);
void init_color(GLfloat r,GLfloat g,GLfloat b);
void init_cube(const char *text, GLfloat x, GLfloat y, GLfloat z);
//~ void init_cube(const char *text);
//~ void init_text(const char *text);
void init_text(const char *text, float x, float y, float z, float h_padding, float v_padding);
void init_text_coordinates(GLfloat x, GLfloat y, GLfloat z);
void draw_square();
void draw_cube();
void textDisplay();
void onIdle();
void free_resources();


#endif /*text.h*/
