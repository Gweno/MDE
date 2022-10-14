//-----------text.h-------------
#ifndef TEXT_H
#define TEXT_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>

#include "MDE.h"
#include "utils.h"

extern GLfloat r,g,b;

struct vertex3D {
    GLfloat x;
    GLfloat y;
    GLfloat z;
};

struct vertex3Dalpha {
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat a;
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

void init_color(vertex3Dalpha &color_alpha, vertex3Dalpha &bg_color_alpha, std::vector<vertex3Dalpha> &font_color_alpha, std::vector<vertex3D> &whd, std::vector<unsigned int> &nb_glyphs_per_text);
void init_vectors(std::vector<vertex3D> &vectors_boxes);

void user_input_bridge(MDE &user_input_MDE, vertex3D &origin, std::vector<vertex3D> &offset,
                    std::vector<vertex3D> &offset_rule, vertex2D &padding, vertex3Dalpha &color_alpha,
                    vertex3Dalpha &bg_color_alpha, std::vector<vertex3Dalpha> &font_color,
                    bool displayDataOnly, bool hideParent);

void displayV2D(std::vector<vertex2D> &vector_2D);
void displayV3D(std::vector<vertex3D> &vector_3D);
void displayV3Dalpha(std::vector<vertex3Dalpha> &vector_3Dalpha);
void displayVuint(std::vector<unsigned int> &vector_uint);
void displayVGLfloat(std::vector<GLfloat> &vector_glf);
void text_display();
void draw_box();
void text_frame(std::vector<vertex3D> &vector_input, std::vector<uint> &nb_glyphs, vertex3D &offset, vertex2D &padding);

void create_vector(std::vector<vertex3D> &vector3D,std::vector<vertex3D> &v_origin, std::vector<vertex3D> &vector_blf, std::vector<vertex3D> &vector_trb);
void create_coord_vector_text(const char *text, atlas * a, vertex3D &origin, vertex2D &window_coord_scale,
                                std::vector<vertex3D> &a_glyphs_box, std::vector<vertex2D> &a_glyphs_vertex);
void create_coord_vector_text_old(const char *text, atlas * a, vertex3D &origin, vertex2D &window_coord_scale);
void onIdle();
void camera();
void display();
void init_matrices();

void free_resources();

void process_v_MDE_data(std::vector<GLdata> & v_MDE_data,std::vector<vertex3D> & v_offset, std::vector<vertex3D> & v_offset_rule);

void keyboard(unsigned char key, int x, int y);
void special_key(int key, int x, int y);
void mouse_wheel(int button, int state, int x, int y);
void mouse_movement(int xpos, int ypos);
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

void set_options(bool move_it, bool display_box, bool display2d=false, bool singleColor=true, uint nlines=1);

void init_v_font_color(std::vector<vertex3Dalpha> &font_color, uint &nlines, bool singleColor=true, bool displayDataOnly=false);
void init_v_font_color(std::vector<vertex3Dalpha> &font_color,std::vector<vertex3Dalpha> &v_font_color, uint &nlines, bool singleColor, bool displayDataOnly);

//~ template <typename T>
//~ void displayVec(std::vector<T> &vect)

////
void onReshape(int width, int height);
int init_resources();
void init_color_box(vertex3Dalpha &color_alpha);
void onDisplay();
void init_vbo_box(std::vector<vertex3D> &vectors_boxes);
void init_ibo_elements_box(std::vector<vertex3D> &vectors_boxes);
void init_box(std::vector<vertex3D> &vectors_boxes);
void resetCameraMatrices();


void init_data();
void init_offset_rules();
void init_origin();
void init_padding();
void init_glyphs();
void init_frame();
void init_color_scheme();
//~ void render_text(atlas * a, const void * pointer);
void render_text(atlas * a, std::vector<vertex3Dalpha> &vector_font_color_part,
             std::vector<vertex3D> &glyphs_box_part, std::vector<vertex2D> &glyphs_vertex_part );
void render_text_old(atlas * a);

#endif /*text.h*/
