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
#include "MDE.h"

#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/freeglut.h>

#include "text.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "./common/shader_utils.h"

#define VERSION_INFO "v0.1.0"

// declarationfor text.cpp
int window_width=800, window_height=600;
const char *userText;
uint userFontSize;
GLfloat userRed,userGreen,userBlue;
const char *userFontFilename;


// use of arguments for testing: '-e' execute main for new class MDE,
// no argument execute the normal main for Entity,
// "-t" execute the text GL display with argument text,font size, r,g,b,font name.

int main (int argc, char **argv){

    // init/declare string variables
    std::string choice[argc];
    std::string filename;
    std::string datasource_path="../datafiles/";//hardcoded data source
    std::string fullfilename;
    std::string default_filename = "testAny.xml";
    
    // set switches default values
    bool GL_on = false;    // GL_on false by default
    bool input_file = false;  // No input file by default
    bool input_color = false;  // input color not implemented yet
    bool input_coord = true;  // input coordinates set by default
    bool input_padding= true;  // input padding not implemented yet
    
    // set variables default values
    float X_user = 0.0;
    float Y_user = 0.0;
    float Z_user = 0.0;
    float H_padding_user ;
    float V_padding_user ;

    // display runtime arguments
    std::cout << "Have " << argc << " arguments:" << std::endl;
    for (int i = 0; i < argc; ++i) {
        std::cout << argv[i] << std::endl;
        choice[i]=std::string(argv[i]);
    }
    
    std::cout << VERSION_INFO << std::endl;
    
    // Check all arguments provided
    // 'Turn on' Relevant boolean variables and
    // stores other useful variables.
    // arguments are:
    // -f filename : for loading a file before start
    // -g : for graphic display directly
    // -c r,g,b : to set text box color with RGB (not implemented yet)
    // -v: get version 
    for (int i = 0; i < argc; ++i) {
        std::cout << "arguments " << i << ": " << choice[i] << std::endl;
        // get version (then exit)
        if(choice[i].find("-v", 0) != std::string::npos)
            {
                printf("Version: %s\n", VERSION_INFO);
                return 0;
            }
        if(choice[i].find("-g", 0) != std::string::npos)
            {
                cout << choice[i]<< endl;
                GL_on = true;  //switch to openGL 3D mode
                printf("OpenGL 3D mode is switched on\n");
            }
        // file
        if(choice[i].find("-f", 0) != std::string::npos)
            {
                cout << choice[i]<< endl;
                input_file = true;
                if (i<argc-1){
                    cout << choice[i+1]<< endl;
                    filename=choice[i+1];
                    fullfilename=datasource_path + filename;
                }
                else {
                    printf("Missing File name\n");
                    printf("Setting default File name\n");
                }
            }
        // colors
        if(choice[i].find("-c", 0) != std::string::npos)
            {
                cout << choice[i]<< endl;
                input_color= true;                
                if (i<argc-1){
                    cout << choice[i+1]<< endl;
                }
                else {
                    printf("Missing Color code\n");
                }
            }
    }
    
    // variables
        string key_input;
        int n=0;
        const int n_space = 2;
        MDE newMDE;
        
        // testing if file entered in argument is valid
        if (input_file){
            const std::string const_fullfilename=fullfilename;
            input_file = newMDE.load_XML_File_to_MDE(const_fullfilename);
        }
    
        while (key_input!="quit"){
            
            std::cout << std::endl;
            std::cout << "**********************************" << std::endl;
            std::cout << "*                                *" << std::endl;
            std::cout << "*         Welcome";
            if(n) std::cout << " Back!          *" << std::endl;
            else std::cout << "!               *" << std::endl;
            std::cout << "*                                *" << std::endl;
            std::cout << "*                                *" << std::endl;
            std::cout << "*   This is session " << ++n << "            *" << std::endl;
            std::cout << "*                                *" << std::endl;
            std::cout << "**********************************" << std::endl;
            std::cout << std::endl;
            
            if (!input_file){
                std::cout << "************** Data **************" << std::endl;
                std::cout << "*                                *" << std::endl;
                std::cout << "* f)    Load Data File (xml)     *" << std::endl;
                std::cout << "* s)    Load Data testing Set    *" << std::endl;
                std::cout << "* u)    User Data Input          *" << std::endl;
                std::cout << "* quit) Quit                     *" << std::endl;
                std::cout << "*                                *" << std::endl;
                std::cout << "**********************************" << std::endl;
                std::cout << "Your choice:";
                std::cin >> key_input;
            }
            else {
                std::cout <<"filename already set: " << fullfilename << std::endl;
            }
            if (key_input=="quit"){
                return 0;
            }
            else if (key_input=="u"){
                std:: cout << "Enter name:" << std::endl;
                std:: cout << "(Or type 'quit' anytime to quit )" << std::endl;
                std:: cout << "(Or type 'end' in name when it is the last entity of the current level)" << std::endl;
                std:: cin >> key_input;
            
                if (key_input!="quit")
                {
                    newMDE.set_name(key_input);
                    std::cout << "enter data" << std::endl;
                    cin.ignore(); 
                    std:: getline(std:: cin, key_input);
                    if (key_input!="quit")
                    {
                        newMDE.set_data(key_input);
                    }
                    else return 0;
                }
                else return 0;
                newMDE.user_input_vMDE();
                
                std::cout << std::endl;
                newMDE.format_display(0,n_space,"<",">","</",">");
                std::cout << std::endl;
                std::cout << "Do you want to save the file?(y)" << std::endl;
                std::cin >> key_input;
                if (key_input=="y") newMDE.user_save_to_XML_file();
            
            // TODO fix bug if no vMDE -> ! vMDE is private -> need a 'is_empty' function?    
            }
            else if (key_input=="f"){
                while(!input_file){
                    std::cout << "******************** Load File *********************" << std::endl;
                    std::cout << "*                                                  *" << std::endl;
                    std::cout << "* Enter name of xml file (including extension)     *" << std::endl;
                    std::cout << "* Or choose between examples:                      *" << std::endl; 
                    std::cout << "* 1: perma.xml (this file is on a single line)     *" << std::endl; 
                    std::cout << "* 2: bookstore.xml                                 *" << std::endl; 
                    std::cout << "* 3: activities.xml                                *" << std::endl; 
                    std::cout << "*                                                  *" << std::endl;
                    std::cout << "****************************************************" << std::endl;
                    std::cout << "Your choice:";
                    std::cin >> key_input;
                    if (key_input=="1") {
                        input_file = newMDE.load_XML_File_to_MDE("../datafiles/perma.xml");
                        fullfilename="../datafiles/perma.xml";
                    }
                    else if (key_input=="2") {
                        input_file = newMDE.load_XML_File_to_MDE("../datafiles/bookstore.xml");
                        fullfilename="../datafiles/bookstore.xml";
                    }
                    else if (key_input=="3") {
                        input_file = newMDE.load_XML_File_to_MDE("../datafiles/activities.xml");
                        fullfilename="../datafiles/activities.xml";
                    }
                    else {
                        input_file = newMDE.load_XML_File_to_MDE("../datafiles/"+key_input);
                        fullfilename="../datafiles/"+key_input;
                    }
                }
                
            }
            else if (key_input=="s"){
                newMDE.testing();
                std::cout << std::endl;
                newMDE.format_display(0,n_space,"<",">","</",">");
                std::cout << std::endl;
                std::cout << "Do you want to save the file?(y)" << std::endl;
                std::cin >> key_input;
                if (key_input=="y") newMDE.user_save_to_XML_file();
                
            }
            else{
                std::cout << "No comprendo" << std::endl;
            }

            if (!GL_on){
                std::cout << "File " << fullfilename << " is loaded." << std::endl;
                std::cout << "********** Display Mode **********" << std::endl;
                std::cout << "*                                *" << std::endl;
                std::cout << "* t)    Display in Text mode     *" << std::endl;
                std::cout << "* g)    Display in Graphic Mode  *" << std::endl;
                std::cout << "* quit) Quit                     *" << std::endl;
                std::cout << "*                                *" << std::endl;
                std::cout << "**********************************" << std::endl;
                std::cout << "Your choice:";
                std::cin >> key_input;
            }
            else key_input="g";
            
            if (key_input=="quit"){
                return 0;
            }
            else if (key_input=="t") {
                newMDE.display_all(0, n_space);
                
            }
            else if (key_input=="g"){
                
                if(!input_coord){
                    std::cout << "Enter coordinates" << std::endl;
                    std::cin >> X_user ;
                    std::cin >> Y_user ;
                    std::cin >> Z_user ;
                    std::cout << std::endl;
                    std::cout << "You have chosen coordinates: " << X_user << ", "<< Y_user << ", "<< Z_user << std::endl;
                    }
                        
                if (!input_padding){
                    std::cout << "Enter padding" << std::endl;
                    std::cin >> H_padding_user ;
                    std::cin >> V_padding_user ;
                    std::cout << "You have chosen padding: " << H_padding_user << ", "<< V_padding_user << std::endl;
                }
                
                glutInit(&argc, argv);
                glutInitContextVersion(2,0);
                glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);    
                glutInitWindowSize(window_width, window_height);
                glutCreateWindow("Basic Text");
    
                // set default font and font size
                userFontSize=48;
                userFontFilename = "../fonts/FreeSans.ttf";
                
                // set default colors
                if (!input_color){
                    userRed = 0;
                    userGreen = 1;
                    userBlue = 1;
                }
    
                GLenum glew_status = glewInit();
                
                if (GLEW_OK != glew_status) {
                    fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
                    return 1;
                }
        
                if (!GLEW_VERSION_2_0) {
                    fprintf(stderr, "No support for OpenGL 2.0 found\n");
                    return 1;
                }
        
                if (!init_font(userFontSize, userFontFilename)  ||  !init_program()) {
                    
                    fprintf(stderr, "Problem with loading fonts, or program\n");
                    return 1;
                    
                }
                    
                std::cout << "User coordinates: " << X_user << ", "<< Y_user << ", "<< Z_user << std::endl;
                vertex3D user_origin={X_user,Y_user, Z_user};
                std::vector<vertex3D> offset;
                //~ vertex2D user_padding = {H_padding_user,V_padding_user};
                vertex2D user_padding = {0.0,0.0};
                
                vertex3D user_color = {userRed,userBlue,userGreen};
                init_text_MDE(newMDE,user_origin, offset, user_padding, user_color);
                
                glutDisplayFunc(textDisplay);
                glutKeyboardFunc(keyDown);
                glutIdleFunc(onIdle);
                glEnable(GL_BLEND);
                glEnable(GL_DEPTH_TEST);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glutMainLoop();
                printf("GL Done!");
                
                free_resources();
                            
            }
            else{
                std::cout << "No comprendo" << std::endl;
            }
            
            // clear
            newMDE.destructor_MDE();
            fullfilename ="";
            
            
            //reset all booleans
            GL_on = false;
            input_file = false;
            input_color = false;
            input_coord = true;
            input_padding= true;
            
            printf("Clearing Done!");
        }
    exit(0);
}


#endif /* main.cpp */
