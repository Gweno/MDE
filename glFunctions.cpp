//------------- glFunctions.cpp-----------

#ifndef GLFUNCTIONS_CPP
#define GLFUNCTIONS_CPP

#define GLUT_DISABLE_ATEXIT_HACK 
//#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <string.h>
#include <string>
#include <vector>

//~ #include "Entity.h"
#include "glFunction.h"
//~ #include "DataFile.h"

// Include GLM
#include <glm/glm.hpp>

//~ #include "../include/Leap.h"

    
//~ using namespace std;

//Init Static member from glDisplay

int array_size;
char keyChoice;
bool text_input_on=false;
bool cameraMoveEnable=false;
bool moveEnable=false;
bool displayMenu=false;
bool mouse=false;
bool mouse_button_down=false;
bool mouseMove = false;
bool keyboard=false;
bool grabHand = false;
bool grabbed = false;
std::string input_text;

//int maxCol = 10;
char storeNameGl[10][24];

//~ std::vector<double> Xpos;
//~ std::vector<double> Ypos;
//~ std::vector<double> Zpos;
//~ std::vector<float> fRed;
//~ std::vector<float> fGreen;
//~ std::vector<float> fBlue;
//~ std::vector<float> fAlpha;
//~ std::vector<int> structureEntity;             //MEMBER0: flag 0 if direct value; flag 1 if structure
//~ std::vector<unsigned int>  typeEntity;          //MEMBER1: the type of the entity
//~ std::vector<unsigned int>  nbChildren;          //MEMBER2: number of dependants
//~ std::vector<unsigned int> masterEntity;        //MEMBER3: address of the Entity from which this Entity depends
//~ std::vector<int> layerEntity;                  //MEMBER4: Layer or level of this Entity
//~ std::vector<double> valueEntity;               //MEMBER5: The basic value of Entity is of type 'double' for now
std::vector<std::string> text;
std::vector<double> hand_grab(3);

// init static memebrs from other classes:

// from DataFile:
//~ bool dataFile::load_File = false;

//std::vector<string> vector_swap_glTextEntity;


/*
TVector* ColEntity::ArrayVel;                     //holds velocity of Entities
TVector* ColEntity::ArrayPos;                     //position of Entities
TVector* ColEntity::OldPos; 
*/

// init for  lighting
bool LightEnabled = true;

GLfloat MatSpec[] = {1.0,1.0,1.0,1.0};
GLfloat MatShininess[] = {128.0};
GLfloat LightPos[] = {-1.0,1.0,0.0,0.0};

// init for camera
float x_pos = 0, y_pos = 0, z_pos = 0, x_rot = 0, y_rot = 0, angle=0.0;
//~ float xpos_k = 0, ypos_k = 0, zpos_k = 0, xrot_k = 0, yrot_k = 0;

float x_view=0, y_view=0;

//float xpos = 0, ypos = 0, zpos = 0, xrot = 0, yrot = 0, angle=0.0;
//~ float xpos2 = 0, ypos2 = 0, zpos2 = 0, xrot2 = 0, yrot2 = 0, angle2=0.0;
float xrot2 = 0, yrot2 = 0, angle2=0.0;
GLdouble xpos2 = 0, ypos2 = 0, zpos2 = 0;

// Variable definition for the grab handle
//~ float x_grab = 0, y_grab = 0, z_grab = 5;
GLdouble x_grab, y_grab, z_grab = 0.0;
GLfloat fRed_grab = 0.0f;
GLfloat fGreen_grab = 0.0f;
GLfloat fBlue_grab = 1.0f;
GLfloat fAlpha_grab = 0.5f;
//~ GLdouble radius_grab = 0.05;    // Radius: The radius of the sphere. 
GLint slices_grab = 6;          // Slices: The number of subdivisions 
                // around the Z axis (similar to lines of longitude). 
GLint stacks_grab = 6;  // Stacks: The number of subdivisions along 
                        //the Z axis (similar to lines of latitude). 

GLdouble obj_winX;
GLdouble obj_winY;
GLdouble obj_winZ;
 
GLdouble radius_grab = 10.0;
 
float lastx, lasty;
float lastx2, lasty2;

float xrotrad = 0, yrotrad = 0;

float red=1, green=0, blue=0;

// init for iterator for Moving Entity
unsigned int index=0;

const unsigned char tmp[10] = "Entity";

//init the char array for text display:
char gMouseXLoc[20];
char gMouseYLoc[20];
char PrintAll[20][100];
float x_text=1;
float y_text=1;
float z_text=1;

//Entity newToto;

GLfloat matrix[16];

//Init all method from glDisplay including constructors, destructors, static and non-static methodes

// --------init functions--------

void drawAxes(GLdouble length)
{
	  // Draw a red x-axis, a green y-axis, and a blue z-axis.  Each of the
	  // axes are ten units long.
	  glBegin(GL_LINES);
	    glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(length, 0, 0);
	    glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0, length, 0);
	    glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, length);
	  glEnd();

	  glFlush();
	}

void vertex_Lines()
{

	glBegin(GL_LINE_STRIP);
	glColor3f(1, 0, 0);
	for (int i = 0; i < int(Xpos.size()); i++)
	    {
		glVertex3d(Xpos.at(i), Ypos.at(i),Zpos.at(i));
	    }
	glVertex3d(Xpos.at(0), Ypos.at(0),Zpos.at(0));
	glEnd();
	glFlush();
	}

void update(){

	Xpos.at(index)=double(xpos2);
	Ypos.at(index)=double(ypos2);
	Zpos.at(index)=double(zpos2);

}

void draw3DCursor(){

    glPushMatrix();
    //~ glRotatef(-x_rot,1.0,0.0,0.0);  //rotate our camera on the x-axis (left and right)
    //~ glRotatef(-y_rot,0.0,1.0,0.0);  //rotate our camera on the y-axis (up and down)
    glTranslated(-x_pos,-y_pos,-z_pos); //translate the screen to the position of our camera
    glRotatef(-y_rot,0.0,1.0,0.0);  //rotate our camera on the y-axis (up and down)
    glRotatef(-x_rot,1.0,0.0,0.0);  //rotate our camera on the x-axis (left and right)
    glTranslated(x_grab,y_grab,z_grab); //translate the screen to the grabition of our camera
//~ std::cout << "in draw " << std::endl;
//~ cout<<x_grab<<" "<<"Printed out variable x_grab"<<endl;
//~ cout<<y_grab<<" "<<"Printed out variable y_grab"<<endl;
//~ cout<<z_grab<<" "<<"Printed out variable z_grab"<<endl;
    glEnable (GL_BLEND);
    glEnable(GL_COLOR_MATERIAL);
//    glColor4f(red, green, blue, 1.0f);
    //~ glColor4f(0.0, 0.0, 1.0, alpha_grab);
    glColor4f(fRed_grab, fGreen_grab, fBlue_grab, fAlpha_grab);
    //~ glutSolidSphere (0.02,8,8);
    //~ glutSolidSphere (0.05,8,8);
    //~ glutWireSphere (0.05,8,8);
    glutWireSphere (radius_grab,slices_grab,stacks_grab);
    
    glPopMatrix();
	}

// pass object ob from class entity as paramameter to the function
void draw(){

//double XposCumul;
    
for (int i = 0; i < int(Xpos.size()); i++)
    {

	glPushMatrix();
    glTranslatef (Xpos.at(i), Ypos.at(i),Zpos.at(i));
    glEnable (GL_BLEND);
    glEnable(GL_COLOR_MATERIAL);
//    glColor4f(red, green, blue, 1.0f);
    glColor4f(fRed.at(i), fGreen.at(i), fBlue.at(i), fAlpha.at(i));
    glutSolidSphere (0.3,20,20);


/*

GLint viewport[4];
GLdouble modelview[16];
GLdouble projection[16];
glGetIntegerv(GL_VIEWPORT, viewport);
glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
glGetDoublev(GL_PROJECTION_MATRIX, projection);

	std::cout << "MODELVIEW0: " << i << std::endl;
	cout << " " << modelview[0] << " " << modelview[4] << " " << modelview[8] <<  " " << modelview[12] <<endl;
	cout << " " << modelview[1] << " " << modelview[5] << " " << modelview[9] <<  " " << modelview[13] <<endl;
	cout << " " << modelview[2] << " " << modelview[6] << " " << modelview[10] <<  " " << modelview[14] <<endl;
	cout << " " << modelview[3] << " " << modelview[7] << " " << modelview[11] <<  " " << modelview[15] <<endl;
	cout << endl;

GLdouble coordE[4];
coordE[0]=Xpos.at(i);
coordE[1]=Ypos.at(i);
coordE[2]=Zpos.at(i);
coordE[3]=1.0;

GLdouble oppIdentity[16];
oppIdentity[0]=-1.0;
oppIdentity[1]= 0;
oppIdentity[2]= 0;
oppIdentity[3]= 0;

oppIdentity[4]=0;
oppIdentity[5]=-1.0;
oppIdentity[6]= 0;
oppIdentity[7]= 0;

oppIdentity[8]=0;
oppIdentity[9]=0;
oppIdentity[10]=-1.0;
oppIdentity[11]=0;

oppIdentity[12]=1.0;
oppIdentity[13]=1.0;
oppIdentity[12]=1.0;
oppIdentity[13]=1.0;

glMultMatrixd(oppIdentity);
glMultMatrixd(coordE);

		//~ std::cout << "x_rot(1): " << x_rot << std::endl;
		//~ std::cout << "y_rot(1): " << y_rot << std::endl;

	//~ std::cout << "VIEWPORT:" << std::endl;
//~ for (int j=0; j<4;j++){
	//~ cout << " " << viewport[j];
//~ }
	//~ cout << endl;



	std::cout << "MODELVIEW1: " << i << std::endl;
	cout << " " << modelview[0] << " " << modelview[4] << " " << modelview[8] <<  " " << modelview[12] <<endl;
	cout << " " << modelview[1] << " " << modelview[5] << " " << modelview[9] <<  " " << modelview[13] <<endl;
	cout << " " << modelview[2] << " " << modelview[6] << " " << modelview[10] <<  " " << modelview[14] <<endl;
	cout << " " << modelview[3] << " " << modelview[7] << " " << modelview[11] <<  " " << modelview[15] <<endl;
	cout << endl;


	//~ std::cout << "PROJECTION: " << i << std::endl;
	//~ cout << " " << projection[0] << " " << projection[4] << " " << projection[8] <<  " " << projection[12] <<endl;
	//~ cout << " " << projection[1] << " " << projection[5] << " " << projection[9] <<  " " << projection[13] <<endl;
	//~ cout << " " << projection[2] << " " << projection[6] << " " << projection[10] <<  " " << projection[14] <<endl;
	//~ cout << " " << projection[3] << " " << projection[7] << " " << projection[11] <<  " " << projection[15] <<endl;
	//~ cout << endl;
*/

	


    glRasterPos2i(-2, -2);
    glColor4f(0, 1, 0, 1.0f);


    sprintf(PrintAll[0], "index:%i",i);
    sprintf(PrintAll[1], "x:%2.3f",Xpos.at(i));
    sprintf(PrintAll[2], "y:%2.3f",Ypos.at(i));
    sprintf(PrintAll[3], "z:%2.3f",Zpos.at(i));
    drawString(GLUT_BITMAP_HELVETICA_18, PrintAll[0], x_text, y_text, z_text);
    drawString(GLUT_BITMAP_HELVETICA_18, PrintAll[1], x_text, y_text-0.5, z_text);
    drawString(GLUT_BITMAP_HELVETICA_18, PrintAll[2],x_text, y_text-1, z_text);
    drawString(GLUT_BITMAP_HELVETICA_18, PrintAll[3],x_text, y_text-1.5, z_text);

	if (text.size()>0) {
		sprintf (PrintAll[4], "text:%s",text.at(i).c_str());
	    drawString(GLUT_BITMAP_HELVETICA_18, PrintAll[4], x_text, y_text-2.0, z_text);
	}
	
    glPopMatrix();
}

}


void stillObject(void){
	/// testing fixed object
	glPushMatrix();

    glTranslated(-x_pos,-y_pos,-z_pos); //translate the screen to the position of our camera
    glRotatef(-y_rot,0.0,1.0,0.0);  //rotate our camera on the y-axis (up and down)
    glRotatef(-x_rot,1.0,0.0,0.0);  //rotate our camera on the x-axis (left and right)

	glTranslated(-5.0, 3.0, 1.0); //translate the screen to the position
	glEnable (GL_BLEND);
	glEnable(GL_COLOR_MATERIAL);
	glColor4f(1.0, 1.0, 1.0f, 1.0f);
	glutSolidCube (0.3);
	sprintf(PrintAll[6], "Rotation x_rot:%2.3f y_rot:%2.3f",x_rot,y_rot);
	sprintf(PrintAll[7], "Position x_pos:%2.3f x_pos:%2.3f z_pos:%2.3f",x_pos,y_pos,z_pos);
	//~ sprintf(ePrintAll[8], "KeyBoard xrot:%2.3f yrot:%2.3f",xrot_k,yrot_k);
	//~ sprintf(ePrintAll[9], "KeyBoard xpos:%2.3f ypos:%2.3f ypos:%2.3f",xpos_k,ypos_k,zpos_k);

	drawString(GLUT_BITMAP_HELVETICA_12, PrintAll[6],x_text, y_text-0.25, z_text);
	drawString(GLUT_BITMAP_HELVETICA_12, PrintAll[7],x_text, y_text-0.5, z_text);
	//~ drawString(GLUT_BITMAP_HELVETICA_12, PrintAll[8],x_text, y_text-0.75, z_text);
	//~ drawString(GLUT_BITMAP_HELVETICA_12, PrintAll[9],x_text, y_text-1, z_text);

	sprintf(PrintAll[10], "cameraMoveEnable :%d",cameraMoveEnable);
	sprintf(PrintAll[11], "mouse :%d",mouse);
	sprintf(PrintAll[12], "keyboard :%d",keyboard);
	sprintf(PrintAll[13], "displayMenu :%d",displayMenu);
	sprintf(PrintAll[14], "text_input_on :%d",text_input_on);
	sprintf(PrintAll[15], "moveEnable :%d",moveEnable);

	drawString(GLUT_BITMAP_HELVETICA_12, PrintAll[10],x_text, y_text-1.5, z_text);
	drawString(GLUT_BITMAP_HELVETICA_12, PrintAll[11],x_text, y_text-1.75, z_text);
	drawString(GLUT_BITMAP_HELVETICA_12, PrintAll[12],x_text, y_text-2.0, z_text);
	drawString(GLUT_BITMAP_HELVETICA_12, PrintAll[13],x_text, y_text-2.25, z_text);
	drawString(GLUT_BITMAP_HELVETICA_12, PrintAll[14],x_text, y_text-2.5, z_text);
	drawString(GLUT_BITMAP_HELVETICA_12, PrintAll[15],x_text, y_text-3.0, z_text);

	sprintf(PrintAll[16], "Input Text :");
	drawString(GLUT_BITMAP_HELVETICA_18, PrintAll[16],x_text, y_text-4, z_text);
	drawString(GLUT_BITMAP_HELVETICA_18, PrintAll[5],x_text, y_text-4.5, z_text);

	sprintf(PrintAll[17], "Entity selected : %i",index);
	drawString(GLUT_BITMAP_HELVETICA_18, PrintAll[17],x_text, y_text-5, z_text);



	glPopMatrix();
}


void console(void) {

	glPushMatrix();
    drawString(GLUT_BITMAP_HELVETICA_18, PrintAll[2],0,0,0);
    glGetFloatv (GL_MODELVIEW_MATRIX, matrix);
	glPopMatrix();

}


void display(void){
    glClearDepth (1);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glLightfv(GL_LIGHT0, GL_POSITION, LightPos);
    glTranslatef (0,0,-10);
    camera();
    drawAxes(10);
    //~ draw3DCursor();
    //~ load();
    update();
    draw();
    vertex_Lines();
    stillObject();
    //~ projectEntity();
    //~ grabEntity();
    glutSwapBuffers();
}

void reshape(int w, int h){
    glViewport (0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode (GL_MODELVIEW);
}


void initDisplay(){
    glEnable(GL_DEPTH_TEST);
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
}


void camera (void) {
	glRotatef(x_rot,1.0,0.0,0.0);  //rotate our camera on the x-axis (left and right)
	glRotatef(y_rot,0.0,1.0,0.0);  //rotate our camera on the y-axis (up and down)
	glTranslated(x_pos,y_pos,z_pos); //translate the screen to the position of our camera
	glFlush();
	}
	
void camera2 (void) {
	gluLookAt(-x_pos, -y_pos, -z_pos,0,0,0,0,1,0);
}

void mouseMovement(int x, int y) {
	mouse_button_down = false;
	mouseMove = true;
	process_move(x,y);
    //~ grabEntity(x,y);
	}
	
void mouseMovement_Rclick(int x, int y){
	mouse_button_down = true;
	mouseMove = true;
	process_move(x,y);
	grabEntity(x,y);
    }

void projectEntity()
{
    int it_yz;
    
    for(std::vector<double>::iterator it_x = Xpos.begin(); it_x != Xpos.end(); ++it_x)
    {
        it_yz= it_x-Xpos.begin();
        project((*it_x),Ypos.at(it_yz),Zpos.at(it_yz));
        std::cout << "in projectEntity() for index:" << it_yz << std::endl;
        cout<<obj_winX<<" "<<"Printed out variable obj_winX"<<endl;
        cout<<obj_winY<<" "<<"Printed out variable obj_winY"<<endl;
        cout<<obj_winZ<<" "<<"Printed out variable obj_winZ"<<endl;
        
    }
}

void grabEntity(int x, int y)
{
    if (!grabHand) return;
    else
    {
    int it_yz;
    glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    for(std::vector<double>::iterator it_x = Xpos.begin(); it_x != Xpos.end(); ++it_x)
    {
        it_yz= it_x-Xpos.begin();
        project((*it_x),Ypos.at(it_yz),Zpos.at(it_yz));
        
        if (((x > int(obj_winX) - radius_grab) && (x < int(obj_winX) + radius_grab)) &&
            ((y > int(500-obj_winY) - radius_grab) && (y < int(500-obj_winY) + radius_grab)))
        {
            std::cout << "grabbed index:" << it_yz << std::endl;
            grabbed = true;
            xpos2 = (*it_x);
            ypos2 = Ypos.at(it_yz);
            zpos2 = Zpos.at(it_yz);
            index = it_yz;
            glutSetCursor(GLUT_CURSOR_INFO);
            return;
        }
    }
    }
}

void process_move(int x, int y){
    unsigned char key;
    
    if ((cameraMoveEnable) and mouseMove and !(mouse_button_down)){
        mouse = true;
        keyboard = false;
        float diffx=x-lastx; //check the difference between the current x and the last x position
        float diffy=y-lasty; //check the difference between the current y and the last y position
        lastx=x; //set lastx to the current x position
        lasty=y; //set lasty to the current y position
        x_rot += (float) diffy; //set the xrot to xrot with the addition of the difference in the y position
        y_rot += (float) diffx;    //set the xrot to yrot with the addition of the difference in the x position
        if (x_rot >360) x_rot -= 360;
        if (x_rot < -360) x_rot += 360;
        if (y_rot >360) y_rot -= 360;
        if (y_rot < -360) y_rot += 360;
        //~ std::cout<<"move" << std::endl;
    }
    
    if ((cameraMoveEnable) and mouseMove and mouse_button_down){
        mouse = true;
        keyboard = false;
        float diffx=x-lastx; //check the difference between the current x and the last x position
        float diffy=y-lasty; //check the difference between the current y and the last y position
        lastx=x; //set lastx to the current x position
        lasty=y; //set lasty to the current y position
        if(diffx<0) x_pos += ((float) diffx-250)/500;
        if(diffx>0) x_pos += (250-(float) diffx)/500;
        if(diffy<0) y_pos += (250-(float) diffy)/500;
        if(diffy>0) y_pos += ((float) diffy-250)/500;
        //~ std::cout<<"button_down" << std::endl;
    }
    
        //~ if ((moveEnable) and mouseMove and !(mouse_button_down)){
        if (moveEnable and mouseMove and mouse_button_down){
        xrotrad = (x_rot / 180 * 3.141592654f);
        yrotrad = (y_rot / 180 * 3.141592654f);
    
    
        float diffx=(x-lastx); //check the difference between the current x and the last x position
        float diffy=(y-lasty); //check the difference between the current y and the last y position
        lastx=x; //set lastx to the current x position
        lasty=y; //set lasty to the current y position
        std::cout << "diffx: " << diffx << std::endl;
        std::cout << "lastx: " << lastx << std::endl;
        std::cout << "x:     " << x << std::endl;

        if(diffx<0) x_view = (((float) diffx-250)/500);
        if(diffx>0) x_view = ((250-(float) diffx)/500);
        if(diffy<0) y_view = ((250-(float) diffy)/500);
        if(diffy>0) y_view = (((float) diffy-250)/500);

        unProject2(x,y);
    }

    if (cameraMoveEnable and keyboard){
        mouse = false;
        keyboard = true;
        key = keyChoice;
        
            switch(key)
            {
            case 'q':
                x_rot += 1;
                if (x_rot >360) x_rot -= 360;
            break;
    
            case 'z':
                x_rot -= 1;
                if (x_rot < -360) x_rot += 360;
            break;
    
            case 'e':
                y_rot += 1;
                if (y_rot >360) y_rot -= 360;
            break;
    
            case 'r':
                y_rot -= 1;
                if (y_rot < -360) y_rot += 360;
            break;
    
    
            case 'w':
                yrotrad = (y_rot / 180 * 3.141592654f);
                xrotrad = (x_rot / 180 * 3.141592654f);
                x_pos += float(sin(yrotrad)) ;
                z_pos -= float(cos(yrotrad)) ;
                y_pos -= float(sin(xrotrad)) ;
            break;
    
            case 's':
                yrotrad = (y_rot / 180 * 3.141592654f);
                xrotrad = (x_rot / 180 * 3.141592654f);
                x_pos -= float(sin(yrotrad));
                z_pos += float(cos(yrotrad)) ;
                y_pos += float(sin(xrotrad));
                break;
    
            case 'd':
                yrotrad = (y_rot / 180 * 3.141592654f);
                x_pos += float(cos(yrotrad)) * 0.2;
                z_pos += float(sin(yrotrad)) * 0.2;
                break;
    
            case'a':
                yrotrad = (y_rot / 180 * 3.141592654f);
                x_pos -= float(cos(yrotrad)) * 0.2;
                z_pos -= float(sin(yrotrad)) * 0.2;
                break;

            case 'l':
                LightEnabled = !LightEnabled;
                if (LightEnabled) glEnable(GL_LIGHTING);
                else glDisable(GL_LIGHTING);
            break;
        }

    }

    if (grabHand and mouseMove and mouse_button_down){
        mouse = true;
        keyboard = false;
        glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    }

}

void keyDown(unsigned char key, int x, int y)
//Note: because there is an Idle-func, we don't have to call Display here
{
    //~ float xrotrad, yrotrad;
keyChoice=key;

if (!text_input_on){

    switch(key)
    {
    case 'c':
        displayMenu = false;  //force switch;
        cameraMoveEnable = !cameraMoveEnable;//change switch position;
        moveEnable = false;
        text_input_on = false;
        grabHand = false;
        glutSetCursor(GLUT_CURSOR_INHERIT);
    break;

    case 'y':
        displayMenu = false; //force switch;
        cameraMoveEnable = false; //force switch;
        moveEnable = !moveEnable;   //change switch position;
        text_input_on = false;
        grabHand = false;
        glutSetCursor(GLUT_CURSOR_INFO);
        warpCursorFromEntity(Xpos.at(index),Ypos.at(index),Zpos.at(index));
    break;

    case 'm':
        displayMenu = !displayMenu; //change switch position;
        cameraMoveEnable = false; //force switch;
        moveEnable = false;	//force switch;
        text_input_on = false;
        grabHand = false;
        glutSetCursor(GLUT_CURSOR_INHERIT);
    break;

    case 'g':
        displayMenu = !displayMenu; //change switch position;
        cameraMoveEnable = false; //force switch;
        moveEnable = false;	//force switch;
        text_input_on = false;
        grabHand = true;
        //~ glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    break;

    case 27:	//ESC to exit program
        exit(0);
    break;


    default:
    break;
    }
    if (cameraMoveEnable) {
        
        mouse = false;
        keyboard = true;
        //~ std::cout << "key2 x y"  << x << "," << y << std::endl;
        mouseMove = false;
        mouse_button_down = false;
        process_move(x,y);
    }
            
    if (moveEnable) {
        switch (key){
            case',':
                xpos2 -= 1;
            break;

            case'.':
                xpos2 += 1;
            break;

            case'k':
                ypos2 -= 1;
            break;

            case'o':
                ypos2 += 1;
            break;

            case'/':
                zpos2 -= 1;
            break;

            case';':
                zpos2 += 1;
            break;

            case'-':
                if (index!=0) index-=1;
                    xpos2=Xpos.at(index);
                    ypos2=Ypos.at(index);
                    zpos2=Zpos.at(index);
                    for (unsigned int i = 0; i < (unsigned int)(Xpos.size()); i++)
                        {
                        if(i!=index){
                            fRed.at(i)=1.0;
                            fBlue.at(i)=0.0;
                            fAlpha.at(i)=0.0;
                        }
                        }
                    fRed.at(index)=0.0;
                    fBlue.at(index)=1.0;
                    fAlpha.at(index)=1.0;
                    //~ warpCursorFromEntity(xpos2,ypos2,zpos2);
                    warpCursorFromEntity(Xpos.at(index),Ypos.at(index),Zpos.at(index));
                    break;

            case'=':
                    if (index<Xpos.size()-1) index+=1;
                    xpos2=Xpos.at(index);
                    ypos2=Ypos.at(index);
                    zpos2=Zpos.at(index);
                    for (unsigned int i = 0; i < (unsigned int)(Xpos.size()); i++)
                        {
                        if(i!=index){
                    fRed.at(i)=1.0;
                    fBlue.at(i)=0.0;
                    fAlpha.at(i)=0.0;
                        }
                        }
                    fRed.at(index)=0.0;
                    fBlue.at(index)=1.0;
                    fAlpha.at(index)=1.0;
                    warpCursorFromEntity(xpos2,ypos2,zpos2);
                    break;
        }
    }
    
    if (displayMenu){

        switch (key)
        {
            case'r':
        //      Return to main menu
            glutLeaveMainLoop();
            break;

// !!! following section need re-work from here

            case 'n':                                   //new
                newGlObject();
                index=0;
                text_input_on = true;
                break;

            case 'v':                                   //copy
                copy();
                if ( Xpos.size()!=0 ) {
                        index=Xpos.size()-1;
                    }
                else {
                            index=0;
                    };
                text_input_on = true;
            break;

            case 'b':                                   //delete
                deleteObject();
                index=0;
            break;

            case '1':
            case '2':
            case 't':                                   //to load file just turn on bool loadFile
                dataFile::load_File = true;
            break;

        }
    }

// ... to here: used to have method inherited from Entity -> not anymore as independant class now

}	// end of 'when text_input_on is off'

else {			// when text_input is on

    if (key!=char(13)){
        input_text.push_back(key);
//	    	sprintf (ePrintAll[0], "test");
        sprintf (PrintAll[5], "%s",input_text.c_str());
    }
    else{
        text_input_on = false;
//	    	copyEntity();
        text.pop_back();
        text.push_back(input_text);
        std::cout << input_text << std::endl;
        input_text="";
    }

}	// end of 'when text_input is on' (else)
}

// Test to move Entity 1 with the mouse
void movement(int x, int y) {
if (moveEnable) {
float diffx=x-lastx; //check the difference between the current x and the last x position
float diffy=y-lasty; //check the difference between the current y and the last y position
lastx=x; //set lastx to the current x position
lasty=y; //set lasty to the current y position
if(diffx<0) xpos2 += ((float) diffx-250)/500;
if(diffx>0) xpos2 += (250-(float) diffx)/500;
if(diffy<0) ypos2 += (250-(float) diffy)/500;
if(diffy>0) ypos2 += ((float) diffy-250)/500;
//    double xloc = x;
//    sprintf(gMouseXLoc, "%2.15f", xloc);

//    glTranslated(-xpos,-ypos,-zpos); //translate the screen to the position of our camera
}
}

// need rework
void::coordinate(Entity * objEntity){

Xpos.clear();
Ypos.clear();
Zpos.clear();
fRed.clear();
fGreen.clear();
fBlue.clear();
fAlpha.clear();
text.clear();

            for (int i=0;i<int(objEntity->valueEntity.size());i++){
                Xpos.push_back(objEntity->valueEntity.at(i)*1.0);
                Ypos.push_back(objEntity->layerEntity.at(i)*1.0);
                Zpos.push_back(objEntity->masterEntity.at(i)*1.0);
                fRed.push_back(1.0);
                fGreen.push_back(0.0);
                fBlue.push_back(0.0);
                fAlpha.push_back(1.0);
                text.push_back(objEntity->textEntity.at(i));
        }
}


//~ void::glCoordinateToglEntity_All(){
//~ valueEntity=Xpos;
//~ for (unsigned int i=0; i<valueEntity.size();i++){
    //~ layerEntity.at(i)=int(Ypos.at(i));
    //~ masterEntity.at(i)=int(Zpos.at(i));
    //~ typeEntity.at(i)=0;
    //~ nbChildren.at(i)=0;
    //~ masterEntity.at(i)=0;

//~ }

//~ }

//~ void::glCoordinateToglEntity(unsigned int index){
//~ valueEntity.push_back(Xpos.at(index));
//~ layerEntity.push_back(int(Ypos.at(index)));
//~ masterEntity.push_back(int(Zpos.at(index)));
//~ typeEntity.push_back(index);
//~ nbChildren.push_back(index);
//~ masterEntity.push_back(index);

//~ }


void load(Entity *objEntity) {
if ( (!text_input_on) && displayMenu) {

//	if ( (!text_input_on) && displayMenu && (keyChoice=='t') ){
    if (keyChoice=='t'){
    dataFile dataGlEntity(0);
    //~ Entity newTiti;
    resetIndex();
    dataGlEntity.loadFile('t',objEntity);
    //~ dataGlEntity.loadFile('t',toto);
    coordinate(objEntity);
    //~ dataToGl(dataGlEntity);
    //~ coordinate_glDisplay();
    keyChoice='\0';
}
//	if( (!text_input_on)  && displayMenu && keyChoice=='1'){
    if(keyChoice=='1'){
    dataFile dataGlEntity(0);
    Entity newTiti;
    //~ dataGlEntity.loadFile('1', newTiti);
    resetIndex();
    //~ dataToGl(dataGlEntity);
    //~ coordinate_glDisplay();
    keyChoice='\0';
}
//	if( (!text_input_on)  && displayMenu && keyChoice=='2'){
if(keyChoice=='2'){
    resetIndex();
    dataFile dataGlEntity(0);
    Entity newTiti;
    //~ dataGlEntity.loadFile('2',newTiti);
    //~ dataToGl(dataGlEntity);
    //~ coordinate_glDisplay();
    keyChoice='\0';
}

//	if ( (!text_input_on) && displayMenu && (keyChoice=='s') ){
if(keyChoice=='s'){
    dataFile dataglDisplay(0);
    //~ glCoordinateToglDisplay_All();
//		dataglDisplay.saveFileFromGL("test01.csv");
    //~ dataglDisplay.saveFileFromGL("test01");
    keyChoice='\0';
}

}
}

void::resetIndex(){
index=0;
}

/* test */


void::drawString (void * font, char *s, float x, float y, float z){
 unsigned int i;
//     glTranslatef (X, Y, Z);
 glRasterPos3f(x, y, z);

 for (i = 0; i < strlen (s); i++)
      glutBitmapCharacter (font, s[i]);
}

void::mouseMotionForGlut(int x, int y){
 double xloc = x;
 double yloc = y;
 sprintf(gMouseXLoc, "x:%2.3f", xloc);
 sprintf(gMouseYLoc, "y:%2.3f", yloc);
}


void::newGlObject(){
Xpos.push_back(0);
Ypos.push_back(0);
Zpos.push_back(0);
fRed.push_back(1.0);
fGreen.push_back(0.0);
fBlue.push_back(0.0);
fAlpha.push_back(1.0);
text.push_back("Enter Text");

}
void::copy(){
Xpos.push_back(Xpos.at(index));
Ypos.push_back(Ypos.at(index));
Zpos.push_back(Zpos.at(index));
fRed.push_back(1.0);
fGreen.push_back(0.0);
fBlue.push_back(0.0);
fAlpha.push_back(1.0);
text.push_back("Enter Text");
}

void::deleteObject(){
unsigned int iterator =index;
if (Xpos.size()>1){

Xpos.erase(Xpos.begin() + iterator);
Ypos.erase(Ypos.begin() + iterator);
Zpos.erase(Zpos.begin() + iterator);
fRed.erase(fRed.begin() + iterator);
fGreen.erase(fGreen.begin() + iterator);
fBlue.erase(fBlue.begin() + iterator);
fAlpha.erase(fAlpha.begin() + iterator);

text.erase(text.begin() + iterator);
}

}

//~ void dataToGl (dataFile objData){
    //~ structureEntity=objData.structureEntity;
    //~ typeEntity=objData.typeEntity;
    //~ nbChildren=objData.nbChildren;
    //~ masterEntity=objData.masterEntity;
    //~ layerEntity=objData.layerEntity;
    //~ valueEntity=objData.valueEntity;
    //~ text=objData.storeDataText;
//~ }


void unProject(int x_cursor, int y_cursor){

GLint viewport[4];
GLdouble modelview[16];
GLdouble projection[16];
GLfloat winX,winY;
glGetIntegerv(GL_VIEWPORT, viewport);
glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
glGetDoublev(GL_PROJECTION_MATRIX, projection);

// obtain the Z position (not world coordinates but in range 0 - 1)
GLfloat z_cursor;
winX = (float)x_cursor;
winY = (float)viewport[3]-(float)y_cursor;
z_cursor = 0.01;
// obtain the world coordinates
gluUnProject(winX, winY, z_cursor, modelview, projection, viewport, &x_grab, &y_grab, &z_grab);
}


void project(GLdouble objX,  GLdouble objY,  GLdouble objZ)
{

GLint viewport[4];
GLdouble modelview[16];
GLdouble projection[16];
GLfloat winX,winY;
glGetIntegerv(GL_VIEWPORT, viewport);
glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
glGetDoublev(GL_PROJECTION_MATRIX, projection);

gluProject(objX, objY, objZ,
            modelview, projection, viewport,
            &obj_winX,  &obj_winY,  &obj_winZ);
            
}

void warpCursorFromEntity(GLdouble objX, GLdouble objY, GLdouble objZ)
{
project(objX, objY, objZ);
std::cout << "obj_winX" << obj_winX <<std::endl;
std::cout << "obj_winY" << obj_winY <<std::endl;
glutWarpPointer(obj_winX, 500 - obj_winY);
}

void unProject2(int x_cursor, int y_cursor){

GLint viewport[4];
GLdouble modelview[16];
GLdouble projection[16];
GLfloat winX,winY;
glGetIntegerv(GL_VIEWPORT, viewport);
glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
glGetDoublev(GL_PROJECTION_MATRIX, projection);

// obtain the Z position (not world coordinates but in range 0 - 1)
GLfloat z_cursor;
winX = (float)x_cursor;
winY = (float)viewport[3]-(float)y_cursor;
//~ glReadPixels(winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z_cursor);
z_cursor = obj_winZ;
// obtain the world coordinates
gluUnProject(winX, winY, z_cursor, modelview, projection, viewport, &xpos2, &ypos2, &zpos2);
//~ std::cout << "in unProject " << std::endl;
//~ cout<<x_grab<<" "<<"Printed out variable x_grab"<<endl;
//~ cout<<y_grab<<" "<<"Printed out variable y_grab"<<endl;
//~ cout<<z_grab<<" "<<"Printed out variable z_grab"<<endl;
}


//~ void testLookat(){


//~ }


#endif /*glFunctions.cpp*/
