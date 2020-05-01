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

// for terminal window size in linux
#include <sys/ioctl.h>
// to get process id from system
#include <sys/types.h>
#include <unistd.h>


// to get process id from system


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

#define VERSION_INFO "v0.3.2"

// declaration for text.cpp
int window_width=800, window_height=600;
//~ const char *userText;
uint user_font_size;
//~ GLfloat red_user,green_user,blue_user;
//~ const char *userFontFilename;

// init const char
const char * user_font;

std::string font_name;


//predifined colors:
//~ darkgrey = (0.4, 0.4, 0.4)
//~ red = (1.0, 0.0, 0.0)
//~ green = (0.0, 1.0, 0.0)
//~ blue = (0.0, 0.0, 1.0)
//~ cyan = (0.0, 1.0, 1.0)
//~ magenta = (1.0, 0.0, 1.0)
//~ yellow = (1.0, 1.0, 0.0)
//~ white = (1.0, 1.0, 1.0)
//~ black = (0.0, 0.0, 0.0)
//~ darkred = (0.5, 0.0, 0.0)
//~ darkgreen = (0.0, 0.5, 0.0)
//~ darkblue = (0.0, 0.0, 0.5)
//~ darkcyan = (0.0, 0.5, 0.5)
//~ darkmagenta = (0.5, 0.0, 0.5)
//~ darkyellow = (0.5, 0.5, 0.0)
//~ lightgrey = (0.8, 0.8, 0.8)

void byebye(){
    printf("bye bye!\n");
}

int main (int argc, char **argv){

    // set switches default values - true mean use default values/ no user entry
    bool GL_on = false;    // GL_on false by default
    bool input_file = false;  // No input file by default
    bool move_it = false;   // bool switch for testing moving display
    bool display_box = false;      //bool to switch box display on/off
    bool display_menu = true;   // display start menu

    // initialise default value
    user_font= "../fonts/FreeSans.ttf";
    user_font_size = 48;
    
    // init/declare string variables
    std::string choice[argc];
    std::string filename;
    std::string datasource_path="../datafiles/";//hardcoded data source
    std::string full_filename = "../datafiles/default.xml";
    
    // init GLfloat colors
    GLfloat red_user = 0.5,green_user = 0.5,blue_user = 0.5,alpha_user = 0.5;
    GLfloat red_font_user = 0.0,green_font_user = 0.0, blue_font_user = 0.0,alpha_font_user = 1.0;
    GLfloat red_bg_user = 1.0,green_bg_user = 1.0,blue_bg_user = 1.0,alpha_bg_user = 1.0;
    
    // set variables default valueo
    float X_user = 0.0;
    float Y_user = 0.0;
    float Z_user = 0.0;
    float H_padding_user = 0.0;
    float V_padding_user = 0.0;

    // Offset rules
    std::vector<vertex3D> offset_rule;
    vertex3D off_rule_name_data = {0.5,0.2,0.0};
    vertex3D off_rule_index = {0.0,0.0,-0.2};
    vertex3D off_rule_level = {0.0,0.0,0.0};
    
    std::cout << "Current path: " << fs::current_path() << std::endl;
    
    // display terminal window number of rows and columns
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    printf ("lines %d\n", w.ws_row);
    printf ("columns %d\n", w.ws_col);

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
    // -v, --version: get version
    // -e, --nomenu: GL display only, use all default or runtime arguments
    // -g, --glmode: for graphic display mode
    // -f, --file filename: for loading a file before start
    // -n, --font font: set the font
    // -s, --font-size size: set the font size
    // -o, --font-color r,g,b,a: set color and alpha for the font
    // -b, --box display a frame box for each text
    // -c, --box-color r,g,b,a: to set text box color with RGBA
    // -p, --padding x,y: to set the padding with floats x and y
    // -a, --origin x,y,z: to set the coordinates of origin
    // -r, --offset-rule  :to set the offset rules
    // -m, --move  :for moving mode (makes the MDE move and all axis)
    // -d, --background-color r,g,b,a: to set background color with RGBA

    // Create the master instance of MDE
    MDE newMDE;
    
    newMDE.set_name("MDE");
    newMDE.set_data("master");
    newMDE.new_vMDE_element();
    
    pid_t pid = getpid();
    printf("pid: %lu\n", pid);
    
    newMDE.set_name_vMDE_back("pid");
    newMDE.set_data_vMDE_back(to_string(pid));
    
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
        // test. box display or not
        if (std::regex_match (choice[i], std::regex("(-b)|(--box)")))
            {
                printf("Display box!\n");
                display_box = true;
            }
        // set GL mode on
        if (std::regex_match (choice[i], std::regex("(-g)|(--glmode)")))

            {
                GL_on = true;  //switch to openGL 3D mode
                printf("OpenGL 3D mode is switched on\n");
            }
        // set GL mode on
        if (std::regex_match (choice[i], std::regex("(-e)|(--nomenu)")))

            {
                display_menu = false;  // no text mode menu
                GL_on = true;  //switch to openGL 3D mode
                printf("No Text Menu, straigth to GL\n");
            }
        // load file
        if (std::regex_match (choice[i], std::regex("(-f)|(--file)")))
            {
                cout << choice[i]<< endl;
                //~ input_file = true;
                if (i<argc-1){
                    cout << choice[i+1]<< endl;
                    //~ filename=choice[i+1];
                    //~ fullfilename=datasource_path + filename;
                    //~ fullfilename=datasource_path + filename;
                    full_filename=choice[i+1];
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
                //~ input_color= true;                
                if (i<argc-1){
                    //~ cout << choice[i+1]<< endl;
                    istringstream is(choice[i+1]);
                    std::string part;
                    getline(is, part, ',');
                    red_user = std::stof(part);
                    getline(is, part, ',');
                    green_user = std::stof(part);
                    getline(is, part, ',');
                    blue_user = std::stof(part);
                    getline(is, part, ',');
                    alpha_user = std::stof(part);
                    
                    std::cout << "r,g,b: " << red_user << " , " << green_user << " , " << blue_user << " , " << alpha_user << std::endl;

                }
                else {
                    printf("Missing Color value\n");
                }
            }
        // set font
        if (std::regex_match (choice[i], std::regex("(-n)|(--font)")))
            {
                //~ input_font= true;                
                if (i<argc-1){
                    cout << choice[i+1]<< endl;
                    istringstream is(choice[i+1]);
                    std::string part;
                    //~ std::string font_name;
                    getline(is, part, ',');
                    font_name = "../fonts/"+part+".ttf";
                    //~ font_name= "../fonts/OpenSans-Bold.ttf";
                    //~ font_name= "../fonts/FreeSans.ttf";
                    user_font = font_name.c_str();       //convert the string to const char*
                    

                    std::cout << "font: " << user_font << std::endl;
                }
                else {
                    printf("Missing Font Name\n");
                }
            }
            

            
        // set font size
        if (std::regex_match (choice[i], std::regex("(-s)|(--font-size)")))
            {
                //~ cout << choice[i]<< endl;
                //~ cout << std::string::npos<< endl;
                //~ input_font_size= true;                
                if (i<argc-1){
                    cout << choice[i+1]<< endl;
                    istringstream is(choice[i+1]);
                    std::string part;
                    getline(is, part, ',');
                    user_font_size = std::stof(part);

                    std::cout << "font size: " << user_font_size << std::endl;
                }
                else {
                    printf("Missing Font Size\n");
                }
            }



        // set font color
        if (std::regex_match (choice[i], std::regex("(-o)|(--font-color)")))
            {
                //~ cout << choice[i]<< endl;
                //~ cout << std::string::npos<< endl;
                //~ input_font_color= true;                
                if (i<argc-1){
                    cout << choice[i+1]<< endl;
                    istringstream is(choice[i+1]);
                    std::string part;
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
        // set background color
        if (std::regex_match (choice[i], std::regex("(-d)|(--background-color)")))
            {
                //~ cout << choice[i]<< endl;
                //~ cout << std::string::npos<< endl;
                //~ input_bg_color= true;                
                if (i<argc-1){
                    cout << choice[i+1]<< endl;
                    istringstream is(choice[i+1]);
                    std::string part;
                    getline(is, part, ',');
                    red_bg_user = std::stof(part);
                    getline(is, part, ',');
                    green_bg_user = std::stof(part);
                    getline(is, part, ',');
                    blue_bg_user = std::stof(part);
                    getline(is, part, ',');
                    alpha_bg_user = std::stof(part);
                    
                    std::cout << "background: r,g,b,a: " << red_bg_user << " , " << green_bg_user << " , " << blue_bg_user << " , " << alpha_bg_user << std::endl;

                }
                else {
                    printf("Missing Font Color and Alpha values\n");
                }
            }
        // set coordinates of origin
        if (std::regex_match (choice[i], std::regex("(-a)|(--origin)")))
            {
                //~ cout << choice[i]<< endl;
                //~ input_coord= true;                
                if (i<argc-1){

                    istringstream is(choice[i+1]);
                    std::string part;
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
                //~ input_padding= true;                
                if (i<argc-1){
                    
                    istringstream is(choice[i+1]);
                    std::string part;
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
                //~ input_offset_rule= true;                
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
    
    // variables for the main while loop
    
        
        string key_input="";
        int n=0;
        const int n_space = 2;
        
        //~ // testing if file not entered as arguement set default file.
        //~ if (!input_file){
            //~ fullfilename=datasource_path + filename;
        //~ }
        
        const std::string const_full_filename=full_filename;

        newMDE.new_vMDE_element();
        newMDE.set_name_vMDE_back("file");
        newMDE.set_data_vMDE_back(full_filename);
        
        
        //~ newMDE.new_vMDE_element();
        std::vector<int> vect_index_file = {1};
        int index_file = 0;
        std::vector<int>::const_iterator it_file;
        
        // add a new element for loading file into it;
        (newMDE.vMDE_get_by_index(vect_index_file,it_file))->new_vMDE_element();
        newMDE.display_vector_int(vect_index_file);
        input_file = (newMDE.vMDE_get_by_index(vect_index_file,it_file))->load_XML_File_to_vMDE(index_file, const_full_filename);

        // add new index in vector of indexes to reflect start of data from file in MDE, for later replacong with new loaded file
        vect_index_file.push_back(0);

        while (key_input!="q"){
                printf("\n");
                printf("**********************************\n");
                printf("*                                *\n");
                printf("*     MDE Version %s       *\n",VERSION_INFO);
                printf("*                                *\n");
                printf("*********************************\n");
                printf("*                                *\n");
                printf("*        Welcome%s", (n)?" Back!           *\n":"!                *\n");
                printf("*                                *\n");
                printf("*                                *\n");
                printf("*  This is session %i             *\n",++n);
                printf("*                                *\n");
                printf("**********************************\n");
                printf("\n");
                
                //~ display_menu=true;
                
                while(display_menu){
                
                        printf("******************************Main Menu******************************\n");
                        printf("\n");
                        printf("  1) Data Menu\n");
                        printf("          File: %s  \n", full_filename.c_str());
                        printf("  2) Font Menu\n");
                        printf("          Font: %s\n",user_font);
                        printf("          Size: %u\n",user_font_size);
                        printf("          Color & Alpha (r,g,b,a): (%.2f, %.2f, %.2f, %.2f)\n",red_font_user, green_font_user, blue_font_user,alpha_font_user);
                        printf("  3) Coordinates Menu\n");
                        printf("          Coordinates(x,y,z): (%.2f,%.2f,%.2f)\n",X_user,Y_user,Z_user);
                        printf("          Name <-> data offset Rule (x,y,z): (%.2f,%.2f,%.2f)\n",off_rule_name_data.x,off_rule_name_data.y,off_rule_name_data.z);
                        printf("          Index offset Rule (x,y,z):         (%.2f,%.2f,%.2f)\n",off_rule_index.x,off_rule_index.y,off_rule_index.z);
                        printf("          Level offset Rule (x,y,z):        (%.2f,%.2f,%.2f)\n",off_rule_level.x,off_rule_level.y,off_rule_level.z);                                        
                        printf("  4) Frame Menu\n");
                        printf("          Frame Box:%s\n",(display_box)?"ON":"OFF");
                        printf("          Color & Alpha (r,g,b,a): (%.2f,%.2f,%.2f,%.2f)\n",red_user, green_user, blue_user, alpha_user);
                        printf("          Padding(px) (horizontal, vertical): (%.2f,%.2f)\n",H_padding_user, V_padding_user);
                        printf("  5) Color Menu\n");
                        printf("          Background color&alpha (r,g,b,a): (%.2f,%.2f,%.2f,%.2f)\n", red_bg_user, green_bg_user, blue_bg_user, alpha_bg_user);
                        printf("          Font Color&Alpha (r,g,b,a): (%.2f,%.2f,%.2f,%.2f)\n", red_font_user, green_font_user, blue_font_user, alpha_font_user);
                        if(display_box) printf("          Frame Box Color&Alpha (r,g,b,a): (%.2f,%.2f,%.2f,%.2f)\n", red_user, green_user, blue_user, alpha_user);
                        printf("\n");
                        printf("  t) Display In Terminal\n");
                        printf("  g) Display in Gl mode\n");
                        printf("\n");
                        printf("  q) Quit\n");
                        printf("\n");
                        printf("*********************************************************************\n");
                        printf("Your choice:\n");
                        //~ std::cin >> key_input;
                        char key;
                        std::cin >> key;
                        switch(key){
                            
                            case '1':
                                printf("******************************* Data ********************************\n");
                                printf("\n");
                                printf("  1) Load Data File (xml)\n");
                                printf("             Current File: %s  \n", full_filename.c_str());
                                printf("  2) Load Data testing Set\n");
                                printf("  3) User Data Input\n");
                                printf("\n");
                                printf("  b) Back\n");
                                printf("  q) Quit\n");
                                printf("\n");
                                printf("*********************************************************************\n");
                                char key1;
                                printf("Your choice:\n");
                                std::cin >> key1;
                                
                                switch(key1){
                                    
                                    case '1':
                                        {
                                            printf("**************************** Load File ******************************\n");
                                            printf("\n");
                                            printf("              Current File: %s  \n", full_filename.c_str());
                                            printf("\n");
                                            printf("  1) Enter name of xml file (including extension)\n");
                                            printf("\n");
                                            printf("           Or choose between examples:\n"); 
                                            printf("\n");
                                            printf("  2: perma.xml (this file is on a single line)\n"); 
                                            printf("  3: bookstore.xml\n"); 
                                            printf("  4: activities.xml\n");
                                            printf("\n");
                                            printf("  b) Back\n");
                                            printf("  q) Quit\n");
                                            printf("\n");
                                            printf("*********************************************************************\n");
                                            printf("Your choice:\n");
                                            char key11;
                                            std::cin >> key11;
                                            
                                            switch(key11){
                                                
                                                case '1':
                                                    {
                                                        std::cin >> key_input;
                                                        const std::string file_to_load="../datafiles/"+key_input;
                                                        newMDE.replace_vMDE_with_XML(vect_index_file,file_to_load);
                                                        full_filename=file_to_load;
                                                        break;
                                                    }
                                                case '2':
                                                    {
                                                        const std::string file_to_load="../datafiles/perma.xml";
                                                        newMDE.replace_vMDE_with_XML(vect_index_file,file_to_load);
                                                        full_filename=file_to_load;
                                                        break;
                                                    }
                                                    
                                                case '3':
                                                    {
                                                        const std::string file_to_load="../datafiles/bookstore.xml";
                                                        newMDE.replace_vMDE_with_XML(vect_index_file,file_to_load);
                                                        full_filename=file_to_load;
                                                        break;
                                                    }
                                                    
                                                case '4':
                                                    {
                                                        const std::string file_to_load="../datafiles/activities.xml";
                                                        newMDE.replace_vMDE_with_XML(vect_index_file,file_to_load);
                                                        full_filename=file_to_load;
                                                        break;
                                                    }
                                                    
                                                case 'b':
                                                    break;

                                                case 'q':
                                                    return 0;
                                                    break;

                                                default:
                                                    printf("Choose a correct option!\n");
                                                    break;
                                            }
                                            break;
                                        }

                                    //~ case '2':
                                        //~ input_file = newMDE.load_XML_File_to_MDE(fullfilename);                
                                        //~ std::cout <<"Keeping current file " << fullfilename << std::endl;
                                        //~ break;

                                    case '2':
                                        newMDE.destructor_MDE();
                                        newMDE.testing();
                                        std::cout << std::endl;
                                        newMDE.format_display(0,n_space,"<",">","</",">");
                                        std::cout << std::endl;
                                        std::cout << "Do you want to save the file?(y)" << std::endl;
                                        std::cin >> key_input;
                                        if (key_input=="y") newMDE.user_save_to_XML_file();
                                        break;
                                        
                                    case '3':
                                        newMDE.destructor_MDE();
                                        std:: cout << "Enter name:" << std::endl;
                                        std:: cout << "(Or type 'q' anytime to quit )" << std::endl;
                                        std:: cout << "(Or type 'end' in name when it is the last entity of the current level)" << std::endl;
                                        std:: cin >> key_input;
                                    
                                        if (key_input!="q")
                                        {
                                            newMDE.set_name(key_input);
                                            std::cout << "enter data" << std::endl;
                                            cin.ignore(); 
                                            std:: getline(std:: cin, key_input);
                                            if (key_input!="q")
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
                                        break;
                                        
                                    
                                    case 'b':
                                        break;
                                        
                                    case 'q':
                                        return 0;
                                        break;
                                        
                                    default:
                                        printf("Choose a correct option!\n");
                                        break;
                                }
                                break;
                            
                            
                            case '2':
                                {
                                    bool loop = true;
                                    while(loop){
                                        printf("**************************** Font Menu ******************************\n");
                                        printf("\n");
                                        printf("  1) Select Font\n");
                                        printf("            Current Font: %s\n",user_font);
                                        printf("  2) Set the font Size\n");
                                        printf("            Current Font Size: %u\n",user_font_size);
                                        printf("  3) Set the font Color & Alpha\n");
                                        printf("            Current Color&Alpha(r,g,b,a): (%.2f, %.2f, %.2f, %.2f)\n",red_font_user, green_font_user, blue_font_user,alpha_font_user);
                                        printf("\n");
                                        printf("  b) Back\n");
                                        printf("  q) Quit\n");
                                        printf("\n");
                                        printf("*********************************************************************\n");
                                        char key2;
                                        printf("Your choice:\n");
                                        std::cin >> key2;
                                        
                                        switch(key2){
                                            case '1':
                                                printf("****************************** Font *********************************\n");
                                                printf("    Current Font: %s\n",user_font);
                                                printf("\n");
                                                printf("  1) FreeSans\n");
                                                printf("  2) FreeSansBold\n");
                                                printf("  3) OpenSans-Bold\n");
                                                printf("  4) OpenSans-Regular\n");
                                                printf("\n");
                                                printf("  b) Back\n");
                                                printf("  q) Quit\n");
                                                printf("\n");
                                                printf("*********************************************************************\n");
                                                printf("Your choice:\n");
                                                char key21;
                                                std::cin >> key21;
                                                switch(key21){
                                                    case '1':
                                                        user_font= "../fonts/FreeSans.ttf";
                                                        break;
                                                        
                                                    case '2':
                                                        user_font= "../fonts/FreeSansBold.ttf";
                                                        break;
                                                        
                                                    case '3':
                                                        user_font= "../fonts/OpenSans-Bold.ttf";
                                                        break;
                                                        
                                                    case '4':
                                                        user_font= "../fonts/OpenSans-Regular.ttf";
                                                        break;
                                                        
                                                    case 'b':
                                                        break;
                                                        
                                                    case 'q':
                                                        return 0;
                                                        break;
                                                        
                                                    default:
                                                        printf("Choose a correct option!\n");
                                                        break;
                                                }
                                                break;
                                            
                                            case '2':
                                                std::cout << "Enter Font size (in pixels):" << std::endl;
                                                std::cin >> key_input;
                                                user_font_size = std::stof(key_input);
                                                //~ input_font_size=true;
                                                break;
                                            
                                            case '3':
                                                printf("**************************** Font Color *****************************\n");
                                                printf("\n");
                                                printf("  1) Enter RGBA\n");
                                                //~ printf("  2) Choose From List of RGB\n");
                                                printf("\n");
                                                printf("  b) Back\n");
                                                printf("\n");
                                                printf("*********************************************************************\n");

                                                char key23;
                                                printf("Your choice:\n");
                                                std::cin >> key23;
    
                                                if (key23=='1') {
                                                    std::cout << "Enter R,G,B,A (float 0.0 to 1.0)" << std::endl;
                                                    std::cin >> key_input;
                                                    istringstream is(key_input);
                                                    std::string part;
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
                                                //~ input_font_color= true;   
                                                break;
                                            
                                            case 'b':
                                                loop = false;
                                                break;
                                            
                                            case 'q':
                                                return 0;
                                                break;
                                            
                                            default:
                                                printf("Choose a correct option!\n");
                                                break;
                                        }
                                    }
                                    break;
                                }
                            
                            case '3':
                                {
                                    bool loop=true;
                                    while(loop){
                                        printf("************************** Coordinates Menu *************************\n");
                                        printf("\n");
                                        printf("  1) Set the coordinate of origin\n");
                                        printf("            Current coordinates(x,y,z): (%.2f,%.2f,%.2f)\n",X_user,Y_user,Z_user);
                                        printf("  2) Set the offset rules\n");
                                        printf("            Current name <-> data offset Rule (x,y,z):(%.2f,%.2f,%.2f)\n",off_rule_name_data.x,off_rule_name_data.y,off_rule_name_data.z);
                                        printf("            Current index offset Rule (x,y,z):        (%.2f,%.2f,%.2f)\n",off_rule_index.x,off_rule_index.y,off_rule_index.z);
                                        printf("            Current level offset Rule (x,y,z):        (%.2f,%.2f,%.2f)\n",off_rule_level.x,off_rule_level.y,off_rule_level.z);                                        
                                        printf("\n");
                                        printf("  b) Back\n");
                                        printf("  q) Quit\n");
                                        printf("\n");
                                        printf("*********************************************************************\n");
                                        printf("Your choice:\n");
                                        char key3;
                                        std::cin >> key3;
                                        switch(key3){
                                            case '1':
                                                {
                                                    std::cout << "Enter coordinates x,y,z" << std::endl;
                                                    std::cin >> key_input;
                                                    istringstream is(key_input);
                                                    std::string part;
                                                    getline(is, part, ',');
                                                    X_user = std::stof(part);
                                                    getline(is, part, ',');
                                                    Y_user = std::stof(part);
                                                    getline(is, part, ',');
                                                    Z_user = std::stof(part);
                                                    std::cout << std::endl;
                                                    std::cout << "You have chosen coordinates: " << X_user << ", "<< Y_user << ", "<< Z_user << std::endl;
                                                    break;
                                                }
                                            
                                            case '2':
                                                {
                                                    bool loop = true;
                                                    while(loop){
                                                        printf("******************************* Offset Rules ************************\n");
                                                        printf("\n");
                                                        printf("  1) Set the name <-> data offset\n");
                                                        printf("            Current rule (x,y,z): (%.2f,%.2f,%.2f)\n",off_rule_name_data.x,off_rule_name_data.y,off_rule_name_data.z);
                                                        printf("  2) Set the index offset\n");
                                                        printf("            Current rule (x,y,z): (%.2f,%.2f,%.2f)\n",off_rule_index.x,off_rule_index.y,off_rule_index.z);
                                                        printf("  3) Set the level offset\n");
                                                        printf("            Current rule (x,y,z): (%.2f,%.2f,%.2f)\n",off_rule_level.x,off_rule_level.y,off_rule_level.z);
                                                        printf("\n");
                                                        printf("  b) Back\n");
                                                        printf("  q) Quit\n");
                                                        printf("\n");
                                                        printf("*********************************************************************\n");
                                                        printf("Your choice:\n");
                                                        char key32;
                                                        std::cin >> key32;
                                                        
                                                        switch(key32){
                                                            
                                                            case '1':
                                                            {
                                                                std::cout << "Set the name <-> data offset x,y,z" << std::endl;
                                                                std::cin >> key_input;
                                                                istringstream is(key_input);
                                                                std::string part;
                                                                getline(is, part, ',');
                                                                off_rule_name_data.x = std::stof(part);
                                                                getline(is, part, ',');
                                                                off_rule_name_data.y = std::stof(part);
                                                                getline(is, part, ',');
                                                                off_rule_name_data.z = std::stof(part);
                    
                                                                std::cout << std::endl;
                                                                std::cout << "You have set rule: " << off_rule_name_data.x << ", "<< off_rule_name_data.y << ", "<< off_rule_name_data.z << std::endl;
                                                                //~ input_offset_rule_name_data=true;
                                                                break;
                                                            }
                                                            
                                                            case '2':
                                                            {
                                                                std::cout << "Set the name <-> data offset x,y,z" << std::endl;
                                                                std::cin >> key_input;
                                                                istringstream is(key_input);
                                                                std::string part;
                                                                getline(is, part, ',');
                                                                off_rule_index.x = std::stof(part);
                                                                getline(is, part, ',');
                                                                off_rule_index.y = std::stof(part);
                                                                getline(is, part, ',');
                                                                off_rule_index.z = std::stof(part);
                    
                                                                std::cout << std::endl;
                                                                std::cout << "You have set rule: " << off_rule_index.x << ", "<< off_rule_index.y << ", "<< off_rule_index.z << std::endl;
                                                                //~ input_offset_rule_index=true;
                                                                break;
                                                            }
                                                            
                                                            case '3':
                                                            {
                                                                std::cout << "Set the name <-> data offset x,y,z" << std::endl;
                                                                std::cin >> key_input;
                                                                istringstream is(key_input);
                                                                std::string part;
                                                                getline(is, part, ',');
                                                                off_rule_level.x = std::stof(part);
                                                                getline(is, part, ',');
                                                                off_rule_level.y = std::stof(part);
                                                                getline(is, part, ',');
                                                                off_rule_level.z = std::stof(part);
                    
                                                                std::cout << std::endl;
                                                                std::cout << "You have set rule: " << off_rule_level.x << ", "<< off_rule_level.y << ", "<< off_rule_level.z << std::endl;
                                                                //~ input_offset_rule_level=true;
                                                                break;
                                                            }
                                                            
                                                            case 'b':
                                                                loop=false;
                                                            break;
                                                            
                                                            case 'q':
                                                                return 0;
                                                            break;
                                                            
                                                            default:
                                                                printf("Choose a correct option!\n");
                                                            break; 
                                                        }
                                                    }
                                                break;
                                                }
                                            
                                            case 'b':
                                                loop=false;
                                                break;
                                            
                                            case 'q':
                                                return 0;
                                                break;
                                            
                                            default:
                                                printf("Choose a correct option!\n");
                                                break; 
                                                
                                        }
                                    }
                                    break;
                                }
                            
                            case '4':
                                {
                                    bool loop=true;
                                    while(loop){
                                        printf("******************************** Frame Menu ************************\n");
                                        printf("\n");
                                        printf("  1) Display a frame Box            %s\n",(display_box)?"ON":"OFF");
                                        printf("  2) Set the box's colors and alpha\n");
                                        printf("         Current Color&Alpha (r,g,b,a): (%.2f,%.2f,%.2f,%.2f)\n",red_user, green_user, blue_user, alpha_user);
                                        printf("  3) Set the padding values\n");
                                        printf("         Current Padding in pixels (horizontal, vertical): (%.2f,%.2f)\n",H_padding_user, V_padding_user);
                                        printf("\n");
                                        printf("  b) Back\n");
                                        printf("  q) Quit\n");
                                        printf("\n");
                                        printf("*********************************************************************\n");

                                        printf("Your choice:\n");
                                        char key4;
                                        std::cin >> key4;
                                        
                                        switch(key4){
                                            
                                            case '1':
                                                display_box = !display_box;
                                                break;
                                            
                                            case '2':
                                                {
                                                    std::cout << "Enter R,G,B,A (float 0.0 to 1.0)" << std::endl;
                                                    std::cin >> key_input;
                                                    std::istringstream is(key_input);
                                                    std::string part;
                                                    getline(is, part, ',');
                                                    red_user = std::stof(part);
                                                    getline(is, part, ',');
                                                    green_user = std::stof(part);
                                                    getline(is, part, ',');
                                                    blue_user = std::stof(part);
                                                    getline(is, part, ',');
                                                    alpha_user = std::stof(part);
                                                    std::cout << "r,g,b,a: " << red_user << " , " << green_user << " , " << blue_user << " , " << alpha_user << std::endl;
                                                    //~ input_color= true;
                                                    break;
                                                }
                                            
                                            case '3':
                                                std::cout << "Enter horizontal padding" << std::endl;
                                                std::cin >> H_padding_user ;
                                                std::cout << "Enter Vertical padding" << std::endl;
                                                std::cin >> V_padding_user ;
                                                std::cout << "You have chosen padding: " << H_padding_user << ", "<< V_padding_user << std::endl;
                                                break;
                                            
                                            case 'b':
                                                printf("Back\n");
                                                loop = false;
                                                break;
                                            
                                            case 'q':
                                                return 0;
                                                break;
                                            
                                            default:
                                                printf("Choose a correct option!\n");
                                                break; 
                                        }
                                    }
                                    break;
                                }
                            
                            case '5':
                                {
                                    bool loop=true;
                                    while(loop){
                                        printf("******************************** Color Menu *************************\n");
                                        printf("\n");
                                        printf("  1) Set the background color & alpha\n");
                                        printf("            Current Color&Alpha (r,g,b,a): (%.2f,%.2f,%.2f,%.2f)\n", red_bg_user, green_bg_user, blue_bg_user, alpha_bg_user);
                                        printf("  2) Set the font color & alpha\n");
                                        printf("            Current Color&Alpha (r,g,b,a): (%.2f,%.2f,%.2f,%.2f)\n", red_font_user, green_font_user, blue_font_user, alpha_font_user);
                                        if(display_box) {
                                            printf("  3) Set the box color & alpha\n");
                                            printf("            Current Color&Alpha (r,g,b,a): (%.2f,%.2f,%.2f,%.2f)\n", red_user, green_user, blue_user, alpha_user);
                                        }
                                        printf("\n");
                                        printf("  b) Back\n");
                                        printf("  q) Quit\n");
                                        printf("\n");
                                        printf("\n");
                                        printf("*********************************************************************\n");
                                        printf("Your choice:\n");
                                        char key5;
                                        std::cin >> key5;
                                        switch (key5){
                                            case '1':
                                                {
                                                    std::cout << "Enter R,G,B,A (float 0.0 to 1.0)" << std::endl;
                                                    std::cin >> key_input;
                                                    std::istringstream is(key_input);
                                                    std::string part;
                                                    getline(is, part, ',');
                                                    red_bg_user = std::stof(part);
                                                    getline(is, part, ',');
                                                    green_bg_user = std::stof(part);
                                                    getline(is, part, ',');
                                                    blue_bg_user = std::stof(part);
                                                    getline(is, part, ',');
                                                    alpha_bg_user = std::stof(part);
                                                    std::cout << "r,g,b,a: " << red_bg_user << " , " << green_bg_user << " , " << blue_bg_user << " , " << alpha_bg_user << std::endl;
                                                    //~ input_color= true;
                                                    break;
                                                }
                                                
                                            case'2':
                                                {
                                                    std::cout << "Enter R,G,B,A (float 0.0 to 1.0)" << std::endl;
                                                    std::cin >> key_input;
                                                    istringstream is(key_input);
                                                    std::string part;
                                                    getline(is, part, ',');
                                                    red_font_user = std::stof(part);
                                                    getline(is, part, ',');
                                                    green_font_user = std::stof(part);
                                                    getline(is, part, ',');
                                                    blue_font_user = std::stof(part);
                                                    getline(is, part, ',');
                                                    alpha_font_user = std::stof(part);
                                                    
                                                    std::cout << "r,g,b,a: " << red_font_user << " , " << green_font_user << " , " << blue_font_user << " , " << alpha_font_user << std::endl;
                                                    break;
                                                }
                                                
                                            case '3':
                                                {
                                                    if (!display_box) break;
                                                    std::cout << "Enter R,G,B,A (float 0.0 to 1.0)" << std::endl;
                                                    std::cin >> key_input;
                                                    std::istringstream is(key_input);
                                                    std::string part;
                                                    getline(is, part, ',');
                                                    red_user = std::stof(part);
                                                    getline(is, part, ',');
                                                    green_user = std::stof(part);
                                                    getline(is, part, ',');
                                                    blue_user = std::stof(part);
                                                    getline(is, part, ',');
                                                    alpha_user = std::stof(part);
                                                    std::cout << "r,g,b,a: " << red_user << " , " << green_user << " , " << blue_user << " , " << alpha_user << std::endl;
                                                    //~ input_color= true;
                                                    break;
                                                }
                                                
                                            case'b':
                                                loop = false;
                                                break;
                                                
                                            case'q':
                                                break;
                                                
                                            default:
                                                break;
                                        }
                                    }
                                    break;
                                }
                            
                            case 't':
                                {
                                    newMDE.display_all(0, n_space);
                                    while (key_input!="b"){
                                        std::cout << std::endl;
                                        std::cout <<"Press b to come Back to Main Menu" << std::endl;
                                        std::cin >> key_input;
                                    }                                
                                    break;
                                }
                                
                            case 'g':
                                GL_on=true;
                                display_menu=false;
                            break;

                            case 'q':
                                return 0;
                            break;
                            
                            default:
                                printf("Wrong choice\n");
                            break;
                        }
                        key_input="";
                
                }
                printf("Done!\n");
                // if any of the bool is false, then display the menu...


                        if (GL_on){
                            
                            glutInit(&argc, argv);
                            glutInitContextVersion(2,0);
                            glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);    
                            glutInitWindowSize(window_width, window_height);
                            //~ glutInitWindowPosition(100, 100);
                            glutCreateWindow("Basic Text");
                
                            GLenum glew_status = glewInit();
                            
                            if (GLEW_OK != glew_status) {
                                fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
                                return 1;
                            }
                    
                            if (!GLEW_VERSION_2_0) {
                                fprintf(stderr, "No support for OpenGL 2.0 found\n");
                                return 1;
                            }
                            
                            if (!init_font(user_font_size, user_font)  ||  !init_program()) {
                                
                                fprintf(stderr, "Problem with loading fonts, or program\n");
                                return 1;
                                
                            }
            
                            std::cout << "User coordinates: " << X_user << ", "<< Y_user << ", "<< Z_user << std::endl;
                            vertex3D user_origin={X_user,Y_user, Z_user};
                            
                            // declare the offset vector of vertex3D for later calculation of offset from offset rules, text coordinates and padding.
                            std::vector<vertex3D> offset;

                            offset_rule={
                                off_rule_name_data,
                                off_rule_index,
                                off_rule_level
                            };
                            displayV3D(offset_rule);
            
                            vertex2D user_padding = {H_padding_user,V_padding_user};
                            vertex3Dalpha user_color = {red_user,green_user,blue_user,alpha_user};
                            vertex3Dalpha user_bg_color = {red_bg_user,green_bg_user,blue_bg_user,alpha_bg_user};
                            printf("user_bg_color = {%.2f,%.2f,%.2f,%.2f)",red_bg_user,green_bg_user,blue_bg_user,alpha_bg_user);
                            
                            std::vector<GLfloat> user_font_color = {
                                red_font_user,
                                green_font_user,
                                blue_font_user,
                                alpha_font_user
                            };
                                      
                            set_options(move_it,display_box);
                            
                            init_text_MDE(newMDE,user_origin, offset, offset_rule, user_padding, user_color,user_bg_color);
                            init_font_color(user_font_color);
                            init_matrices();
                            
                            glutDisplayFunc(display);
                            glutKeyboardFunc(keyboard);
                            glutSpecialFunc(special_key);
                            
                            // check glut event: keyboard and mouse
                            glutMotionFunc(mouse_movement); //check for Mouse movement with button down
                            glutMouseFunc(mouse_wheel);
                            glutIdleFunc(onIdle);
                            
            
                            glEnable(GL_BLEND);
                            glEnable(GL_DEPTH_TEST);
                            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                            
                            glutCloseFunc(byebye);
                            
                            glutMainLoop();
                            printf("GL Done!");
                            
                            free_resources();
                                        
                        }
                        else{
                            std::cout << "Choice not recognised, please choose one of the listed options" << std::endl;
                            //~ display_menu=true;
                        }
                        
            //~ }
                //~ }
                
                // clear
                newMDE.destructor_MDE();
                //~ fullfilename ="";
                
                
                //reset booleans
                GL_on = false;    // GL_on false by default
                display_box = false;
                move_it = false;   // switch for testing moving display            
                
                printf("Clearing Done!");
        }
    exit(0);
}


#endif /* main.cpp */
