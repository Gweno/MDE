// Testing font texturing
// In this program text, there are 2 shader object programs
// One is to render a text input by user.
// The other one is a colored background for the rendered text.
// Both shaders are independant from each others but share
// a same input which is a transformation matrix.
// So the text and the colored background can move together.

#include "text.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <array>
#include <math.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "./common/shader_utils.h"

#include "utils.h"
#include "MDE.h"

int screen_width=800, screen_height=600;

bool ortho_mode=false;
bool display3d=false;

// programs
GLuint program_text;
GLuint program_box;
// testing 2d
GLuint program_text2d;


// uniforms
GLint uniform_tex_text;
GLint uniform_color_text;
GLint uniform_bgcolor_text;
GLint uniform_m_transform;
GLint uniform_m_view;
GLint uniform_m_model;
GLint uniform_m_projection;
GLint uniform_m_camera;
GLint uniform_m_view_b;
GLint uniform_m_model_b;
GLint uniform_m_projection_b;
// testing 2d
GLint uniform_m_projection2d;
GLint uniform_m_model2d;
GLint uniform_color;
GLint uniform_tex2d;
GLint uniform_bgcolor_text2d;
// testing box bug

///////////

GLuint vbo_cube_vertices, vbo_cube_colors;
GLuint ibo_cube_elements;
GLuint program;
GLint attribute_coord3d, attribute_v_color;
GLint uniform_mvp;

GLint uniform_mvp_box;


////////////


// attributes
GLint attribute_coord_text;
GLint attribute_coord;
GLint attribute_tex_coord;
GLint attribute_coord3d_box;
GLint attribute_v_color_box;
GLint attribute_font_color;
// testing 2d
GLint attribute_coord2d;
GLint attribute_coord_text2d;
GLint attribute_tex_coord2d;
GLint attribute_font_color2d;

// vbo
GLuint vbo_coord;
GLuint vbo_tex_coord;
GLuint vbo_vertices_box;
GLuint vbo_color_box;
GLuint vbo_font_color;
// testing 2d
//~ GLuint vbo;
GLuint vbo_coord2d;
GLuint vbo_tex_coord2d;
GLuint vbo_font_color2d;

// ibo
GLuint ibo_box_elements;

const char *inputText  = nullptr;
//~ uint fontSize=48;
uint fontSize=16;
GLfloat r,g,b,a;
GLfloat bg_r = 1, bg_g = 1, bg_b = 1, bg_a =1;
//~ bg_r = 1;
//~ bg_g = 1;
//~ bg_b = 1;
//~ bg_a = 1;

float x_pos = 0, y_pos = 0, z_pos = 0, x_rot = 0, y_rot = 0;
float lastx, lasty;
float multiply = 4.0f;
float unit = 1;

// for testing
float scale = 0.001f;

vertex3D text_coordinates ={0.0,0.0,0.0};
vertex2D padding ={0.0,0.0};

FT_Library ft;
FT_Face face;
FT_Face face2;

const char *fontfilename  = nullptr;
const char *fontfilename2 = "../fonts/FreeSansBold.ttf";

// Maximum texture width
#define MAXWIDTH 1024



// global variable for glyphs and their coordinates, to be used in the bridge method 

MDE *my_entity = nullptr;
vertex3D *origin = nullptr;
std::vector<vertex3D> *offset = nullptr;
std::vector<vertex3D> *offset_rule = nullptr;
colorRGBA *color_alpha = nullptr;
colorRGBA *bg_color_alpha = nullptr;

// Vectors color, font size and nb chars
std::vector<colorRGBA> *font_color  = nullptr;
std::vector<int> *font_size  = nullptr;
//~ std::vector<colorRGBA> *font_color2;
std::vector<uint> nb_chars;


// boolean for tag display
//~ bool *displayDataOnly  = nullptr;
//~ bool *hideParent = nullptr;
bool displayDataOnly  = true;
bool hideParent = false;

// parse html file is false by default to bypass using
// a vector of font size and use the default size
// for all xml tags instead
// pointer to pointing to bool declared and initialised in main
bool parse_html_file = false;
//~ bool *ptParse_html_file = &parse_html_file;


//
std::vector<GLdata> my_v_MDE_data;

std::vector<vertex3D> v_origin;      // coordinates of origin for the group of glyphs, text
std::vector<vertex3D> v_origin_fb;   // coordinates of origin for the frame box to include padding



//~ GLfloat *font_color[4] = {NULL};
//~ GLfloat *font_color = NULL;

// some boolean flags
bool move_it = false;
bool display_box = false;

bool left_click=false;
bool right_click=false;

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

// variables to control orthographic view
float orthoX = -0.6f;
//~ float orthoXmax = 1.0;
float orthoY = -0.5f;
//~ float orthoYmax = 1.0;
float orthoZ = 0.0;
//~ float orthoZmax = 1.0;


//~ const float cameraSpeed = 0.05f; // adjust accordingly
float cameraSpeed = 0.05f; // adjust accordingly


float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;



bool firstMouse = true;

uint nlines = 1;
//~ int xOrigin = -1;

//~ // actual vector representing the camera's direction
//~ float lx=0.0f,lz=-1.0f, ly = 0.0f;


//~ glm::mat4 m_view;

// For debugging misaligned box
glm::vec3 check_coord3d_b;

// 
uint log_level=0;


/**
 * The atlas struct holds a texture that contains the visible US-ASCII characters
 * of a certain font rendered with a certain character height.
 * It also contains an array that contains all the information necessary to
 * generate the appropriate vertex and texture coordinates for each character.
 *
 * After the constructor is run, you don't need to use any FreeType functions anymore.
 */
struct atlas {
	GLuint tex;		// texture object

	unsigned int w;			// width of texture in pixels
	unsigned int h;			// height of texture in pixels

	struct {
		float ax;	// advance.x
		float ay;	// advance.y

		float bw;	// bitmap.width;
		float bh;	// bitmap.height;

		float bl;	// bitmap_left;
		float bt;	// bitmap_top;

		float tx;	// x offset of glyph in texture coordinates
		float ty;	// y offset of glyph in texture coordinates
	} c[128];		// character information

	 atlas(FT_Face face, int height) {
		//~ FT_Set_Pixel_Sizes(face, 0, height);
		int error = FT_Set_Pixel_Sizes(face, 0, height);
        fprintf(stderr,"FT_set_Pixel_Sizes:%d\n",error);
		FT_GlyphSlot g = face->glyph;
        PRINT_LOG("Check point 0",1);
		unsigned int roww = 0;
		unsigned int rowh = 0;
		 w = 0;
		 h = 0;

		 memset(c, 0, sizeof c);

		/* Find minimum size for a texture holding all visible ASCII characters */
		for (int i = 32; i < 128; i++) {
			if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
				fprintf(stderr, "Loading character %c failed!\n", i);
				continue;
			}
            
            //~ if (FT_Render_Glyph( face,FT_RENDER_MODE_NORMAL)){
                //~ fprintf(stderr, "Special Render character %c failed!\n", i);
                //~ continue;
            //~ }
            
			if (roww + g->bitmap.width + 1 >= MAXWIDTH) {
				w = std::max(w, roww);
				h += rowh;
				roww = 0;
				rowh = 0;
			}
			roww += g->bitmap.width + 1;
			rowh = std::max(rowh, g->bitmap.rows);
		}

		w = std::max(w, roww);
		h += rowh;

		/* Create a texture that will be used to hold all ASCII glyphs */
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glUniform1i(uniform_tex2d, 0);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, w, h, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);

		/* We require 1 byte alignment when uploading texture data */
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		/* Clamping to edges is important to prevent artifacts when scaling */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		/* Linear filtering usually looks best for text */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		/* Paste all glyph bitmaps into the texture, remembering the offset */
		int ox = 0;
		int oy = 0;

		rowh = 0;

		for (int i = 32; i < 128; i++) {
			if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
				fprintf(stderr, "Loading character %c failed!\n", i);
				continue;
			}

			if (ox + g->bitmap.width + 1 >= MAXWIDTH) {
				oy += rowh;
				rowh = 0;
				ox = 0;
			}

			glTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy, g->bitmap.width, g->bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);
			c[i].ax = g->advance.x >> 6;
			c[i].ay = g->advance.y >> 6;

			c[i].bw = g->bitmap.width;
			c[i].bh = g->bitmap.rows;

			c[i].bl = g->bitmap_left;
			c[i].bt = g->bitmap_top;

			c[i].tx = ox / (float)w;
			c[i].ty = oy / (float)h;

			rowh = std::max(rowh, g->bitmap.rows);
			ox += g->bitmap.width + 1;
		}

		fprintf(stderr, "Generated a %d x %d (%d kb) texture atlas\n", w, h, w * h / 1024);
	}

	~atlas() {
		glDeleteTextures(1, &tex);
	}
};

atlas *a96;
atlas *a48;
atlas *a40;
atlas *a24;
atlas *a12;

atlas *a_any_size;

uint nb_indices=0;

std::vector<vertex3D> glyphs_box;
std::vector<vertex2D> glyphs_vertex;

// testing with a second set of glyhs for testing multiple size of ont on screen
std::vector<vertex3D> glyphs_box2;
std::vector<vertex2D> glyphs_vertex2;

std::vector<colorRGBA> v_font_color;
std::vector<colorRGBA> vector_font_color ={};
std::vector<colorRGBA> v_font_color2;
std::vector<colorRGBA> vector_font_color2;

std::vector<uint> nb_glyphs_per_text;
std::vector<uint> nglyphs0;
std::vector<uint> nglyphs1;

void create_coord_vector_text(const char *text, atlas * a, vertex3D &origin, vertex2D &window_coord_scale,
                                std::vector<vertex3D> &a_glyphs_box, std::vector<vertex2D> &a_glyphs_vertex){    
    const uint8_t *p;
    uint c = 0;

    PRINT_LOG("In create_coord_vector_text",5);
    std::string str(text);
    PRINT_LOG("text: " + str , 5);
    /* Loop through all characters */

    for (p = (const uint8_t *)text; *p; p++) {
        
        /* Calculate the vertex and texture coordinates */
        float x2 = origin.x + a->c[*p].bl * window_coord_scale.x;
        float y2 = -origin.y - a->c[*p].bt * window_coord_scale.y;
        float w = a->c[*p].bw * window_coord_scale.x;
        float h = a->c[*p].bh * window_coord_scale.y;
        
        /* Advance the cursor to the start of the next character */
        origin.x += a->c[*p].ax * window_coord_scale.x;
        origin.y += a->c[*p].ay * window_coord_scale.y;
    
        /* Skip glyphs that have no pixels */
        if (!w || !h)
            continue;
        c+=6;
        
        a_glyphs_box.push_back((vertex3D) {
        x2, -y2, origin.z});
        a_glyphs_box.push_back((vertex3D) {
        x2 + w, -y2, origin.z});
        a_glyphs_box.push_back((vertex3D) {
        x2, -y2 - h, origin.z});
        a_glyphs_box.push_back((vertex3D) {
        x2 + w, -y2, origin.z});
        a_glyphs_box.push_back((vertex3D) {
        x2, -y2 - h, origin.z});
        a_glyphs_box.push_back((vertex3D) {
        x2 + w, -y2 - h, origin.z});

        a_glyphs_vertex.push_back((vertex2D) {
        a->c[*p].tx, a->c[*p].ty});
        a_glyphs_vertex.push_back((vertex2D) {
        a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty});
        a_glyphs_vertex.push_back((vertex2D) {
        a->c[*p].tx, a->c[*p].ty + a->c[*p].bh / a->h});
        a_glyphs_vertex.push_back((vertex2D) {
        a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty});
        a_glyphs_vertex.push_back((vertex2D) {
        a->c[*p].tx, a->c[*p].ty + a->c[*p].bh / a->h});
        a_glyphs_vertex.push_back((vertex2D) {
        a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty + a->c[*p].bh / a->h});

    }
    nb_chars.push_back(c/6);
    
    //~ displayV3D(a_glyphs_box);
    nb_glyphs_per_text.push_back(c);
    nb_indices+=c;
    PRINT_LOG(" size of glyphs_vertex "+ std::to_string(glyphs_vertex.size()),1);
    PRINT_LOG(" size of nb_chars " + std::to_string(nb_chars.size()),1);
    if(log_level>=1){
        displayV2D(glyphs_vertex);
    }
    
}

void text_frame(std::vector<vertex3D> &vector_input, std::vector<uint> &nb_glyphs, std::vector<vertex3D> &vector_origin, vertex2D &padding){
    
    
    PRINT_LOG("##### text_frame #####", 1);
    std::vector<vertex3D> vertex_3D;
    std::vector<vertex3D>::iterator it=vector_input.begin();
    std::vector<uint>::iterator it2=nb_glyphs.begin();
    std::vector<vertex3D> bottom_left_front;
    std::vector<vertex3D> top_right_back;
    
    while (it<vector_input.end()){
        float hmin=(*it).y;
        float hmax=(*it).y;
        float wmin=(*it).x;
        float wmax=(*it).x;
        float h=0;
        float d=0;
        
        // loop though each glyphs from current iterator until nb of glyphs for current text is reached
        for (std::vector<vertex3D>::iterator it3=it; it3!=(it+(*it2));++it3){
            hmin= std::min(hmin,(*it3).y);
            hmax = std::max(hmax,(*it3).y);
            wmin= std::min(wmin,(*it3).x);
            wmax = std::max(wmax,(*it3).x);
        }
        h = std::abs(hmax - hmin);
        // for now depth is the same as the width of the text
        d=h;
        
        bottom_left_front.push_back({wmin,hmin,0});
        top_right_back.push_back({wmax,hmax,d});
        
        std::advance(it,(*it2));
        std::advance(it2,1);
    }

    create_vector(vertex_3D, vector_origin, bottom_left_front, top_right_back, padding);
    
    init_box(vertex_3D);
    PRINT_LOG("##### End text_frame #####", 1);
    
    
}

void render_text(atlas * a,
            std::vector<colorRGBA> &vector_font_color_part,
            std::vector<vertex3D> &glyphs_box_part,
            std::vector<vertex2D> &glyphs_vertex_part,
            int nbChars,
            int startChar
            ) {
    PRINT_LOG("Total nb of characters: " +  std::to_string(glyphs_vertex_part.size()/6),5);
    displayColorRGBA(vector_font_color);
    /* test color per block*/
    glEnableVertexAttribArray(attribute_font_color);
    glGenBuffers(1,&vbo_font_color);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_font_color);
    glVertexAttribPointer(attribute_font_color, 4, GL_FLOAT, GL_FALSE, 0, 0);
    //~ glBufferData(GL_ARRAY_BUFFER, nFontColorIndex * 6 * sizeof(colorRGBA),&vector_font_color_part.front(),GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, nbChars * 6 * sizeof(colorRGBA),&vector_font_color_part[startChar * 6],GL_STATIC_DRAW);
    //~ glBufferData(GL_ARRAY_BUFFER,(nFontColorIndex + 1) * sizeof(colorRGBA),&vector_font_color_part,GL_STATIC_DRAW);
 
    //~ /* Use the texture containing the atlas */
    glBindTexture(GL_TEXTURE_2D, a->tex);
    
    /* Set up the VBO for our vertex data */
    glEnableVertexAttribArray(attribute_coord);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_coord);
    glVertexAttribPointer(attribute_coord, 3, GL_FLOAT, GL_FALSE, 0, 0);
    /* Draw the character on the screen */
    //~ glBufferData(GL_ARRAY_BUFFER, glyphs_box_part.size() * sizeof(vertex3D), &glyphs_box_part.front(), GL_STATIC_DRAW);
    //~ glBufferData(GL_ARRAY_BUFFER, nGlyphBoxIndex * 6 * sizeof(vertex3D), &glyphs_box_part.front(), GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, nbChars * 6 * sizeof(vertex3D), &glyphs_box_part[startChar * 6], GL_STATIC_DRAW);
    //~ displayV3D(glyphs_box);
    
    /* Set up the VBO for our vertex data */
    glEnableVertexAttribArray(attribute_tex_coord);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_tex_coord);
    glVertexAttribPointer(attribute_tex_coord, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    //~ displayV2D(glyphs_vertex);
    /* Draw all the character on the screen in one go */
    //~ glBufferData(GL_ARRAY_BUFFER,glyphs_vertex_part.size() * sizeof(vertex2D),&glyphs_vertex_part.front(),GL_STATIC_DRAW);
    //~ glBufferData(GL_ARRAY_BUFFER,nGlyphVertexIndex * 6 * sizeof(vertex2D),&glyphs_vertex_part.front(),GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER,nbChars * 6 * sizeof(vertex2D),&glyphs_vertex_part[startChar * 6],GL_STATIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, nb_indices);

    
    glDisableVertexAttribArray(attribute_font_color);
    glDisableVertexAttribArray(attribute_coord);
    glDisableVertexAttribArray(attribute_tex_coord);
}

bool init_size(uint uFontSize, std::vector<int> &sizes){
    /*Set the font size from argument */
    fontSize = uFontSize;
    font_size = &sizes;
    return 1;
}

bool init_font(const char *uFontFilename) {
    
    /*Set the font size from argument */
    //~ fontSize=uFontSize;
    
    /*Set the font Filename from argument*/
    fontfilename = uFontFilename;
    
    /* Initialize the FreeType2 library */
    if (FT_Init_FreeType(&ft)) {
        fprintf(stderr, "Could not init freetype library\n");
        return 0;
    }
    
    /* Load a font */
    if (FT_New_Face(ft, fontfilename, 0, &face)) {
        fprintf(stderr, "Could not open font %s\n", fontfilename);
        return 0;
    }
    
    /* TEST Load another font */
    if (FT_New_Face(ft, fontfilename2, 0, &face2)) {
        fprintf(stderr, "Could not open font %s\n", fontfilename2);
        return 0;
    }
    
    // Create the vertex buffer object
    glGenBuffers(1, &vbo_coord);
    glGenBuffers(1, &vbo_tex_coord);
    // Create the vertex buffer object for 2d display
	glGenBuffers(1, &vbo_coord2d);
	glGenBuffers(1, &vbo_tex_coord2d);
    
    return 1;
}

bool init_program(){

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    PRINT_LOG("Maximum nr of vertex attributes supported: " + 
    std::to_string(nrAttributes));
    
    // testing boxes
    program_box = create_program("../source/shaders/box.v.glsl", "../source/shaders/box.f.glsl");
    if(program_box == 0)
        return 0;

    attribute_coord3d_box = get_attrib(program_box, "coord3d_b");
    attribute_v_color_box = get_attrib(program_box, "v_color_b");

    uniform_m_model_b = get_uniform(program_box, "m_model_b");
    uniform_m_projection_b = get_uniform(program_box, "m_projection_b");
    uniform_m_view_b = get_uniform(program_box, "m_view_b");
    
    //~ uniform_mvp = get_uniform(program_box, "mvp");

    if(attribute_coord3d_box == -1 || attribute_v_color_box == -1 || uniform_m_model_b == -1 || uniform_m_projection_b == -1 || uniform_m_view_b == -1)
        return 0;

    // program for the text
    program_text = create_program("../source/shaders/text.v.glsl", "../source/shaders/text.f.glsl");
    if(program_text == 0)
        return 0;
    
    attribute_coord_text = get_attrib(program_text, "coord");
    attribute_tex_coord = get_attrib(program_text,"tex_coord");
    attribute_font_color = get_attrib(program_text,"fontcolor");
    uniform_tex_text = get_uniform(program_text, "tex");
    //~ uniform_color_text = get_uniform(program_text, "color");
    uniform_bgcolor_text = get_uniform(program_text, "bgcolor");

    uniform_m_model = get_uniform(program_text, "m_model");
    uniform_m_projection = get_uniform(program_text, "m_projection");
    uniform_m_view = get_uniform(program_text, "m_view");
    
    if(attribute_coord_text == -1 ||attribute_tex_coord == -1 ||attribute_font_color == -1 || uniform_tex_text == -1 || uniform_m_model == -1 || uniform_m_projection == -1 || uniform_m_view == -1)
        return 0;
    
    // program for the text in 2D
    program_text2d = create_program("../source/shaders/text2d.v.glsl", "../source/shaders/text2d.f.glsl");
    if(program_text2d == 0)
    {
        fprintf(stderr,"Problem with creating program from shaders\n");
        return 0;
    }

    attribute_font_color2d = get_attrib(program_text2d,"fontcolor");
    attribute_tex_coord2d = get_attrib(program_text2d,"tex_coord");
	attribute_coord2d = get_attrib(program_text2d, "coord");
	uniform_tex2d = get_uniform(program_text2d, "tex");
    uniform_bgcolor_text2d = get_uniform(program_text, "bgcolor");


	if(attribute_coord2d == -1 || attribute_tex_coord2d == -1 || attribute_font_color2d == -1 || uniform_tex2d == -1 || uniform_bgcolor_text2d ==-1)
    {
        fprintf(stderr,"Problem with attribute or uniform\n");
		return 0;
    }

    return 1;
}

void init_color_box(colorRGBA &color_alpha)
{
    PRINT_LOG("######## init_color_box #######");

    GLfloat cube_colors_box[] = {
    // front colors
    0.0,0.5,1.0,0.2,
    0.0,0.5,1.0,0.2,
    0.0,0.5,1.0,0.2,
    0.0,0.5,1.0,0.2,
    // back colors
    0.0,0.5,0.5,1.0,
    0.0,0.5,0.5,1.0,
    0.0,0.5,0.5,1.0,
    0.0,0.5,0.5,1.0
    };
    
    glGenBuffers(1, &vbo_color_box);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_color_box);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors_box), cube_colors_box, GL_STATIC_DRAW);
    
    PRINT_LOG("######## End init_color_box #######");
  
}

void init_color(colorRGBA &color_alpha, colorRGBA &bg_color_alpha,
                std::vector<colorRGBA> &font_color_alpha,
                std::vector<vertex3D> &whd,
                std::vector<unsigned int> &nb_glyphs_per_text,
                std::vector<colorRGBA> &output_font_color){
        
    PRINT_LOG("######## init_color #######");
        
    // window background color
    bg_r=bg_color_alpha.r;
    bg_g=bg_color_alpha.g;
    bg_b=bg_color_alpha.b;
    bg_a=bg_color_alpha.a;
    
    // frame box color
    r=color_alpha.r;
    g=color_alpha.g;
    b=color_alpha.b;
    a=color_alpha.a;
        
    GLfloat box_vertices_color3[]={
        r,g,b,a,
        r,g,b,a,
        r,g,b,a,
        r,g,b,a,
        r,g,b,a,
        r,g,b,a,
        r,g,b,a,
        r,g,b,a
    };
    
    std::vector<GLfloat> vect_color3;
    PRINT_LOG("(before) Size of output_font_color" + std::to_string(output_font_color.size()) ,5);
    PRINT_LOG("size of nb_glyphs_per_text: "+std::to_string(nb_glyphs_per_text.size()),5);
    PRINT_LOG("nb_glyphs_per_text[0]: "+std::to_string(nb_glyphs_per_text[0]),5);
    PRINT_LOG("output_font_color: ",5);
    displayColorRGBA(output_font_color, 5);

    fprintf(stderr,"whd.size: %lu\n",whd.size());
    fprintf(stderr,"font_color_alpha.size: %lu\n",font_color_alpha.size());
    //~ int max_loop = whd.size();
    int max_loop = font_color_alpha.size();
    //~ int max_loop = 6;
    for(int i=0;i<max_loop;i++){
        vect_color3.insert (vect_color3.end(), box_vertices_color3, box_vertices_color3+32);
        }

    glGenBuffers(1,&vbo_color_box);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_color_box);
    glBufferData(GL_ARRAY_BUFFER,vect_color3.size() * sizeof(GLfloat),&vect_color3.front(),GL_STATIC_DRAW);
    
    // font colors
    int ind = 0;
    for(std::vector<unsigned int>::iterator it=nb_glyphs_per_text.begin(); it!=nb_glyphs_per_text.end();it++){
        for(unsigned int i=0;i<(*it);i++){
            output_font_color.push_back(font_color_alpha[ind]);
            PRINT_LOG("(*it):" + std::to_string((*it)) + " i:" + std::to_string(i) + " ind:" + std::to_string(ind)
            + " output_font_color: | " + std::to_string(output_font_color.back().r)
             + ", "  + std::to_string(output_font_color.back().g)
            + ", "  + std::to_string(output_font_color.back().b)
            + ", "  + std::to_string(output_font_color.back().a) + " |", 5);
        }
        ind++;
    }
    PRINT_LOG("(After) Size of output_font_color" + 
    std::to_string(output_font_color.size()), 5);
    PRINT_LOG("output_font_color: ",5);
    displayColorRGBA(output_font_color, 5);
    PRINT_LOG("######## End init_color #######", 5);
}

// bypassing singlecolor for testing
//~ void init_v_font_color(std::vector<vertex3Dalpha> &font_color, uint &nlines, bool singleColor, bool displayDataOnly){
void init_v_font_color(std::vector<colorRGBA> &font_color,
                    std::vector<colorRGBA> &v_font_color, uint &nlines,
                    bool singleColor, bool displayDataOnly){
    PRINT_LOG("####### init_v_font_color ########",5);



    if(singleColor)
    {
        // add same color for each glyph
        PRINT_LOG("Check number of indices: " + std::to_string(nb_indices),5);
        PRINT_LOG("Check number of lines: " + std::to_string(nlines),5);
        for(uint i=0;i<nb_indices;i++)
        {
            for(std::vector<colorRGBA>::iterator it=font_color.begin();it!=font_color.end();++it){
                v_font_color.push_back((*it));
                //TODO introduce different color for name and data
                if(!displayDataOnly) v_font_color.push_back((*it));
            }
            
        }
    }

    PRINT_LOG("Size of  font_color: " + std::to_string((font_color).size()),5);
    PRINT_LOG("Size of v_font_color: " + std::to_string((v_font_color).size()),5);
    displayColorRGBA(v_font_color);    
    
    PRINT_LOG("####### End init_v_font_color #######",5);
    
}

void displayV3D(std::vector<vertex3D> &vector_3D, uint log_level){
    PRINT_LOG("loop V3D", log_level);
    PRINT_LOG("Size: " + std::to_string(vector_3D.size()), log_level);
    for(std::vector<vertex3D>::iterator it=vector_3D.begin();it!=vector_3D.end();++it)
    {
        PRINT_LOG("| " + std::to_string((*it).x) + ", " + std::to_string((*it).y) + ", " + std::to_string((*it).z) + " |" ,log_level);
    }
    
}

void displayV2D(std::vector<vertex2D> &vector_2D, uint log_level){
    PRINT_LOG("loop V2D",log_level);
    for(std::vector<vertex2D>::iterator it=vector_2D.begin();it!=vector_2D.end();++it){
        PRINT_LOG("| " + std::to_string((*it).x) + ", " + std::to_string((*it).y) + " |", log_level );
        
    }
    
}

void displayColorRGBA(std::vector<colorRGBA> &vector_rgba, uint log_level){
    for(std::vector<colorRGBA>::iterator it=vector_rgba.begin();it!=vector_rgba.end();++it){
        PRINT_LOG("| " + std::to_string((*it).r) + ", " + std::to_string((*it).g) + ", " + std::to_string((*it).b) + ", " + std::to_string((*it).a) + " |" ,log_level);
    }
}

 // blf: bottom-left-front, trb: top-right-back
void create_vector(std::vector<vertex3D> &vector3D,std::vector<vertex3D> &vector_origin, std::vector<vertex3D> &vector_blf, std::vector<vertex3D> &vector_trb, vertex2D &padding)
{
    PRINT_LOG("######## create_vector #######", 1);

    std::vector<vertex3D>::iterator vector_origin_it;
    std::vector<vertex3D>::iterator v_blf_it;   // bottom left front
    std::vector<vertex3D>::iterator v_trb_it;   // top right back 

    for(vector_origin_it=vector_origin.begin(), v_blf_it=vector_blf.begin(),
     v_trb_it=vector_trb.begin(); vector_origin_it!=vector_origin.end() &&
      v_blf_it!=vector_blf.end() && v_trb_it!=vector_trb.end();
      ++vector_origin_it, ++v_blf_it, ++v_trb_it){
        // 1st vertex is bottom left corner, front face
        vector3D.push_back({(*v_blf_it).x - padding.x,
            (*v_blf_it).y - padding.y,
            (*vector_origin_it).z});
        // 2nd vertex, keep 1st vertex x and z, add y from vector2D, to get upper left corner, front face
        vector3D.push_back({vector3D.back().x,
            (*v_trb_it).y + padding.y,
            vector3D.back().z});
        // 3rd vertex, back is now 2nd vertex, so just add y, add x from, to get upper right corner, front face
        vector3D.push_back({(*v_trb_it).x + padding.x,
            vector3D.back().y,
            vector3D.back().z});
        // 4th vertex, back is now 3rd vertex, so deduct y, keeping x, to get bottom right corner, front face
        vector3D.push_back({vector3D.back().x,
            (*v_blf_it).y - padding.y,
            vector3D.back().z});
        // 5th vertex deduct x to come back to bottom left corner, and deduct a depth for z to get back face
        vector3D.push_back({(*v_blf_it).x - padding.x,
            vector3D.back().y,
            vector3D.back().z-(*v_trb_it).z});
        // 6nd vertex, keep 1st vertex x and z, add y from vector2D, to get upper left corner, back face
        vector3D.push_back({vector3D.back().x,
            (*v_trb_it).y + padding.y,
            vector3D.back().z});
        // 7th vertex, back is now 2nd vertex, so just add y, add x from, to get upper right corner, back face
        vector3D.push_back({(*v_trb_it).x + padding.x,
            vector3D.back().y,
            vector3D.back().z});
        // 8th vertex, back is now 3rd vertex, so deduct y, keeping x, to get bottom right corner, back face
        vector3D.push_back({vector3D.back().x,
            (*v_blf_it).y - padding.y,
            vector3D.back().z});

            }
        PRINT_LOG("######## End create_vector #######", 1);

}

void init_vbo_box(std::vector<vertex3D> &vectors_boxes)
{
    PRINT_LOG("######## init_vbo_box #######", 1);

    glGenBuffers(1,&vbo_vertices_box);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_vertices_box);
    glBufferData(GL_ARRAY_BUFFER,vectors_boxes.size() * sizeof(vertex3D),&vectors_boxes.front(),GL_STATIC_DRAW);
    
    PRINT_LOG("######## End init_vbo_box #######", 1);

}

//~ void init_ibo_elements_box(std::vector<vertex3D> &vectors_boxes)
void init_ibo_elements_box(int size_of_vectors_boxes)
{
    PRINT_LOG("######## init_ibo_elements_box #######", 1);
    
    // set the array of indexes for a rectangle
    std::array<GLushort,36> box_elements3 = {
    //~ GLushort box_elements3[] = {
        // front
        0, 1, 2,
        2, 3, 0,
        // right
        1, 5, 6,
        6, 2, 1,
        // back
        7, 6, 5,
        5, 4, 7,
        // left
        4, 0, 3,
        3, 7, 4,
        // bottom
        4, 5, 1,
        1, 0, 4,
        // top
        3, 2, 6,
        6, 7, 3,
        };

    std::vector<GLushort> vect_index;

    //~ int max_loop = vectors_boxes.size();
    //~ displayV3D(vectors_boxes);
    //~ for(GLushort i=0;i<max_loop;i++){
    for(GLushort i=0;i<size_of_vectors_boxes;i++){
        for ( auto it = box_elements3.begin(); it != box_elements3.end(); ++it ){
            vect_index.push_back((*it)+8*i);
        }
    }
    PRINT_LOG("vect_index size: " + std::to_string(vect_index.size()), 1);
    

    glGenBuffers(1, &ibo_box_elements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_box_elements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vect_index.size() * sizeof(GLushort), &vect_index.front(), GL_STATIC_DRAW);

    PRINT_LOG("######## End init_ibo_elements_box #######", 1);
    
}

//~ void init_box(std::vector<vertex3D> &vectors_boxes, vertex3Dalpha &color_alpha)
void init_box(std::vector<vertex3D> &vectors_boxes)
{
    PRINT_LOG("######## init_box #######", 1);

    init_vbo_box(vectors_boxes);
    init_ibo_elements_box(vectors_boxes.size());

    PRINT_LOG("######## End init_box #######", 1);
}

//~ void init_vectors(std::vector<vertex3D> &vectors_boxes)
void init_vectors()
{
    PRINT_LOG("######## init_vectors #######", 1);

    // set the array of indexes for a rectangle
    GLushort box_elements3[] = {
        // front
        0, 1, 2,
        2, 3, 0,
        // right
        1, 5, 6,
        6, 2, 1,
        // back
        7, 6, 5,
        5, 4, 7,
        // left
        4, 0, 3,
        3, 7, 4,
        // bottom
        4, 5, 1,
        1, 0, 4,
        // top
        3, 2, 6,
        6, 7, 3,
        };

    glGenBuffers(1, &ibo_box_elements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_box_elements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(box_elements3), box_elements3, GL_STATIC_DRAW);
    
    PRINT_LOG("######## End init_vectors #######");
    
    }
    
void draw_box(){
    
    glUseProgram(program_box);
    
    glEnableVertexAttribArray(attribute_coord3d_box);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices_box);
    glVertexAttribPointer(
    attribute_coord3d_box,   // attribute
    3,                   // number of elements per vertex, here (x,y,z)
    GL_FLOAT,            // the type of each element
    GL_FALSE,            // take our values as-is
    0,  // next coord3d appears every 5 floats
    0                    // offset of first element
    );
    glEnableVertexAttribArray(attribute_v_color_box);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_color_box);
    glVertexAttribPointer(
    attribute_v_color_box,      // attribute
    //~ 3,                      // number of elements per vertex, here (r,g,b)
    4,                      // number of elements per vertex, here (r,g,b,a)
    GL_FLOAT,               // the type of each element
    GL_FALSE,               // take our values as-is
    0,
    0
    );
    
    /* Push each element in buffer_vertices to the vertex shader */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_box_elements);
    int size;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    
    glDisableVertexAttribArray(attribute_coord3d_box);
    glDisableVertexAttribArray(attribute_v_color_box);
    //~ glutSwapBuffers();
    
}

void init_user_input(MDE &user_input_MDE, vertex3D &user_input_origin, std::vector<vertex3D> &user_input_offset,
                    std::vector<vertex3D> &user_input_offset_rule, vertex2D &user_input_padding, colorRGBA &user_input_color_alpha,
                    colorRGBA &user_input_bg_color_alpha, std::vector<colorRGBA> &user_input_font_color,
                    bool _displayDataOnly, bool _hideParent, bool &user_parse_html){
                        
                        
    PRINT_LOG("######## init_user_input #######");

    my_entity = &user_input_MDE;
    origin = &user_input_origin;
    
    
    offset = &user_input_offset;
    offset_rule = &user_input_offset_rule;
    color_alpha = &user_input_color_alpha;
    bg_color_alpha = &user_input_bg_color_alpha;
    
    font_color = &user_input_font_color;
    // for testing
    //~ font_color2 = &user_input_font_color;
    PRINT_LOG("Tracking 't' path... In text.cpp/init_user_input", 2);
    displayColorRGBA(*font_color);
    
    
    // boolean for html rendering
    displayDataOnly = _displayDataOnly;
    hideParent = _hideParent;

    // non-pointer variable
    padding = user_input_padding;
    
    // boolean for html parsing
    parse_html_file = user_parse_html;
    
    PRINT_LOG("######## End init_user_input #######");
}

// prepare MDE Data for further processing, hidding parent and 'name' if boolean set to True
void init_data()
{
    PRINT_LOG("######## init_data #######");
    
    int level=0;
    int index=0;
    extractMDEdataForVoffset(*my_entity, my_v_MDE_data, level,index ,displayDataOnly, hideParent);
    
    PRINT_LOG("######## End init_data #######");
}

void init_offset_rules()
{
    PRINT_LOG("######## init_offset_rules #######");
    
    process_v_MDE_data(my_v_MDE_data,*offset, *offset_rule);

    PRINT_LOG("######## End init_offset_rules #######");
}

void init_origin()
{
    PRINT_LOG("######## init_origin #######");

    for (std::vector<vertex3D>::iterator it_offset=offset->begin(); it_offset!=offset->end();++it_offset)
    {
        v_origin.push_back({origin->x+(*it_offset).x,origin->y+(*it_offset).y,origin->z+(*it_offset).z});
    }
        
    PRINT_LOG("######## End init_origin #######");
}

void init_padding(){
    
    PRINT_LOG("###### init_padding #######",1);

    // Adding padding at first and last elements of v_origin
    add_padding(v_origin, v_origin_fb, padding);

    PRINT_LOG("###### End init_padding #######",1);
}

void init_glyphs(){
    
    PRINT_LOG("###### init_glyphs ######");
    
    vertex2D window_scale;
    window_scale.x = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
    window_scale.y = 2.0 / glutGet(GLUT_WINDOW_HEIGHT);
    /* Create texture atlasses for a given font size */
    a_any_size = new atlas(face,(int)fontSize);
    a96 = new atlas(face2,96);
    //~ a48 = new atlas(face,48);
    //~ a40;
    //~ a24;
    //~ a12;    
    uint index_v_origin=0;
    
    for (std::vector<GLdata>::iterator it_v_MDE_data=my_v_MDE_data.begin(); it_v_MDE_data!=my_v_MDE_data.end();++it_v_MDE_data)
    {
        if(!parse_html_file){
                create_coord_vector_text(((*it_v_MDE_data).MDE_data).c_str(), a_any_size, v_origin[index_v_origin], window_scale, glyphs_box, glyphs_vertex );
            }
            else{
                create_coord_vector_text(((*it_v_MDE_data).MDE_data).c_str(), ((*font_size)[index_v_origin]==16)?a_any_size:a96, v_origin[index_v_origin], window_scale, glyphs_box, glyphs_vertex );
                
            }
        
        // increment index
        index_v_origin++;
        
    }
    
    PRINT_LOG("###### End init_glyphs ######");
}


void init_frame(){
    
    PRINT_LOG("###### init_frame ######");    
    
    text_frame(glyphs_box, nb_glyphs_per_text, v_origin_fb, padding);

    PRINT_LOG("###### End init_frame ######");    
}

void init_color_scheme(){

    PRINT_LOG("###### init_color_scheme ######");    
 
    nlines = (uint)nb_glyphs_per_text.size();
    if(!parse_html_file){
        init_v_font_color(*font_color, vector_font_color,nlines , true, false);
    }
    else
    {
        init_color(*color_alpha,*bg_color_alpha,*font_color,v_origin,nb_glyphs_per_text, vector_font_color);
    }
    
    PRINT_LOG("###### End init_color_scheme ######");    
}

void process_v_MDE_data(std::vector<GLdata> & v_MDE_data,std::vector<vertex3D> & v_offset, std::vector<vertex3D> & v_offset_rule){
    
    // offset rule.x is the name-data offset
    // offset rule.y is the index offset
    // offset rule.x is the level offset
    
    for (std::vector<GLdata>::iterator it_v_MDE_data=v_MDE_data.begin();it_v_MDE_data!=v_MDE_data.end(); ++it_v_MDE_data){
        v_offset.push_back({
            (*it_v_MDE_data).mde_member*v_offset_rule[0].x+(*it_v_MDE_data).index*v_offset_rule[0].y+(*it_v_MDE_data).level*v_offset_rule[0].z ,
            (*it_v_MDE_data).mde_member*v_offset_rule[1].x+(*it_v_MDE_data).index*v_offset_rule[1].y+(*it_v_MDE_data).level*v_offset_rule[1].z ,
            (*it_v_MDE_data).mde_member*v_offset_rule[2].x+(*it_v_MDE_data).index*v_offset_rule[2].y+(*it_v_MDE_data).level*v_offset_rule[2].z ,
            });
    }
}

void add_padding(std::vector<vertex3D> & v_origin, std::vector<vertex3D> & v_origin_fb, vertex2D & padding){
    
    PRINT_LOG("###### add_padding ######");
    
    for (std::vector<vertex3D>::iterator it=v_origin.begin();it!=v_origin.end(); ++it){
        v_origin_fb.push_back({(*it).x-padding.x,(*it).y-padding.y,(*it).z});
    }

    PRINT_LOG("###### End add_padding ######");
}

void text_display() {

    PRINT_LOG("###### text_display ######");
    // set the color of the non-glyph part of the glyph box to same color as the
    // cube color: r,g,b
    // keep alpha to zero
    GLfloat bg_rgb[4] = { r, g, b, 0 };
    
    glUseProgram(program_text);
    glUniform4fv(uniform_bgcolor_text, 1, bg_rgb);


    if(!parse_html_file){
        // testing with dynamic number of characters
        render_text(a_any_size, vector_font_color, glyphs_box, glyphs_vertex, glyphs_vertex.size()/6 ,0 );
    }
    else{
        int it0 = 0;
        uint previous = 0;
        for(auto it:nb_chars){
           if((*font_size)[it0]==96){
               render_text(a96, vector_font_color, glyphs_box, glyphs_vertex,it, previous );
           }
           else{
               render_text(a_any_size, vector_font_color, glyphs_box, glyphs_vertex,it,  previous);
           }
           previous+= it;
           it0++;
        }
    }

    PRINT_LOG("###### End text_display ######");
}

void init_options(bool a_move_it, bool a_display_box, bool a_ortho_mode, bool singleColor,uint a_nlines, uint a_log_level){
    
    PRINT_LOG("###### init_options ######");
    
    move_it = a_move_it;
    display_box = a_display_box;
    ortho_mode = a_ortho_mode;
    nlines = a_nlines;
    log_level = a_log_level;

    PRINT_LOG("###### End init_options ######");
}

void mouse_wheel(int button, int state, int x, int y)
{
    //~ fprintf(stderr,"Button %d is %s At %d %d\n", button, (state == GLUT_DOWN) ? "Down" : "Up", x, y);
    if ((button == 0) && (state == GLUT_DOWN))
    {
        // first reset firstMouse to get init mouse position in mouse callback
        firstMouse = true;
        left_click=true;// It's a wheel event
    }
    if ((button == 0) && (state == GLUT_UP)) left_click=false;// It's a wheel event
    if ((button == 2) && (state == GLUT_DOWN)) right_click=true;// It's a wheel event
    if ((button == 2) && (state == GLUT_UP)) right_click=false;// It's a wheel event
    // if middle click do nothing
    if (button == 1)
        {
           left_click=false;
           right_click=false;
        }
    if ((button == 3) && (state == GLUT_UP)) // Scroll up
        {
            if (fov >= 1.0f && fov <= 45.0f) fov --;
            if (fov <= 1.0f) fov = 1.0f;
            if (fov >= 45.0f) fov = 45.0f;
            if(ortho_mode) multiply-=0.05f;
        }
    if ((button == 4) && (state == GLUT_UP)) // Scroll down
        {
            if (fov >= 1.0f && fov <= 45.0f) fov ++;
            if (fov <= 1.0f) fov = 1.0f;
            if (fov >= 45.0f) fov = 45.0f;
            if(ortho_mode) multiply+=0.05f;

        }
    }
   
void display(){

    // calculate frame rate for consistent camera velocity
    float currentFrame = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    
    // Set the colored background, default is white.
    glClearColor(bg_r, bg_g, bg_b, bg_a);
    // need the depth buffer for 3d
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_NOTEQUAL);
    // camera operations and text display
    camera();
    if(display_box) draw_box();
    text_display();
    
    glutSwapBuffers();
}


void camera(){
    //Thanks to https://learnopengl.com/Getting-started/Camera
    
    // Camera direction
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget); //The name direction vector is not the best chosen name, since it is actually pointing in the reverse direction of what it is targeting. 
    
    // Right axis
    // Product of 'world space' right by camera 
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); 
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    
    // Up axis
    cameraUp = glm::cross(cameraDirection, cameraRight);
    
    glm::mat4 m_view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp); //eye,center,up
    if (ortho_mode) m_view = glm::ortho((-1.0f+orthoX)*multiply, (1.0f+orthoX)*multiply, (-1.0f+orthoY)*multiply, (1.0f+orthoY)*multiply, (0.1f+orthoZ)*multiply, (100.0f+orthoZ)*multiply);
    
    // Projection matrix modified by field of view (fov) directed by the camera zoom from the mouse wheel
    glm::mat4 m_projection = glm::perspective(glm::radians(fov), 1.0f*screen_width/screen_height, 0.1f, 10.0f);
    if(ortho_mode) m_projection = glm::ortho(0.0f,1.0f*(float)screen_width*scale, 0.0f,1.0f*(float)screen_height*scale);
    
    glm::mat4 m_view_b = m_view;
    glm::mat4 m_projection_b = m_projection;
        
    glUseProgram(program_text);
    glUniformMatrix4fv(uniform_m_view, 1, GL_FALSE, glm::value_ptr(m_view));
    glUniformMatrix4fv(uniform_m_projection, 1, GL_FALSE, glm::value_ptr(m_projection));

    glUseProgram(program_box);
    glUniformMatrix4fv(uniform_m_view_b, 1, GL_FALSE, glm::value_ptr(m_view_b));
    glUniformMatrix4fv(uniform_m_projection_b, 1, GL_FALSE, glm::value_ptr(m_projection_b));
    
    glutPostRedisplay();
}

void init_matrices(){
    
    PRINT_LOG("###### init_matrices ######");
     
    glm::mat4 m_model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0, 0.0, -1.0));
    glm::mat4 m_projection = glm::perspective(glm::radians(fov), 1.0f*screen_width/screen_height, 0.1f, 10.0f);    
    if(ortho_mode) m_projection = glm::ortho(0.0f,1.0f*(float)screen_width*scale, 0.0f,1.0f*(float)screen_height*scale);

    glm::mat4 m_view = glm::lookAt(glm::vec3(0.0, 0.0, 4.0), glm::vec3(0.0, 0.0,-1.0), glm::vec3(0.0, 1.0, 0.0));//eye,center,up
    glm::mat4 m_view_b = m_view;
    glm::mat4 m_projection_b = m_projection;
    glm::mat4 m_model_b = m_model;
 
    // Can use glm::to_string() to display matrices
    //~ PRINT_LOG(glm::to_string(m_model_b));
    
    glUseProgram(program_text);
    glUniformMatrix4fv(uniform_m_model, 1, GL_FALSE, glm::value_ptr(m_model));
    glUniformMatrix4fv(uniform_m_projection, 1, GL_FALSE, glm::value_ptr(m_projection));
    glUniformMatrix4fv(uniform_m_view, 1, GL_FALSE, glm::value_ptr(m_view));
    
    glUseProgram(program_box);
    glUniformMatrix4fv(uniform_m_model_b, 1, GL_FALSE, glm::value_ptr(m_model_b));
    glUniformMatrix4fv(uniform_m_projection_b, 1, GL_FALSE, glm::value_ptr(m_projection_b));
    glUniformMatrix4fv(uniform_m_view_b, 1, GL_FALSE, glm::value_ptr(m_view_b));
 
    PRINT_LOG("###### End init_matrices ######");
}

/*
int init_resources()
{
  //~ GLfloat cube_vertices[] = {
    //~ // front
    //~ -1.0, -1.0,  1.0,
     //~ 1.0, -1.0,  1.0,
     //~ 1.0,  1.0,  1.0,
    //~ -1.0,  1.0,  1.0,
    //~ // back
    //~ -1.0, -1.0, -1.0,
     //~ 1.0, -1.0, -1.0,
     //~ 1.0,  1.0, -1.0,
    //~ -1.0,  1.0, -1.0,
  //~ };
  GLfloat cube_vertices[] = {

    //~ 0,0,0,
    //~ 0.41666633,0.322222,0,
    //~ 0.41666633,0.404444,0,
    //~ 0,0.404444,0.0822222,
    //~ 0,0.322222,0.0822222,
    //~ 0.41666633,0.322222,0.0822222,
    //~ 0.41666633,0.404444,0.0822222

      
    //~ 0,0,0,
    //~ 0.41666633,0,0,
    //~ 0.41666633,0.082222,0,
    //~ 0,0.082222,0.0822222,
    //~ 0,0,0.0822222,
    //~ 0.41666633,0,0.0822222,
    //~ 0.41666633,0.082222,0.0822222
      
    0.00166667, -0.404444, 0,
    0.00166667, -0.322222, 0,
    0.418333, -0.322222, 0,
    0.418333, -0.404444, 0,
    
    0.00166667, -0.404444, -0.0822222,
    0.00166667, -0.322222, -0.0822222,
    0.418333, -0.322222, -0.0822222,
    0.418333, -0.404444, -0.0822222      

    //~ // front
    //~ -1.0, -1.0,  1.0,
     //~ 1.0, -1.0,  1.0,
     //~ 1.0,  1.0,  1.0,
    //~ -1.0,  1.0,  1.0,
    //~ // back
    //~ -1.0, -1.0, -1.0,
     //~ 1.0, -1.0, -1.0,
     //~ 1.0,  1.0, -1.0,
    //~ -1.0,  1.0, -1.0,
  };
  glGenBuffers(1, &vbo_cube_vertices);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

  GLfloat cube_colors[] = {
    //~ // front colors
    //~ 1.0, 0.0, 0.0,
    //~ 0.0, 1.0, 0.0,
    //~ 0.0, 0.0, 1.0,
    //~ 1.0, 1.0, 1.0,
    //~ // back colors
    //~ 1.0, 0.0, 0.0,
    //~ 0.0, 1.0, 0.0,
    //~ 0.0, 0.0, 1.0,
    //~ 1.0, 1.0, 1.0,
    // front colors
    0.5,0.5,0.5,0.5,
    0.5,0.5,0.5,0.5,
    0.5,0.5,0.5,0.5,
    0.5,0.5,0.5,0.5,
    // back colors
    0.5,0.5,0.5,0.5,
    0.5,0.5,0.5,0.5,
    0.5,0.5,0.5,0.5,
    0.5,0.5,0.5,0.5
  };
  glGenBuffers(1, &vbo_cube_colors);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);

  GLushort cube_elements[] = {
    // front
    0, 1, 2,
    2, 3, 0,
    // top
    1, 5, 6,
    6, 2, 1,
    // back
    7, 6, 5,
    5, 4, 7,
    // bottom
    4, 0, 3,
    3, 7, 4,
    // left
    4, 5, 1,
    1, 0, 4,
    // right
    3, 2, 6,
    6, 7, 3,
  };
  glGenBuffers(1, &ibo_cube_elements);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);

  GLint link_ok = GL_FALSE;

  GLuint vs, fs;
  if ((vs = create_shader("../source/shaders/cube.v.glsl", GL_VERTEX_SHADER))   == 0) return 0;
  if ((fs = create_shader("../source/shaders/cube.f.glsl", GL_FRAGMENT_SHADER)) == 0) return 0;

  program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
  if (!link_ok) {
    fprintf(stderr, "glLinkProgram:");
    print_log(program);
    return 0;
  }

  const char* attribute_name;
  attribute_name = "coord3d";
  attribute_coord3d = glGetAttribLocation(program, attribute_name);
  if (attribute_coord3d == -1) {
    fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    return 0;
  }
  attribute_name = "v_color";
  attribute_v_color = glGetAttribLocation(program, attribute_name);
  if (attribute_v_color == -1) {
    fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    return 0;
  }
  const char* uniform_name;
  uniform_name = "mvp";
  uniform_mvp = glGetUniformLocation(program, uniform_name);
  if (uniform_mvp == -1) {
    fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
    return 0;
  }

  return 1;
}
*/

void onDisplay()
{

  glUseProgram(program);
  glEnableVertexAttribArray(attribute_coord3d);
  // Describe our vertices array to OpenGL (it can't guess its format automatically)
  glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
  glVertexAttribPointer(
    attribute_coord3d, // attribute
    3,                 // number of elements per vertex, here (x,y,z)
    GL_FLOAT,          // the type of each element
    GL_FALSE,          // take our values as-is
    0,                 // no extra data between each position
    0                  // offset of first element
  );

  glEnableVertexAttribArray(attribute_v_color);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
  glVertexAttribPointer(
    attribute_v_color, // attribute
    4,                 // number of elements per vertex, here (R,G,B)
    GL_FLOAT,          // the type of each element
    GL_FALSE,          // take our values as-is
    0,                 // no extra data between each position
    0                  // offset of first element
  );

  /* Push each element in buffer_vertices to the vertex shader */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
  int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
  glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

  glDisableVertexAttribArray(attribute_coord3d);
  glDisableVertexAttribArray(attribute_v_color);
  //~ glutSwapBuffers();
}


void onReshape(int width, int height) {
  screen_width = width;
  screen_height = height;
  glViewport(0, 0, screen_width, screen_height);
}

void orthoTest()
{
    //~ gluOrtho2D(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top);
}


void onIdle() {
//~ void camera() {
    float move;
    float angle;
    if (!move_it){
        move = 0;
        angle = 0;
    }
    else {
        move = 2*sinf(glutGet(GLUT_ELAPSED_TIME) / 1000.0 * (2*3.14) / 30); // -1<->+1 every 2 seconds
        angle = glutGet(GLUT_ELAPSED_TIME) / 1000.0 * 10;  // 25° per second
        //~ angle = 0;
    }
    screen_width=glutGet(GLUT_WINDOW_WIDTH);
    screen_height=glutGet(GLUT_WINDOW_HEIGHT);
    glm::vec3 axis_x(1, 0, 0);
    glm::vec3 axis_y(0, 1, 0);
    glm::vec3 axis_z(0, 0, 1);
    glm::mat4 m_translate = glm::translate(glm::mat4(1.0f), glm::vec3(move, 0.0 ,0.0));
    
    glm::mat4 anim = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis_y);
    glm::mat4 anim2 = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis_z);
    
    glm::mat4 m_model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0, 0.0, -1.0));
    glm::mat4 m_projection = glm::perspective(glm::radians(fov), 1.0f*screen_width/screen_height, 0.1f, 10.0f);
    if(ortho_mode) m_projection = glm::ortho(0.0f,1.0f*(float)screen_width*scale, 0.0f,1.0f*(float)screen_height*scale);


    m_model = m_model * m_translate * anim2 * anim;

    glm::mat4 m_projection_b = m_projection;
    glm::mat4 m_model_b = m_model;

    // can use glm::to_string() to dispay matrix
    //~ PRINT_LOG(glm::to_string(m_model));

    glUseProgram(program_text);
    glUniformMatrix4fv(uniform_m_model, 1, GL_FALSE, glm::value_ptr(m_model));
    glUniformMatrix4fv(uniform_m_projection, 1, GL_FALSE, glm::value_ptr(m_projection));
    
    glUseProgram(program_box);
    glUniformMatrix4fv(uniform_m_model_b, 1, GL_FALSE, glm::value_ptr(m_model_b));
    glUniformMatrix4fv(uniform_m_projection_b, 1, GL_FALSE, glm::value_ptr(m_projection_b));
 
    glutPostRedisplay();

}

void free_resources() {
    glDeleteProgram(program_box);
	glDeleteProgram(program_text);
	glDeleteProgram(program_text2d);
    glDeleteBuffers(1, &vbo_coord2d);
    glDeleteBuffers(1, &vbo_tex_coord2d);
    glDeleteBuffers(1, &vbo_coord);
    glDeleteBuffers(1, &vbo_tex_coord);
    glDeleteBuffers(1, &vbo_vertices_box);
    glDeleteBuffers(1, &vbo_color_box);
    glDeleteBuffers(1, &vbo_font_color);
    glDeleteBuffers(1, &vbo_font_color2d);
    glDeleteBuffers(1, &ibo_box_elements);
    //~ glDeleteBuffers(1, &vbo);
    
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo_cube_vertices);
    glDeleteBuffers(1, &vbo_cube_colors);
    glDeleteBuffers(1, &ibo_cube_elements);    
}

void mouse_movement(int xpos, int ypos)
{

    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        //~ firstMouse = false;
        firstMouse = false;
    }
    if(left_click)
    {
        
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; 
        lastX = xpos;
        lastY = ypos;
    
        float sensitivity = 0.05;
        xoffset *= sensitivity;
        yoffset *= sensitivity;
    
        yaw   += xoffset;
        pitch += yoffset;
    
        if(pitch > 89.0f)
            pitch = 89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;
    
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        if(ortho_mode){
            orthoX+=xoffset;
            orthoY+=yoffset;
        }
        cameraFront = glm::normalize(direction);
    }
}  

void special_key(int key, int x, int y){
    
    //set consistent camera speed with frame rate
    cameraSpeed = (2.5f * deltaTime)/1000;
    
    switch(key)
    {
        case GLUT_KEY_UP:
            cameraPos += cameraSpeed * cameraFront;
            break;
        case GLUT_KEY_DOWN:
            cameraPos -= cameraSpeed * cameraFront;
            break;
        case GLUT_KEY_LEFT:
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            break;
        case GLUT_KEY_RIGHT:
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            break;
    }
}


void keyboard(unsigned char key, int x, int y)
{
    // delete all programs and buffers then quit when 'ESC' is pressed.
    switch (key)
    {
        case 'w':
            cameraPos += cameraSpeed * cameraFront;
            if(ortho_mode)orthoZ -= 1.0;
            break;
        case 's':
            cameraPos -= cameraSpeed * cameraFront;
            if(ortho_mode)orthoZ += 1.0;
            break;
        case 'a':
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            if(ortho_mode)orthoX -= 0.1;
            break;
        case 'd':
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            if(ortho_mode)orthoX += 0.1;
            break;
        case 'q':
            cameraPos += cameraSpeed * cameraUp;
            if(ortho_mode)orthoY += 0.1;
            break;
        case 'z':
            cameraPos -= cameraSpeed * cameraUp;
            if(ortho_mode)orthoY -= 0.1;
            break;
        case 'b':
            display_box = !display_box;
            break;
        case 'm':
            move_it = !move_it;
            break;
        case '2':
            ortho_mode = !ortho_mode;
            break;
        case '3':
            display3d = !display3d;
            break;
        case '<':
            multiply -=1.0;
            break; 
        case '>':
            multiply +=1.0;
            break; 
        case ',':
            unit /=10.0;
            break; 
        case '.':
            unit *=10.0;
            break; 
        case 'r':
            resetCameraMatrices();
            break; 
        //~ case '+':
            //~ cameraSpeed +=0.05;
            //~ break;
        //~ case '-':
            //~ (cameraSpeed >= 0) ? cameraSpeed -=0.05 : cameraSpeed = 0.05;
            //~ fprintf(stderr,"speed: %.2f/n", cameraSpeed);
            //~ break;
            
       // Check if 'ESC' is pressed
        case 27:
            free_resources();
            glutLeaveMainLoop();
            break;
        
    }
    
}

void resetCameraMatrices(){
    cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
    if(ortho_mode){
        orthoX = -0.6f;
        orthoY = -0.5f;
        orthoZ = 0.0f;
        multiply = 4.0; 
    }        
}
