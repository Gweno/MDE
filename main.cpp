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
#include "DataFile.h"
#include "E.h"

#include "../include/Leap.h"

//~ using namespace std;
// TODO check if commenting line above creates problem, track std functions and add std:: if needed

// use of arguments for testing: e execute main for new class e, no argument execute the normal main for Entity.

int main (int argc, char **argv) {

	
    std::cout << "Have " << argc << " arguments:" << std::endl;
    for (int i = 0; i < argc; ++i) {
        std::cout << argv[i] << std::endl;
    }

if (argc==1){
    
    //~ //Create an instance of Entity
    //~ Entity titi;
    //~ Entity *toto= & titi;    

    E masterEntity;
    E *pt_masterEntity = &masterEntity;

    masterEntity.newEntity();
    dataFile::load_File = false;


	// Create an instance of glDisplay Class
    glDisplay tata;
    dataFile dataGlEntity(0);

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
    tata.initDisplay();

	// call the glut display functions
    glutDisplayFunc (tata.display);
    glutIdleFunc (tata.display);
    glutReshapeFunc (tata.reshape);

    // Lighting option
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, tata.MatSpec);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, tata.MatShininess);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

	// check glut event: keyboard and mouse
    glutPassiveMotionFunc(tata.mouseMovement); //check for Mouse movement with button up
    glutMotionFunc(tata.mouseMovement_Rclick); //check for Mouse movement with button down
    glutKeyboardFunc(tata.keyDown);

    // set Mouse cursor image
    glutSetCursor(cursor);
    
    while (1)
    {
        // glut Loop
        glutMainLoopEvent();

        // Load "internal" file 1,2 or t
        if (dataFile::load_File){
            toto->clearEntity();
            glDisplay::resetIndex();
            dataGlEntity.loadFile(glDisplay::keyChoice,toto);
            tata.coordinate(toto);
            // resetting keys
            dataFile::load_File = false;
            glDisplay::keyChoice='\0';
         }
        tata.display();

    }
}

// else argc

else{

    // variables
    string key_input;
    
    E newEntity;

    dataFile dataGlEntity(0);

    while (key_input!="quit")
    {
        std::cout << "Load testing set or User Input (or quit)? ('t'/'u'/'quit')" << std::endl;
        std::cin >> key_input;
        if (key_input=="quit") return 0;
        else if (key_input=="t") newEntity.testing();
        else if (key_input=="u")
        {
            std:: cout << "Enter name:" << std::endl;
            std:: cout << "(Or type 'quit' anytime to quit)" << std::endl;
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
        }
        
        // In search for toto
        std::cout << newEntity.search_For("toto")->data << std::endl;;
        
        newEntity.E_display_all(0);
        newEntity.E_save_to_file();
        //~ dataGlEntity.save_data_to_file(&newEntity);     // does exactely the same but through DataFile class
        
    }
}
   exit(0);

}


#endif /* main.cpp */
