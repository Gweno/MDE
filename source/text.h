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
 * ##name: init_size(uint uFontSize)
 * @param uFontSize is a unsigned integer
 * @return bool.
 * @see MDE().
 * @see ~MDE().
 * @exception No exception handling.
 * @brief Create an instance of class MDE and sets its public
 * member 'name' to standard string init_name, its public member
 * 'data' to standard string init_data and its private member 
 * vMDE, vector of pointers to instance of MDE, to the vector
 * init_vMDE, standard vector of pointer to instance of MDE.
 */
bool init_size(uint uFontSize, std::vector<int> &sizes);

 /**
 * 
 * ##name: init_font(const char *uFontFilename)
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
bool init_font(const char *uFontFilename);

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


 /**
 * 
 * ##name: init_vectors(std::vector<vertex3D> &vectors_boxes)
 * @param std::vector<vertex3D> &vectors_boxes, the vector that holds
 * @return Nothing is returned (void).
 * @exception No exception handling.
 * @brief This Method generates the index buffer for the boxes to be
 * displayed as background or frame of each text.
 * @note The name of this method is not good and should be replaced by
 * a better suited one, indeed there is no vectors initialised in this
 * method.
 */
void init_vectors();


 /**
 * 
 * ##name: init_color(colorRGBA &color_alpha,
                     colorRGBA &bg_color_alpha,
                     std::vector<colorRGBA> &font_color_alpha,
                     std::vector<vertex3D> &whd,
                     std::vector<unsigned int> &nb_glyphs_per_text,
                     std::vector<colorRGBA> &output_font_color)
 * @param colorRGBA &color_alpha,
 * @param colorRGBA &bg_color_alpha,
 * @param std::vector<colorRGBA> &font_color_alpha,
 * @param std::vector<vertex3D> &whd,
 * @param std::vector<unsigned int> &nb_glyphs_per_text,
 * @param std::vector<colorRGBA> &output_font_color)
 * @return Nothing is returned (void).
 * @see render_text()
 * @exception No exception handling.
 * @brief This Method sets color values for the openGL window background
 * color. It also calculates values 
 * generates the vertex buffer object vbo_color_box for the color box.
 * Finally it sets the font color for each glyphs into output_font_color
 * that will be used to generate the vbo ( @see render_text() ).
 * @note This method should be split in smaller steps each steps being
 * a specific method (e.g. font color, background color, box color,
 * generating vbo...).
 */
void init_color(colorRGBA &color_alpha, colorRGBA &bg_color_alpha,
                 std::vector<colorRGBA> &font_color_alpha, std::vector<vertex3D> &whd,
                 std::vector<unsigned int> &nb_glyphs_per_text, std::vector<colorRGBA> &output_font_color);

//~ void init_color_old(colorRGBA &color_alpha, colorRGBA &bg_color_alpha, std::vector<colorRGBA> &font_color_alpha, std::vector<vertex3D> &whd, std::vector<unsigned int> &nb_glyphs_per_text);


/**
 *##name: init_user_input(MDE &user_input_MDE, vertex3D &origin, std::vector<vertex3D> &offset,
 *                  std::vector<vertex3D> &offset_rule, vertex2D &padding, colorRGBA &color_alpha,
 *                  colorRGBA &bg_color_alpha, std::vector<colorRGBA> &font_color,
 *                  bool displayDataOnly, bool hideParent);
 * @param & user_input_MDE is a reference to a MDE object
 * @param & origin is a reference to a vertex3D object
 * @param & offset is a reference to a vector of vertex3D object
 * @param & offset_rule is a reference to a vector of vertex3D object
 * @param & origin is a reference to a vertex3D object
 * @param & origin is a reference to a vertex3D object
 * @return Nothing is returned (void).
 * @exception No exception handling.
 * @brief Set all relevant variables in the text.cpp global scope, from  
 * user input done within main.cpp.
 */

void init_user_input(MDE &user_input_MDE, vertex3D &origin, std::vector<vertex3D> &offset,
                    std::vector<vertex3D> &offset_rule, vertex2D &padding, colorRGBA &color_alpha,
                    colorRGBA &bg_color_alpha, std::vector<colorRGBA> &font_color,
                    bool displayDataOnly, bool hideParent, bool &user_parse_html);

/**
 *##name: init_offset_rules();
 * @return Nothing is returned (void).
 * @see process_v_MDE_data().
 * @brief This method is a wrapper without any argument, called from
 * main.cpp, for method process_v_MDE_data.
 */
void init_offset_rules();


/**
 *##name: init_origin();
 * @return Nothing is returned (void).
 * @brief This method loops through the offset vector and add in the
 * single coordinates of origin (x,y,z) of the Main MDE, storing
 * the result in v_origin, a vector of vertex3D that holds
 * the coordinates of MDE elements (to be displayed).
 */
void init_origin();

/**
 *##name: init_padding();
 * @return Nothing is returned (void).
 * @see add_padding().
 * @brief This method is a wrapper without any argument, called from
 * main.cpp, for method add_padding.
 */
void init_padding();

/**
 *##name: init_glyphs();
 * @see create_coord_vector_text().
 * @return Nothing is returned (void).
 * @brief This method firstly initiates a map of all characters for the
 * font and fontsize to be used, 
 * secondly this method loops through vector my_v_MDE_data calling method
 * create_coord_vector_text() to initialise two vectors:
 * glyphs_box and glyphs_vertex which are inputs for opengl and
 * gpu processing.
 */
void init_glyphs();

/**
 *##name: init_frame();
 * @return Nothing is returned (void).
 * @see text_frame().
 * @brief This method is a wrapper without any argument, called from
 * main.cpp, for method text_frame.
 */
void init_frame();

 /**
 * 
 * ##name: init_box(std::vector<vertex3D> &vectors_boxes)
 * @param  std::vector<vertex3D> &vectors_boxes,
 * @return Nothing is returned (void).
 * @see init_vbo_box();
 * @see init_ibo_elements_box();
 * @exception No exception handling.
 * @brief This method is to initialise the vertex and index buffers for
 * the frame boxes to be displayed in background of every text to be
 * displayed.
 */
void init_box(std::vector<vertex3D> &vectors_boxes);


 /**
 * 
 * ##name: init_vbo_box(std::vector<vertex3D> &vectors_boxes)
 * @param  std::vector<vertex3D> &vectors_boxes,
 * @return Nothing is returned (void).
 * @see init_box();
 * @see init_ibo_elements_box();
 * @exception No exception handling.
 * @brief This method is to initialise the vertex buffer object using 
 * the vector of vertex3D vector_boxes that holds all the coordinates
 * for boxes to be displayed behind the MDE text elements to be
 * displayed in the openGL context.
 */
void init_vbo_box(std::vector<vertex3D> &vectors_boxes);

 /**
 * 
 * ##name: init_ibo_elements_box(int size_of_vectors_boxes)
 * @param  int size_of_vectors_boxes,
 * @return Nothing is returned (void).
 * @see init_box();
 * @see init_vbo_box();
 * @exception No exception handling.
 * @brief This method is to initialise the index buffer object using the
 * size of the vector of vertex3D vector_boxes that holds all the
 * coordinates for boxes to be displayed behind the MDE text elements
 * to be displayed in the openGL context.
 */
void init_ibo_elements_box(int size_of_vectors_boxes);



 /**
 * 
 * ##name: init_color_scheme()
 * @return Nothing is returned (void).
 * @exception No exception handling.
 * @brief This method is to initialise the index buffer object using the
 * size of the vector of vertex3D vector_boxes that holds all the
 * coordinates for boxes to be displayed behind the MDE text elements
 * to be displayed in the openGL context.
 */
void init_color_scheme();

 /**
 * 
 * ##name: init_matrices()
 * @return Nothing is returned (void).
 * @exception No exception handling.
 * @brief This method is to initialise the uniform matrices for the text
 * to be displayed and for the corresponding background boxes.
 */
void init_matrices();

/** @} */ // end of group_init

/**
 *##name: create_coord_vector_text(const char *text, atlas * a, vertex3D &origin, vertex2D &window_coord_scale,
                                std::vector<vertex3D> &a_glyphs_box, std::vector<vertex2D> &a_glyphs_vertex);
 * @param const char *text, is the pointer to the text to be displayed
 * @param atlas * a, is a pointer to a structure atlas
 * @param vertex3D & origin, is a reference to a vector of Vertex3D vectors
 * @param vertex2D & window_coord_scale, ??
 * @param std::vector<vertex3D> & a_glyphs_box,
 * @param std::vector<vertex2D> &a_glyphs_vertex,
 * @return Nothing is returned (void).         
 * @see init_glyph()
 * @exception No exception handling.
 * @brief This method is called in a loop on 
 * std::vector<GLdata> my_v_MDE_data within init_glyph().
 * At each iteration a mde_member name or data of class std::string
 * is processed. During each of these iteration create_coord_vector_text
 * loops through each character of the string from first to last.
 * calculating vertices and texture coordinates respectively pushing
 * back in vectors glyphs_vertex and glyphs_box. It also pushes back
 * the number of glyphs per text in vector nb_glyphs_per_text.
 */
void create_coord_vector_text(const char *text, atlas * a, vertex3D &origin, vertex2D &window_coord_scale,
                                std::vector<vertex3D> &a_glyphs_box, std::vector<vertex2D> &a_glyphs_vertex);

/**
 *##name: text_frame(std::vector<vertex3D> &vector_input,
 *  std::vector<uint> &nb_glyphs, vertex3D &offset, vertex2D &padding);
 * @param std::vector<vertex3D> &vector_input,
 * @param std::vector<uint> &nb_glyphs,
 * @param vertex3D &offset,
 * @param vertex2D &padding,
 * @see init_frame().
 * @see create_vector().
 * @see init_vectors().
 * @see init_box().
 * @exception No exception handling.
 * @brief This method is to calculate the coordinates of each glyphs
 * to be displayed in vertex_3D a vector of vertex3D.
 * It first gets the min and max
 * values for each text element of the MDE to be displayed and stores
 * them in 2 vectors bottom_left_front and top_right_back.
 * It then uses these two vectors and calls create_vector()
 * to fill-in vector_input. Next in the process
 * it calls init_vectors() and then init_box() to initialise
 * GPU buffers for display of the text's backgound 3d box.
 */
void text_frame(std::vector<vertex3D> &vector_input,
     std::vector<uint> &nb_glyphs, vertex3D &offset, vertex2D &padding);


/**
 *##name: create_vector(std::vector<vertex3D> &vector3D,
    std::vector<vertex3D> &v_origin, std::vector<vertex3D> &vector_blf,
    std::vector<vertex3D> &vector_trb, vertex2D &padding;
 * @param std::vector<vertex3D> &vector3D, the vector where glyphs
 * coordinates are calculated.
 * @param std::vector<vertex3D> &vector_blf, the vector for bottom
 * left front corner of the box without padding,
 * @param std::vector<vertex3D> &vector_trb, the vector for the top
 * right back corner of the box without padding,
 * @param vertex2D &padding, the horizontal and vertical padding (x,y).
 * (! user padding is enter first vertical, second is horizontal,
 * to follow css order (top, right, bottom, left))
 * (! for now it's only symmetrical padding:
 * same padding value for left and right sides,
 * same padding value top and bottom sides)
 * @exception No exception handling.
 * @brief This method calculates the coordinates for the 3d boxes used
 * as background or frame for the MDE texts to be displayed.
 * It includes the padding values to create larger boxes to surround
 * texts.
 */
void create_vector(std::vector<vertex3D> &vector3D,
    std::vector<vertex3D> &v_origin, std::vector<vertex3D> &vector_blf,
    std::vector<vertex3D> &vector_trb, vertex2D &padding);


void displayV2D(std::vector<vertex2D> &vector_2D, uint log_level = 1);
void displayV3D(std::vector<vertex3D> &vector_3D, uint log_level = 1);
//~ void displayV3Dalpha(std::vector<vertex3Dalpha> &vector_3Dalpha);
void displayColorRGBA(std::vector<colorRGBA> &vector_rgba, uint log_level = 1);
void displayVGLfloat(std::vector<GLfloat> &vector_glf);
void text_display();
void draw_box();

void onIdle();
void camera();
void display();

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
 * @brief This method is calculating each text coordinate offset by 
 * adding the offse rules for members separation
 * (between name and data members), for distance between levels, and 
 * distance indexes of the same level of vMDE;
 */
void process_v_MDE_data(std::vector<GLdata> & v_MDE_data,std::vector<vertex3D> & v_offset, std::vector<vertex3D> & v_offset_rule);

void keyboard(unsigned char key, int x, int y);
void special_key(int key, int x, int y);
void mouse_wheel(int button, int state, int x, int y);
void mouse_movement(int xpos, int ypos);

 /**
 * 
 * ##name: add_padding(std::vector<vertex3D> & v_origin,
 * std::vector<vertex3D> & v_origin_fb, vertex2D & padding)
 * @param  & v_origin is a reference to a standard vector of vertex3D.
 * @param  & v_origin_fb,std is a reference to standard vector of vertex3D.
 * @param  & padding is a reference to a vertex2D.
 * @return Nothing is returned (void).
 * @exception No exception handling.
 * @brief This method reads the vector of VerteX3D v_origin and
 * for each x and y, the associated padding.
 * Then stores it in vector of Vertex3D v_origin_fb (fb for Frame Box)
 * so that the frame box coordinates include padding but not the text
 * coordinates parameters themselves.
 */
void add_padding(std::vector<vertex3D> & v_origin,
            std::vector<vertex3D> & v_origin_fb, vertex2D & padding);


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
void init_options(bool move_it, bool display_box, bool display2d=false, bool singleColor=true, uint nlines=1, uint log_level=0);

void init_v_font_color(std::vector<colorRGBA> &font_color,std::vector<colorRGBA> &v_font_color, uint &nlines, bool singleColor, bool displayDataOnly);

////
void onReshape(int width, int height);
void init_color_box(colorRGBA &color_alpha);
//~ void init_ibo_elements_box(std::vector<vertex3D> &vectors_boxes);
void resetCameraMatrices();

void render_text(atlas * a, std::vector<colorRGBA> &vector_font_color_part,
            std::vector<vertex3D> &glyphs_box_part,
            std::vector<vertex2D> &glyphs_vertex_part,
            int nbChars,
            int startIndex
            );

#endif /*text.h*/
