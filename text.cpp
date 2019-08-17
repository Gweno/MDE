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

int screen_width=800, screen_height=600;

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
GLint attribute_v_color_box2;

GLint uniform_tex;
GLint uniform_color;
GLint uniform_bgcolor;
GLint uniform_m_transform;

GLuint vbo_coord;
GLuint vbo_tex_coord;
GLuint vbo_vertices;
GLuint vbo_vertices_box;
GLuint vbo_vertices_box2;
GLuint vbo_vertices_color;
GLuint vbo_vertices_color_box;
GLuint vbo_vertices_color_box2;

// for cube:
GLuint ibo_cube_elements;
GLuint ibo_box_elements;
GLuint ibo_box_elements2;

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
vertex2D padding ={0.0,0.0};

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
        //~ {x-horizontal_padding, -y-vertical_padding, z },
        //~ {x + w+horizontal_padding, -y-vertical_padding, z},
        //~ {x + w+horizontal_padding, -y + h+vertical_padding,z},
        //~ {x-horizontal_padding, -y + h+vertical_padding, z},
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
    
    //~ vertex_coord_box2.push_back({-2.5, 0, 0 });
    //~ vertex_coord_box2.push_back({-2.5, 0.5, 0});
    //~ vertex_coord_box2.push_back({-2, 0.5,0});
    //~ vertex_coord_box2.push_back({-2,0,0});
    //~ vertex_coord_box2.push_back({-2.5, 0, -0.5 });
    //~ vertex_coord_box2.push_back({-2.5, 0.5, -0.5});
    //~ vertex_coord_box2.push_back({-2, 0.5,-0.5});
    //~ vertex_coord_box2.push_back({-2,0,-0.5});

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
    //~ glBufferData(GL_ARRAY_BUFFER,sizeof vertex_coord_box2, vertex_coord_box2,GL_STATIC_DRAW);
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


    //~ vect_index.assign(box_elements2,box_elements2+36);   // assigning from array.

    for(GLushort i=0;i<20;i++){
        for ( auto it = box_elements2.begin(); it != box_elements2.end(); ++it ){
            vect_index.push_back((*it)+8*i);
        }
    }

  //~ for ( auto it = myarray.begin(); it != myarray.end(); ++it )
    //~ std::cout << ' ' << *it;

    glGenBuffers(1, &ibo_box_elements2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_box_elements2);
    //~ glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(box_elements2), box_elements2, GL_STATIC_DRAW);
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


void init_text(const char *text,float x, float y, float z, float h_padding, float v_padding) {
    
    inputText=text;
    text_coordinates = {x,y,z};
    padding = {h_padding,v_padding};

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
    glUseProgram(program_box);
    draw_box();
    glUseProgram(program_cube);
    draw_cube();
    glUseProgram(program_box);
    draw_box2();
    
    

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
    
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0, 0.0, -1.0));
    glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 5.0), glm::vec3(0.0, 0.0,-1.0), glm::vec3(0.0, 1.0, 0.0));//eye,center,up
    glm::mat4 projection = glm::perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 10.0f);
    
    glm::mat4 m_transform = projection * view * model * m_translate * anim2 * anim;
    //~ glm::mat4 m_transform = projection * view * model;
    
    // program_bg is for the colored cube
    glUseProgram(program_cube);
    glUniformMatrix4fv(uniform_m_transform, 1, GL_FALSE, glm::value_ptr(m_transform));
    
    
    // program_text is for the text
    glUseProgram(program_text);
    glUniformMatrix4fv(uniform_m_transform, 1, GL_FALSE, glm::value_ptr(m_transform));
    
    // program_box is for the text
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
    glDeleteBuffers(1, &vbo_vertices_color);
    glDeleteBuffers(1, &vbo_vertices_color_box);
    glDeleteBuffers(1, &vbo_vertices_color_box2);
    glDeleteBuffers(1, &ibo_cube_elements);
    glDeleteBuffers(1, &ibo_box_elements);
    glDeleteBuffers(1, &ibo_box_elements2);
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
