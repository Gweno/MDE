//-----------E.h-------------
#ifndef E_H
#define E_H


#include <iostream>
#include <vector>
#include <string>
//~ #include <stdexcept>




using namespace std;

class E {
    
    private:
    
        // private members
        std::vector<E *> vE;
        
    public:
        
        static std::vector<E**> search_result;

        E();
        E(std::string init_name, std::string init_data, std::vector<E*> init_vE);
        
        //Destructors;
/*
 * 
 * name: unknown
 * @param
 * @return
 * 
 */
        
        ~E();
        void vE_clear_all();
        void E_clear_name();
        void E_clear_data();
    
        //public members:
        std::string name;
        std::string data;
        //~ T t_data;
        
        // public methods
        
        // E methods
        
        
        // Methode to create new Entity
        
        // Entity creates a new (pointers to) Entity in the vector vE
        void new_vE_element();


        // set a name from a string into member 'name' of an instance of
        // an object of class E
        void E_set_name(string eName);
        
        // set a single data from a string into member 'data' of 
        //an instance of an object of class E
        void E_set_data(string data);

        // Entity set name for its last Entity in the vector vE
        void set_last_vEe_name(string vName);
        
        // Entity set data for its last Entity in the vector vE
        void set_last_vEe_data(string vData);
        
        // Entity set name for an Entity in its vector of pointers
        // of Entities at a given index (no exception handling)
        void set_vEe_name(int index_vEe,string vName);

        // Entity set data for an Entity in its vector of pointers
        // of Entities at a given index (no exception handling)
        void set_vEe_data(int index_vEe,string vData);
        
        
        // get methods
        
        // Entity gets its own name and display        
        void E_display_name();
        
        void E_display_data();
        // Entity gets its own data and display;
                        
        void vE_display_all_names();
        
        void vE_display_all_data();
        
        void E_display_all(int n_space);

        void fetch_search_result(std::vector<E**> &search_result);

        void search_For(int & index, int & level, string searchName);

        void simple_loop_VE(string searchName);
        
        void simple_loop_VE_next_level(string searchName);
        
        void input_E();
    
        void try_copy(int index);
        
         /**
         * 
         * ##name: print_formatted_E()
         * @param n_space is an integer corresponding to the initial indentation.
         * @param start_opening_tag is a string corresponding to the start of the opening tag ("<" for xml).
         * @param end_opening_tag is a string corresponding to the end of the opening tag (">" for xml).
         * @param start_closing_tag is a string corresponding to the start of the closing tag ("<\" for xml).
         * @param end_closing_tag is a string corresponding to the end of the closing tag (">" for xml).
         * @param &file (address of a file) is an ostream corresponding to the name of the file .
         *  
         * @return Nothing is returned (void)
         * @see E_save_to_file()
         * @exception
         * @brief This method is to save the instance of a class into an XML file.
         */        
        
        void print_formatted_E(int n_space, std::string start_opening_tag,
            std::string end_opening_tag, std::string start_closing_tag,
            std::string end_closing_tag);

         /**
         * 
         * ##name: E_write_to_file()
         * @param start_opening_tag is a string corresponding to the start of the opening tag ("<" for xml).
         * @param end_opening_tag is a string corresponding to the end of the opening tag (">" for xml).
         * @param start_closing_tag is a string corresponding to the start of the closing tag ("<\" for xml).
         * @param end_closing_tag is a string corresponding to the end of the closing tag (">" for xml).
         * @param &file (address of a file) is an ostream corresponding to the name of the file .
         *  
         * @return Nothing is returned (void)
         * 
         * @brief This method is to save the instance of a class into an XML file.
         * 
         * @see E_save_to_file()
         */
        //~ void E_write_to_file(std::string start_opening_tag,
        void E_write_to_file(int &n_space, std::string start_opening_tag,
            std::string end_opening_tag, std::string start_closing_tag,
            std::string end_closing_tag, ostream &file);
            
         /**
         * 
         * ##name: E_save_to_file()
         * @param No parameter
         *  
         * @return Nothing is returned (void)
         * 
         * @brief This method is to save the instance of a class into an XML file.
         * 
         * The file's name is chosen by user input.
         *  
         * @see E_write_to_file()
         */
        void E_save_to_file();
        
        void load_File();
        
        void load_File2();
        
        void load_File3();
        void load_File4();
        void load_File5();
        
         /**
         * 
         * ##name: load_XML_File_to_E()
         * @param fullFileName is a standard string
         *  
         * @return Nothing is returned (void)
         * 
         * @brief This method is to read an XML file and store it into an instance of class  E.
         * 
         * @see str_token_tag()
         */
        void load_XML_File_to_E(std::string fullFileName);

        /**
         * 
         * ##name: str_token_tag()
         * @param buffer_Input is a pointer an array of char that holds the line input
         * @param output_Tag is a pointer an array of char for the token extracted
         * @param buffer_Output is a pointer an array of char for the remainder of the line input minus the token extracted and after the delimiter2
         * @param delimiter1 is an integer corresponding to the char used as the front delimiter for the token
         * @param delimiter2 is an integer corresponding to the char used as the end delimiter for the token
         * 
         * @return void
         * 
         */
        void str_token_tag(char* buffer_Input, char* output_Tag, char* buffer_Output, int delimiter1, int delimiter2);
        
        void load_File_token();

        void testing();
        
        // saving
        void print_flat_E();
        
        E * test(int,std::vector<int>);
        
        E * vE_get_by_index(std::vector<int>&, std::vector<int>::const_iterator);
        void display_vector_int(std::vector<int>& vect_index);
        
        std::vector<int> set_vector_of_indexes(std::string);

        void vE_copy_To_Vector_Float(std::vector<float>& vFloat);
        void extractEColorDataToGL(
                            std::vector<float> &colors,
                            std::vector< vector <float> > &color_faces);
        void extractEVertexToGL(
                std::vector<float> &coordinate,
                std::vector< vector <float> > &vertex,
            std::vector< std::vector< std::vector <float> > > &vCube);
            
        void SearchResultsToVectorFloat(std::vector<float> &v_float, std::vector< vector <float> > &v_v_float);            
};



#endif /*E.h*/
