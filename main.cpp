// ------------------------main.cpp-------------------
#ifndef MAIN
#define MAIN

#define GLUT_DISABLE_ATEXIT_HACK
#define GLM_FORCE_RADIANS

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <stack>
#include "E.h"

#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/freeglut.h>

#include "glFunctions.h"
#include "text.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "./common/shader_utils.h"

// declarationfor text.cpp
int window_width=800, window_height=600;
const char *userText;
uint userFontSize;
GLfloat userRed,userGreen,userBlue;
//~ FT_Library ft;
//~ FT_Face face;
const char *userFontFilename;


// use of arguments for testing: '-e' execute main for new class E,
// no argument execute the normal main for Entity,
// "-gl" option is for the openGL testing
// "-t" execute the text GL display with argument text,font size, r,g,b,font name.

int main (int argc, char **argv)
{

    std::string choice;
    std::cout << "Have " << argc << " arguments:" << std::endl;
    for (int i = 0; i < argc; ++i) {
        std::cout << argv[i] << std::endl;
    }
    if (argc>1) choice= std::string(argv[1]);
    //~ if (argc==1){
    if (choice=="-gl")
    {
    
        E testEntity;

        GLfloat MatSpec[] = {1.0,1.0,1.0,1.0};
        GLfloat MatShininess[] = {128.0};

        std::vector< vector <float> > vertex;
        std::vector<float> coordinate;
        std::vector< std::vector< std::vector <float> > > vCube;
        std::vector<std::vector<float> > vTriangle_face;
        std::vector<float> colors;
        std::vector< vector <float> > color_faces;
        
        testEntity.load_XML_File_to_E("../datafiles/testCube.xml");
        const int n_space = 4;
        testEntity.display_all(0, n_space);
        int index=0;
        int level=0;
        testEntity.search_For(index , level, "color");
        testEntity.SearchResultsToVectorFloat(colors, color_faces);
        testEntity.extractEVertexToGL(coordinate, vertex, vCube);
        // transfer extracted data to vector in glFunctions scope
        vect2vect(vCube);
        vect2vect_colors(color_faces);
    
        // Initialise GLUT and setup the window
        int cursor = GLUT_CURSOR_INHERIT;
        
        glutInit (&argc, argv);
        glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
        glutInitWindowSize (500, 500);
        glutInitWindowPosition (700, 300);
        glutCreateWindow ("Universe");
        glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
                      GLUT_ACTION_CONTINUE_EXECUTION);

        // initialise the glut Depth and Lighting
        initDisplay();

        // call the glut display functions
        glutDisplayFunc (display);
        glutIdleFunc (display);
        glutReshapeFunc (reshape);
    
        // Lighting option
        glShadeModel(GL_SMOOTH);
        glEnable(GL_DEPTH_TEST);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpec);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, MatShininess);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
    
        // check glut event: keyboard and mouse
        glutPassiveMotionFunc(mouseMovement); //check for Mouse movement with button up
        glutMotionFunc(mouseMovement_Rclick); //check for Mouse movement with button down
        glutKeyboardFunc(keyDown);
    
        // set Mouse cursor image
        glutSetCursor(cursor);
        
        while (1)
        {
            // glut Loop
            glutMainLoopEvent();
    
            // Load "internal" file 1,2 or t
            // TODO need to think about keeping dataFile class or not
            /*
            if (dataFile::load_File){
                pt_masterEntity->clearEntity();
                //~ glDisplay::resetIndex();
                resetIndex();
                //~ dataGlEntity.loadFile(glDisplay::keyChoice,toto);
                dataGlEntity.loadFile(keyChoice,pt_masterEntity);
                //~ tata.coordinate(toto);
                // TODO need to replace coordinate function with new coord system
                coordinate(pt_masterEntity);
                // resetting keys
                dataFile::load_File = false;
                //~ glDisplay::keyChoice='\0';
                keyChoice='\0';
             }
             */
    
             
             
            //~ tata.display();
            display();
    
        }
    }

// argument is -t for text
    else if(choice=="-t"){
        glutInit(&argc, argv);
        glutInitContextVersion(2,0);
        //~ glutInitDisplayMode(GLUT_RGB);
        glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);    
        //~ glutInitWindowSize(640, 480);
        glutInitWindowSize(window_width, window_height);
        glutCreateWindow("Basic Text");
    
        if (argc > 2)
            userText=argv[2];
        else{
            
            std::cout << "Enter text" << std::endl;
            std::string strtmp;
            std::cin >> strtmp;
            userText = strtmp.c_str();
        }
        if (argc > 3)
            userFontSize=atoi(argv[3]);
        else
            userFontSize=48;
            
        if (argc > 4)
            userRed = atof(argv[4]);
        else
            userRed = 1;
        if (argc > 5)
            userGreen = atof(argv[5]);
        else
            userGreen = 1;
        if (argc > 6)
            userBlue = atof(argv[6]);
        else
            userBlue = 1;
    
        std::cout << userRed << std::endl;
        std::cout << userGreen << std::endl;
        std::cout << userBlue << std::endl;
        
        if (argc > 7)
            userFontFilename = argv[7];
        else
            userFontFilename = "./fonts/FreeSans.ttf";
    
        GLenum glew_status = glewInit();
        
        if (GLEW_OK != glew_status) {
            fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
            return 1;
        }
        
        if (!GLEW_VERSION_2_0) {
            fprintf(stderr, "No support for OpenGL 2.0 found\n");
            return 1;
        }
        
        if (init_font(userFontSize, userFontFilename)  && init_program()) {
            //~ init_background(inputText,0,0);
            init_color(userRed,userBlue,userGreen);
            init_cube(userText,0,0,0);
            
            glutDisplayFunc(textDisplay);
            glutIdleFunc(onIdle);
            glEnable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);;
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glutMainLoop();
    
        }
        
        free_resources();
        return 0;
    }

// else argc

// argument is -e for entity
    else if(argc==1 || choice=="-e")
    {

    // variables
        string key_input;
        int n=0;
        const int n_space = 2;
        E newEntity;
    
        while (key_input!="quit")
        {
            std::cout << std::endl;
            std::cout << "This is session " << ++n << std::endl;
            std::cout << std::endl;
            std::cout << "Load testing set or User Input (or quit)? ('t'/'u'/'quit')" << std::endl;
            std::cout << "Or load testing file to display for testing ('l')?" << std::endl;
            std::cin >> key_input;
            if (key_input=="quit") 
            {
                return 0;
            }
            else if (key_input=="u")
            {
                std:: cout << "Enter name:" << std::endl;
                std:: cout << "(Or type 'quit' anytime to quit)" << std::endl;
                std:: cout << "(Or type 'end' in name when it is the last entity of the current level)" << std::endl;
                std:: cin >> key_input;
            
                if (key_input!="quit")
                {
                    newEntity.set_name(key_input);
                    std::cout << "enter data" << std::endl;
                    cin.ignore(); 
                    std:: getline(std:: cin, key_input);
                    if (key_input!="quit")
                    {
                        newEntity.set_data(key_input);
                    }
                    else return 0;
                }
                else return 0;
                newEntity.user_input_V();
                
                std::cout << std::endl;
                newEntity.format_display(0,n_space,"<",">","</",">");
                std::cout << std::endl;
                std::cout << "Do you want to save the file?(y)" << std::endl;
                std::cin >> key_input;
                if (key_input=="y") newEntity.E_save_to_file();
                
            newEntity.clear_all_vE();
            newEntity.clear_name();
            newEntity.clear_data();                
            }
            else if (key_input=="l")
            {
                std::cout << "1 for single line XML or 2 for multiline, 3 for both" << std::endl; 
                std::cin >> key_input;
                if (key_input=="1") newEntity.load_XML_File_to_E("../datafiles/merte.xml");
                if (key_input=="2") newEntity.load_XML_File_to_E("../datafiles/testEOF7.xml");
                if (key_input=="3") newEntity.load_XML_File_to_E("../datafiles/testAny.xml");
                //~ newEntity.format_display(0,"<",">","</",">");
                
                newEntity.display_all(0, n_space);

                //~ //cleare and delete
                newEntity.clear_all_vE();
                newEntity.clear_name();
                newEntity.clear_data();
            }
            else if (key_input=="t")
            {
                newEntity.testing();
                std::cout << std::endl;
                newEntity.format_display(0,n_space,"<",">","</",">");
                std::cout << std::endl;
                std::cout << "Do you want to save the file?(y)" << std::endl;
                std::cin >> key_input;
                if (key_input=="y") newEntity.E_save_to_file();
                
                newEntity.clear_all_vE();
                newEntity.clear_name();
                newEntity.clear_data();
            }
            else
            {
                std::cout << "No comprendo" << std::endl;
            }
        }
        
    
    }
    
    exit(0);
}


#endif /* main.cpp */
