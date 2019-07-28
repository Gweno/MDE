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

int screen_width=800, screen_height=600;

GLuint program_text;
GLuint program_bg;
GLuint program_cube;

GLint attribute_coord;
GLint attribute_tex_coord;
GLint attribute_coord2d;
GLint attribute_v_color;

GLint uniform_tex;
GLint uniform_color;
GLint uniform_bgcolor;
GLint uniform_m_transform;

GLuint vbo_coord;
GLuint vbo_tex_coord;
GLuint vbo_vertices;
GLuint vbo_vertices_color;

// for cube:
GLuint ibo_cube_elements;
GLint attribute_coord3d;

const char *inputText;
uint fontSize=48;
GLfloat r,g,b;

struct vertex2D {
    GLfloat x;
    GLfloat y;
};
struct vertex3D {
    GLfloat x;
    GLfloat y;
    GLfloat z;
};


vertex3D text_coordinates ={0.0,0.0,0.0};

FT_Library ft;
FT_Face face;

const char *fontfilename;

//~ // this declaration is used to handle the coordinates and colors for each element
//~ struct attributes {
  //~ GLfloat coord3d[3];
  //~ GLfloat v_color[3];
//~ };

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
    if(attribute_coord3d == -1 || attribute_v_color == -1)
        return 0;

    return 1;
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
        vertex2D box[4] = {
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
    glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
        
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
        //~ vertex2D box[4] = {
            //~ {x2, -y2},
            //~ {x2 + w, -y2},
            //~ {x2, -y2 - h},
            //~ {x2 + w, -y2 - h},
        //~ };
        vertex3D box[4] = {
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
    glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
        
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

    glGenBuffers(1,&vbo_vertices_color);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_vertices_color);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices_color), vertices_color, GL_STATIC_DRAW);
}

void init_text_coordinates(float x, float y, float z) {
    
    text_coordinates = {x,y,z};
    
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
    //~ float padding=0.3;
    float horizontal_padding=0.5;
    float vertical_padding=0.1;
    
    if (x<0.0) horizontal_padding=-horizontal_padding;
    if (y<0.0) vertical_padding=-vertical_padding;
    
    //We use -y instead of y because we stored the bitmap_top-height in
    // y which is the origin of the y axis in freetype, so it's inverted
    // compared with our y coordinate.
    
    // to get a square we keep the bigger of w and h,
    // it should always be w
    float s=w;
    //~ if (h>w) s=h;

    //~ vertex2D vertex_coord[4] = {
            //~ {x-padding, -y-padding},
            //~ {x + s+padding, -y-padding},
            //~ {x-padding, s+padding},
            //~ {x + s+padding, s+padding},
        //~ };
    //~ vertex3D vertex_coord2[8] = {
        //~ //front
        //~ {x-padding, -y-padding, z },
        //~ {x + s+padding, -y-padding, z},
        //~ {x-padding, s+padding, z},
        //~ {x + s+padding, s+padding,z},
        //~ //back
        //~ {x-padding, -y-padding, z -s-padding },
        //~ {x + s+padding, -y-padding, z-s-padding},
        //~ {x-padding, s+padding, z-s-padding},
        //~ {x + s+padding, s+padding,z-s-padding},
        //~ };
    vertex3D vertex_coord2[8] = {
        //~ //front
        //~ {x-padding, -y-padding, z },
        //~ {x + s+padding, -y-padding, z},
        //~ {x + s+padding, s+padding,z},
        //~ {x-padding, s+padding, z},
        //~ //back
        //~ {x-padding, -y-padding, z -s-padding },
        //~ {x + s+padding, -y-padding, z-s-padding},
        //~ {x + s+padding, s+padding,z-s-padding},
        //~ {x-padding, s+padding, z-s-padding},
        //~ };
        //front
        {x-horizontal_padding, -y-vertical_padding, z },
        {x + w+horizontal_padding, -y-vertical_padding, z},
        {x + w+horizontal_padding, -y + h+vertical_padding,z},
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
        std::cout << horizontal_padding << " , " << vertical_padding << std::endl;


    glGenBuffers(1,&vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER,sizeof vertex_coord2, vertex_coord2,GL_STATIC_DRAW);
    
    //~ GLfloat vertices_color[]={
        //~ 0.8,0.8,0.8,
        //~ 0.8,0.8,0.8,
        //~ 0.8,0.8,0.8,
        //~ 0.8,0.8,0.8,
        //~ 1,0.8,0.8,
        //~ 1,0.8,0.8,
        //~ 1,0.8,0.8,
        //~ 1,0.8,0.8,
    //~ };

    //~ glGenBuffers(1,&vbo_vertices_color);
    //~ glBindBuffer(GL_ARRAY_BUFFER,vbo_vertices_color);
    //~ glBufferData(GL_ARRAY_BUFFER,sizeof(vertices_color), vertices_color, GL_STATIC_DRAW);
    
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
//~ GLushort cube_elements[] = {
		//~ // front
		//~ 0, 1, 3,
		//~ 3, 2, 0,
		//~ // right
		//~ 1, 5, 2,
		//~ 2, 6, 1,
		//~ // back
		//~ 7, 6, 4,
		//~ 4, 5, 7,
		//~ // left
		//~ 4, 0, 7,
		//~ 7, 3, 4,
		//~ // bottom
		//~ 4, 5, 0,
		//~ 0, 1, 4,
		//~ // top
		//~ 3, 2, 7,
		//~ 7, 6, 3,
	//~ };
	glGenBuffers(1, &ibo_cube_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);
    
}

//~ int init_resources()
//~ {
    
    //~ GLfloat cube_vertices[] = {
        //~ //top
        //~ -1.0,-1.0,1.0,
        //~ 1.0, -1.0,1.0,
        //~ 1.0,1.0,1.0,
        //~ -1.0, 1.0,1.0,
        //~ //back
        //~ -1.0,-1.0,-1.0,
        //~ 1.0,-1.0,-1.0,
        //~ 1.0,1.0,-1.0,
        //~ -1.0,1.0,-1.0
    //~ };
    
    //~ GLfloat cube_colors[] = {
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
      //~ };

  //~ glGenBuffers(1, &vbo_cube_vertices);
  //~ glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
  //~ glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

  //~ glGenBuffers(1, &vbo_cube_colors);
  //~ glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
  //~ glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);

//~ GLushort cube_elements[] = {
		//~ // front
		//~ 0, 1, 2,
		//~ 2, 3, 0,
		//~ // right
		//~ 1, 5, 6,
		//~ 6, 2, 1,
		//~ // back
		//~ 7, 6, 5,
		//~ 5, 4, 7,
		//~ // left
		//~ 4, 0, 3,
		//~ 3, 7, 4,
		//~ // bottom
		//~ 4, 5, 1,
		//~ 1, 0, 4,
		//~ // top
		//~ 3, 2, 6,
		//~ 6, 7, 3,
	//~ };
	//~ glGenBuffers(1, &ibo_cube_elements);
	//~ glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	//~ glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);



  //~ GLint link_ok = GL_FALSE;

  //~ GLuint vs, fs;
  //~ if ((vs = create_shader("cube.v.glsl", GL_VERTEX_SHADER))   == 0) return 0;
  //~ if ((fs = create_shader("cube.f.glsl", GL_FRAGMENT_SHADER)) == 0) return 0;

  //~ program = glCreateProgram();
  //~ glAttachShader(program, vs);
  //~ glAttachShader(program, fs);
  //~ glLinkProgram(program);
  //~ glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
  //~ if (!link_ok) {
    //~ fprintf(stderr, "glLinkProgram:");
    //~ print_log(program);
    //~ return 0;
  //~ }

  //~ const char* attribute_name;
  //~ attribute_name = "coord3d";
  //~ attribute_coord3d = glGetAttribLocation(program, attribute_name);
  //~ if (attribute_coord3d == -1) {
    //~ fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    //~ return 0;
  //~ }
  //~ attribute_name = "v_color";
  //~ attribute_v_color = glGetAttribLocation(program, attribute_name);
  //~ if (attribute_v_color == -1) {
    //~ fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    //~ return 0;
  //~ }
  
  //~ const char* uniform_name;
  //~ uniform_name = "m_transform";
  //~ uniform_m_transform = glGetUniformLocation(program, uniform_name);
  //~ if (uniform_m_transform == -1) {
    //~ fprintf(stderr, "Could not bind uniform_fade %s\n", uniform_name);
    //~ return 0;
  //~ }

  //~ return 1;
//~ }





void draw_square(){
    
    glEnableVertexAttribArray(attribute_coord2d);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_vertices);
    glVertexAttribPointer(
        attribute_coord2d,
        2,
        GL_FLOAT,            // the type of each element
        GL_FALSE,            // take our values as-is
        0, 
        0                    // offset of the first element
    );
    
    glEnableVertexAttribArray(attribute_v_color);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_vertices_color);
    glVertexAttribPointer(
        attribute_v_color,      // attribute
        3,                      // number of elements per vertex, here (r,g,b)
        GL_FLOAT,               // the type of each element
        GL_FALSE,               // take our values as-is
        0,
        0  // offset of first element
    );

    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    
    glDisableVertexAttribArray(attribute_coord2d);
    glDisableVertexAttribArray(attribute_v_color);
    
}

void draw_cube(){
    //~ glClearColor(1.0, 1.0, 1.0, 1.0);
    //~ glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  
  //~ glUseProgram(program);
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
  
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
    int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    
    glDisableVertexAttribArray(attribute_coord3d);
    glDisableVertexAttribArray(attribute_v_color);
    glutSwapBuffers();
}


void init_text(const char *text) {
    
    inputText=text;
    
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
    
    
    init_cube(inputText, x,-y,z);
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

    /* Drawing the text */
    glUseProgram(program_text);
    GLfloat white[4] = { 1, 1, 1, 1 };
    GLfloat green[4] = { 0, 1, 0, 1 };
    GLfloat black[4] = { 0, 0, 0, 1 };
    
    //set the color of the non-glyph part of the glyph box to same color as the
    // cube color: r,g,b
    GLfloat bg_rgb[4] = { r, g, b, 1 };
    
    FT_Set_Pixel_Sizes(face, 0, fontSize);
    glUniform4fv(uniform_color, 1, black);
    glUniform4fv(uniform_bgcolor, 1, bg_rgb);
    //~ render_text(inputText, 0.0, 0.0, sx, sy);
    //~ render_text(inputText, -1.5, 0.0, sx, sy);
    //~ render_text(inputText, x, y, sx, sy);
    render_text_Z(inputText, x, y, z, sx, sy);

    /* Drawing a cube */
    glUseProgram(program_cube);
    draw_cube();

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
    float move = sinf(glutGet(GLUT_ELAPSED_TIME) / 1000.0 * (2*3.14) / 10); // -1<->+1 every 5 seconds
    float angle = glutGet(GLUT_ELAPSED_TIME) / 1000.0 * 45;  // 45° per second
    //~ float move = 0;
    //~ float angle = 0;
    screen_width=glutGet(GLUT_WINDOW_WIDTH);
    screen_height=glutGet(GLUT_WINDOW_HEIGHT);
    glm::vec3 axis_y(0, 1, 0);
    glm::vec3 axis_z(0, 0, 1);
    glm::mat4 m_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0 ,move));
    glm::mat4 anim = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis_y);
    glm::mat4 anim2 = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis_z);
    
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -1.0));
    glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 2.0), glm::vec3(0.0, 0.0,-1.0), glm::vec3(0.0, 1.0, 0.0));//eye,center,up
    glm::mat4 projection = glm::perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 10.0f);
    
    glm::mat4 m_transform = projection * view * model * m_translate * anim2 * anim;
    //~ glm::mat4 m_transform = projection * view * model;
    
    // program_bg is for the colored cube
    glUseProgram(program_cube);
    glUniformMatrix4fv(uniform_m_transform, 1, GL_FALSE, glm::value_ptr(m_transform));
    
    
    // program_text is for the text
    glUseProgram(program_text);
    glUniformMatrix4fv(uniform_m_transform, 1, GL_FALSE, glm::value_ptr(m_transform));
    
    glutPostRedisplay();
    
    //~ glUseProgram(program);
    //~ glUniformMatrix4fv(uniform_m_transform, 1, GL_FALSE, glm::value_ptr(m_transform));
    //~ glutPostRedisplay();
}

void free_resources() {
    glDeleteProgram(program_text);
    //~ glDeleteProgram(program_bg);
    glDeleteProgram(program_cube);
    glDeleteBuffers(1, &vbo_coord);
    glDeleteBuffers(1, &vbo_tex_coord);
    glDeleteBuffers(1, &vbo_vertices);
    glDeleteBuffers(1, &vbo_vertices_color);
    glDeleteBuffers(1, &ibo_cube_elements);
}

//~ int main(int argc, char *argv[]) {
    
    
    //~ glutInit(&argc, argv);
    //~ glutInitContextVersion(2,0);
    //glutInitDisplayMode(GLUT_RGB);
    //~ glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);    
    //glutInitWindowSize(640, 480);
    //~ glutInitWindowSize(screen_width, screen_height);
    //~ glutCreateWindow("Basic Text");

    //~ if (argc > 1)
        //~ inputText=argv[1];
    //~ else{
        
        //~ std::cout << "Enter text" << std::endl;
        //~ std::string strtmp;
        //~ std::cin >> strtmp;
        //~ inputText = strtmp.c_str();
    //~ }
    //~ if (argc > 2)
        //~ fontSize=atoi(argv[2]);
    //~ else
        //~ fontSize=48;
        
    //~ if (argc > 3)
        //~ r = atof(argv[3]);
    //~ else
        //~ r = 1;
    //~ if (argc > 4)
        //~ g = atof(argv[4]);
    //~ else
        //~ g = 1;
    //~ if (argc > 5)
        //~ b = atof(argv[5]);
        
    //~ else
        //~ b = 1;

    //~ std::cout << r << std::endl;
    //~ std::cout << g << std::endl;
    //~ std::cout << b << std::endl;
    
    //~ if (argc > 6)
        //~ fontfilename = argv[6];
    //~ else
        //~ fontfilename = "FreeSans.ttf";

    //~ GLenum glew_status = glewInit();
    
    //~ if (GLEW_OK != glew_status) {
        //~ fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
        //~ return 1;
    //~ }
    
    //~ if (!GLEW_VERSION_2_0) {
        //~ fprintf(stderr, "No support for OpenGL 2.0 found\n");
        //~ return 1;
    //~ }
    
    //~ if (init_font()  && init_program()) {
        //init_background(inputText,0,0);
        //~ init_color(r,g,b);
        //~ init_cube(inputText,0,0,0);
        
        //~ glutDisplayFunc(display);
        //~ glutIdleFunc(onIdle);
        //~ glEnable(GL_BLEND);
        //~ glEnable(GL_DEPTH_TEST);;
        //~ glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //~ glutMainLoop();

    //~ }
    
    //~ free_resources();
    //~ return 0;
//~ }
