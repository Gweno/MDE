// ------------------------main.cpp-------------------
#ifndef MAIN
#define MAIN

#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <vector>
//~ #include "Entity.h"
//~ #include "glDisplay.h"
#include "glFunctions.h"
//~ #include "DataFile.h"
#include "E.h"

#include "../include/Leap.h"


//~ using namespace std;
// TODO check if commenting line above creates problem, track std functions and add std:: if needed

// use of arguments for testing: e execute main for new class e, no argument execute the normal main for Entity.

int main (int argc, char **argv)
{

    std::string choice;
    std::cout << "Have " << argc << " arguments:" << std::endl;
    for (int i = 0; i < argc; ++i) {
        std::cout << argv[i] << std::endl;
    }
    if (argc>1) choice= std::string(argv[1]);
    //~ if (argc==1){
    if (choice=="gl")
    {
    
    //~ //Create an instance of Entity
    //~ Entity titi;
    //~ Entity *toto= & titi;    

    //~ E masterEntity;
    //~ E *pt_masterEntity = &masterEntity;

    //~ masterEntity.newEntity();
    //~ dataFile::load_File = false;

        E testEntity;

        GLfloat MatSpec[] = {1.0,1.0,1.0,1.0};
        GLfloat MatShininess[] = {128.0};
//~ GLfloat LightPos[] = {-1.0,1.0,0.0,0.0};
//


/*
std::vector< vector <float> > vertex;
std::vector<float> coordinate;
std::vector< std::vector< std::vector <float> > > vCube3;
std::vector<std::vector<float> > vTriangle_face;


        //~ std::vector< vector <float> > vertex;
        //~ std::vector<float> coordinate;
        std::vector<int> index;
        testEntity.load_XML_File_to_E("../datafiles/testCube0.xml");
        testEntity.E_display_all(0);
        for (int f=0; f < 6 ; ++f)
        {
            
            for (int v=0; v < 4; ++v)
            {
                index = {f,v};
                (testEntity.vE_get_by_index(index, index.begin()))->vE_copy_To_Vector_Float(coordinate);
                vertex.push_back(coordinate);
                //~ testVector_Display_2d(vertex);
                coordinate.clear();
            }
            quads_to_triangles(vertex, vTriangle_face);
            testVector_Display_2d(vertex);
            testVector_Display_2d(vTriangle_face);
            vCube3.push_back(vTriangle_face);
            vTriangle_face.clear();
            vertex.clear();
        }
        testVector_Display(vCube3);
//      
        vect2vect(vCube3);
*/

        std::vector< vector <float> > vertex;
        std::vector<float> coordinate;
        std::vector< std::vector< std::vector <float> > > vCube;
        std::vector<std::vector<float> > vTriangle_face;
        std::vector<float> colors;
        std::vector< vector <float> > color_faces;
        
        testEntity.load_XML_File_to_E("../datafiles/testCube.xml");
        testEntity.E_display_all(0);
        testEntity.extractEColorDataToGL(colors, color_faces);
        testEntity.extractEVertexToGL(coordinate, vertex, vCube);
        // transfer extracted data to vector in glFunctions scope
        vect2vect(vCube);
        vect2vect_colors(color_faces);
    
        // Create an instance of glDisplay Class
        //~ glDisplay tata;
        //~ dataFile dataGlEntity(0);
    
        // Initialise GLUT and setup the window
        int cursor = GLUT_CURSOR_INHERIT;
        
        glutInit (&argc, argv);
        glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
        glutInitWindowSize (500, 500);
        glutInitWindowPosition (700, 300);
        glutCreateWindow ("Universe");
        glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
                      GLUT_ACTION_CONTINUE_EXECUTION);

        // initialise the instance of glDisplay Class
        //~ tata.initDisplay();
        initDisplay();

        // call the glut display functions
        //~ glutDisplayFunc (tata.display);
        glutDisplayFunc (display);
        //~ glutIdleFunc (tata.display);
        glutIdleFunc (display);
        //~ glutReshapeFunc (tata.reshape);
        glutReshapeFunc (reshape);
    
        // Lighting option
        glShadeModel(GL_SMOOTH);
        glEnable(GL_DEPTH_TEST);
        //~ glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, tata.MatSpec);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpec);
        //~ glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, tata.MatShininess);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, MatShininess);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
    
        // check glut event: keyboard and mouse
        //~ glutPassiveMotionFunc(tata.mouseMovement); //check for Mouse movement with button up
        glutPassiveMotionFunc(mouseMovement); //check for Mouse movement with button up
        //~ glutMotionFunc(tata.mouseMovement_Rclick); //check for Mouse movement with button down
        glutMotionFunc(mouseMovement_Rclick); //check for Mouse movement with button down
        //~ glutKeyboardFunc(tata.keyDown);
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

// else argc

    else if(argc=1 || choice=="e")
    {

    // variables
        string key_input;
        int n=0;
        
        E newEntity;
    
        while (key_input!="quit")
        {
            std::cout << std::endl;
            std::cout << "This is session " << ++n << std::endl;
            std::cout << std::endl;
            std::cout << "Load testing set or User Input (or quit)? ('t'/'u'/'quit')" << std::endl;
            std::cout << "Or load testing file to display for testing ('l')?" << std::endl;
            std::cin >> key_input;
            if (key_input=="quit") return 0;
            else if (key_input=="t") newEntity.testing();
            else if (key_input=="u")
            {
                std:: cout << "Enter name:" << std::endl;
                std:: cout << "(Or type 'quit' anytime to quit)" << std::endl;
                std:: cout << "(Or type 'end' in name when it is the last entity of the current level)" << std::endl;
                std:: cin >> key_input;
            
                if (key_input!="quit")
                {
                    newEntity.E_set_name(key_input);
                    std::cout << "enter data" << std::endl;
                    cin.ignore(); 
                    std:: getline(std:: cin, key_input);
                    if (key_input!="quit")
                    {
                        newEntity.E_set_data(key_input);
                    }
                    else return 0;
                }
                else return 0;
                newEntity.input_E();
                
                std::cout << std::endl;
                newEntity.print_formatted_E(0,"<",">","</",">");
                std::cout << std::endl;
                std::cout << "Do you want to save the file?(y)" << std::endl;
                std::cin >> key_input;
                if (key_input=="y") newEntity.E_save_to_file();
            }
            else if (key_input=="l")
            {
                newEntity.load_XML_File_to_E("../datafiles/testCube.xml");
                newEntity.print_formatted_E(0,"<",">","</",">");
                int index=0;
                int level=0;
                (newEntity.search_For(index , level, "vertex"));
                newEntity.fetch_search_result(E::search_result);
            }
            else if (key_input=="+")
            {
                std::vector<int> test_vect;
                test_vect.push_back(0);
                test_vect.push_back(2);
                test_vect.push_back(0);
                newEntity.display_vector_int(test_vect);
            }
            else if (key_input=="i")
            {
                std:cout<<"Enter the list of indexes separated by comma:" << std::endl;
                std::cin >> key_input;
                //~ std::vector<int> vect_index=newEntity.set_vector_of_indexes("1,2,3,4");
                std::vector<int> vect_index=newEntity.set_vector_of_indexes(key_input);
                newEntity.display_vector_int(vect_index);
                
            }
            if (key_input=="t")
            {
                //~ newEntity.E_display_all(0);
                std::cout << std::endl;
                newEntity.print_formatted_E(0,"<",">","</",">");
                std::cout << std::endl;
                std::cout << "Do you want to save the file?(y)" << std::endl;
                std::cin >> key_input;
                if (key_input=="y") newEntity.E_save_to_file();
            
            }
            newEntity.vE_clear_all();
            newEntity.E_clear_name();
            newEntity.E_clear_data();
        }
        
    
    }
    exit(0);
}


#endif /* main.cpp */
