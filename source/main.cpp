// ------------------------main.cpp-------------------
#ifndef MAIN
#define MAIN

#define GLUT_DISABLE_ATEXIT_HACK
#define GLM_FORCE_RADIANS

// C++ Standarc
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <stack>
#include <regex>

// Headers
#include "MDE.h"

//~ #include <unistd.h>
//~ #include <algorithm>

//~ #include <filesystem>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

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

#define VERSION_INFO "v0.2.1"

// declaration for text.cpp
int window_width=800, window_height=600;
const char *userText;
uint userFontSize;
//~ GLfloat red_user,green_user,blue_user;
const char *userFontFilename;

int main (int argc, char **argv){

    // set switches default values - true mean use default values/ no user entry
    bool GL_on = false;    // GL_on false by default
    bool input_file = false;  // No input file by default
    bool input_color = false;  // input color
    bool input_font_color = false;  // input font color
    bool input_coord = false;  // input coordinates set by default
    bool input_padding= false;  // input padding
    bool input_offset_rule= false;  // input offset rule
    bool move_it = false;   // bool switch for testing moving display

    
    // init/declare string variables
    std::string choice[argc];
    std::string filename;
    std::string datasource_path="../datafiles/";//hardcoded data source
    std::string fullfilename;
    std::string default_filename = "testAny.xml";
    
    // init GLfloat colors
    GLfloat red_user,green_user,blue_user;
    GLfloat red_font_user,green_font_user,blue_font_user,alpha_font_user;
    
    // set variables default values
    float X_user = 0.0;
    float Y_user = 0.0;
    float Z_user = 0.0;
    float H_padding_user = 0.0;
    float V_padding_user = 0.0;
    //~ float offrul_user = 0.0;

    std::vector<vertex3D> offset_rule;



    std::cout << "Current path: " << fs::current_path() << std::endl;

    // display runtime arguments
    std::cout << "Have " << argc << " arguments:" << std::endl;
    for (int i = 0; i < argc; ++i) {
        std::cout << i << ", " << argv[i] << std::endl;
        choice[i]=std::string(argv[i]);
    }
    

    std::cout << VERSION_INFO << std::endl;
    
    // Check all arguments provided
    // 'Turn on' Relevant boolean variables and
    // stores other useful variables.
    // arguments are:
    // -v or --version: get version
    // -g or --glmode: for graphic display mode
    // -f filename or --file filename: for loading a file before start
    // -c r,g,b or --box-color r,g,b: to set text box color with RGB
    // -o r,g,b,a or --font-color r,g,b,a: set color and alpha for the font
    // -p x,y or --padding x,y: to set the padding with floats x and y
    // -a x,y,z or --origin x,y,z: to set the coordinates of origin
    // -r or --offset-rule  :to set the offset rules
    // -m or --move  :for moving mode (makes the MDE move and all axis)
    
    for (int i = 0; i < argc; ++i) {
        std::cout << "arguments " << i << ": " << choice[i] << std::endl;
        
        // get version (then exit)
        if (std::regex_match (choice[i], std::regex("(-v)|(--version)")))
            {
                printf("Version: %s\n", VERSION_INFO);
                return 0;
            }
        // test. move it or not
        if (std::regex_match (choice[i], std::regex("(-m)|(--move)")))
            {
                printf("Move it!\n");
                move_it = true;
            }
        // set GL mode on
        if (std::regex_match (choice[i], std::regex("(-g)|(--glmode)")))

            {
                //~ cout << choice[i]<< endl;
                GL_on = true;  //switch to openGL 3D mode
                printf("OpenGL 3D mode is switched on\n");
            }
        // load file
        if (std::regex_match (choice[i], std::regex("(-f)|(--file)")))
            {
                cout << choice[i]<< endl;
                input_file = true;
                if (i<argc-1){
                    cout << choice[i+1]<< endl;
                    filename=choice[i+1];
                    fullfilename=datasource_path + filename;
                    std::cout << "set file to load" << std::endl;
                }
                else {
                    printf("Missing File name\n");
                    printf("Setting default File name\n");
                }
            }
        // set box frame color
        if (std::regex_match (choice[i], std::regex("(-c)|(--box-color)")))
            {
                //~ cout << choice[i]<< endl;
                input_color= true;                
                if (i<argc-1){
                    //~ cout << choice[i+1]<< endl;
                    istringstream is(choice[i+1]);
                    string part;
                    getline(is, part, ',');
                    red_user = std::stof(part);
                    getline(is, part, ',');
                    green_user = std::stof(part);
                    getline(is, part, ',');
                    blue_user = std::stof(part);
                    
                    std::cout << "r,g,b: " << red_user << " , " << green_user << " , " << blue_user << std::endl;

                }
                else {
                    printf("Missing Color value\n");
                }
            }
        // set font alpha and color
        if (std::regex_match (choice[i], std::regex("(-o)|(--font-color)")))
            {
                //~ cout << choice[i]<< endl;
                //~ cout << std::string::npos<< endl;
                input_font_color= true;                
                if (i<argc-1){
                    cout << choice[i+1]<< endl;
                    istringstream is(choice[i+1]);
                    string part;
                    getline(is, part, ',');
                    red_font_user = std::stof(part);
                    getline(is, part, ',');
                    green_font_user = std::stof(part);
                    getline(is, part, ',');
                    blue_font_user = std::stof(part);
                    getline(is, part, ',');
                    alpha_font_user = std::stof(part);
                    
                    std::cout << "r,g,b,a: " << red_font_user << " , " << green_font_user << " , " << blue_font_user << " , " << alpha_font_user << std::endl;
                }
                else {
                    printf("Missing Font Color and Alpha values\n");
                }
            }
        // set coordinates of origin
        if (std::regex_match (choice[i], std::regex("(-a)|(--origin)")))
            {
                //~ cout << choice[i]<< endl;
                input_coord= true;                
                if (i<argc-1){

                    istringstream is(choice[i+1]);
                    string part="";
                    getline(is, part, ',');
                    X_user = std::stof(part);
                    getline(is, part, ',');
                    Y_user = std::stof(part);
                    getline(is, part, ',');
                    Z_user = std::stof(part);
                    
                    std::cout << "x,y,z: " << X_user << " , " << Y_user << " , " << Z_user << std::endl;

                }
                else {
                    printf("Missing Coordinates of origin values\n");
                }
            }
        // padding
        if (std::regex_match (choice[i], std::regex("(-p)|(--padding)")))
            {
                //~ cout << choice[i]<< endl;
                input_padding= true;                
                if (i<argc-1){
                    
                    istringstream is(choice[i+1]);
                    string part;
                    getline(is, part, ',');
                    V_padding_user = std::stof(part);
                    getline(is, part, ',');
                    H_padding_user = std::stof(part);
                                        
                    std::cout << "padding set" << std::endl;
                 }
                else {
                    printf("Missing Padding values\n");
                }
            }
        // offset rules
        if (std::regex_match (choice[i], std::regex("(-r)|(--offset-rule)")))

            {
                //~ cout << choice[i]<< endl;
                input_offset_rule= true;                
                if (i<argc-1){
                    //~ std::cout << choice[i+1]<< std::endl;
                    //~ char * arg_offrul;
                    
                    // offset rule.x is the name-data offset
                    // offset rule.y is the index offset
                    // offset rule.z is the level offset
                    
                    vertex3D off_rul;

                    std::string part;
                    istringstream is(choice[i+1]);
                    
                    for(int j=0; j<3;++j){
                            
                            getline(is, part, ',');
                            off_rul.x = std::stof(part);

                            getline(is, part, ',');
                            off_rul.y = std::stof(part);

                            getline(is, part, ',');
                            off_rul.z = std::stof(part);

                            offset_rule.push_back(off_rul);
                            
                            // reset off_rul struct
                            off_rul = vertex3D();
                        
                    }
                    
                    displayV3D(offset_rule);

                }
                else {
                    printf("Missing values for the Offset rules\n");
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
                    std::cout << "* quit) Quit                     *" << std::endl;
                    std::cout << "*                                                  *" << std::endl;
                    std::cout << "****************************************************" << std::endl;
                    std::cout << "Your choice:";
                    std::cin >> key_input;
                    if (key_input=="quit"){
                        return 0;
                    }
                    else if (key_input=="1") {
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
                    red_user = 0.0;
                    green_user = 1.0;
                    blue_user = 1.0;
                }
                // set default font colors
                if (!input_font_color){
                    red_font_user = 0.0;
                    green_font_user = 0.0;
                    blue_font_user = 1.0;
                    alpha_font_user = 1.0;
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
                // offset rule.x is the name-data offset
                // offset rule.y is the index offset
                // offset rule.z is the level offset
                if (!input_offset_rule){
                    offset_rule = {
                        {0.5,0.2,0},   // for offset on x-axis
                        {0.0,0.0,-0.2},   // for offset on y-axis
                        {0.0,0.0,0.0}    // for offset on z-axis
                    };
                }

                vertex2D user_padding = {H_padding_user,V_padding_user};
                //~ vertex2D user_padding = {0.0,0.0};
                
                vertex3D user_color = {red_user,green_user,blue_user};
                
                std::vector<GLfloat> user_font_color = {
                    red_font_user,
                    green_font_user,
                    blue_font_user,
                    alpha_font_user
                };
                          
                get_move_it(move_it);
                          
                init_text_MDE(newMDE,user_origin, offset, offset_rule, user_padding, user_color);
                init_font_color(user_font_color);
                
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
            input_coord = false;
            input_padding= true;
            
            printf("Clearing Done!");
        }
    exit(0);
}


#endif /* main.cpp */
