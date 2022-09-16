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

const char *inputText;
//~ uint fontSize=48;
uint fontSize=12;
GLfloat r,g,b,a;
GLfloat bg_r, bg_g, bg_b, bg_a;

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

const char *fontfilename;

// Maximum texture width
#define MAXWIDTH 1024

struct point {
	GLfloat x;
	GLfloat y;
	GLfloat s;
	GLfloat t;
};

struct point3D {
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat s;
	GLfloat t;
};


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

std::vector<vertex3Dalpha> v_font_color;
std::vector<vertex3Dalpha> vector_font_color;

std::vector<uint> nb_glyphs_per_text;

void create_coord_vector_text(const char *text, atlas * a, vertex3D origin, vertex2D &window_coord_scale){
    
    const uint8_t *p;
    uint c = 0;

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
        
        glyphs_box.push_back((vertex3D) {
        x2, -y2, origin.z});
        glyphs_box.push_back((vertex3D) {
        x2 + w, -y2, origin.z});
        glyphs_box.push_back((vertex3D) {
        x2, -y2 - h, origin.z});
        glyphs_box.push_back((vertex3D) {
        x2 + w, -y2, origin.z});
        glyphs_box.push_back((vertex3D) {
        x2, -y2 - h, origin.z});
        glyphs_box.push_back((vertex3D) {
        x2 + w, -y2 - h, origin.z});

        glyphs_vertex.push_back((vertex2D) {
        a->c[*p].tx, a->c[*p].ty});
        glyphs_vertex.push_back((vertex2D) {
        a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty});
        glyphs_vertex.push_back((vertex2D) {
        a->c[*p].tx, a->c[*p].ty + a->c[*p].bh / a->h});
        glyphs_vertex.push_back((vertex2D) {
        a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty});
        glyphs_vertex.push_back((vertex2D) {
        a->c[*p].tx, a->c[*p].ty + a->c[*p].bh / a->h});
        glyphs_vertex.push_back((vertex2D) {
        a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty + a->c[*p].bh / a->h});

    }
    
    //~ displayV3D(glyphs_box);
    nb_glyphs_per_text.push_back(c);
    nb_indices+=c;
    //~ std::clog << "nb_glyphs_per_text" << std::endl;
    //~ displayVuint(nb_glyphs_per_text);
    //~ std::clog << " nb_indices " << nb_indices << std::endl;
    //~ std::clog << " size of glyphs_vertex " << glyphs_vertex.size() << std::endl;
    
}

void text_frame(std::vector<vertex3D> &vector_input, std::vector<uint> &nb_glyphs, std::vector<vertex3D> &v_origin, vertex2D &padding){
    
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
            //~ std::clog << " x: "<< (*it3).x << " y: " << (*it3).y << " z: " << (*it3).z << std::endl;
            //~ std::clog << " hmin: "<< hmin << " hmax: " << hmax << std::endl;
        }
        //~ std::clog << std::endl;
        h = std::abs(hmax - hmin);
        // for now depth is the same as the width of the text
        //~ std::clog << "h: " << h << " hmax: " << hmax << " hmin: " << hmin << std::endl;
        d=h;
        
        bottom_left_front.push_back({wmin,hmin,0});
        top_right_back.push_back({wmax,hmax,d});
        
        std::advance(it,(*it2));
        std::advance(it2,1);
    }
    std::clog << " bottom_left_front :" << std::endl;
    displayV3D(bottom_left_front);
    std::clog << " top_right_back :" << std::endl;
    displayV3D(top_right_back);

    create_vector(vertex_3D, v_origin, bottom_left_front, top_right_back);
    // !!!!!! commented below for testing box display bug
    std::clog << "1) vertex_3D :" << std::endl;
    displayV3D(vertex_3D);
    init_vectors(vertex_3D);
    std::clog << "2) vertex_3D :" << std::endl;
    displayV3D(vertex_3D);
    init_box(vertex_3D);
    std::clog << "3) vertex_3D :" << std::endl;
    displayV3D(vertex_3D);
    
}

void render_text(atlas * a) {
    
    //~ std::clog<<"######## In render_text ###########"<<std::endl;
    //~ std::clog<<"1"<<std::endl;
    //~ displayV3Dalpha(vector_font_color);
    /* test color per block*/
    glEnableVertexAttribArray(attribute_font_color);
    glGenBuffers(1,&vbo_font_color);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_font_color);
    glVertexAttribPointer(attribute_font_color, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glBufferData(GL_ARRAY_BUFFER,vector_font_color.size() * sizeof(vertex3Dalpha),&vector_font_color.front(),GL_STATIC_DRAW);
    //~ std::clog<<"2"<<std::endl;
 
    /* Use the texture containing the atlas */
    glBindTexture(GL_TEXTURE_2D, a->tex);
    
    /* Set up the VBO for our vertex data */
    glEnableVertexAttribArray(attribute_coord);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_coord);
    glVertexAttribPointer(attribute_coord, 3, GL_FLOAT, GL_FALSE, 0, 0);
    /* Draw the character on the screen */
    glBufferData(GL_ARRAY_BUFFER, glyphs_box.size() * sizeof(vertex3D), &glyphs_box.front(), GL_STATIC_DRAW);
    //~ displayV3D(glyphs_box);
    //~ std::clog<<"3"<<std::endl;
    
    /* Set up the VBO for our vertex data */
    glEnableVertexAttribArray(attribute_tex_coord);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_tex_coord);
    glVertexAttribPointer(attribute_tex_coord, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    //~ std::clog<<"4"<<std::endl;
    //~ std::clog<<"nb_indices: "<<nb_indices<<std::endl;
    //~ displayV2D(glyphs_vertex);
    /* Draw all the character on the screen in one go */
    glBufferData(GL_ARRAY_BUFFER,glyphs_vertex.size() * sizeof(vertex2D),&glyphs_vertex.front(),GL_STATIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, nb_indices);

    //~ std::clog<<"5"<<std::endl;
    
    glDisableVertexAttribArray(attribute_font_color);
    glDisableVertexAttribArray(attribute_coord);
    glDisableVertexAttribArray(attribute_tex_coord);
    //~ std::clog<<"######## End render_text ###########"<<std::endl;
}

bool init_font(uint uFontSize,const char *uFontFilename) {
    
    /*Set the font size from argument */
    fontSize=uFontSize;
    
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
    //~ std::clog << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
    
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


	//~ if(attribute_coord2d == -1 || attribute_tex_coord2d == -1 || uniform_tex == -1 || uniform_color == -1 || attribute_font_color2d == -1)
	if(attribute_coord2d == -1 || attribute_tex_coord2d == -1 || attribute_font_color2d == -1 || uniform_tex2d == -1 || uniform_bgcolor_text2d ==-1)
    {
        fprintf(stderr,"Problem with attribute or uniform\n");
		return 0;
    }

    return 1;
}

void init_color_box(vertex3Dalpha &color_alpha)
{

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
  
}

void init_color(vertex3Dalpha &color_alpha, vertex3Dalpha &bg_color_alpha, std::vector<vertex3Dalpha> &font_color_alpha, std::vector<vertex3D> &whd, std::vector<unsigned int> &nb_glyphs_per_text){
    r=color_alpha.x;
    g=color_alpha.y;
    b=color_alpha.z;
    a=color_alpha.a;
    
    bg_r=bg_color_alpha.x;
    bg_g=bg_color_alpha.y;
    bg_b=bg_color_alpha.z;
    bg_a=bg_color_alpha.a;
    
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

    fprintf(stderr,"whd.size: %u\n",whd.size());
    int max_loop = whd.size();
    for(int i=0;i<max_loop;i++){
        vect_color3.insert (vect_color3.end(), box_vertices_color3, box_vertices_color3+32);
        }

  GLfloat cube_colors[] = {

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

    glGenBuffers(1,&vbo_color_box);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_color_box);
    glBufferData(GL_ARRAY_BUFFER,vect_color3.size() * sizeof(GLfloat),&vect_color3.front(),GL_STATIC_DRAW);
    
    // font colors
    std::clog << "########init font color########" << std::endl;
    int ind = 0;
    for(std::vector<unsigned int>::iterator it=nb_glyphs_per_text.begin(); it!=nb_glyphs_per_text.end();it++){
        //~ std::clog << "nb glyphs: " << (*it) << std::endl;
        for(unsigned int i=0;i<(*it);i++){
            
            vector_font_color.push_back(font_color_alpha[ind]);
        }
        ind++;
    }
    //~ std::clog << "display vect_color3 size: " << vect_color3.size() << std::endl;
    //~ displayVGLfloat(vect_color3);
    //~ std::clog << "display font_color_alpha size: " << font_color_alpha.size() << std::endl;
    //~ std::clog << "display font_color_alpha:" << std::endl;
    //~ displayV3Dalpha(font_color_alpha);
    //~ std::clog << "display nb_glyphs_per_text size: " << nb_glyphs_per_text.size() << std::endl;
    //~ std::clog << "display vector_font_color size: " << vector_font_color.size() << std::endl;
    //~ std::clog << "display vector_font_color:" << std::endl;
    //~ displayV3Dalpha(vector_font_color);
    //~ std::clog << "########init font color done########" << std::endl;
    
}

void init_v_font_color(std::vector<vertex3Dalpha> &font_color, uint &nlines, bool singleColor, bool displayDataOnly){
    
    std::clog<<"####### init_v_font_color ########" << std::endl;
    if(singleColor)
    {
        // add same color for each glyph
        for(uint i=0;i<nlines;i++)
        {
            for(std::vector<vertex3Dalpha>::iterator it=font_color.begin();it!=font_color.end();++it){
                v_font_color.push_back((*it));
                //~ v_font_color.push_back((*it).x,(*it).y,(*it).z,(*it).a);
                // if displaying name and data we push back another set of same color
                //TODO introduce different color for name and data
                //~ if(!displayDataOnly) v_font_color.push_back((*it).x,(*it).y,(*it).z,(*it).a);
                if(!displayDataOnly) v_font_color.push_back((*it));
            }
            
        }
    }
    font_color = v_font_color;
    displayV3Dalpha(font_color);
    std::clog << "####### End init_v_font_color #######" << std::endl;
    
}

void displayVGLfloat(std::vector<GLfloat> &vector_glf){
    std::clog<<"loop GLfloat"<<std::endl;
        std::clog<<"| ";
    for(std::vector<GLfloat>::iterator it=vector_glf.begin();it!=vector_glf.end();++it){
        std::clog<<(*it)<<", ";
    }
    std::clog<<" |"<<std::endl;
    
}

void displayVuint(std::vector<unsigned int> &vector_uint){
    std::clog<<"loop Vint"<<std::endl;
        std::clog<<"| ";
    for(std::vector<unsigned int>::iterator it=vector_uint.begin();it!=vector_uint.end();++it){
        std::clog<<(*it)<<", ";
    }
    std::clog<<" |"<<std::endl;
    
}

void displayV3D(std::vector<vertex3D> &vector_3D){
    std::clog<<"loop V3D"<<std::endl;
    for(std::vector<vertex3D>::iterator it=vector_3D.begin();it!=vector_3D.end();++it){
        
        std::clog<<"| ";
        std::clog<<(*it).x<<", ";
        std::clog<<(*it).y<<", ";        
        std::clog<<(*it).z;        
        std::clog<<" |"<<std::endl;
    }
    
}

void displayV2D(std::vector<vertex2D> &vector_2D){
    std::clog<<"loop V2D"<<std::endl;
    for(std::vector<vertex2D>::iterator it=vector_2D.begin();it!=vector_2D.end();++it){
        
        std::clog<<"| ";
        std::clog<<(*it).x<<", ";
        std::clog<<(*it).y;
        std::clog<<" |"<<std::endl;
    }
    
}

void displayV3Dalpha(std::vector<vertex3Dalpha> &vector_3Dalpha){
    std::clog<<"loop V3Dalpha"<<std::endl;
    for(std::vector<vertex3Dalpha>::iterator it=vector_3Dalpha.begin();it!=vector_3Dalpha.end();++it){
        
        std::clog<<"| ";
        std::clog<<(*it).x<<", ";
        std::clog<<(*it).y<<", ";        
        std::clog<<(*it).z<<", ";;        
        std::clog<<(*it).a;        
        std::clog<<" |"<<std::endl;
    }
    
}

 // blf: bottom-left-front, trb: top-right-back
void create_vector(std::vector<vertex3D> &vector3D,std::vector<vertex3D> &v_origin, std::vector<vertex3D> &vector_blf, std::vector<vertex3D> &vector_trb)
{
    
    std::vector<vertex3D>::iterator v_origin_it;
    std::vector<vertex3D>::iterator v_blf_it;   // bottom left front
    std::vector<vertex3D>::iterator v_trb_it;   // top right back 

    for(v_origin_it=v_origin.begin(), v_blf_it=vector_blf.begin(), v_trb_it=vector_trb.begin(); v_origin_it!=v_origin.end() && v_blf_it!=vector_blf.end() && v_trb_it!=vector_trb.end();++v_origin_it, ++v_blf_it, ++v_trb_it){
        // 1st vertex is bottom left corner, front face
        vector3D.push_back({(*v_blf_it).x,(*v_blf_it).y,(*v_origin_it).z});
        // 2nd vertex, keep 1st vertex x and z, add y from vector2D, to get upper left corner, front face
        vector3D.push_back({vector3D.back().x,(*v_trb_it).y,vector3D.back().z});
        // 3rd vertex, back is now 2nd vertex, so just add y, add x from, to get upper right corner, front face
        vector3D.push_back({(*v_trb_it).x,vector3D.back().y,vector3D.back().z});
        // 4th vertex, back is now 3rd vertex, so deduct y, keeping x, to get bottom right corner, front face
        vector3D.push_back({vector3D.back().x,(*v_blf_it).y,vector3D.back().z});
        // 5th vertex deduct x to come back to bottom left corner, and deduct a depth for z to get back face
        vector3D.push_back({(*v_blf_it).x,vector3D.back().y,vector3D.back().z-(*v_trb_it).z});
        // 6nd vertex, keep 1st vertex x and z, add y from vector2D, to get upper left corner, back face
        vector3D.push_back({vector3D.back().x,(*v_trb_it).y,vector3D.back().z});
        // 7th vertex, back is now 2nd vertex, so just add y, add x from, to get upper right corner, back face
        vector3D.push_back({(*v_trb_it).x,vector3D.back().y,vector3D.back().z});
        // 8th vertex, back is now 3rd vertex, so deduct y, keeping x, to get bottom right corner, back face
        vector3D.push_back({vector3D.back().x,(*v_blf_it).y,vector3D.back().z});

            }
}

void init_vbo_box(std::vector<vertex3D> &vectors_boxes)
{
    glGenBuffers(1,&vbo_vertices_box);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_vertices_box);
    glBufferData(GL_ARRAY_BUFFER,vectors_boxes.size() * sizeof(vertex3D),&vectors_boxes.front(),GL_STATIC_DRAW);

}

void init_ibo_elements_box(std::vector<vertex3D> &vectors_boxes)
{
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

    int max_loop = vectors_boxes.size();
    std::clog<< "######init vectors######"<<std::endl;
    std::clog<< "max_loop(vectors_boxes.size): "<< max_loop << std::endl;
    displayV3D(vectors_boxes);
    for(GLushort i=0;i<max_loop;i++){
        for ( auto it = box_elements3.begin(); it != box_elements3.end(); ++it ){
            vect_index.push_back((*it)+8*i);
        }
    }
    std::clog<< "vect_index size: "<< vect_index.size() << std::endl;
    std::clog<< "######End init vectors######"<<std::endl;

    glGenBuffers(1, &ibo_box_elements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_box_elements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vect_index.size() * sizeof(GLushort), &vect_index.front(), GL_STATIC_DRAW);
    
}

//~ void init_box(std::vector<vertex3D> &vectors_boxes, vertex3Dalpha &color_alpha)
void init_box(std::vector<vertex3D> &vectors_boxes)
{
    //~ std::vector<vertex3D> vectors_boxes;
    
    std::clog << "######## init box #########" << std::endl;
    displayV3D(vectors_boxes);
    init_vbo_box(vectors_boxes);
    //~ init_color_box(vertex3Dalpha color_alpha);
    init_ibo_elements_box(vectors_boxes);
    std::clog << "######## End init box #########" << std::endl;
}

void init_vectors(std::vector<vertex3D> &vectors_boxes)
{

    // set the array of indexes for a rectangle
    //~ std::array<GLushort,36> box_elements3 = {
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

    std::vector<GLushort> vect_index;

    // Creating s vec3 from the first element of vectors_boxes for debugging bax misaligment
    check_coord3d_b = glm::vec3(vectors_boxes[0].x,vectors_boxes[0].y,vectors_boxes[0].z);
    

    int max_loop = vectors_boxes.size();
    std::clog<< "######init vectors######"<<std::endl;
    std::clog<< "max_loop(vectors_boxes.size): "<< max_loop << std::endl;
    displayV3D(vectors_boxes);
    //~ for(GLushort i=0;i<max_loop;i++){
        //~ for ( auto it = box_elements3.begin(); it != box_elements3.end(); ++it ){
            //~ vect_index.push_back((*it)+8*i);
        //~ }
    //~ }
    std::clog<< "vect_index size: "<< vect_index.size() << std::endl;
    

    std::clog<<"check_coord3d_b:"<< std::endl;
    std::clog<<glm::to_string(check_coord3d_b)<<std::endl;
    
    std::clog<< "######End init vectors######"<<std::endl;



    //~ glGenBuffers(1, &ibo_box_elements);
    //~ glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_box_elements);
    //~ glBufferData(GL_ELEMENT_ARRAY_BUFFER, vect_index.size() * sizeof(GLushort), &vect_index.front(), GL_STATIC_DRAW);
    glGenBuffers(1, &ibo_box_elements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_box_elements);
    //~ glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort), box_elements3, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(box_elements3), box_elements3, GL_STATIC_DRAW);
    
    //~ return 1;
    
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

void init_text_MDE(MDE &my_entity,vertex3D &origin, std::vector<vertex3D> &offset, std::vector<vertex3D> &offset_rule, vertex2D &padding, vertex3Dalpha &color_alpha, vertex3Dalpha &bg_color_alpha, std::vector<vertex3Dalpha> &font_color, bool displayDataOnly, bool hideParent) {
    
    /* Create texture atlasses for a given font size */
    a_any_size = new atlas(face,(int)fontSize);
    
    vertex2D window_scale;
    window_scale.x = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
    window_scale.y = 2.0 / glutGet(GLUT_WINDOW_HEIGHT);
    //~ window_scale.x = 1.0 / glutGet(GLUT_WINDOW_WIDTH);
    //~ window_scale.y = 1.0 / glutGet(GLUT_WINDOW_HEIGHT);
    std::vector<vertex3D> v_origin;      // coordinates of origin for the glyphs/text
    std::vector<vertex3D> v_origin_fb;   // coordinates of origin for the frame box to include padding

    std::vector<GLdata> my_v_MDE_data;
    int level=0;
    int index=0;
    extractMDEdataForVoffset(my_entity, my_v_MDE_data, level,index ,displayDataOnly = displayDataOnly, hideParent = hideParent);
    process_v_MDE_data(my_v_MDE_data,offset, offset_rule);
    for (std::vector<vertex3D>::iterator it_offset=offset.begin(); it_offset!=offset.end();++it_offset){
        v_origin.push_back({origin.x+(*it_offset).x,origin.y+(*it_offset).y,origin.z+(*it_offset).z});
    }
    
    // Adding padding at first and last elements of v_origin
    add_padding(v_origin, v_origin_fb, padding);
    std::clog<<"offset_rule: "<<std::endl;
    displayV3D(offset_rule);
    std::clog<<"offset: "<<std::endl;
    displayV3D(offset);
    std::clog<<"v_origin: "<<std::endl;
    displayV3D(v_origin);
    std::clog<<"v_origin_fb: "<<std::endl;
    displayV3D(v_origin_fb);
    

    
    uint index_v_origin=0;
    for (std::vector<GLdata>::iterator it_v_MDE_data=my_v_MDE_data.begin(); it_v_MDE_data!=my_v_MDE_data.end();++it_v_MDE_data)
    {
        create_coord_vector_text(((*it_v_MDE_data).MDE_data).c_str(), a_any_size, v_origin[index_v_origin], window_scale );
        index_v_origin++;
    }
    std::clog << "size of glyphs box: " << glyphs_box.size() << std::endl;  
    text_frame(glyphs_box, nb_glyphs_per_text, v_origin_fb, padding);
    nlines = (uint)nb_glyphs_per_text.size();
    //~ bool singleColor = true;
    fprintf(stderr,"number of lines in init_text_MDE:%u\n",nlines);
    init_v_font_color(font_color,nlines , true, true);
    init_color(color_alpha,bg_color_alpha,font_color,v_origin,nb_glyphs_per_text);
    
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
    
    for (std::vector<vertex3D>::iterator it=v_origin.begin();it!=v_origin.end(); ++it){
        v_origin_fb.push_back({(*it).x-padding.x,(*it).y-padding.y,(*it).z});
    }

}

void text_display() {

    // set the color of the non-glyph part of the glyph box to same color as the
    // cube color: r,g,b
    // keep alpha to zero
    //~ GLfloat bg_rgb[4] = { r, g, b, a };
    GLfloat bg_rgb[4] = { r, g, b, 0 };
    
    FT_Set_Pixel_Sizes(face, 0, fontSize);

    glUseProgram(program_text);
    glUniform4fv(uniform_bgcolor_text, 1, bg_rgb);
    render_text(a_any_size);
    
}

void set_options(bool a_move_it, bool a_display_box, bool a_ortho_mode, bool singleColor,uint a_nlines){
    move_it = a_move_it;
    display_box = a_display_box;
    ortho_mode = a_ortho_mode;
    nlines = a_nlines;
    fprintf(stderr,"display box:%u\n", display_box);
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
    
    //~ // Set the colored background, default is white.
    glClearColor(bg_r, bg_g, bg_b, bg_a);
    // need the depth buffer for 3d
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //~ if(ortho_mode)
    //~ {
        //~ glEnable(GL_DEPTH_TEST);
        //~ glDepthFunc(GL_NOTEQUAL);
    //~ }
    //~ else
    //~ {
        //~ glDisable(GL_DEPTH_TEST);
    //~ }
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_NOTEQUAL);
    // camera operations and text display
    camera();
    //~ std::clog<<"####### In display ######"<<std::endl;
    //~ std::clog<<"####### first is draw_box ######"<<std::endl;
    //~ std::clog<<"####### next is text_display ######"<<std::endl;
    if(display_box) draw_box();
    text_display();
    //~ std::clog<<"####### End display ######"<<std::endl;
    //~ onDisplay();
    
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
    //~ if (ortho_mode) m_view = glm::ortho(-1.0f*multiply, 1.0f*multiply, -1.0f*multiply, 1.0f*multiply, 0.1f*multiply, 100.0f*multiply);
    if (ortho_mode) m_view = glm::ortho((-1.0f+orthoX)*multiply, (1.0f+orthoX)*multiply, (-1.0f+orthoY)*multiply, (1.0f+orthoY)*multiply, (0.1f+orthoZ)*multiply, (100.0f+orthoZ)*multiply);
    //~ if (display2d) m_view = glm::ortho(-1000.0f, 1000.0f, -1000.0f, 1000.0f, -1000.0f, 1000.0f);
    
    // Projection matrix modified by field of view (fov) directed by the camera zoom from the mouse wheel
    glm::mat4 m_projection = glm::perspective(glm::radians(fov), 1.0f*screen_width/screen_height, 0.1f, 10.0f);
    if(ortho_mode) m_projection = glm::ortho(0.0f,1.0f*(float)screen_width*scale, 0.0f,1.0f*(float)screen_height*scale);
    
    glm::mat4 m_view_b = m_view;
    glm::mat4 m_projection_b = m_projection;
        
    //~ std::clog<<"######## camera ########:"<< std::endl;
    //~ if(ortho_mode) std::clog<<"!!!! Ortho Mode !!!!:"<< std::endl;
    //~ std::clog<<"m_projection:"<< std::endl;
    //~ std::clog<<glm::to_string(m_projection)<<std::endl;
    //~ std::clog<<"m_projection_b:"<< std::endl;
    //~ std::clog<<glm::to_string(m_projection_b)<<std::endl;
    //~ std::clog<<"m_view:"<< std::endl;
    //~ std::clog<<glm::to_string(m_view)<<std::endl;
    //~ std::clog<<"m_view_b:"<< std::endl;
    //~ std::clog<<glm::to_string(m_view_b)<<std::endl;
    //~ std::clog<<"screen: " << screen_width << " , " << screen_height << std::endl;
    //~ std::clog<<"multiply: " << multiply << " , unit: " << unit << std::endl;
    
    glUseProgram(program_text);
    glUniformMatrix4fv(uniform_m_view, 1, GL_FALSE, glm::value_ptr(m_view));
    glUniformMatrix4fv(uniform_m_projection, 1, GL_FALSE, glm::value_ptr(m_projection));

    glUseProgram(program_box);
    glUniformMatrix4fv(uniform_m_view_b, 1, GL_FALSE, glm::value_ptr(m_view_b));
    glUniformMatrix4fv(uniform_m_projection_b, 1, GL_FALSE, glm::value_ptr(m_projection_b));
    
    glutPostRedisplay();
}

void init_matrices(){
    
    std::clog << "######## Init Matrices ########" << std::endl;
     
    glm::mat4 m_model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0, 0.0, -1.0));
    glm::mat4 m_projection = glm::perspective(glm::radians(fov), 1.0f*screen_width/screen_height, 0.1f, 10.0f);    
    if(ortho_mode) m_projection = glm::ortho(0.0f,1.0f*(float)screen_width*scale, 0.0f,1.0f*(float)screen_height*scale);

    glm::mat4 m_view = glm::lookAt(glm::vec3(0.0, 0.0, 4.0), glm::vec3(0.0, 0.0,-1.0), glm::vec3(0.0, 1.0, 0.0));//eye,center,up
    glm::mat4 m_view_b = m_view;
    glm::mat4 m_projection_b = m_projection;
    glm::mat4 m_model_b = m_model;
 
    std::clog<<"m_model:"<< std::endl;
    std::clog<<glm::to_string(m_model)<<std::endl;
    std::clog<<"m_model_b:"<< std::endl;
    std::clog<<glm::to_string(m_model_b)<<std::endl;
    
    glUseProgram(program_text);
    glUniformMatrix4fv(uniform_m_model, 1, GL_FALSE, glm::value_ptr(m_model));
    glUniformMatrix4fv(uniform_m_projection, 1, GL_FALSE, glm::value_ptr(m_projection));
    glUniformMatrix4fv(uniform_m_view, 1, GL_FALSE, glm::value_ptr(m_view));
    
    //~ glUseProgram(program_text2d);
    //~ glUniformMatrix4fv(uniform_m_model, 1, GL_FALSE, glm::value_ptr(m_model));
    //~ glUniformMatrix4fv(uniform_m_projection2d, 1, GL_FALSE, glm::value_ptr(m_projection2d));
    //~ glUniformMatrix4fv(uniform_m_view, 1, GL_FALSE, glm::value_ptr(m_view));
    
    glUseProgram(program_box);
    glUniformMatrix4fv(uniform_m_model_b, 1, GL_FALSE, glm::value_ptr(m_model_b));
    glUniformMatrix4fv(uniform_m_projection_b, 1, GL_FALSE, glm::value_ptr(m_projection_b));
    glUniformMatrix4fv(uniform_m_view_b, 1, GL_FALSE, glm::value_ptr(m_view_b));
 
    std::clog<<"m_projection:"<< std::endl;
    std::clog<<glm::to_string(m_projection)<<std::endl;
    std::clog<<"screen: " << screen_width << " , " << screen_height << std::endl;
    std::clog << "End init Matrices" << std::endl;
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

    //~ std::clog<<"######### onIdle ########"<< std::endl;
    //~ std::clog<<"m_model:"<< std::endl;
    //~ std::clog<<glm::to_string(m_model)<<std::endl;
    //~ std::clog<<"m_projection:"<< std::endl;
    //~ std::clog<<glm::to_string(m_projection)<<std::endl;
    //~ std::clog<<"m_view:"<< std::endl;
    //~ std::clog<<glm::to_string(m_view)<<std::endl;
    //~ std::clog<<"test:"<< std::endl;
    //~ std::clog<<glm::to_string(test)<<std::endl;
    //~ std::clog<<"screen: " << screen_width << " , " << screen_height << std::endl;

    glUseProgram(program_text);
    glUniformMatrix4fv(uniform_m_model, 1, GL_FALSE, glm::value_ptr(m_model));
    glUniformMatrix4fv(uniform_m_projection, 1, GL_FALSE, glm::value_ptr(m_projection));
    
    glUseProgram(program_box);
    glUniformMatrix4fv(uniform_m_model_b, 1, GL_FALSE, glm::value_ptr(m_model_b));
    glUniformMatrix4fv(uniform_m_projection_b, 1, GL_FALSE, glm::value_ptr(m_projection_b));
 
    glutPostRedisplay();

}

void onIdle2() {
    //~ float angle = glutGet(GLUT_ELAPSED_TIME) / 1000.0 * 45;  // 45° per second
    float angle = 0;
    glm::vec3 axis_y(0, 1, 0);
    glm::mat4 anim = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis_y);
    
    //~ glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0, 0.0, -1.0));
    glm::mat4 view = glm::lookAt(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
    //~ glm::mat4 projection = glm::perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 10.0f);
    glm::mat4 projection = glm::perspective(glm::radians(fov), 1.0f*screen_width/screen_height, 0.1f, 10.0f);    

    glm::mat4 mvp = projection * view * model * anim;

    //~ std::clog<<"######### onIdle2 ########"<< std::endl;
    //~ std::clog<<"anim:"<< std::endl;
    //~ std::clog<<glm::to_string(anim)<<std::endl;
    //~ std::clog<<"model:"<< std::endl;
    //~ std::clog<<glm::to_string(model)<<std::endl;
    //~ std::clog<<"view:"<< std::endl;
    //~ std::clog<<glm::to_string(view)<<std::endl;
    //~ std::clog<<"projection:"<< std::endl;
    //~ std::clog<<glm::to_string(projection)<<std::endl;
    //~ std::clog<<"mvp:"<< std::endl;
    //~ std::clog<<glm::to_string(mvp)<<std::endl;

    //~ glUseProgram(program);
    //~ glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
    glUseProgram(program_box);
    glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
    glutPostRedisplay();
}


//####### Testing 2d

void render_text2d(atlas * a) {

	/* Use the texture containing the atlas */
	glBindTexture(GL_TEXTURE_2D, a->tex);
    
    /* test color per block*/
    glEnableVertexAttribArray(attribute_font_color2d);
    glGenBuffers(1,&vbo_font_color2d);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_font_color2d);
    glVertexAttribPointer(attribute_font_color2d, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glBufferData(GL_ARRAY_BUFFER,vector_font_color.size() * sizeof(vertex3Dalpha),&vector_font_color.front(),GL_STATIC_DRAW);

    /* Set up the VBO for our vertex data */
    glEnableVertexAttribArray(attribute_coord2d);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_coord2d);
    glVertexAttribPointer(attribute_coord2d, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBufferData(GL_ARRAY_BUFFER, glyphs_box.size() * sizeof(vertex3D), &glyphs_box.front(), GL_STATIC_DRAW);
   
    /* Set up the VBO for our vertex data */
    glEnableVertexAttribArray(attribute_tex_coord2d);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_tex_coord2d);
    glVertexAttribPointer(attribute_tex_coord2d, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glBufferData(GL_ARRAY_BUFFER,glyphs_vertex.size() * sizeof(vertex2D),&glyphs_vertex.front(),GL_STATIC_DRAW);

    /* Draw all the character on the screen in one go */
    glDrawArrays(GL_TRIANGLES, 0, nb_indices);

	glDisableVertexAttribArray(attribute_font_color2d);
	glDisableVertexAttribArray(attribute_coord2d);
	glDisableVertexAttribArray(attribute_tex_coord2d);
}

void display_text2d() {
	float sx = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
	float sy = 2.0 / glutGet(GLUT_WINDOW_HEIGHT);
    
	glUseProgram(program_text2d);
    
	/* White background */
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	/* Enable blending, necessary for our alpha texture */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

	GLfloat black[4] = { 0, 0, 0, 1 };
	GLfloat red[4] = { 1, 0, 0, 1 };
	GLfloat transparent_green[4] = { 0, 1, 0, 0.5 };

	/* Set color to black */
	//~ glUniform4fv(uniform_color, 1, black);

	//~ render_text2d("The Quick Brown Fox Jumps Over The Lazy Dog", a_any_size, -1 + 8 * sx, 1 - 50 * sy, sx, sy);
    render_text2d(a_any_size);
    //~ render_text(a_any_size);

	glutSwapBuffers();
}



//#######



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
