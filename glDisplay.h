//------------glDisplay.h--------------
#ifndef GLDISPLAY_H
#define GLDISPLAY_H

#include <GL/freeglut.h>
#include "Entity.h"
#include "DataFile.h"
#include <vector>



class glDisplay {
      public:
            glDisplay();//Declare constructor w/o parameters
            ~glDisplay();//Declare Destructor

            static void initDisplay();
            static void draw();
            static void update();
            static void display(void);
            static void reshape(int w, int h);
            static void keyDown(unsigned char key, int x, int y);
            static void mouseMovement(int x, int y);
            static void mouseMovement_Rclick(int x, int y);
            static void camera (void);
            static void camera2 (void);
            static void movement(int x, int y);
            static void load(Entity * objEntity);
            static void resetIndex();
            static void drawString (void * font, char *s, float x, float y, float z);
            static void mouseMotionForGlut(int x, int y);
            static void console();
            static void drawAxes(GLdouble length);
            static void vertex_Lines();
            static void stillObject();
            static void newGlObject();
            static void copy();
            static void deleteObject();
            static void coordinate(Entity * objEntity);
            //~ static void dataToGl(dataFile);
            //~ static void glCoordinateToGlEntity_All();
            //~ static void glCoordinateToGlEntity(unsigned int index);
            static void process_move(int x, int y);
            static void draw3DCursor();
            static void unProject(int x_cursor, int y_cursor);
            static void unProject2(int x_cursor, int y_cursor);
            static void project(GLdouble objX,  GLdouble objY,  GLdouble objZ);
            static void projectEntity();
            static void grabEntity(int x, int y);
            static void warpCursorFromEntity(GLdouble objX, GLdouble objY, GLdouble objZ);

      public:

            static int array_size;         // parameter for the size of the Dynamic Array
            static char keyChoice;
            static GLfloat MatSpec[];
            static GLfloat MatShininess[];
            static GLfloat LightPos[];
            static unsigned int index;

            //~ static std::vector<int> structureEntity;             //MEMBER0: flag 0 if direct value; flag 1 if structure
            //~ static std::vector<unsigned int>  typeEntity;          //MEMBER1: the type of the entity
            //~ static std::vector<unsigned int>  nbChildren;          //MEMBER2: number of dependants
            //~ static std::vector<unsigned int> masterEntity;        //MEMBER3: address of the Entity from which this Entity depends
         	//~ static std::vector<int> layerEntity;                  //MEMBER4: Layer or level of this Entity
         	//~ static std::vector<double> valueEntity;               //MEMBER5: The basic value of Entity is of type 'double' for now
         	static std::vector<std::string> text;
            
            static bool grabHand;
            static bool grabbed;



// 02/08/2012
// public members
//               public:

                static std::vector<double> Xpos;
                static std::vector<double> Ypos;
                static std::vector<double> Zpos;
                static std::vector<double> Xmov;
                static std::vector<double> Ymov;
                static std::vector<double> Zmov;
                static std::vector<double> Xacc;
                static std::vector<double> Yacc;
                static std::vector<double> Zacc;
                static std::vector<float> fRed;
                static std::vector<float> fGreen;
                static std::vector<float> fBlue;
                static std::vector<float> fAlpha;
//                static std::vector<std::string> text_Entity;
//                static std::vector<std::vector<unsigned int> > parent;
//                static std::vector<std::vector<unsigned int> > child;
//                static std::vector<std::vector<unsigned int> > sibling;



      };

#endif /* glDisplay.h */
