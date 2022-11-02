//-----------text.h-------------
#ifndef TEXT_H
#define TEXT_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>

#include "MDE.h"
#include "utils.h"

extern GLfloat r,g,b;

/**
 * 
 * @file text.h
 * 
*/

/**

##name vertex2D
 * @param x a GLfloat for coordinate on the x axis.
 * @param y a GLfloat for coordinate on the y axis.
 * @brief Define a structure of an object with coordinate in 2 dimensions: x,y.
 * @see vertex3D
 */

struct vertex2D {
    GLfloat x;
    GLfloat y;
};

/**

##name vertex3D
 * @param x a GLfloat for coordinate on the x axis.
 * @param y a GLfloat for coordinate on the y axis.
 * @param z a GLfloat for coordinate on the z axis.
 * @brief Define a structure of an object with coordinate in 3 dimensions: x,y,z.
 * @see vertex2D 
 */

struct vertex3D {
    GLfloat x;
    GLfloat y;
    GLfloat z;
};

/**

##name colorRGBA
 * @param r a GLfloat for the value of red color between 0 and 1.
 * @param g a GLfloat for the value of green color between 0 and 1.
 * @param b a GLfloat for the value of blue color between 0 and 1.
 * @param a a GLfloat for the value of alpha transparency a.
 * @brief Define a structure of an object with coordinate in 4 dimensions: x,y,z,a.
 * Used for an object of 3 dimesnions with an 
 * @see vertex2D 
 */
 
struct colorRGBA {
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;
};

struct atlas;

//~ /**

//~ ##name point
 //~ * @param r a GLfloat for the value of red color between 0 and 1.
 //~ * @param g a GLfloat for the value of green color between 0 and 1.
 //~ * @param b a GLfloat for the value of blue color between 0 and 1.
 //~ * @param a a GLfloat for the value of alpha transparency a.
 //~ * @brief Define a structure of an object with coordinate in 4 dimensions: x,y,z,a.
 //~ * Used for an object of 3 dimesnions with an 
 //~ * @see vertex2D 
 //~ */
//~ struct point {
	//~ GLfloat x;
	//~ GLfloat y;
	//~ GLfloat s;
	//~ GLfloat t;
//~ };

//~ struct point3D {
	//~ GLfloat x;
	//~ GLfloat y;
	//~ GLfloat z;
	//~ GLfloat s;
	//~ GLfloat t;
//~ };

/** @defgroup group_init Initialisation methods
 * Methods to initialise various relevant variables, vectors and containers
 * before them being processed within text.cpp
 *  @{
 */

/** ##name: init_data()
 * @return Nothing is returned (void).
 * @exception No exception handling.
 * @see extractMDEdataForVoffset().
 * @brief This method is a wrapper, and is used to call
 * extractMDEdataForVoffset without any arguments passed from 
 * the main.cpp scope, to avoid 'out of scope' issues. It initialise
 * and declare 2 local integer variables: level and index, that 
 * are arguments in extractMDEdataForVoffset. All other arguments passed
 * to extractMDEdataForVoffset are in the utils.cpp global scope.
 * 
*/

void init_data();


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

//~ void init_color(vertex3Dalpha &color_alpha, vertex3Dalpha &bg_color_alpha,
                 //~ std::vector<vertex3Dalpha> &font_color_alpha, std::vector<vertex3D> &whd,
                 //~ std::vector<unsigned int> &nb_glyphs_per_text, std::vector<vertex3Dalpha> &output_font_color);
void init_color(colorRGBA &color_alpha, colorRGBA &bg_color_alpha,
                 std::vector<colorRGBA> &font_color_alpha, std::vector<vertex3D> &whd,
                 std::vector<unsigned int> &nb_glyphs_per_text, std::vector<colorRGBA> &output_font_color);

void init_color_old(colorRGBA &color_alpha, colorRGBA &bg_color_alpha, std::vector<colorRGBA> &font_color_alpha, std::vector<vertex3D> &whd, std::vector<unsigned int> &nb_glyphs_per_text);
void init_vectors(std::vector<vertex3D> &vectors_boxes);

//~ void init_user_input(MDE &user_input_MDE, vertex3D &origin, std::vector<vertex3D> &offset,
                    //~ std::vector<vertex3D> &offset_rule, vertex2D &padding, vertex3Dalpha &color_alpha,
                    //~ vertex3Dalpha &bg_color_alpha, std::vector<vertex3Dalpha> &font_color,
                    //~ bool displayDataOnly, bool hideParent);
/**
 *##name: init_user_input(MDE &user_input_MDE, vertex3D &origin, std::vector<vertex3D> &offset,
 *                  std::vector<vertex3D> &offset_rule, vertex2D &padding, colorRGBA &color_alpha,
 *                  colorRGBA &bg_color_alpha, std::vector<colorRGBA> &font_color,
 *                  bool displayDataOnly, bool hideParent);
 * @return Nothing is returned (void).
 * @param & user_input_MDE is a reference to a MDE object
 * @param & origin is a reference to a vertex3D object
 * @param & offset is a reference to a vector of vertex3D object
 * @param & offset_rule is a reference to a vector of vertex3D object
 * @param & origin is a reference to a vertex3D object
 * @param & origin is a reference to a vertex3D object
 * @exception No exception handling.
 * @brief Set all relevant variables in the text.cpp global scope, from  
 * user input done within main.cpp.
 */

void init_user_input(MDE &user_input_MDE, vertex3D &origin, std::vector<vertex3D> &offset,
                    std::vector<vertex3D> &offset_rule, vertex2D &padding, colorRGBA &color_alpha,
                    colorRGBA &bg_color_alpha, std::vector<colorRGBA> &font_color,
                    bool displayDataOnly, bool hideParent);



/**
 *##name: init_offset_rules();
 * @return Nothing is returned (void).
 * @see process_v_MDE_data().
 * @brief This method is a wrapper without any argument, called from
 * main.cpp, for method process_v_MDE_data.
 */
void init_offset_rules();
/** @} */ // end of group_init



void displayV2D(std::vector<vertex2D> &vector_2D);
void displayV3D(std::vector<vertex3D> &vector_3D);
//~ void displayV3Dalpha(std::vector<vertex3Dalpha> &vector_3Dalpha);
void displayColorRGBA(std::vector<colorRGBA> &vector_rgba);
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

 /**
 * 
 * ##name: process_v_MDE_data(std::vector<GLdata> & v_MDE_data,
 * std::vector<vertex3D> & v_offset,
 * std::vector<vertex3D> & v_offset_rule)
 * @param &v_MDE_data is a reference to a standard vector of
 * struct GLdata.
 * @param  & v_offset is a reference to a standard vector of vertex3D.
 * @param  & v_offset_rule is a reference to a standard vector of
 * vertex3D.
 * @return Nothing is returned (void).
 * @exception No exception handling.
 * @brief This method continue to build my_v_MDE_data vector og GLdata,
 * and also calculating each text coordinate by adding the offset rule....
 */
void process_v_MDE_data(std::vector<GLdata> & v_MDE_data,std::vector<vertex3D> & v_offset, std::vector<vertex3D> & v_offset_rule);

void keyboard(unsigned char key, int x, int y);
void special_key(int key, int x, int y);
void mouse_wheel(int button, int state, int x, int y);
void mouse_movement(int xpos, int ypos);

 /**
 * 
 * ##name: add_padding(std::vector<vertex3D> & v_origin,std::vector<vertex3D> & v_origin_fb, vertex2D & padding)
 * @param  & v_origin is a reference to a standard vector of vertex3D.
 * @param  & v_origin_fb,std is a reference to standard vector of vertex3D.
 * @param  & padding is a reference to a vertex2D.
 * @return Nothing is returned (void).
 * @exception No exception handling.
 * @briefThis method reads the vector of VerteX3D v_origin and for each x and y, the associated padding.
 * Then stores it in vector of Vertex3D v_origin_fb (fb for Frame Box) so that the frame box
 * coordinates include padding but not the text coordinates themselves.
 * parameters. 
 */
void add_padding(std::vector<vertex3D> & v_origin,std::vector<vertex3D> & v_origin_fb, vertex2D & padding);


void init_font_color(std::vector<GLfloat> &color);

 /**
 * 
 * ##name: init_options(bool move_it, bool display_box, bool display2d=false, bool singleColor=true, uint nlines=1)
 * @param  move_it is a boolean, a test feature if set to true makes the whole MDE instance to move.
 * @param  display_box is a boolean to turn on the frame box display as background for the displayed text.
 * @param  display2d is a boolean by default set to false, to set the camera in 2d ortho view.
 * @param  singleColor is a boolean y default set to true is used to bypass anycolor settings and use only 1 color to display the MDE instance.
 * @param  nlines is a unsigned integer by default set to 1, holds arbitrarily the number of 'lines' to display.
 * @return Nothing is returned (void).
 * @see onIdle();
 * @exception No exception handling.
 * @brief set options entered by user through runtime arguments.
 */
void init_options(bool move_it, bool display_box, bool display2d=false, bool singleColor=true, uint nlines=1);

//~ void init_v_font_color_old(std::vector<vertex3Dalpha> &font_color, uint &nlines, bool singleColor=true, bool displayDataOnly=false);
//~ void init_v_font_color(std::vector<vertex3Dalpha> &font_color,std::vector<vertex3Dalpha> &v_font_color, uint &nlines, bool singleColor, bool displayDataOnly);
void init_v_font_color(std::vector<colorRGBA> &font_color,std::vector<colorRGBA> &v_font_color, uint &nlines, bool singleColor, bool displayDataOnly);

//~ template <typename T>
//~ void displayVec(std::vector<T> &vect)

////
void onReshape(int width, int height);
int init_resources();
//~ void init_color_box(vertex3Dalpha &color_alpha);
void init_color_box(colorRGBA &color_alpha);
void onDisplay();
void init_vbo_box(std::vector<vertex3D> &vectors_boxes);
void init_ibo_elements_box(std::vector<vertex3D> &vectors_boxes);
void init_box(std::vector<vertex3D> &vectors_boxes);
void resetCameraMatrices();




void init_origin();
void init_padding();
void init_glyphs();
void init_frame();
void init_color_scheme();
//~ void render_text(atlas * a, const void * pointer);
//~ void render_text(atlas * a, std::vector<vertex3Dalpha> &vector_font_color_part,
             //~ std::vector<vertex3D> &glyphs_box_part, std::vector<vertex2D> &glyphs_vertex_part );
void render_text(atlas * a, std::vector<colorRGBA> &vector_font_color_part,
             std::vector<vertex3D> &glyphs_box_part, std::vector<vertex2D> &glyphs_vertex_part );
void render_text_old(atlas * a);

#endif /*text.h*/
