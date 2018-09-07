//----------------glFunctions.h----------------
#ifndef GLFUNCTIONS_H
#define GLFUNCTIONS_H

#include <GL/freeglut.h>
#include <vector>
#include "E.h"

void initDisplay();
void draw();
void update();
void display(void);
void reshape(int w, int h);
void keyDown(unsigned char key, int x, int y);
void mouseMovement(int x, int y);
void mouseMovement_Rclick(int x, int y);
void camera (void);
void camera2 (void);
void movement(int x, int y);
//~ void load(Entity * objEntity);
void load(E * objE);
void resetIndex();
void drawString (void * font, char *s, float x, float y, float z);
void mouseMotionForGlut(int x, int y);
void console();
void drawAxes(GLdouble length);
void vertex_Lines();
void stillObject();
void newGlObject();
void copy();
void deleteObject();
//~ void coordinate(Entity * objEntity);
//~ void coordinate(E * objE);
//~  void dataToGl(dataFile);
//~  void glCoordinateToGlEntity_All();
//~  void glCoordinateToGlEntity(unsigned int index);
void process_move(int x, int y);
void draw3DCursor();
void unProject(int x_cursor, int y_cursor);
void unProject2(int x_cursor, int y_cursor);
void project(GLdouble objX,  GLdouble objY,  GLdouble objZ);
void projectEntity();
void grabEntity(int x, int y);
void warpCursorFromEntity(GLdouble objX, GLdouble objY, GLdouble objZ);

void quads_to_triangles(std::vector< std::vector<float> > & vect_quads, 
                    std::vector< std::vector<float> > & vect_triangles);
void testVector_Display_2d
                    (std::vector< std::vector <float> > &testVector);
void testVector_Display
        (std::vector< std::vector< std::vector <float> > > &testVector);
void vect2vect
    (std::vector< std::vector< std::vector <float> > > & vCubeSouce);
void vect2vect_colors(
    std::vector< std::vector <float> > & vColorSouce);
void glTrianlges_cubeVec6 (
                        std::vector<std::vector<float> > & vect_colors,
            std::vector< std::vector < std::vector <float> > >& vect);
void quads_to_triangles(std::vector< std::vector<float> > & vect_quads,
                    std::vector< std::vector<float> > & vect_triangles);
#endif /*glFunctions.h*/
