//-----------text.h-------------
#ifndef TEXT_H
#define TEXT_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>

#include "MDE.h"

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

 /**
 * 
 * ##name: init_font(uint uFontSize,const char *uFontFilename)
 * @param init_name is a standard string
 * @param init_data is a standard string
 * @param init_vMDE is a standard vector of pointers to instances 
 * of object of class MDE.
 * @return Nothing is returned (void).
 * @see MDE().
 * @see ~MDE().
 * @exception No exception handling.
 * @brief Create an instance of class MDE and sets its public
 * member 'name' to standard string init_name, its public member
 * 'data' to standard string init_data and its private member 
 * vMDE, vector of pointers to instance of MDE, to the vector
 * init_vMDE, standard vector of pointer to instance of MDE.
 */
bool init_font(uint uFontSize,const char *uFontFilename);

 /**
 * 
 * ##name: init_program()
 * @return Nothing is returned (void).
 * @see MDE().
 * @see ~MDE().
 * @exception No exception handling.
 * @brief Initialize and create the GLSL shader programs and their
 * parameters. 
 */
bool init_program();

void init_color(vertex3D color, std::vector<vertex3D> &whd);
void init_vectors(std::vector<vertex3D> &vectors_boxes);
void init_text_MDE(MDE &my_entity,vertex3D &origin, std::vector<vertex3D> &offset, std::vector<vertex3D> &offset_rule, vertex2D &padding, vertex3D color);
void init_text_coordinates(GLfloat x, GLfloat y, GLfloat z);

void displayV3D(std::vector<vertex3D> &vector_3D);
void textDisplay();
void draw_box();
void text_frame(std::vector<vertex3D> &vector_input, std::vector<uint> &nb_glyphs, vertex3D &offset, vertex2D &padding);
void render_text(atlas * a) ;

void create_vector(std::vector<vertex3D> &vector3D,std::vector<vertex3D> &v_origin, std::vector<vertex3D> &vector_blf, std::vector<vertex3D> &vector_trb);
void create_coord_vector_text(const char *text, atlas * a, vertex3D origin, vertex2D &window_coord_scale);
void onIdle();
void free_resources();

void process_v_MDE_data(std::vector<GLdata> & v_MDE_data,std::vector<vertex3D> & v_offset, std::vector<vertex3D> & v_offset_rule);

void keyDown(unsigned char key, int x, int y);
 
 /**
 * 
 * ##name: add_padding(std::vector<vertex3D> & v_origin,std::vector<vertex3D> & v_origin_fb, vertex2D & padding)
 * @param  & v_origin is a reference to a stadnard vector of vertex3D.
 * @param  & v_origin_fb,std is a reference to standard vector of vertex3D.
 * @param  & & padding is a reference to a vertex2D.
 * @return Nothing is returned (void).
 * @exception No exception handling.
 * @brief read the vector of VerteX3D v_origin and for each x and y, the associated padding.
 * Then stores it in vector of Vertex3D v_origin_fb (fb for Frame Box) so that the frame box
 * coordinates include padding but not the text coordinates themselves.
 * parameters. 
 */
void add_padding(std::vector<vertex3D> & v_origin,std::vector<vertex3D> & v_origin_fb, vertex2D & padding);


void init_font_color(std::vector<GLfloat> &color);
//~ void init_font_color(std::vector<GLfloat> color);


void get_move_it(bool move_it);

#endif /*text.h*/
