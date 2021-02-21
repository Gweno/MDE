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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "./common/shader_utils.h"

#include "MDE.h"

int screen_width=800, screen_height=600;

// programs
GLuint program_text;
GLuint program_box;

// uniforms
GLint uniform_tex_text;
GLint uniform_color_text;
GLint uniform_tex;
GLint uniform_bgcolor_text;
GLint uniform_m_transform;
GLint uniform_m_view;
GLint uniform_m_model;
GLint uniform_m_projection;
GLint uniform_m_camera;

// attributes
GLint attribute_coord_text;
GLint attribute_coord;
GLint attribute_tex_coord;
GLint attribute_coord3d_box;
GLint attribute_v_color_box;

// vbo
GLuint vbo_coord;
GLuint vbo_tex_coord;
GLuint vbo_vertices_box;
GLuint vbo_vertices_color_box;

// ibo
GLuint ibo_box_elements;

const char *inputText;
//~ uint fontSize=48;
uint fontSize=12;
GLfloat r,g,b,a;
GLfloat bg_r, bg_g, bg_b, bg_a;

float x_pos = 0, y_pos = 0, z_pos = 0, x_rot = 0, y_rot = 0;
float lastx, lasty;

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
GLfloat *font_color = NULL;

// some boolean flags
bool move_it = false;
bool display_box = false;

bool left_click=false;
bool right_click=false;

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

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

//~ int xOrigin = -1;

//~ // actual vector representing the camera's direction
//~ float lx=0.0f,lz=-1.0f, ly = 0.0f;


//~ glm::mat4 m_view;

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
        printf("FT_set_Pixel_Sizes:%d\n",error);
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

atlas *a96;
atlas *a48;
atlas *a40;
atlas *a24;
atlas *a12;

atlas *a_any_size;

uint nb_indices=0;

std::vector<vertex3D> glyphs_box;
std::vector<vertex2D> glyphs_vertex;

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
            //~ std::cout << " x: "<< (*it3).x << " y: " << (*it3).y << " z: " << (*it3).z << std::endl;
            //~ std::cout << " hmin: "<< hmin << " hmax: " << hmax << std::endl;
        }
        //~ std::cout << std::endl;
        h = std::abs(hmax - hmin);
        // for now depth is the same as the width of the text
        //~ std::cout << "h: " << h << " hmax: " << hmax << " hmin: " << hmin << std::endl;
        d=h;
        
        bottom_left_front.push_back({wmin,hmin,0});
        top_right_back.push_back({wmax,hmax,d});
        
        std::advance(it,(*it2));
        std::advance(it2,1);
    }
    
    create_vector(vertex_3D, v_origin, bottom_left_front, top_right_back);
    init_vectors(vertex_3D);
    
}

void render_text(atlas * a) {
    
    /* Use the texture containing the atlas */
    glBindTexture(GL_TEXTURE_2D, a->tex);
    
    /* Set up the VBO for our vertex data */
    glEnableVertexAttribArray(attribute_coord);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_coord);
    glVertexAttribPointer(attribute_coord, 3, GL_FLOAT, GL_FALSE, 0, 0);
    /* Draw the character on the screen */
    glBufferData(GL_ARRAY_BUFFER, glyphs_box.size() * sizeof(vertex3D), &glyphs_box.front(), GL_STATIC_DRAW);
    
    
    /* Set up the VBO for our vertex data */
    glEnableVertexAttribArray(attribute_tex_coord);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_tex_coord);
    glVertexAttribPointer(attribute_tex_coord, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    /* Draw all the character on the screen in one go */
    glBufferData(GL_ARRAY_BUFFER,glyphs_vertex.size() * sizeof(vertex2D),&glyphs_vertex.front(),GL_STATIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, nb_indices);
    
    glDisableVertexAttribArray(attribute_coord);
    glDisableVertexAttribArray(attribute_tex_coord);
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
    
    return 1;
}

bool init_program(){

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
    
    // testing boxes
    //~ program_box = create_program("./shaders/box.v.glsl", "./shaders/box.f.glsl");
    
    program_box = create_program("../source/shaders/box.v.glsl", "../source/shaders/box.f.glsl");
    if(program_box == 0)
        return 0;

    attribute_coord3d_box = get_attrib(program_box, "coord3d_b");
    attribute_v_color_box = get_attrib(program_box, "v_color_b");

    uniform_m_model = get_uniform(program_box, "m_model");
    uniform_m_projection = get_uniform(program_box, "m_projection");
    uniform_m_view = get_uniform(program_box, "m_view");
        

    if(attribute_coord3d_box == -1 || attribute_v_color_box == -1 || uniform_m_model == -1 || uniform_m_projection == -1 || uniform_m_view == -1)
        return 0;

    // program for the text
    //~ program_text = create_program("shaders/text.v.glsl", "shaders/text.f.glsl");
    program_text = create_program("../source/shaders/text.v.glsl", "../source/shaders/text.f.glsl");
    if(program_text == 0)
        return 0;
    
    attribute_coord_text = get_attrib(program_text, "coord");
    attribute_tex_coord = get_attrib(program_text,"tex_coord");
    uniform_tex_text = get_uniform(program_text, "tex");
    uniform_color_text = get_uniform(program_text, "color");
    uniform_bgcolor_text = get_uniform(program_text, "bgcolor");

    uniform_m_model = get_uniform(program_text, "m_model");
    uniform_m_projection = get_uniform(program_text, "m_projection");
    uniform_m_view = get_uniform(program_text, "m_view");
    
    if(attribute_coord_text == -1 || uniform_tex_text == -1 || uniform_color_text == -1 || uniform_m_model == -1 || uniform_m_projection == -1 || uniform_m_view == -1)
        return 0;
    

    return 1;
}

void init_color(vertex3Dalpha color_alpha, vertex3Dalpha bg_color_alpha, std::vector<vertex3D> &whd){
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

    int max_loop = whd.size();
    for(int i=0;i<max_loop;i++){
        vect_color3.insert (vect_color3.end(), box_vertices_color3, box_vertices_color3+32);
        }


    glGenBuffers(1,&vbo_vertices_color_box);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_vertices_color_box);
    glBufferData(GL_ARRAY_BUFFER,vect_color3.size() * sizeof(GLfloat),&vect_color3.front(),GL_STATIC_DRAW);
}

void init_font_color(std::vector<GLfloat> &color){
    
    font_color=&color[0];
    
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

 // blf: bottom-left-front, trb: top-right-back
void create_vector(std::vector<vertex3D> &vector3D,std::vector<vertex3D> &v_origin, std::vector<vertex3D> &vector_blf, std::vector<vertex3D> &vector_trb){
    
    std::vector<vertex3D>::iterator v_origin_it;
    std::vector<vertex3D>::iterator v_blf_it;
    std::vector<vertex3D>::iterator v_trb_it;

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

void init_vectors(std::vector<vertex3D> &vectors_boxes){
    
    glGenBuffers(1,&vbo_vertices_box);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_vertices_box);
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

    glGenBuffers(1, &ibo_box_elements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_box_elements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vect_index.size() * sizeof(GLushort), &vect_index.front(), GL_STATIC_DRAW);
    
    }

void draw_box(){
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
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices_color_box);
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
}

void init_text_MDE(MDE &my_entity,vertex3D &origin, std::vector<vertex3D> &offset, std::vector<vertex3D> &offset_rule, vertex2D &padding, vertex3Dalpha &color_alpha, vertex3Dalpha &bg_color_alpha) {
    
    /* Create texture atlasses for a given font size */
    //~ a_any_size = new atlas(face, 48);
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
    //~ my_entity.extract_MDE_data_for_v_offset(my_v_MDE_data, level);
    extractMDEdataForVoffset(my_entity, my_v_MDE_data, level);
    process_v_MDE_data(my_v_MDE_data,offset, offset_rule);
    for (std::vector<vertex3D>::iterator it_offset=offset.begin(); it_offset!=offset.end();++it_offset){
        v_origin.push_back({origin.x+(*it_offset).x,origin.y+(*it_offset).y,origin.z+(*it_offset).z});
    }
    
    // Adding padding at first and last elements of v_origin
    add_padding(v_origin, v_origin_fb, padding);
    

    
    uint index_v_origin=0;
    for (std::vector<GLdata>::iterator it_v_MDE_data=my_v_MDE_data.begin(); it_v_MDE_data!=my_v_MDE_data.end();++it_v_MDE_data){
        
        //~ create_coord_vector_text(((*it_v_MDE_data).MDE_data).c_str(), a48, v_origin[index_v_origin], window_scale );
        create_coord_vector_text(((*it_v_MDE_data).MDE_data).c_str(), a_any_size, v_origin[index_v_origin], window_scale );
        index_v_origin++;
    }
    //~ text_frame(glyphs_box, nb_glyphs_per_text, v_origin);
    text_frame(glyphs_box, nb_glyphs_per_text, v_origin_fb, padding);
    init_color(color_alpha,bg_color_alpha,v_origin);
}

void process_v_MDE_data(std::vector<GLdata> & v_MDE_data,std::vector<vertex3D> & v_offset, std::vector<vertex3D> & v_offset_rule){
    
    //~ GLfloat nd_offset = 0.5; // name-data offset, distance between the name and the data 
    //~ GLfloat index_offset = 0.5; // index offset, distance to separate index values 
    //~ GLfloat level_offset = -0.2; // level offset, distance to separate level values 
    
    // offset rule.x is the name-data offset
    // offset rule.y is the index offset
    // offset rule.x is the level offset
    
    for (std::vector<GLdata>::iterator it_v_MDE_data=v_MDE_data.begin();it_v_MDE_data!=v_MDE_data.end(); ++it_v_MDE_data){
        //~ v_offset.push_back({(*it_v_MDE_data).mde_member*.5+(*it_v_MDE_data).index*0.5,(*it_v_MDE_data).level*(-0.2),0});
        //~ v_offset.push_back({(*it_v_MDE_data).mde_member*nd_offset+(*it_v_MDE_data).index*index_offset,(*it_v_MDE_data).level*level_offset,0});
        //~ v_offset.push_back({(*it_v_MDE_data).mde_member*v_offset_rule.x+(*it_v_MDE_data).index*v_offset_rule.y,(*it_v_MDE_data).level*v_offset_rule.z,0});
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

    // Set the colored background, default is white.
    glClearColor(bg_r, bg_g, bg_b, bg_a);
    // need the depth buffer for 3d
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


    /* Enable blending, necessary for our alpha texture */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLfloat white[4] = { 1, 1, 1, 1 };
    GLfloat green[4] = { 0, 1, 0, 1 };
    GLfloat black[4] = { 0, 0, 0, 1 };
    GLfloat red[4] = { 1, 0, 0, 1 };
    
    // set the color of the non-glyph part of the glyph box to same color as the
    // cube color: r,g,b
    // keep alpha to zero
    //~ GLfloat bg_rgb[4] = { r, g, b, a };
    GLfloat bg_rgb[4] = { r, g, b, 0 };
    
    FT_Set_Pixel_Sizes(face, 0, fontSize);

    // check option to display the box frame for the text.
    if (display_box){
        // Drawing a box
        glUseProgram(program_box);
        draw_box();
        }
    //~ else{
        //~ // if no box, set box alpha to 0.0 to discard the non-glyph part of the glyhp's box
        //~ a=0.0;
    //~ }
    
    /* Drawing the text */    
    glUseProgram(program_text);
    glUniform4fv(uniform_color_text, 1, font_color);
    glUniform4fv(uniform_bgcolor_text, 1, bg_rgb);
    render_text(a_any_size);
    
    glutSwapBuffers();
    
}

void set_options(bool a_move_it, bool a_display_box){
    move_it = a_move_it;
    display_box = a_display_box;
    printf("display box:%u\n", display_box);
}

void mouse_wheel(int button, int state, int x, int y)
{
    //~ printf("Button %d is %s At %d %d\n", button, (state == GLUT_DOWN) ? "Down" : "Up", x, y);
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
        }
    if ((button == 4) && (state == GLUT_UP)) // Scroll down
        {
            if (fov >= 1.0f && fov <= 45.0f) fov ++;
            if (fov <= 1.0f) fov = 1.0f;
            if (fov >= 45.0f) fov = 45.0f;
        }
    }
   
void display(){

    // calculate frame rate for consistent camera velocity
    float currentFrame = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    
    camera();
    text_display();
}


void camera(){
    //https://learnopengl.com/Getting-started/Camera
    
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
    
    // Projection matrix modified by field of view (fov) directed by the camera zoom from the mouse wheel
    glm::mat4 m_projection = glm::perspective(glm::radians(fov), 1.0f*screen_width/screen_height, 0.1f, 10.0f);
    
    glUseProgram(program_text);
    glUniformMatrix4fv(uniform_m_view, 1, GL_FALSE, glm::value_ptr(m_view));
    glUniformMatrix4fv(uniform_m_projection, 1, GL_FALSE, glm::value_ptr(m_projection));
    
    glUseProgram(program_box);
    glUniformMatrix4fv(uniform_m_view, 1, GL_FALSE, glm::value_ptr(m_view));
    glUniformMatrix4fv(uniform_m_projection, 1, GL_FALSE, glm::value_ptr(m_projection));

    glutPostRedisplay();
}

void init_matrices(){
    
    glm::mat4 m_model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0, 0.0, -1.0));
    glm::mat4 m_projection = glm::perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 10.0f);
    glm::mat4 m_view = glm::lookAt(glm::vec3(0.0, 0.0, 4.0), glm::vec3(0.0, 0.0,-1.0), glm::vec3(0.0, 1.0, 0.0));//eye,center,up
    
    glUseProgram(program_text);
    glUniformMatrix4fv(uniform_m_model, 1, GL_FALSE, glm::value_ptr(m_model));
    glUniformMatrix4fv(uniform_m_projection, 1, GL_FALSE, glm::value_ptr(m_projection));
    glUniformMatrix4fv(uniform_m_view, 1, GL_FALSE, glm::value_ptr(m_view));
    
    glUseProgram(program_box);
    glUniformMatrix4fv(uniform_m_model, 1, GL_FALSE, glm::value_ptr(m_model));
    glUniformMatrix4fv(uniform_m_projection, 1, GL_FALSE, glm::value_ptr(m_projection));
    glUniformMatrix4fv(uniform_m_view, 1, GL_FALSE, glm::value_ptr(m_view));
    
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
    //~ glm::mat4 m_projection = glm::perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 10.0f);
    
    m_model = m_model * m_translate * anim2 * anim;
    
    glUseProgram(program_text);
    glUniformMatrix4fv(uniform_m_model, 1, GL_FALSE, glm::value_ptr(m_model));
    //~ glUniformMatrix4fv(uniform_m_projection, 1, GL_FALSE, glm::value_ptr(m_projection));
    
    glUseProgram(program_box);
    glUniformMatrix4fv(uniform_m_model, 1, GL_FALSE, glm::value_ptr(m_model));
    //~ glUniformMatrix4fv(uniform_m_projection, 1, GL_FALSE, glm::value_ptr(m_projection));

    glutPostRedisplay();

}


void free_resources() {
    glDeleteProgram(program_box);
	glDeleteProgram(program_text);
    glDeleteBuffers(1, &vbo_coord);
    glDeleteBuffers(1, &vbo_tex_coord);
    glDeleteBuffers(1, &vbo_vertices_box);
    glDeleteBuffers(1, &vbo_vertices_color_box);
    glDeleteBuffers(1, &ibo_box_elements);
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
            break;
        case 's':
            cameraPos -= cameraSpeed * cameraFront;
            break;
        case 'a':
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            break;
        case 'd':
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            break;
        //~ case '+':
            //~ cameraSpeed +=0.05;
            //~ break;
        //~ case '-':
            //~ (cameraSpeed >= 0) ? cameraSpeed -=0.05 : cameraSpeed = 0.05;
            //~ printf("speed: %.2f/n", cameraSpeed);
            //~ break;
            
       // Check if 'ESC' is pressed
        case 27:
            free_resources();
            glutLeaveMainLoop();
            break;
        
    }
    
    //~ if (key==27) {
        //~ free_resources();
        //~ glutLeaveMainLoop();   // Check if 'ESC' is pressed
    //~ }
}
