// Testing font texturing
// In this program_text, there are 2 shader object programs
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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "./common/shader_utils.h"

#include "E.h"

int screen_width=800, screen_height=600;

GLuint program_test;
GLint attribute_coord_test;
GLint uniform_tex_test;
GLint uniform_color_test;

GLuint program_text;
GLuint program_box;
GLuint program_bg;
GLuint program_cube;

GLint attribute_coord;
GLint attribute_tex_coord;
GLint attribute_coord2d;
GLint attribute_coord3d;
GLint attribute_coord3d_box;
GLint attribute_v_color;
GLint attribute_v_color_box;

GLint uniform_tex;
GLint uniform_color;
GLint uniform_bgcolor;
GLint uniform_bgcolor_test;
GLint uniform_m_transform;

GLuint vbo_coord;
GLuint vbo_tex_coord;
GLuint vbo_vertices;
GLuint vbo_vertices_box;
GLuint vbo_vertices_box2;
GLuint vbo_vertices_box3;
GLuint vbo_vertices_color;
GLuint vbo_vertices_color_box;
GLuint vbo_vertices_color_box2;
GLuint vbo_vertices_color_box3;
GLuint vbo_test;

// for cube:
GLuint ibo_cube_elements;
GLuint ibo_box_elements;
GLuint ibo_box_elements2;
GLuint ibo_box_elements3;

const char *inputText;
uint fontSize=48;
GLfloat r,g,b;

//~ struct vertex2D {
    //~ GLfloat x;
    //~ GLfloat y;
//~ };
//~ struct vertex3D {
    //~ GLfloat x;
    //~ GLfloat y;
    //~ GLfloat z;
//~ };


vertex3D text_coordinates ={0.0,0.0,0.0};
vertex2D padding ={0.0,0.0};

// define 3 boxes with width, height and depth
std::vector<vertex3D> testing ={
    //~ {1.0,0.3,0.5},
    //~ {1.5,0.5,0.5},
    //~ {2.2,0.8,0.5}
    {0.5,0.3,0.2},
    {0.2,0.3,0.2}
    };


FT_Library ft;
FT_Face face;

const char *fontfilename;

//~ // this declaration is used to handle the coordinates and colors for each element
//~ struct attributes {
  //~ GLfloat coord3d[3];
  //~ GLfloat v_color[3];
//~ };


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
		FT_Set_Pixel_Sizes(face, 0, height);
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
		glUniform1i(uniform_tex, 0);

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

atlas *a48;
atlas *a24;
atlas *a12;

std::vector<point> textVect;
int nb_indices=0;

void create_coord_vector(const char *text, atlas * a, float x, float y, float sx, float sy){
    
    const uint8_t *p;
    
    std::cout<<"check vector creation" << std::endl;
    
    //~ point coords[6 * strlen(text)];
    int c = 0;
    
    /* Loop through all characters */
    for (p = (const uint8_t *)text; *p; p++) {
        /* Calculate the vertex and texture coordinates */
        float x2 = x + a->c[*p].bl * sx;
        float y2 = -y - a->c[*p].bt * sy;
        float w = a->c[*p].bw * sx;
        float h = a->c[*p].bh * sy;
    
        /* Advance the cursor to the start of the next character */
        x += a->c[*p].ax * sx;
        y += a->c[*p].ay * sy;
    
        /* Skip glyphs that have no pixels */
        if (!w || !h)
            continue;
        
        c+=6;
        //~ textVect.push_back((point) {
        //~ x2, -y2, a->c[*p].tx, a->c[*p].ty});
        //~ textVect.push_back((point) {
        //~ x2 + w, -y2, a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty});
        //~ textVect.push_back((point) {
        //~ x2, -y2 - h, a->c[*p].tx, a->c[*p].ty + a->c[*p].bh / a->h});
        //~ textVect.push_back((point) {
        //~ x2 + w, -y2, a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty});
        //~ textVect.push_back((point) {
        //~ x2, -y2 - h, a->c[*p].tx, a->c[*p].ty + a->c[*p].bh / a->h});
        //~ textVect.push_back((point) {
        //~ x2 + w, -y2 - h, a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty + a->c[*p].bh / a->h});
        textVect.push_back((point) {
        x2, -y2, a->c[*p].tx, a->c[*p].ty});
        textVect.push_back((point) {
        x2 + w, -y2, a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty});
        textVect.push_back((point) {
        x2, -y2 - h, a->c[*p].tx, a->c[*p].ty + a->c[*p].bh / a->h});
        textVect.push_back((point) {
        x2 + w, -y2, a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty});
        textVect.push_back((point) {
        x2, -y2 - h, a->c[*p].tx, a->c[*p].ty + a->c[*p].bh / a->h});
        textVect.push_back((point) {
        x2 + w, -y2 - h, a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty + a->c[*p].bh / a->h});

    }
    
    nb_indices=c;
}

uint nb_indices2=0;
int nb_indices3=0;

std::vector<vertex3D> glyphs_box;
std::vector<vertex2D> glyphs_vertex;
std::vector<vertex3D> glyphs_box2;
std::vector<vertex2D> glyphs_vertex2;
std::vector<point3D> textVect3D;

std::vector<uint> nb_glyphs_per_text;

void create_coord_vector_text(const char *text, atlas * a, float x, float y, float z, float sx, float sy){
    
    const uint8_t *p;
    
    std::cout<<"check vector creation for text" << std::endl;
    
    //~ point coords[6 * strlen(text)];
    int c = 0;
    //~ std::cout<<"text: " << text << std::endl;
    //~ std::cout<<"x: " << x << std::endl;
    //~ std::cout<<"a: " << a << std::endl;
    //~ std::cout<<"sx: " << sx << std::endl;
    //~ std::cout<<"sy: " << sy << std::endl;
    
    //~ float xmin, xmax = x;
    //~ float ymin, ymax = y;


    /* Loop through all characters */



    for (p = (const uint8_t *)text; *p; p++) {
        
        std::cout<<"check p: " << *p << std::endl;
        /* Calculate the vertex and texture coordinates */
        //~ std::cout<<"control 1" << std::endl;
        float x2 = x + a->c[*p].bl * sx;
        float y2 = -y - a->c[*p].bt * sy;
        float w = a->c[*p].bw * sx;
        float h = a->c[*p].bh * sy;
        
        //~ xmin = std::min(xmin,x2);
        //~ xmax = std::max(xmax,x2);
        //~ ymin = std::min(ymin,y2);
        //~ ymax = std::max(ymax,y2);
        
        //~ std::cout << "xmin, xmax, ymin, ymax: " << xmin << ", " <<
        //~ xmax << ", " << ymin << ", " <<ymax << std::endl;
        
        /* Advance the cursor to the start of the next character */
        x += a->c[*p].ax * sx;
        y += a->c[*p].ay * sy;
    
        /* Skip glyphs that have no pixels */
        if (!w || !h)
            continue;
        std::cout<<"check start textVect2D" << std::endl;
        c+=6;
        
        textVect3D.push_back((point3D) {
        x2, -y2, z, a->c[*p].tx, a->c[*p].ty});
        textVect3D.push_back((point3D) {
        x2 + w, -y2, z, a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty});
        textVect3D.push_back((point3D) {
        x2, -y2 - h, z, a->c[*p].tx, a->c[*p].ty + a->c[*p].bh / a->h});
        textVect3D.push_back((point3D) {
        x2 + w, -y2, z, a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty});
        textVect3D.push_back((point3D) {
        x2, -y2 - h, z, a->c[*p].tx, a->c[*p].ty + a->c[*p].bh / a->h});
        textVect3D.push_back((point3D) {
        x2 + w, -y2 - h, z, a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty + a->c[*p].bh / a->h});

    }
    
    //~ std::vector<vertex3D> background_frame;
    //~ background_frame.push_back({xmin,ymin,0});
    //~ vertex3D origin_background={x,y,z};
    //~ std::vector<vertex3D> background_vertices=create_vector3D(background_frame,origin_background);
    //~ displayV3D(background_vertices);

    //~ background_vertices=
    //~ {
        //~ {-10,-10,0},
        //~ {-10,10,0},
        //~ {10,10,0},
        //~ {10,-10,0}
    //~ };
    
    nb_indices2=c;
}

void create_coord_vector_text2(const char *text, atlas * a, float x, float y, float z, float sx, float sy){
    
    const uint8_t *p;
    
    std::cout<<"Control create coord vector text -- start" << std::endl;
    std::cout<<"number of chars: " << std::strlen(text)<< std::endl;
    
    //~ point coords[6 * strlen(text)];
    uint c = 0;
    //~ std::cout<<"text: " << text << std::endl;
    //~ std::cout<<"x: " << x << std::endl;
    //~ std::cout<<"a: " << a << std::endl;
    //~ std::cout<<"sx: " << sx << std::endl;
    //~ std::cout<<"sy: " << sy << std::endl;
    
    //~ float xmin, xmax = x;
    //~ float ymin, ymax = y;


    /* Loop through all characters */



    for (p = (const uint8_t *)text; *p; p++) {
        
        std::cout<<"check p: " << *p << std::endl;
        /* Calculate the vertex and texture coordinates */
        //~ std::cout<<"control 1" << std::endl;
        float x2 = x + a->c[*p].bl * sx;
        float y2 = -y - a->c[*p].bt * sy;
        float w = a->c[*p].bw * sx;
        float h = a->c[*p].bh * sy;
        
        //~ xmin = std::min(xmin,x2);
        //~ xmax = std::max(xmax,x2);
        //~ ymin = std::min(ymin,y2);
        //~ ymax = std::max(ymax,y2);
        
        //~ std::cout << "xmin, xmax, ymin, ymax: " << xmin << ", " <<
        //~ xmax << ", " << ymin << ", " <<ymax << std::endl;
        
        /* Advance the cursor to the start of the next character */
        x += a->c[*p].ax * sx;
        y += a->c[*p].ay * sy;
    
        /* Skip glyphs that have no pixels */
        if (!w || !h)
            continue;
        std::cout<<"check start textVect2D" << std::endl;
        c+=6;
        
        glyphs_box.push_back((vertex3D) {
        x2, -y2, z});
        glyphs_box.push_back((vertex3D) {
        x2 + w, -y2, z});
        glyphs_box.push_back((vertex3D) {
        x2, -y2 - h, z});
        glyphs_box.push_back((vertex3D) {
        x2 + w, -y2, z});
        glyphs_box.push_back((vertex3D) {
        x2, -y2 - h, z});
        glyphs_box.push_back((vertex3D) {
        x2 + w, -y2 - h, z});

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
    
    //~ std::vector<vertex3D> background_frame;
    //~ background_frame.push_back({xmin,ymin,0});
    //~ vertex3D origin_background={x,y,z};
    //~ std::vector<vertex3D> background_vertices=create_vector3D(background_frame,origin_background);
    //~ displayV3D(background_vertices);
    displayV3D(glyphs_box);
    //~ displayV3D(glyphs_vertex);

    //~ background_vertices=
    //~ {
        //~ {-10,-10,0},
        //~ {-10,10,0},
        //~ {10,10,0},
        //~ {10,-10,0}
    //~ };
    nb_glyphs_per_text.push_back(c);
    nb_indices2+=c;
    std::cout << "c: " << c << std::endl;
    std::cout << "nb_indices_2: " << nb_indices2 << std::endl;
    std::cout << "Control create coord vector text -- end" << std::endl;
}
void create_coord_vector_text4(const char *text, atlas * a, vertex3D origin, vertex2D &window_coord_scale){
    
    const uint8_t *p;
    
    std::cout<<"Control create coord vector text -- start" << std::endl;
    std::cout<<"number of chars: " << std::strlen(text)<< std::endl;
    
    //~ point coords[6 * strlen(text)];
    uint c = 0;
    //~ std::cout<<"text: " << text << std::endl;
    //~ std::cout<<"x: " << x << std::endl;
    //~ std::cout<<"a: " << a << std::endl;
    //~ std::cout<<"sx: " << sx << std::endl;
    //~ std::cout<<"sy: " << sy << std::endl;
    
    //~ float xmin, xmax = x;
    //~ float ymin, ymax = y;


    /* Loop through all characters */



    for (p = (const uint8_t *)text; *p; p++) {
        
        std::cout<<"check p: " << *p << std::endl;
        /* Calculate the vertex and texture coordinates */
        //~ std::cout<<"control 1" << std::endl;
        float x2 = origin.x + a->c[*p].bl * window_coord_scale.x;
        float y2 = -origin.y - a->c[*p].bt * window_coord_scale.y;
        float w = a->c[*p].bw * window_coord_scale.x;
        float h = a->c[*p].bh * window_coord_scale.y;
        
        //~ xmin = std::min(xmin,x2);
        //~ xmax = std::max(xmax,x2);
        //~ ymin = std::min(ymin,y2);
        //~ ymax = std::max(ymax,y2);
        
        //~ std::cout << "xmin, xmax, ymin, ymax: " << xmin << ", " <<
        //~ xmax << ", " << ymin << ", " <<ymax << std::endl;
        
        /* Advance the cursor to the start of the next character */
        origin.x += a->c[*p].ax * window_coord_scale.x;
        origin.y += a->c[*p].ay * window_coord_scale.y;
    
        /* Skip glyphs that have no pixels */
        if (!w || !h)
            continue;
        std::cout<<"check start glyphs_box" << std::endl;
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
    
    //~ std::vector<vertex3D> background_frame;
    //~ background_frame.push_back({xmin,ymin,0});
    //~ vertex3D origin_background={x,y,z};
    //~ std::vector<vertex3D> background_vertices=create_vector3D(background_frame,origin_background);
    //~ displayV3D(background_vertices);
    displayV3D(glyphs_box);
    //~ displayV3D(glyphs_vertex);

    //~ background_vertices=
    //~ {
        //~ {-10,-10,0},
        //~ {-10,10,0},
        //~ {10,10,0},
        //~ {10,-10,0}
    //~ };
    nb_glyphs_per_text.push_back(c);
    nb_indices2+=c;
    std::cout << "c: " << c << std::endl;
    std::cout << "nb_indices_2: " << nb_indices2 << std::endl;
    std::cout << "Control create coord vector text -- end" << std::endl;
}

void create_coord_vector_text3(const char *text, atlas * a, float x, float y, float z, float sx, float sy){
    
    const uint8_t *p;
    
    std::cout<<"check vector creation for text" << std::endl;
    
    //~ point coords[6 * strlen(text)];
    int c = 0;
    //~ std::cout<<"text: " << text << std::endl;
    //~ std::cout<<"x: " << x << std::endl;
    //~ std::cout<<"a: " << a << std::endl;
    //~ std::cout<<"sx: " << sx << std::endl;
    //~ std::cout<<"sy: " << sy << std::endl;
    
    //~ float xmin, xmax = x;
    //~ float ymin, ymax = y;


    /* Loop through all characters */



    for (p = (const uint8_t *)text; *p; p++) {
        
        std::cout<<"check p: " << *p << std::endl;
        /* Calculate the vertex and texture coordinates */
        //~ std::cout<<"control 1" << std::endl;
        float x2 = x + a->c[*p].bl * sx;
        float y2 = -y - a->c[*p].bt * sy;
        float w = a->c[*p].bw * sx;
        float h = a->c[*p].bh * sy;
        
        //~ xmin = std::min(xmin,x2);
        //~ xmax = std::max(xmax,x2);
        //~ ymin = std::min(ymin,y2);
        //~ ymax = std::max(ymax,y2);
        
        //~ std::cout << "xmin, xmax, ymin, ymax: " << xmin << ", " <<
        //~ xmax << ", " << ymin << ", " <<ymax << std::endl;
        
        /* Advance the cursor to the start of the next character */
        x += a->c[*p].ax * sx;
        y += a->c[*p].ay * sy;
    
        /* Skip glyphs that have no pixels */
        if (!w || !h)
            continue;
        std::cout<<"check start textVect2D" << std::endl;
        c+=6;
        
        glyphs_box2.push_back((vertex3D) {
        x2, -y2, z});
        glyphs_box2.push_back((vertex3D) {
        x2 + w, -y2, z});
        glyphs_box2.push_back((vertex3D) {
        x2, -y2 - h, z});
        glyphs_box2.push_back((vertex3D) {
        x2 + w, -y2, z});
        glyphs_box2.push_back((vertex3D) {
        x2, -y2 - h, z});
        glyphs_box2.push_back((vertex3D) {
        x2 + w, -y2 - h, z});

        glyphs_vertex2.push_back((vertex2D) {
        a->c[*p].tx, a->c[*p].ty});
        glyphs_vertex2.push_back((vertex2D) {
        a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty});
        glyphs_vertex2.push_back((vertex2D) {
        a->c[*p].tx, a->c[*p].ty + a->c[*p].bh / a->h});
        glyphs_vertex2.push_back((vertex2D) {
        a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty});
        glyphs_vertex2.push_back((vertex2D) {
        a->c[*p].tx, a->c[*p].ty + a->c[*p].bh / a->h});
        glyphs_vertex2.push_back((vertex2D) {
        a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty + a->c[*p].bh / a->h});

    }
    
    //~ std::vector<vertex3D> background_frame;
    //~ background_frame.push_back({xmin,ymin,0});
    //~ vertex3D origin_background={x,y,z};
    //~ std::vector<vertex3D> background_vertices=create_vector3D(background_frame,origin_background);
    //~ displayV3D(background_vertices);

    //~ background_vertices=
    //~ {
        //~ {-10,-10,0},
        //~ {-10,10,0},
        //~ {10,10,0},
        //~ {10,-10,0}
    //~ };
    
    nb_indices3=c;

}
// get width and height from the vectors of glyphs_box
//~ vertex2D text_frame(std::vector<vertex3D> &vector_input){
void text_frame(std::vector<vertex3D> &vector_input){
    
    std::vector<vertex3D>  whd;
    vertex3D first_vector3D;
    vertex3D offset={1,0,0};
    float w=0;
    float hmin=vector_input.front().y;
    float hmax=vector_input.front().y;
    float h=0;
    float d=0;
    
    
    
    first_vector3D = vector_input.front();
    displayV3D(vector_input);
    std::cout <<"control x front: " << vector_input.front().x << std::endl;
    std::cout <<"control x back: " << vector_input.back().x << std::endl;
    w = std::abs(vector_input.back().x-vector_input.front().x);
    for (std::vector<vertex3D>::iterator it=vector_input.begin(); it!=vector_input.end();++it){
        std::cout <<"control y: " << (*it).y << std::endl;
        hmin= std::min(hmin,(*it).y);
        hmax = std::max(hmax,(*it).y);
    }
    h = std::abs(hmax - hmin);
    d=h;
    std::cout <<"w,h: " << w << ", " << h <<std::endl;
    std::cout <<"hmax, hmin: " << hmax << ", " << hmin <<std::endl;
    //~ return {w,hmax-hmin};
    first_vector3D.y = hmin;
    
    whd.push_back({w,h,d});
    std::vector<vertex3D> vertex_3D=create_vector3D2(whd, first_vector3D,offset);
    init_vectors3(vertex_3D);
}
void text_frame2(std::vector<vertex3D> &vector_input, std::vector<uint> &nb_glyphs){
    
    std::vector<vertex3D>  whd;
    std::vector<vertex3D> vertex_3D;
    vertex3D first_vector3D;
    vertex3D offset={1,0,0};
    //~ float w=0;
    //~ float hmin=vector_input.front().y;
    //~ float hmax=vector_input.front().y;
    //~ float h=0;
    //~ float d=0;
    
    std::cout << "Control text_frame2 -- start" << std::endl;
    
    std::vector<vertex3D>::iterator it=vector_input.begin();
    std::vector<uint>::iterator it2=nb_glyphs.begin();
    
        first_vector3D = (*it);
        
    //~ for (std::vector<vertex3D>::iterator it=vector_input.begin(); it!=vector_input.end();std::advance(it,6)){
    while (it<vector_input.end()){
        //~ first_vector3D = vector_input.front();
        float w=0;
        float hmin=(*it).y;
        float hmax=(*it).y;
        float h=0;
        float d=0;
        //~ displayV3D(vector_input);
        std::cout << std::endl;
        std::cout <<"nb indexes: " << (*it2) << std::endl;
        std::cout <<"control x front: " << (*it).x << std::endl;
        std::cout <<"control x back: " << (*(it+(*it2)-1)).x << std::endl;
        //~ w = std::abs(vector_input.back().x-vector_input.front().x);
        // the absolute difference between the x coordinate of the right-most vertex and the left-most
        // of the current box, that are 5 vertices apart
        w = std::abs(((*(it+(*it2)-1)).x-(*it).x));
        // loop though each glyphs from current iterator until nb of glyphsfor current text is reach
        for (std::vector<vertex3D>::iterator it3=it; it3!=(it+(*it2));++it3){
            std::cout <<"control y: " << (*it3).y << std::endl << std::endl;
            hmin= std::min(hmin,(*it3).y);
            hmax = std::max(hmax,(*it3).y);
        }
        h = std::abs(hmax - hmin);
        d=h;
        std::cout <<"w,h: " << w << ", " << h <<std::endl;
        std::cout <<"hmax, hmin: " << hmax << ", " << hmin <<std::endl<<std::endl;
        //~ return {w,hmax-hmin};
        first_vector3D.y = std::min(first_vector3D.y,hmin);
        
        whd.push_back({w,h,d});
        //~ create_vector3D3(vertex_3D, whd, first_vector3D,offset);
        std::advance(it,(*it2));
        std::advance(it2,1);
    }
        create_vector3D3(vertex_3D, whd, first_vector3D,offset);

    init_vectors3(vertex_3D);
    std::cout << "Control text_frame2 -- end" << std::endl;
}
void text_frame3(std::vector<vertex3D> &vector_input, std::vector<uint> &nb_glyphs, std::vector<vertex3D> &v_origin){
    
    std::vector<vertex3D>  whd;
    std::vector<vertex3D> vertex_3D;
    
    std::cout << "Control text_frame2 -- start" << std::endl;
    
    std::vector<vertex3D>::iterator it=vector_input.begin();
    std::vector<uint>::iterator it2=nb_glyphs.begin();
    
    while (it<vector_input.end()){
        float w=0;
        float hmin=(*it).y;
        float hmax=(*it).y;
        float h=0;
        float d=0;
        //~ displayV3D(vector_input);
        std::cout << std::endl;
        std::cout <<"nb indexes: " << (*it2) << std::endl;
        std::cout <<"control x front: " << (*it).x << std::endl;
        std::cout <<"control x back: " << (*(it+(*it2)-1)).x << std::endl;
        //~ w = std::abs(vector_input.back().x-vector_input.front().x);
        // the absolute difference between the x coordinate of the right-most vertex and the left-most
        // of the current box, that are 5 vertices apart
        w = std::abs(((*(it+(*it2)-1)).x-(*it).x));
        // loop though each glyphs from current iterator until nb of glyphsfor current text is reach
        for (std::vector<vertex3D>::iterator it3=it; it3!=(it+(*it2));++it3){
            std::cout <<"control y: " << (*it3).y << std::endl << std::endl;
            hmin= std::min(hmin,(*it3).y);
            hmax = std::max(hmax,(*it3).y);
        }
        h = std::abs(hmax - hmin);
        d=h;
        std::cout <<"w,h: " << w << ", " << h <<std::endl;
        std::cout <<"hmax, hmin: " << hmax << ", " << hmin <<std::endl<<std::endl;
        //~ return {w,hmax-hmin};
        
        // commented for testing, by forcing value of first_vector at start of function
        
        whd.push_back({w,h,d});
        //~ create_vector3D3(vertex_3D, whd, first_vector3D,offset);
        std::advance(it,(*it2));
        std::advance(it2,1);
    }
    create_vector3D4(vertex_3D, v_origin, whd);
    displayV3D(whd);
    init_vectors3(vertex_3D);
    std::cout << "Control text_frame2 -- end" << std::endl;
}

void render_text2(atlas * a) {
    
    /* Use the texture containing the atlas */
    glBindTexture(GL_TEXTURE_2D, a->tex);
    glUniform1i(uniform_tex_test, 0);
    
    /* Set up the VBO for our vertex data */
    glEnableVertexAttribArray(attribute_coord_test);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_test);
    glVertexAttribPointer(attribute_coord_test, 4, GL_FLOAT, GL_FALSE, 0, 0);
    
    /* Draw all the character on the screen in one go */
    glBufferData(GL_ARRAY_BUFFER,textVect.size() * sizeof(point),&textVect.front(),GL_STATIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, nb_indices);
    std::cout<<nb_indices<<std::endl;
    
    glDisableVertexAttribArray(attribute_coord_test);
}

void render_text4(atlas * a) {
    
    /* Use the texture containing the atlas */
    glBindTexture(GL_TEXTURE_2D, a->tex);
    glUniform1i(uniform_tex_test, 0);
    
    /* Set up the VBO for our vertex data */
    glEnableVertexAttribArray(attribute_coord_test);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_test);
    glVertexAttribPointer(attribute_coord_test, 5, GL_FLOAT, GL_FALSE, 0, 0);
    
    /* Draw all the character on the screen in one go */
    glBufferData(GL_ARRAY_BUFFER,textVect3D.size() * sizeof(point3D),&textVect3D.front(),GL_STATIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, nb_indices);
    std::cout<<nb_indices<<std::endl;
    
    glDisableVertexAttribArray(attribute_coord_test);
}

void render_text3(atlas * a) {
    
    /* Use the texture containing the atlas */
    glBindTexture(GL_TEXTURE_2D, a->tex);
    glUniform1i(uniform_tex, 0);
    
    //~ displayV3D(background_vertices);
    
    /* Set up the VBO for our vertex data */
    glEnableVertexAttribArray(attribute_coord);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_coord);
    glVertexAttribPointer(attribute_coord, 3, GL_FLOAT, GL_FALSE, 0, 0);
    /* Draw the character on the screen */
    glBufferData(GL_ARRAY_BUFFER, glyphs_box.size() * sizeof(vertex3D), &glyphs_box.front(), GL_STATIC_DRAW);
    
    //~ glDrawArrays(GL_TRIANGLES, 0, nb_indices2);
    
    
    
    /* Set up the VBO for our vertex data */
    glEnableVertexAttribArray(attribute_tex_coord);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_tex_coord);
    glVertexAttribPointer(attribute_tex_coord, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    /* Draw all the character on the screen in one go */
    glBufferData(GL_ARRAY_BUFFER,glyphs_vertex.size() * sizeof(vertex2D),&glyphs_vertex.front(),GL_STATIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, nb_indices2);
    //~ std::cout<<nb_indices2<<std::endl;
    
    glDisableVertexAttribArray(attribute_coord);
    glDisableVertexAttribArray(attribute_tex_coord);
}

//~ void render_text5(atlas * a) {
    
    //~ /* Use the texture containing the atlas */
    //~ glBindTexture(GL_TEXTURE_2D, a->tex);
    //~ glUniform1i(uniform_tex, 0);
    
    
    //~ /* Set up the VBO for our vertex data */
    //~ glEnableVertexAttribArray(attribute_coord);
    //~ glBindBuffer(GL_ARRAY_BUFFER, vbo_coord);
    //~ glVertexAttribPointer(attribute_coord, 3, GL_FLOAT, GL_FALSE, 0, 0);
    //~ /* Draw the character on the screen */
    //~ glBufferData(GL_ARRAY_BUFFER, glyphs_box.size() * sizeof(vertex3D), &glyphs_box.front(), GL_STATIC_DRAW);
    
    
    //~ /* Set up the VBO for our vertex data */
    //~ glEnableVertexAttribArray(attribute_tex_coord);
    //~ glBindBuffer(GL_ARRAY_BUFFER, vbo_tex_coord);
    //~ glVertexAttribPointer(attribute_tex_coord, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    //~ /* Draw all the character on the screen in one go */
    //~ glBufferData(GL_ARRAY_BUFFER,glyphs_vertex.size() * sizeof(vertex2D),&glyphs_vertex.front(),GL_STATIC_DRAW);
    //~ glDrawArrays(GL_TRIANGLES, 0, nb_indices2);

    //~ /* Set up the VBO for our vertex data */
    //~ glEnableVertexAttribArray(attribute_coord);
    //~ glBindBuffer(GL_ARRAY_BUFFER, vbo_coord);
    //~ glVertexAttribPointer(attribute_coord, 3, GL_FLOAT, GL_FALSE, 0, 0);
    //~ /* Draw the character on the screen */
    //~ glBufferData(GL_ARRAY_BUFFER, glyphs_box.size() * sizeof(vertex3D), &glyphs_box.front(), GL_STATIC_DRAW);
    
    //~ /* Set up the VBO for our vertex data */
    //~ glEnableVertexAttribArray(attribute_tex_coord);
    //~ glBindBuffer(GL_ARRAY_BUFFER, vbo_tex_coord);
    //~ glVertexAttribPointer(attribute_tex_coord, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    //~ /* Draw all the character on the screen in one go */
    //~ glBufferData(GL_ARRAY_BUFFER,glyphs_vertex.size() * sizeof(vertex2D),&glyphs_vertex.front(),GL_STATIC_DRAW);
    //~ glDrawArrays(GL_TRIANGLES, 0, nb_indices2);
    
    //~ glDisableVertexAttribArray(attribute_coord);
    //~ glDisableVertexAttribArray(attribute_tex_coord);
//~ }


bool init_font(uint uFontSize,const char *uFontFilename) {
    
    /*Set the font size from argument */
    fontSize=uFontSize;
    
    /*Set the font Filename from arguement*/
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
    
    return 1;
}

bool init_program(){

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
    
    // this part is using the shader_utils library in folder ./common/
    program_text = create_program("./shaders/text.v.glsl", "./shaders/text.f.glsl");
    if(program_text == 0)
        return 0;

    attribute_coord = get_attrib(program_text, "coord");
    // added tex_coord for a separated set of coordinates for the texture's pixels
    attribute_tex_coord = get_attrib(program_text,"tex_coord");
    uniform_tex = get_uniform(program_text, "tex");
    uniform_color = get_uniform(program_text, "color");
    uniform_bgcolor = get_uniform(program_text, "bgcolor");

    if(attribute_coord == -1 || attribute_tex_coord == -1 || uniform_tex == -1 || uniform_color == -1 || uniform_bgcolor == -1)
        return 0;

    //~ program_bg = create_program("square.v.glsl", "square.f.glsl");
    //~ if(program_bg == 0)
        //~ return 0;

    program_cube = create_program("./shaders/cube.v.glsl", "./shaders/cube.f.glsl");
    if(program_cube == 0)
        return 0;
    
    //~ attribute_coord2d = get_attrib(program_bg, "coord2d");
    attribute_coord3d = get_attrib(program_cube, "coord3d");
    //~ attribute_v_color = get_attrib(program_bg, "v_color");
    attribute_v_color = get_attrib(program_cube, "v_color");

    //~ if(attribute_coord2d == -1 || attribute_v_color == -1)
    //~ if(attribute_coord3d == -1 || attribute_v_color == -1)
    if(attribute_coord3d == -1 || attribute_v_color == -1 )
        return 0;

    // testing boxes
    program_box = create_program("./shaders/box.v.glsl", "./shaders/box.f.glsl");
    if(program_box == 0)
        return 0;

    attribute_coord3d_box = get_attrib(program_box, "coord3d_b");
    attribute_v_color_box = get_attrib(program_box, "v_color_b");

    if(attribute_coord3d_box == -1 || attribute_v_color_box == -1)
        return 0;

    return 1;
}


int init_resources_test() {

	program_test = create_program("shaders/text_test.v.glsl", "shaders/text_test.f.glsl");
	if(program_test == 0)
		return 0;

	attribute_coord_test = get_attrib(program_test, "coord");
    attribute_tex_coord = get_attrib(program_test,"tex_coord");
	uniform_tex_test = get_uniform(program_test, "tex");
	uniform_color_test = get_uniform(program_test, "color");
    uniform_bgcolor_test = get_uniform(program_test, "bgcolor");

	if(attribute_coord_test == -1 || uniform_tex_test == -1 || uniform_color_test == -1)
		return 0;

	// Create the vertex buffer object
	glGenBuffers(1, &vbo_test);

	/* Create texture atlasses for several font sizes */
	a48 = new atlas(face, 48);
	a24 = new atlas(face, 24);
	a12 = new atlas(face, 12);
std::cout<<"check init_resource_test" << std::endl;
    // create the vectors with all the glyphs
    float sx = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
    float sy = 2.0 / glutGet(GLUT_WINDOW_HEIGHT);
    create_coord_vector(inputText, a48, -1 + 8 * sx, 1 - 50 * sy, sx, sy );
    std::cout << "inputText" << inputText << std::endl;
    //~ create_coord_vector_text(inputText, a48, text_coordinates.x, -text_coordinates.y, text_coordinates.z, padding.x,padding.y );

    return 1;
}

int init_program_test() {

	program_test = create_program("shaders/text_test.v.glsl", "shaders/text_test.f.glsl");
	if(program_test == 0)
		return 0;

	attribute_coord_test = get_attrib(program_test, "coord");
    attribute_tex_coord = get_attrib(program_test,"tex_coord");
	uniform_tex_test = get_uniform(program_test, "tex");
	uniform_color_test = get_uniform(program_test, "color");
    uniform_bgcolor_test = get_uniform(program_test, "bgcolor");

	if(attribute_coord_test == -1 || uniform_tex_test == -1 || uniform_color_test == -1)
		return 0;

    return 1;
}


void free_resources_test() {
	glDeleteProgram(program_test);
    glDeleteBuffers(1, &vbo_test);
}

/**
 * Render text using the currently loaded font and currently set font size.
 * Rendering starts at coordinates (x, y), z is always 0.
 * The pixel coordinates that the FreeType2 library uses are scaled by (sx, sy).
 */
void render_text(const char *text, float x, float y, float sx, float sy) {
    const char *p;
    FT_GlyphSlot g = face->glyph;
    
    vertex2D texture_coord[4]={
        {0,0},
        {1,0},
        {0,1},
        {1,1}
    };
    
    
    /* Create a texture that will be used to hold one "glyph" */
    GLuint tex;
    
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glUniform1i(uniform_tex, 0);
    
    /* We require 1 byte alignment when uploading texture data */
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    /* Clamping to edges is important to prevent artifacts when scaling */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    /* Linear filtering usually looks best for text */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    /* Loop through all characters */
    for (p = text; *p; p++) {
        /* Try to load and render the character */
        if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
            continue;
    
        /* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);
    
        /* Calculate the vertex and texture coordinates */
        float x2 = x + g->bitmap_left * sx;
        float y2 = -y - g->bitmap_top * sy;
        float w = g->bitmap.width * sx;
        float h = g->bitmap.rows * sy;
    
    //TODO understand the logic for the coordinates here:
        vertex2D text_box[4] = {
            {x2, -y2},
            {x2 + w, -y2},
            {x2, -y2 - h},
            {x2 + w, -y2 - h},
        };

    
    /* Set up the VBO for our vertex data */
    glEnableVertexAttribArray(attribute_coord);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_coord);
    glVertexAttribPointer(attribute_coord, 2, GL_FLOAT, GL_FALSE, 0, 0);
        

    /* Draw the character on the screen */
    glBufferData(GL_ARRAY_BUFFER, sizeof text_box, text_box, GL_DYNAMIC_DRAW);
        
    /* Set up the VBO for our vertex data */
    glEnableVertexAttribArray(attribute_tex_coord);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_tex_coord);
    glVertexAttribPointer(attribute_tex_coord, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glBufferData(GL_ARRAY_BUFFER, sizeof texture_coord, texture_coord, GL_DYNAMIC_DRAW);
        
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
        // Advance the cursor to the start of the next character
        // The '>> 6' operation is to slice in 2^6=64 bits because
        // g->advance is measured in 1/64 pixels
        x += (g->advance.x >> 6) * sx;
        y += (g->advance.y >> 6) * sy;
    }

    glDisableVertexAttribArray(attribute_coord);
    glDisableVertexAttribArray(attribute_tex_coord);
    glDeleteTextures(1, &tex);
}

void render_text_Z(const char *text, float x, float y, float z, float sx, float sy) {
    const char *p;
    FT_GlyphSlot g = face->glyph;
    
    
    vertex2D texture_coord[4]={
        {0,0},
        {1,0},
        {0,1},
        {1,1}
    };
    
    
    /* Create a texture that will be used to hold one "glyph" */
    GLuint tex;
    
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glUniform1i(uniform_tex, 0);
    
    /* We require 1 byte alignment when uploading texture data */
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    /* Clamping to edges is important to prevent artifacts when scaling */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    /* Linear filtering usually looks best for text */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    /* Loop through all characters */
    for (p = text; *p; p++) 
    {
        std::cout << p << std::endl;
        /* Try to load and render the character */
        if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
            continue;
    
        /* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);
    
        /* Calculate the vertex and texture coordinates */
        float x2 = x + g->bitmap_left * sx;
        float y2 = -y - g->bitmap_top * sy;
        float w = g->bitmap.width * sx;
        float h = g->bitmap.rows * sy;
    
        vertex3D text_box[4] = {
            {x2, -y2, z},
            {x2 + w, -y2, z},
            {x2, -y2 - h, z},
            {x2 + w, -y2 - h, z},
        };

    
        /* Set up the VBO for our vertex data */
        glEnableVertexAttribArray(attribute_coord);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_coord);
        //~ glVertexAttribPointer(attribute_coord, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glVertexAttribPointer(attribute_coord, 3, GL_FLOAT, GL_FALSE, 0, 0);
        /* Draw the character on the screen */
        glBufferData(GL_ARRAY_BUFFER, sizeof text_box, text_box, GL_DYNAMIC_DRAW);
            
        /* Set up the VBO for our vertex data */
        glEnableVertexAttribArray(attribute_tex_coord);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_tex_coord);
        glVertexAttribPointer(attribute_tex_coord, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glBufferData(GL_ARRAY_BUFFER, sizeof texture_coord, texture_coord, GL_DYNAMIC_DRAW);
            
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
        // Advance the cursor to the start of the next character
        // The '>> 6' operation is to slice in 2^6=64 bits because
        // g->advance is measured in 1/64 pixels
        x += (g->advance.x >> 6) * sx;
        y += (g->advance.y >> 6) * sy;
    }

    glDisableVertexAttribArray(attribute_coord);
    glDisableVertexAttribArray(attribute_tex_coord);
    glDeleteTextures(1, &tex);
}
/*
void render_text_Z2(const char *text, float x, float y, float z, float sx, float sy) {
    const char *p;
    FT_GlyphSlot g = face->glyph;
    
    
    vertex2D texture_coord[4]={
        {0,0},
        {1,0},
        {0,1},
        {1,1}
    };
    
    
    // Create a texture that will be used to hold one "glyph"
    GLuint tex;
    
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glUniform1i(uniform_tex, 0);
    
    // We require 1 byte alignment when uploading texture data
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // Clamping to edges is important to prevent artifacts when scaling
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    / Linear filtering usually looks best for text
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Loop through all characters
    for (p = text; *p; p++) 
    {
        std::cout << p << std::endl;
        // Try to load and render the character
        if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
            continue;
    
        // Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);
    
        // Calculate the vertex and texture coordinates
        float x2 = x + g->bitmap_left * sx;
        float y2 = -y - g->bitmap_top * sy;
        float w = g->bitmap.width * sx;
        float h = g->bitmap.rows * sy;
    
        vertex3D text_box[4] = {
            {x2, -y2, z},
            {x2 + w, -y2, z},
            {x2, -y2 - h, z},
            {x2 + w, -y2 - h, z},
        };

    
        // Set up the VBO for our vertex data
        glEnableVertexAttribArray(attribute_coord);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_coord);
        //~ glVertexAttribPointer(attribute_coord, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glVertexAttribPointer(attribute_coord, 3, GL_FLOAT, GL_FALSE, 0, 0);
            

        // Draw the character on the screen
        glBufferData(GL_ARRAY_BUFFER, sizeof text_box, text_box, GL_DYNAMIC_DRAW);
            
        // Set up the VBO for our vertex data
        glEnableVertexAttribArray(attribute_tex_coord);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_tex_coord);
        glVertexAttribPointer(attribute_tex_coord, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glBufferData(GL_ARRAY_BUFFER, sizeof texture_coord, texture_coord, GL_DYNAMIC_DRAW);
            
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
        // Advance the cursor to the start of the next character
        // The '>> 6' operation is to slice in 2^6=64 bits because
        // g->advance is measured in 1/64 pixels
        x += (g->advance.x >> 6) * sx;
        y += (g->advance.y >> 6) * sy;
    }

    glDisableVertexAttribArray(attribute_coord);
    glDisableVertexAttribArray(attribute_tex_coord);
    glDeleteTextures(1, &tex);
}
*/

void init_background(const char *text, float x, float y) {
    const char *p;

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    float sx = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
    float sy = 2.0 / glutGet(GLUT_WINDOW_HEIGHT);

    FT_GlyphSlot g = face->glyph;
    float w = 0;
    float h = 0;

for (p = text; *p; p++) {
        /* Try to load and render the character */
        if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
            {
                fprintf(stderr, "Loading character %c failed!\n", *p);
                continue;
            }
    
        w += (g->advance.x >> 6) * sx;
        h = std::max(h, g->bitmap_top*sy);
        y = std::max(y, (g->bitmap.rows - g->bitmap_top)*sy);

    }
    
    // to have a gap between border and text, we set up a padding 
    float padding=0.3;
    
    //We use -y instead of y because we calculated the bitmap_top-height
    
    // to get a square we keep the bigger of w and h,
    // it should always be w
    float s=w;
    //~ if (h>w) s=h;

    vertex2D vertex_coord[4] = {
            {x-padding, -y-padding},
            {x + s+padding, -y-padding},
            {x-padding, s+padding},
            {x + s+padding, s+padding},
        };
    
    glGenBuffers(1,&vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER,sizeof vertex_coord, vertex_coord,GL_STATIC_DRAW);
    
    GLfloat vertices_color[]={
        0,0,1,
        0,0,1,
        0,0,1,
        0,0,1,
    };

    glGenBuffers(1,&vbo_vertices_color);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_vertices_color);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices_color), vertices_color, GL_STATIC_DRAW);

}

void init_color(GLfloat colorRed,GLfloat colorGreen,GLfloat colorBlue)
{
    r=colorRed;
    g=colorGreen;
    b=colorBlue;
    
    GLfloat vertices_color[]={
        r,g,b,
        r,g,b,
        r,g,b,
        r,g,b,
        r,g,b,
        r,g,b,
        r,g,b,
        r,g,b
    };
    
    GLfloat box_vertices_color[]={
        // first box
        1,0.5,0,
        1,0.5,0,
        1,0.5,0,
        1,0.5,0,
        1,0.5,0,
        1,0.5,0,
        1,0.5,0,
        1,0.5,0,
        // second box
        0,0.5,0,
        1,0.5,0,
        0,0.5,1,
        1,0.5,1,
        0.5,0.5,0,
        0,0.5,0.5,
        1,0.5,0.5,
        0.5,0.5,1,
        // third box
        0,0.5,0,
        0,0.5,0,
        0,0.5,0,
        0,0.5,0,
        0,0.5,0,
        0,0.5,0,
        0,0.5,0,
        0,0.5,0
    };

    glGenBuffers(1,&vbo_vertices_color);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_vertices_color);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices_color), vertices_color, GL_STATIC_DRAW);
    
    glGenBuffers(1,&vbo_vertices_color_box);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_vertices_color_box);
    glBufferData(GL_ARRAY_BUFFER,sizeof(box_vertices_color), box_vertices_color, GL_STATIC_DRAW);
}
void init_color2()
{
    
    GLfloat box_vertices_color2[]={
        // first box
        0,0.2,0.8,
        0.2,0,0.2,
        0.2,0.8,0,
        0,0.2,0.8,
        0.2,0.2,0.2,
        0.2,0.2,0.2,
        0.2,0.2,0.2,
        1,1,1
    };

    std::vector<GLfloat> vect_color;


    //~ vect_color.assign(box_vertices_color2,box_vertices_color2+24);   // assigning from array.

    for(int i=0;i<20;i++){
        //~ vect_color.insert(box_vertices_color2+24*i,box_vertices_color2+24*(i+1));   // assigning from array.
        vect_color.insert (vect_color.end(), box_vertices_color2, box_vertices_color2+24);
        }
    //~ for(int i=0;i<6;i++){
        //~ for ( auto it = box_vertices_color2.begin(); it != box_vertices_color2.end(); ++it ){
            //~ vect_index.push_back((*it)+8*i));
        //~ }
    //~ }



    //~ glGenBuffers(1,&vbo_vertices_color_box2);
    //~ glBindBuffer(GL_ARRAY_BUFFER,vbo_vertices_color_box2);
    //~ glBufferData(GL_ARRAY_BUFFER,sizeof(box_vertices_color2), box_vertices_color2, GL_STATIC_DRAW);
    glGenBuffers(1,&vbo_vertices_color_box2);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_vertices_color_box2);
    //~ glBufferData(GL_ARRAY_BUFFER,sizeof(box_vertices_color2), box_vertices_color2, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER,vect_color.size() * sizeof(GLfloat),&vect_color.front(),GL_STATIC_DRAW);
}

void init_color3()
{
    
    GLfloat box_vertices_color3[]={
        // first box
        1,0.2,0.8,
        1.2,0,0.2,
        0.2,0.8,0,
        0,0.2,0.8,
        0.2,0.2,0.2,
        0.2,0.2,0.2,
        0.2,0.2,0.2,
        1,1,1
    };

    std::vector<GLfloat> vect_color3;


    int max_loop = testing.size();
    std::cout<<testing.size()<<std::endl;
    for(int i=0;i<max_loop;i++){
        vect_color3.insert (vect_color3.end(), box_vertices_color3, box_vertices_color3+24);
        }


    glGenBuffers(1,&vbo_vertices_color_box3);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_vertices_color_box3);
    glBufferData(GL_ARRAY_BUFFER,vect_color3.size() * sizeof(GLfloat),&vect_color3.front(),GL_STATIC_DRAW);
}

void init_color4(GLfloat colorRed,GLfloat colorGreen,GLfloat colorBlue)
{
    r=colorRed;
    g=colorGreen;
    b=colorBlue;
    
    GLfloat box_vertices_color3[]={
        r,g,b,
        r,g,b,
        r,g,b,
        r,g,b,
        r,g,b,
        r,g,b,
        r,g,b,
        r,g,b
    };
    
    std::vector<GLfloat> vect_color3;


    //~ int max_loop = testing.size();
    int max_loop = 4;
    std::cout<<testing.size()<<std::endl;
    for(int i=0;i<max_loop;i++){
        vect_color3.insert (vect_color3.end(), box_vertices_color3, box_vertices_color3+24);
        }


    glGenBuffers(1,&vbo_vertices_color_box3);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_vertices_color_box3);
    glBufferData(GL_ARRAY_BUFFER,vect_color3.size() * sizeof(GLfloat),&vect_color3.front(),GL_STATIC_DRAW);
}

void init_color5(vertex3D color, std::vector<vertex3D> &whd)
{
    r=color.x;
    g=color.y;
    b=color.z;
    
    GLfloat box_vertices_color3[]={
        r,g,b,
        r,g,b,
        r,g,b,
        r,g,b,
        r,g,b,
        r,g,b,
        r,g,b,
        r,g,b
    };
    
    std::vector<GLfloat> vect_color3;


    //~ int max_loop = testing.size();
    //~ int max_loop = 4;
    int max_loop = whd.size();
    std::cout<<testing.size()<<std::endl;
    for(int i=0;i<max_loop;i++){
        vect_color3.insert (vect_color3.end(), box_vertices_color3, box_vertices_color3+24);
        }


    glGenBuffers(1,&vbo_vertices_color_box3);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_vertices_color_box3);
    glBufferData(GL_ARRAY_BUFFER,vect_color3.size() * sizeof(GLfloat),&vect_color3.front(),GL_STATIC_DRAW);
}

void displayV3D(std::vector<vertex3D> &vector_3D){
        std::cout<<"loop V3D"<<std::endl;
    for(std::vector<vertex3D>::iterator it=vector_3D.begin();it!=vector_3D.end();++it){
        
        std::cout<<"| ";
        std::cout<<(*it).x<<", ";
        std::cout<<(*it).y<<", ";        
        std::cout<<(*it).z;        
        std::cout<<" |"<<std::endl;
    }
    
}

std::vector<vertex3D> create_vector3D(std::vector<vertex3D> &vector_text_whd, vertex3D &first_vector3D){
    
    int n_vect_whd=0;
    float offset_x=1.5;
    float offset_y=-0.3;
    float offset_z=1.0;
    // first vertex is set to (0,0,0), is the bottom left corner
    //~ vertex3D first_vector3D = {0.0,0.0,0.0};
    //~ vertex3D first_vector3D = vector_coordinates;
    std::vector<vertex3D> vector3D;
    
    for(std::vector<vertex3D>::iterator it=vector_text_whd.begin();it!=vector_text_whd.end();++it){
        std::cout<<"loop"<<std::endl;
        std::cout<<(*it).x<<std::endl;
        std::cout<<(*it).y<<std::endl;
        std::cout<<(*it).z<<std::endl;
        // 1st vertex
        vector3D.push_back({first_vector3D.x+n_vect_whd*(offset_x+(*it).x),first_vector3D.y+n_vect_whd*(offset_y+(*it).y),first_vector3D.z+n_vect_whd*(offset_z+(*it).z)});
        // 2nd vertex, keep 1st vertex x and z, add y from vector2D, to get upper left corner, front face
        vector3D.push_back({vector3D.back().x,vector3D.back().y+(*it).y,vector3D.back().z});
        // 3rd vertex, back is now 2nd vertex, so just add y, add x from, to get upper right corner, front face
        vector3D.push_back({vector3D.back().x+(*it).x,vector3D.back().y,vector3D.back().z});
        // 4th vertex, back is now 3rd vertex, so deduct y, keeping x, to get bottom right corner, front face
        vector3D.push_back({vector3D.back().x,vector3D.back().y-(*it).y,vector3D.back().z});
        // 5th vertex deduct x to come back to bottom left corner, and deduct a depth for z to get back face
        vector3D.push_back({vector3D.back().x-(*it).x,vector3D.back().y,vector3D.back().z-(*it).z});
        // 6nd vertex, keep 1st vertex x and z, add y from vector2D, to get upper left corner, back face
        vector3D.push_back({vector3D.back().x,vector3D.back().y+(*it).y,vector3D.back().z});
        // 7th vertex, back is now 2nd vertex, so just add y, add x from, to get upper right corner, back face
        vector3D.push_back({vector3D.back().x+(*it).x,vector3D.back().y,vector3D.back().z});
        // 8th vertex, back is now 3rd vertex, so deduct y, keeping x, to get bottom right corner, back face
        vector3D.push_back({vector3D.back().x,vector3D.back().y-(*it).y,vector3D.back().z});
        n_vect_whd++;
            }
        std::cout<<"loop end"<<std::endl;
        displayV3D(vector3D);
    return vector3D;
}
std::vector<vertex3D> create_vector3D2(std::vector<vertex3D> &vector_text_whd, vertex3D &first_vector3D, vertex3D &offset){
    
    int n_vect_whd=0;
    //~ float offset.x=1.5;
    //~ float offset.y=-0.3;
    //~ float offset.z=1.0;
    // first vertex is set to (0,0,0), is the bottom left corner
    //~ vertex3D first_vector3D = {0.0,0.0,0.0};
    //~ vertex3D first_vector3D = vector_coordinates;
    std::vector<vertex3D> vector3D;
    
    for(std::vector<vertex3D>::iterator it=vector_text_whd.begin();it!=vector_text_whd.end();++it){
        std::cout<<"loop"<<std::endl;
        std::cout<<(*it).x<<std::endl;
        std::cout<<(*it).y<<std::endl;
        std::cout<<(*it).z<<std::endl;
        // 1st vertex
        vector3D.push_back({first_vector3D.x+n_vect_whd*(offset.x+(*it).x),first_vector3D.y+n_vect_whd*(offset.y+(*it).y),first_vector3D.z+n_vect_whd*(offset.z+(*it).z)});
        // 2nd vertex, keep 1st vertex x and z, add y from vector2D, to get upper left corner, front face
        vector3D.push_back({vector3D.back().x,vector3D.back().y+(*it).y,vector3D.back().z});
        // 3rd vertex, back is now 2nd vertex, so just add y, add x from, to get upper right corner, front face
        vector3D.push_back({vector3D.back().x+(*it).x,vector3D.back().y,vector3D.back().z});
        // 4th vertex, back is now 3rd vertex, so deduct y, keeping x, to get bottom right corner, front face
        vector3D.push_back({vector3D.back().x,vector3D.back().y-(*it).y,vector3D.back().z});
        // 5th vertex deduct x to come back to bottom left corner, and deduct a depth for z to get back face
        vector3D.push_back({vector3D.back().x-(*it).x,vector3D.back().y,vector3D.back().z-(*it).z});
        // 6nd vertex, keep 1st vertex x and z, add y from vector2D, to get upper left corner, back face
        vector3D.push_back({vector3D.back().x,vector3D.back().y+(*it).y,vector3D.back().z});
        // 7th vertex, back is now 2nd vertex, so just add y, add x from, to get upper right corner, back face
        vector3D.push_back({vector3D.back().x+(*it).x,vector3D.back().y,vector3D.back().z});
        // 8th vertex, back is now 3rd vertex, so deduct y, keeping x, to get bottom right corner, back face
        vector3D.push_back({vector3D.back().x,vector3D.back().y-(*it).y,vector3D.back().z});
        n_vect_whd++;
            }
        std::cout<<"loop end"<<std::endl;
        displayV3D(vector3D);
    return vector3D;
}
void create_vector3D3(std::vector<vertex3D> &vector3D, std::vector<vertex3D> &vector_text_whd, vertex3D &first_vector3D, vertex3D &offset){
    
    int n_vect_whd=0;
    //~ float offset.x=1.5;
    //~ float offset.y=-0.3;
    //~ float offset.z=1.0;
    // first vertex is set to (0,0,0), is the bottom left corner
    //~ vertex3D first_vector3D = {0.0,0.0,0.0};
    //~ vertex3D first_vector3D = vector_coordinates;
    //~ std::vector<vertex3D> vector3D;
    std::cout<<"control create_vector3D3 -- start"<<std::endl;
    
    for(std::vector<vertex3D>::iterator it=vector_text_whd.begin();it!=vector_text_whd.end();++it){
        std::cout<<"loop -- start"<<std::endl;
        std::cout<<"n_vect_whd: "<<n_vect_whd<<std::endl;
        std::cout<<first_vector3D.x+n_vect_whd*(offset.x)<<std::endl;
        std::cout<<(*it).x<<std::endl;
        std::cout<<first_vector3D.x<<std::endl;
        std::cout<<offset.x<<std::endl;
        std::cout<<(first_vector3D.y+n_vect_whd*(offset.y))<<std::endl;
        std::cout<<(*it).y<<std::endl;
        std::cout<<first_vector3D.y<<std::endl;
        std::cout<<offset.y<<std::endl;
        std::cout<<(first_vector3D.z+n_vect_whd*(offset.z))<<std::endl;
        std::cout<<(*it).z<<std::endl;
        std::cout<<first_vector3D.z<<std::endl;
        std::cout<<offset.z<<std::endl;
        // 1st vertex
        //~ vector3D.push_back({first_vector3D.x+n_vect_whd*(offset.x+(*it).x),first_vector3D.y+n_vect_whd*(offset.y+(*it).y),first_vector3D.z+n_vect_whd*(offset.z+(*it).z)});
        vector3D.push_back({first_vector3D.x+n_vect_whd*(offset.x),first_vector3D.y+n_vect_whd*(offset.y),first_vector3D.z+n_vect_whd*(offset.z)});
        // 2nd vertex, keep 1st vertex x and z, add y from vector2D, to get upper left corner, front face
        vector3D.push_back({vector3D.back().x,vector3D.back().y+(*it).y,vector3D.back().z});
        // 3rd vertex, back is now 2nd vertex, so just add y, add x from, to get upper right corner, front face
        vector3D.push_back({vector3D.back().x+(*it).x,vector3D.back().y,vector3D.back().z});
        // 4th vertex, back is now 3rd vertex, so deduct y, keeping x, to get bottom right corner, front face
        vector3D.push_back({vector3D.back().x,vector3D.back().y-(*it).y,vector3D.back().z});
        // 5th vertex deduct x to come back to bottom left corner, and deduct a depth for z to get back face
        vector3D.push_back({vector3D.back().x-(*it).x,vector3D.back().y,vector3D.back().z-(*it).z});
        // 6nd vertex, keep 1st vertex x and z, add y from vector2D, to get upper left corner, back face
        vector3D.push_back({vector3D.back().x,vector3D.back().y+(*it).y,vector3D.back().z});
        // 7th vertex, back is now 2nd vertex, so just add y, add x from, to get upper right corner, back face
        vector3D.push_back({vector3D.back().x+(*it).x,vector3D.back().y,vector3D.back().z});
        // 8th vertex, back is now 3rd vertex, so deduct y, keeping x, to get bottom right corner, back face
        vector3D.push_back({vector3D.back().x,vector3D.back().y-(*it).y,vector3D.back().z});
        n_vect_whd++;
        std::cout<<"loop end"<<std::endl;
            }
        std::cout<<"control create_vector3D3 -- end"<<std::endl;
        displayV3D(vector3D);
}
void create_vector3D4(std::vector<vertex3D> &vector3D,std::vector<vertex3D> &v_origin, std::vector<vertex3D> &vector_text_whd){
    
    //~ int n_vect_whd=0;
    //~ float offset.x=1.5;
    //~ float offset.y=-0.3;
    //~ float offset.z=1.0;
    // first vertex is set to (0,0,0), is the bottom left corner
    //~ vertex3D first_vector3D = {0.0,0.0,0.0};
    //~ vertex3D first_vector3D = vector_coordinates;
    //~ std::vector<vertex3D> vector3D;
    std::cout<<"control create_vector3D4 -- start"<<std::endl;
    std::vector<vertex3D>::iterator whd_it;
    std::vector<vertex3D>::iterator v_origin_it;
    for(whd_it=vector_text_whd.begin(),v_origin_it=v_origin.begin() ;whd_it!=vector_text_whd.end() && v_origin_it!=vector_text_whd.end() ;++whd_it, ++v_origin_it){
        std::cout<<"loop -- start"<<std::endl;
        //~ std::cout<<"n_vect_whd: "<<n_vect_whd<<std::endl;
        std::cout<<(*v_origin_it).x<<std::endl;
        std::cout<<(*whd_it).x<<std::endl;
        std::cout<<(*whd_it).y<<std::endl;
        std::cout<<(*v_origin_it).y<<std::endl;
        std::cout<<(*whd_it).z<<std::endl;
        std::cout<<(*v_origin_it).z<<std::endl;
        // 1st vertex
        vector3D.push_back({(*v_origin_it).x,(*v_origin_it).y,(*v_origin_it).z});
        // 2nd vertex, keep 1st vertex x and z, add y from vector2D, to get upper left corner, front face
        vector3D.push_back({vector3D.back().x,vector3D.back().y+(*whd_it).y,vector3D.back().z});
        // 3rd vertex, back is now 2nd vertex, so just add y, add x from, to get upper right corner, front face
        vector3D.push_back({vector3D.back().x+(*whd_it).x,vector3D.back().y,vector3D.back().z});
        // 4th vertex, back is now 3rd vertex, so deduct y, keeping x, to get bottom right corner, front face
        vector3D.push_back({vector3D.back().x,vector3D.back().y-(*whd_it).y,vector3D.back().z});
        // 5th vertex deduct x to come back to bottom left corner, and deduct a depth for z to get back face
        vector3D.push_back({vector3D.back().x-(*whd_it).x,vector3D.back().y,vector3D.back().z-(*whd_it).z});
        // 6nd vertex, keep 1st vertex x and z, add y from vector2D, to get upper left corner, back face
        vector3D.push_back({vector3D.back().x,vector3D.back().y+(*whd_it).y,vector3D.back().z});
        // 7th vertex, back is now 2nd vertex, so just add y, add x from, to get upper right corner, back face
        vector3D.push_back({vector3D.back().x+(*whd_it).x,vector3D.back().y,vector3D.back().z});
        // 8th vertex, back is now 3rd vertex, so deduct y, keeping x, to get bottom right corner, back face
        vector3D.push_back({vector3D.back().x,vector3D.back().y-(*whd_it).y,vector3D.back().z});
        //~ n_vect_whd++;
        std::cout<<"loop end"<<std::endl;
            }
        std::cout<<"control create_vector3D4 -- end"<<std::endl;
        displayV3D(vector3D);
}

void glm_vector(std::vector<vertex3D> &vector_text_whd, vertex3D &first_vector3D){

    for(std::vector<vertex3D>::iterator it=vector_text_whd.begin();it!=vector_text_whd.end();++it){
        std::cout<<"loop"<<std::endl;
        std::cout<<(*it).x<<std::endl;
        std::cout<<(*it).y<<std::endl;
        std::cout<<(*it).z<<std::endl;
            }
        
        // 1st vertex

        //~ glm::vec3 origin(x_0,y_0,z_0);
        //~ glm::vec3 size(w,h,d);
        
        //~ glm::vec3 vertex= origin + size;


        //~ vector3D.push_back({first_vector3D.x+n_vect_whd*(offset_x+(*it).x),first_vector3D.y+n_vect_whd*(offset_y+(*it).y),first_vector3D.z+n_vect_whd*(offset_z+(*it).z)});
        //~ // 2nd vertex, keep 1st vertex x and z, add y from vector2D, to get upper left corner, front face
        //~ vector3D.push_back({vector3D.back().x,vector3D.back().y+(*it).y,vector3D.back().z});
        //~ // 3rd vertex, back is now 2nd vertex, so just add y, add x from, to get upper right corner, front face
        //~ vector3D.push_back({vector3D.back().x+(*it).x,vector3D.back().y,vector3D.back().z});
        //~ // 4th vertex, back is now 3rd vertex, so deduct y, keeping x, to get bottom right corner, front face
        //~ vector3D.push_back({vector3D.back().x,vector3D.back().y-(*it).y,vector3D.back().z});
        //~ // 5th vertex deduct x to come back to bottom left corner, and deduct a depth for z to get back face
        //~ vector3D.push_back({vector3D.back().x-(*it).x,vector3D.back().y,vector3D.back().z-(*it).z});
        //~ // 6nd vertex, keep 1st vertex x and z, add y from vector2D, to get upper left corner, back face
        //~ vector3D.push_back({vector3D.back().x,vector3D.back().y+(*it).y,vector3D.back().z});
        //~ // 7th vertex, back is now 2nd vertex, so just add y, add x from, to get upper right corner, back face
        //~ vector3D.push_back({vector3D.back().x+(*it).x,vector3D.back().y,vector3D.back().z});
        //~ // 8th vertex, back is now 3rd vertex, so deduct y, keeping x, to get bottom right corner, back face
        //~ vector3D.push_back({vector3D.back().x,vector3D.back().y-(*it).y,vector3D.back().z});
        //~ n_vect_whd++;

    }

void testing_vectorshw(){
    std::vector<vertex3D> test3D;
    vertex3D vect_coord={-1,-1,-1};
    //~ test3D=create_vector3D(testing);
    test3D=create_vector3D(testing,vect_coord);
    //~ displayV3D(test3D);
}

void init_cube(const char *text, float x, float y, float z) {
//~ void init_cube(const char *text) {
    const char *p;
    
    //~ GLfloat x = text_coordinates.x;
    //~ GLfloat y = text_coordinates.y;
    //~ GLfloat z = text_coordinates.z;
    
    inputText=text;
    FT_Set_Pixel_Sizes(face, 0, fontSize);

    float sx = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
    float sy = 2.0 / glutGet(GLUT_WINDOW_HEIGHT);

    FT_GlyphSlot g = face->glyph;
    float w = 0;
    float h = 0;
    float tot_h=0;

for (p = text; *p; p++) {
        /* Try to load and render the character */
        if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
            {
                fprintf(stderr, "Loading character %c failed!\n", *p);
                continue;
            }
    
        w += (g->advance.x >> 6) * sx;
        h = std::max(h, g->bitmap_top*sy);
        //~ y = std::max(y, (g->bitmap.rows - g->bitmap_top)*sy);
        tot_h = std::max(tot_h, (g->bitmap.rows - g->bitmap_top)*sy);
        std::cout << "y: " << y << std::endl;
        std::cout << "tot_h: " << tot_h << std::endl;

    }
    
    // to have a gap between border and text, we set up a padding 
    //~ float padding=0.3;
    //~ float horizontal_padding=0.5;
    //~ float vertical_padding=0.5;
    float horizontal_padding=padding.x;
    float vertical_padding=padding.y;
    
    //~ if (x<0.0) horizontal_padding=-horizontal_padding;
    if (y<0.0) vertical_padding=-vertical_padding;
    
    //We use -y instead of y because we stored the bitmap_top-height in
    // y which is the origin of the y axis in freetype, so it's inverted
    // compared with our y coordinate.
    
    // to get a square we keep the bigger of w and h,
    // it should always be w
    float s=w;
    h = tot_h;

    vertex3D vertex_coord2[8] = {
        //front
        {x-horizontal_padding, -y-vertical_padding, z },
        {x + w +horizontal_padding, -y-vertical_padding, z},
        {x + w +horizontal_padding, -y + h+vertical_padding,z},
        {x-horizontal_padding, -y + h+vertical_padding, z},
        //back
        {x-horizontal_padding, -y-vertical_padding, z -s },
        {x + w +horizontal_padding, -y-vertical_padding, z-s},
        {x + w +horizontal_padding, -y + h +vertical_padding,z-s},
        {x-horizontal_padding, -y + h +vertical_padding, z-s},
        };

        //front
        std::cout << x-horizontal_padding << " , " << -y-vertical_padding << " , " << z  << std::endl;
        std::cout << x + w+horizontal_padding << " , " << -y-vertical_padding << " , " << z << std::endl;
        std::cout << x + w+horizontal_padding << " , " << -y + h+vertical_padding << " , " <<z << std::endl;
        std::cout << x-horizontal_padding << " , " << -y + h+vertical_padding << " , " << z << std::endl;
        std::cout << std::endl;
        //back
        std::cout << x-horizontal_padding << " , " << -y-vertical_padding << " , " << z -s  << std::endl;
        std::cout << x + w+horizontal_padding << " , " << -y-vertical_padding << " , " << z-s << std::endl;
        std::cout << x + w+horizontal_padding << " , " << -y + h+vertical_padding << " , " <<z-s << std::endl;
        std::cout << x-horizontal_padding << " , " << -y + h+vertical_padding << " , " << z-s << std::endl;
        
        //padding
        std::cout << std::endl;
        std::cout << "horizontal_padding , vertical_padding" << std::endl;
        std::cout << horizontal_padding << " , " << vertical_padding << std::endl;
        std::cout << "x,y,z,w,h,s" << std::endl;
        std::cout << x << " , " << y <<" , " << z <<" , " << w <<" , " << h <<" , " << s << std::endl;
        


    glGenBuffers(1,&vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER,sizeof vertex_coord2, vertex_coord2,GL_STATIC_DRAW);
    
GLushort cube_elements[] = {
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
    
	glGenBuffers(1, &ibo_cube_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);
    
}

void init_struct() {
    
    float init_x=0;
    float init_y=0;
    float init_z=0;
    float init_distance=0;
    int nb_box=0;
    int * size_struct;
    vertex3D vertex_Boxes[*size_struct];
}

void init_vectors(){
    
    float inter_boxes=0.5;
    std::vector<vertex3D> vertex_coord_box2;

    for(int i=0;i<20;i++){
        vertex_coord_box2.push_back({-2.5, 0, 0+i*inter_boxes });
        vertex_coord_box2.push_back({-2.5, 0.5, 0+i*inter_boxes});
        vertex_coord_box2.push_back({-2, 0.5,0+i*inter_boxes});
        vertex_coord_box2.push_back({-2,0,0+i*inter_boxes});
        
        vertex_coord_box2.push_back({-2.5, 0, -0.5+i*inter_boxes });
        vertex_coord_box2.push_back({-2.5, 0.5, -0.5+i*inter_boxes});
        vertex_coord_box2.push_back({-2, 0.5,-0.5+i*inter_boxes});
        vertex_coord_box2.push_back({-2,0,-0.5+i*inter_boxes});
    }

    glGenBuffers(1,&vbo_vertices_box2);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_vertices_box2);
    glBufferData(GL_ARRAY_BUFFER,vertex_coord_box2.size() * sizeof(vertex3D),&vertex_coord_box2.front(),GL_STATIC_DRAW);

    std::array<GLushort,36> box_elements2 = {
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
        6, 7, 3
        };

    std::vector<GLushort> vect_index;

    for(GLushort i=0;i<20;i++){
        for ( auto it = box_elements2.begin(); it != box_elements2.end(); ++it ){
            vect_index.push_back((*it)+8*i);
        }
    }

    glGenBuffers(1, &ibo_box_elements2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_box_elements2);
    //~ glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(box_elements2), box_elements2, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vect_index.size() * sizeof(GLushort), &vect_index.front(), GL_STATIC_DRAW);
    
        
    }
/*    
void init_vectors3(vertex3D &vect_coord){
    
    float inter_boxes=0.5;
    //~ std::vector<vertex3D> vertex_coord_boxes;
    std::vector<vertex3D> vertex_3D;
    //~ std::vector<vertex3D> test3D;
    //~ vertex3D vect_coord={-5,0,0};
    vertex_3D=create_vector3D(testing,vect_coord);
    displayV3D(vertex_3D);
    //~ init_vectors2(vertex_3D)

    glGenBuffers(1,&vbo_vertices_box3);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_vertices_box3);
    glBufferData(GL_ARRAY_BUFFER,vertex_3D.size() * sizeof(vertex3D),&vertex_3D.front(),GL_STATIC_DRAW);

    std::array<GLushort,36> box_elements3 = {
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
        6, 7, 3
        };

    std::vector<GLushort> vect_index;

    int max_loop = testing.size();
    for(GLushort i=0;i<max_loop;i++){
        for ( auto it = box_elements3.begin(); it != box_elements3.end(); ++it ){
            vect_index.push_back((*it)+8*i);
        }
    }

    glGenBuffers(1, &ibo_box_elements3);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_box_elements3);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vect_index.size() * sizeof(GLushort), &vect_index.front(), GL_STATIC_DRAW);
    
    }
*/    
void init_vectors2(){
    
    //~ float inter_boxes=1.0;
    //~ std::vector<vertex3D> vertex_coord_boxes;
    std::vector<vertex3D> vertex_3D;
    //~ std::vector<vertex3D> test3D;
    vertex3D vect_coord={-1,1,-2};
    vertex_3D=create_vector3D(testing,vect_coord);
    //~ displayV3D(vertex_3D);
    //~ init_vectors2(vertex_3D)

    glGenBuffers(1,&vbo_vertices_box3);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_vertices_box3);
    glBufferData(GL_ARRAY_BUFFER,vertex_3D.size() * sizeof(vertex3D),&vertex_3D.front(),GL_STATIC_DRAW);

    std::array<GLushort,36> box_elements3 = {
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
        6, 7, 3
        };

    std::vector<GLushort> vect_index;

    int max_loop = testing.size();
    for(GLushort i=0;i<max_loop;i++){
        for ( auto it = box_elements3.begin(); it != box_elements3.end(); ++it ){
            vect_index.push_back((*it)+8*i);
        }
    }

    glGenBuffers(1, &ibo_box_elements3);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_box_elements3);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vect_index.size() * sizeof(GLushort), &vect_index.front(), GL_STATIC_DRAW);
    
    }
    
void init_vectors3(std::vector<vertex3D> &vectors_boxes){
    
    glGenBuffers(1,&vbo_vertices_box3);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_vertices_box3);
    glBufferData(GL_ARRAY_BUFFER,vectors_boxes.size() * sizeof(vertex3D),&vectors_boxes.front(),GL_STATIC_DRAW);

    // set the array of indexes for a rectangle
    std::array<GLushort,36> box_elements3 = {
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
        6, 7, 3
        };

    std::vector<GLushort> vect_index;

    int max_loop = vectors_boxes.size();
    for(GLushort i=0;i<max_loop;i++){
        for ( auto it = box_elements3.begin(); it != box_elements3.end(); ++it ){
            vect_index.push_back((*it)+8*i);
        }
    }

    glGenBuffers(1, &ibo_box_elements3);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_box_elements3);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vect_index.size() * sizeof(GLushort), &vect_index.front(), GL_STATIC_DRAW);
    
    }


void init_box() {

    vertex3D vertex_coord_box[24] = {
        //front
        {-0.5, 0, 0 },
        {-0.5, 0.5, 0},
        {0, 0.5,0},
        {0,0,0},
        
        //back
        {-0.5, 0, -0.5 },
        {-0.5, 0.5, -0.5},
        {0, 0.5,-0.5},
        {0,0,-0.5},
        
        //front 2nd box
        {0.5, 0, 0 },
        {0.5, 0.5, 0},
        {1, 0.5,0},
        {1,0,0},
        
        //back 2nd box
        {0.5, 0, -0.5 },
        {0.5, 0.5, -0.5},
        {1, 0.5,-0.5},
        {1,0,-0.5},

        //front 3rd box
        {1.5, 0, 0 },
        {1.5, 0.5, 0},
        {2, 0.5,0},
        {2,0,0},
        
        //back 3rd box
        {1.5, 0, -0.5 },
        {1.5, 0.5, -0.5},
        {2, 0.5,-0.5},
        {2,0,-0.5}
    };
    
    glGenBuffers(1,&vbo_vertices_box);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_vertices_box);
    glBufferData(GL_ARRAY_BUFFER,sizeof vertex_coord_box, vertex_coord_box,GL_STATIC_DRAW);
    
GLushort box_elements[] = {
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
        
        // 2nd box
        // front
        8,9,10,
        10,11,8,
        // right
        9,13,14,
        14,10,9,
        // back
        15,14,13,
        13,12,15,
        // left
        12,8,11,
        11,15,12,
        // bottom
        12,13,9,
        9,8,12,
        // top
        11,10,14,
        14,15,11,
        
        // 3rd Box
        // front
        16, 17, 18, 
        18, 19, 16, 
        // right
        17, 21, 22, 
        22, 18, 17, 
        // back
        23, 22, 21, 
        21, 20, 23, 
        // left
        20, 16, 19, 
        19, 23, 20, 
        // bottom
        20, 21, 17, 
        17, 16, 20, 
        // top
        19, 18, 22, 
        22, 23, 19

    };

	glGenBuffers(1, &ibo_box_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_box_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(box_elements), box_elements, GL_STATIC_DRAW);
    
}

void init_testing(){
    
    std::vector<vertex3D> testing ={
    //~ {1.0,0.3,0.5},
    //~ {1.5,0.5,0.5},
    //~ {2.2,0.8,0.5}
    {0.5,0.3,0.2},
    {0.2,0.3,0.2}
    };
    
    vertex3D origin = {0.0,0.0,0.0};
    vertex3D offset = {0.5,0.5,0.5};
    
    std::vector<vertex3D> boxes=create_vector3D2(testing, origin, offset);
    displayV3D(boxes);
    
    init_vectors3(boxes);
    
}

void draw_cube(){

  glEnableVertexAttribArray(attribute_coord3d);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
  glVertexAttribPointer(
    attribute_coord3d,   // attribute
    3,                   // number of elements per vertex, here (x,y,z)
    GL_FLOAT,            // the type of each element
    GL_FALSE,            // take our values as-is
    //~ sizeof(struct attributes),  // next coord3d appears every 5 floats
    0,  // next coord3d appears every 5 floats
    0                    // offset of first element
  );
  glEnableVertexAttribArray(attribute_v_color);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices_color);
  glVertexAttribPointer(
    attribute_v_color,      // attribute
    3,                      // number of elements per vertex, here (r,g,b)
    GL_FLOAT,             
      // the type of each element
    GL_FALSE,               // take our values as-is
    0,
    //~ sizeof(struct attributes),  // stride
    //(void*) (2 * sizeof(GLfloat))     // offset of first element
    //~ (void*) offsetof(struct attributes, v_color)  // offset
    0
  );

  //~ /* Push each element in buffer_vertices to the vertex shader */
  //~ glDrawArrays(GL_TRIANGLES, 0, 3);
  
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
    int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    
    glDisableVertexAttribArray(attribute_coord3d);
    glDisableVertexAttribArray(attribute_v_color);
    //~ glutSwapBuffers();
}
void draw_box(){
    //~ glClearColor(1.0, 1.0, 1.0, 1.0);
    //~ glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  
  //~ glUseProgram(program);
  glEnableVertexAttribArray(attribute_coord3d_box);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices_box);
  glVertexAttribPointer(
    attribute_coord3d_box,   // attribute
    3,                   // number of elements per vertex, here (x,y,z)
    GL_FLOAT,            // the type of each element
    GL_FALSE,            // take our values as-is
    //~ sizeof(struct attributes),  // next coord3d appears every 5 floats
    0,  // next coord3d appears every 5 floats
    0                    // offset of first element
  );
  glEnableVertexAttribArray(attribute_v_color_box);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices_color_box);
  glVertexAttribPointer(
    attribute_v_color_box,      // attribute
    3,                      // number of elements per vertex, here (r,g,b)
    GL_FLOAT,               // the type of each element
    GL_FALSE,               // take our values as-is
    0,
    //~ sizeof(struct attributes),  // stride
    //(void*) (2 * sizeof(GLfloat))     // offset of first element
    //~ (void*) offsetof(struct attributes, v_color)  // offset
    0
  );

  //~ /* Push each element in buffer_vertices to the vertex shader */
  //~ glDrawArrays(GL_TRIANGLES, 0, 3);
  
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_box_elements);
    int size;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    
    glDisableVertexAttribArray(attribute_coord3d_box);
    glDisableVertexAttribArray(attribute_v_color_box);
    //~ glutSwapBuffers();
}
void draw_box2(){
    //~ glClearColor(1.0, 1.0, 1.0, 1.0);
    //~ glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  
  //~ glUseProgram(program);
  glEnableVertexAttribArray(attribute_coord3d_box);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices_box2);
  glVertexAttribPointer(
    attribute_coord3d_box,   // attribute
    3,                   // number of elements per vertex, here (x,y,z)
    GL_FLOAT,            // the type of each element
    GL_FALSE,            // take our values as-is
    //~ sizeof(struct attributes),  // next coord3d appears every 5 floats
    0,  // next coord3d appears every 5 floats
    0                    // offset of first element
  );
  glEnableVertexAttribArray(attribute_v_color_box);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices_color_box2);
  glVertexAttribPointer(
    attribute_v_color_box,      // attribute
    3,                      // number of elements per vertex, here (r,g,b)
    GL_FLOAT,               // the type of each element
    GL_FALSE,               // take our values as-is
    0,
    //~ sizeof(struct attributes),  // stride
    //(void*) (2 * sizeof(GLfloat))     // offset of first element
    //~ (void*) offsetof(struct attributes, v_color)  // offset
    0
  );

  //~ /* Push each element in buffer_vertices to the vertex shader */
  //~ glDrawArrays(GL_TRIANGLES, 0, 3);
  
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_box_elements2);
    int size;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    
    glDisableVertexAttribArray(attribute_coord3d_box);
    glDisableVertexAttribArray(attribute_v_color_box);
    //~ glutSwapBuffers();
}

void draw_box3(){
    //~ std::cout<<"draw_box3" <<std::endl;
    glEnableVertexAttribArray(attribute_coord3d_box);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices_box3);
    glVertexAttribPointer(
    attribute_coord3d_box,   // attribute
    3,                   // number of elements per vertex, here (x,y,z)
    GL_FLOAT,            // the type of each element
    GL_FALSE,            // take our values as-is
    0,  // next coord3d appears every 5 floats
    0                    // offset of first element
    );
    glEnableVertexAttribArray(attribute_v_color_box);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices_color_box3);
    glVertexAttribPointer(
    attribute_v_color_box,      // attribute
    3,                      // number of elements per vertex, here (r,g,b)
    GL_FLOAT,               // the type of each element
    GL_FALSE,               // take our values as-is
    0,
    0
    );
    
    //~ /* Push each element in buffer_vertices to the vertex shader */
    //~ glDrawArrays(GL_TRIANGLES, 0, 3);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_box_elements3);
    int size;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    
    glDisableVertexAttribArray(attribute_coord3d_box);
    glDisableVertexAttribArray(attribute_v_color_box);
    //~ glutSwapBuffers();
}


void init_text(const char *text,float x, float y, float z, float h_padding, float v_padding) {
    
    inputText=text;
    text_coordinates = {x,y,z};
    padding = {h_padding,v_padding};
    
    /* Create texture atlasses for several font sizes */
    a48 = new atlas(face, 48);
    a24 = new atlas(face, 24);
    a12 = new atlas(face, 12);
    float sx = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
    float sy = 2.0 / glutGet(GLUT_WINDOW_HEIGHT);
    //~ create_coord_vector_text(inputText, a48, text_coordinates.x, -text_coordinates.y, text_coordinates.z, padding.x,padding.y );
    //~ create_coord_vector_text(inputText, a48, text_coordinates.x, -text_coordinates.y, text_coordinates.z, sx, sy );
    std::cout << "init_text: inputText: " << inputText << std::endl;
    create_coord_vector_text2(inputText, a48, x, -y, z, sx, sy );
    text_frame(glyphs_box);
}

void init_text_Entity(E &my_entity,float x, float y, float z, float h_padding, float v_padding) {
    
    //~ inputText=text;
    text_coordinates = {x,y,z};
    padding = {h_padding,v_padding};
    
    /* Create texture atlasses for several font sizes */
    a48 = new atlas(face, 48);
    a24 = new atlas(face, 24);
    a12 = new atlas(face, 12);
    float sx = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
    float sy = 2.0 / glutGet(GLUT_WINDOW_HEIGHT);
    //~ create_coord_vector_text(inputText, a48, text_coordinates.x, -text_coordinates.y, text_coordinates.z, padding.x,padding.y );
    //~ create_coord_vector_text(inputText, a48, text_coordinates.x, -text_coordinates.y, text_coordinates.z, sx, sy );
    //~ std::cout << "init_text: inputText: " << inputText << std::endl;
    create_coord_vector_text2(my_entity.name.c_str(), a48, x, -y, z, sx, sy );
    create_coord_vector_text2(my_entity.data.c_str(), a48, x+1.0, -y, z, sx, sy );
    text_frame2(glyphs_box, nb_glyphs_per_text);
    //~ text_frame(glyphs_box);
}
void init_text_Entity2(E &my_entity,vertex3D &origin, vertex3D &offset, vertex2D &padding) {
    
    //~ inputText=text;
    //~ text_coordinates = {x,y,z};
    //~ padding = {h_padding,v_padding};
    
    /* Create texture atlasses for several font sizes */
    a48 = new atlas(face, 48);
    a24 = new atlas(face, 24);
    a12 = new atlas(face, 12);
    vertex2D window_scale;
    window_scale.x = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
    window_scale.y = 2.0 / glutGet(GLUT_WINDOW_HEIGHT);
    //~ create_coord_vector_text(inputText, a48, text_coordinates.x, -text_coordinates.y, text_coordinates.z, padding.x,padding.y );
    //~ create_coord_vector_text(inputText, a48, text_coordinates.x, -text_coordinates.y, text_coordinates.z, sx, sy );
    //~ std::cout << "init_text: inputText: " << inputText << std::endl;

    create_coord_vector_text4(my_entity.name.c_str(), a48, origin, window_scale );
    origin.x += offset.x;
    origin.y += offset.y;
    origin.z += offset.z;
    create_coord_vector_text4(my_entity.data.c_str(), a48, origin, window_scale );
    
    origin.x += offset.x;
    origin.y += offset.y;
    origin.z += offset.z;
    
    create_coord_vector_text4(my_entity.get_name_vE_by_index(0).c_str(), a48, origin, window_scale );
    origin.x += offset.x;
    origin.y += offset.y;
    origin.z += offset.z;
    create_coord_vector_text4(my_entity.get_data_vE_by_index(0).c_str(), a48, origin, window_scale );
    text_frame2(glyphs_box, nb_glyphs_per_text);
    //~ text_frame3(glyphs_box, nb_glyphs_per_text, offset);
    //~ text_frame(glyphs_box);
}
void init_text_Entity3(E &my_entity,vertex3D &origin, std::vector<vertex3D> &offset, vertex2D &padding, vertex3D color) {
    
    //~ inputText=text;
    //~ text_coordinates = {x,y,z};
    //~ padding = {h_padding,v_padding};
    
    /* Create texture atlasses for several font sizes */
    a48 = new atlas(face, 48);
    a24 = new atlas(face, 24);
    a12 = new atlas(face, 12);
    vertex2D window_scale;
    window_scale.x = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
    window_scale.y = 2.0 / glutGet(GLUT_WINDOW_HEIGHT);
    //~ create_coord_vector_text(inputText, a48, text_coordinates.x, -text_coordinates.y, text_coordinates.z, padding.x,padding.y );
    //~ create_coord_vector_text(inputText, a48, text_coordinates.x, -text_coordinates.y, text_coordinates.z, sx, sy );
    //~ std::cout << "init_text: inputText: " << inputText << std::endl;
    std::vector<vertex3D> v_origin;
    vertex3D temp_origin=origin;
    for(int i=0; i<4;++i){
        v_origin.push_back({temp_origin.x+offset.at(i).x,temp_origin.y+offset.at(i).y,temp_origin.z+offset.at(i).z});
        temp_origin=v_origin.back();
    }

    //~ origin.x += offset.begin()->x;
    //~ origin.y += offset.begin()->y;
    //~ origin.z += offset.begin()->z;
    create_coord_vector_text4(my_entity.name.c_str(), a48, v_origin.at(0), window_scale );
    //~ origin.x += offset.at(1).x;
    //~ origin.y += offset.at(1).y;
    //~ origin.z += offset.at(1).z;
    create_coord_vector_text4(my_entity.data.c_str(), a48, v_origin.at(1), window_scale );
    
    //~ origin.x += offset.at(2).x;
    //~ origin.y += offset.at(2).y;
    //~ origin.z += offset.at(2).z;
    
    create_coord_vector_text4(my_entity.get_name_vE_by_index(0).c_str(), a48, v_origin.at(2), window_scale );
    //~ origin.x += offset.at(3).x;
    //~ origin.y += offset.at(3).y;
    //~ origin.z += offset.at(3).z;
    create_coord_vector_text4(my_entity.get_data_vE_by_index(0).c_str(), a48, v_origin.at(3), window_scale );
    //~ text_frame2(glyphs_box, nb_glyphs_per_text);
    //~ text_frame3(glyphs_box, nb_glyphs_per_text, offset);
    text_frame3(glyphs_box, nb_glyphs_per_text, v_origin);
    init_color5(color,v_origin);
    //~ text_frame(glyphs_box);
}

void textDisplay() {

    float sx = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
    float sy = 2.0 / glutGet(GLUT_WINDOW_HEIGHT);
    
    //~ float x =  -1.5;
    //~ float y =  -1.0;
    //~ float z =  0.0;
    
    GLfloat x = text_coordinates.x;
    GLfloat y = text_coordinates.y;
    GLfloat z = text_coordinates.z;
    
    // working on cube associated
    //~ init_cube(inText.c_str(),-0.5,0,0);
    
    // init cube for the render_Text_Z
    //~ init_cube(inputText, x,-y,z);
    //~ init_cube(inputText);
    
    
    /* White background */
    glClearColor(1, 1, 1, 1);
    //~ glClearDepth(1);
    //~ glClearColor(0, 0, 0, 1);
    //~ glClear(GL_COLOR_BUFFER_BIT);
    // need the depth buffer for 3d
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


    /* Enable blending, necessary for our alpha texture */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //~ glUseProgram(program_text);
    GLfloat white[4] = { 1, 1, 1, 1 };
    GLfloat green[4] = { 0, 1, 0, 1 };
    GLfloat black[4] = { 0, 0, 0, 1 };
    GLfloat red[4] = { 1, 0, 0, 1 };
    
    //set the color of the non-glyph part of the glyph box to same color as the
    // cube color: r,g,b
    GLfloat bg_rgb[4] = { r, g, b, 1 };
    
    FT_Set_Pixel_Sizes(face, 0, fontSize);
    //~ glUniform4fv(uniform_color, 1, black);
    //~ glUniform4fv(uniform_bgcolor, 1, bg_rgb);
    //~ render_text_Z(inputText, x, y, z, sx, sy);

    // Drawing a cube
    //~ glUseProgram(program_box);
    //~ draw_box();
    //~ glUseProgram(program_cube);
    //~ draw_cube();
    //~ glUseProgram(program_box);
    //~ draw_box2();


    /* Drawing the text */    
    glUseProgram(program_test);
    glUniform4fv(uniform_color_test, 1, red);
    glUniform4fv(uniform_bgcolor_test, 1, bg_rgb);
    render_text3(a48);
    
    // Drawing a box
    glUseProgram(program_box);
    draw_box3();
    
    
    glutSwapBuffers();
    
}
/*
void onIdle() {
    
    float move = sinf(glutGet(GLUT_ELAPSED_TIME) / 1000.0 * (2*3.14) / 10); // -1<->+1 every 5 seconds
    float angle = glutGet(GLUT_ELAPSED_TIME) / 1000.0 * 5;  // 45° per second
    glm::vec3 axis_y(0, 1, 0);
    glm::vec3 axis_z(0, 0, 1);
    glm::mat4 m_transform = glm::translate(glm::mat4(1.0f), glm::vec3(move, 0.0, 0.0))
    * glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis_y)* glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis_z);
    //~ * glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis_z);

    //~ // program_bg is for the background color
    //~ glUseProgram(program_bg);
    //~ glUniformMatrix4fv(uniform_m_transform, 1, GL_FALSE, glm::value_ptr(m_transform));
    
    // program_bg is for the colored cube
    glUseProgram(program_cube);
    glUniformMatrix4fv(uniform_m_transform, 1, GL_FALSE, glm::value_ptr(m_transform));
    
    // program_text is for the text
    glUseProgram(program_text);
    glUniformMatrix4fv(uniform_m_transform, 1, GL_FALSE, glm::value_ptr(m_transform));
    
    glutPostRedisplay();
}
*/

void onIdle() {
    //~ float move = sinf(glutGet(GLUT_ELAPSED_TIME) / 1000.0 * (2*3.14) / 5); // -1<->+1 every 2 seconds
    //~ float angle = glutGet(GLUT_ELAPSED_TIME) / 1000.0 * 25;  // 25° per second
    float move = 0;
    float angle = 0;
    screen_width=glutGet(GLUT_WINDOW_WIDTH);
    screen_height=glutGet(GLUT_WINDOW_HEIGHT);
    glm::vec3 axis_y(0, 1, 0);
    glm::vec3 axis_z(0, 0, 1);
    glm::mat4 m_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0 ,move));
    glm::mat4 anim = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis_y);
    glm::mat4 anim2 = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis_z);
    
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0, 0.0, -1.0));
    glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 0.0,-1.0), glm::vec3(0.0, 1.0, 0.0));//eye,center,up
    glm::mat4 projection = glm::perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 10.0f);
    
    glm::mat4 m_transform = projection * view * model * m_translate * anim2 * anim;
    //~ glm::mat4 m_transform = projection * view * model;
    
    // program_bg is for the colored cube
    glUseProgram(program_cube);
    glUniformMatrix4fv(uniform_m_transform, 1, GL_FALSE, glm::value_ptr(m_transform));
    
    
    // program_text is for the text
    glUseProgram(program_text);
    glUniformMatrix4fv(uniform_m_transform, 1, GL_FALSE, glm::value_ptr(m_transform));
   
    // program_test is the current testing: separate texts on separate boxes
    glUseProgram(program_test);
    glUniformMatrix4fv(uniform_m_transform, 1, GL_FALSE, glm::value_ptr(m_transform));
    
    // program_box is for the box
    glUseProgram(program_box);
    glUniformMatrix4fv(uniform_m_transform, 1, GL_FALSE, glm::value_ptr(m_transform));
    
    
    glutPostRedisplay();
    
    //~ glUseProgram(program);
    //~ glUniformMatrix4fv(uniform_m_transform, 1, GL_FALSE, glm::value_ptr(m_transform));
    //~ glutPostRedisplay();
}

void free_resources() {
    glDeleteProgram(program_text);
    glDeleteProgram(program_box);
    //~ glDeleteProgram(program_bg);
    glDeleteProgram(program_cube);
    glDeleteBuffers(1, &vbo_coord);
    glDeleteBuffers(1, &vbo_tex_coord);
    glDeleteBuffers(1, &vbo_vertices);
    glDeleteBuffers(1, &vbo_vertices_box);
    glDeleteBuffers(1, &vbo_vertices_box2);
    glDeleteBuffers(1, &vbo_vertices_box3);
    glDeleteBuffers(1, &vbo_vertices_color);
    glDeleteBuffers(1, &vbo_vertices_color_box);
    glDeleteBuffers(1, &vbo_vertices_color_box2);
    glDeleteBuffers(1, &vbo_vertices_color_box3);
    glDeleteBuffers(1, &ibo_cube_elements);
    glDeleteBuffers(1, &ibo_box_elements);
    glDeleteBuffers(1, &ibo_box_elements2);
    glDeleteBuffers(1, &ibo_box_elements3);
}
