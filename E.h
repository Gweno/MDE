//-----------E.h-------------
#ifndef E_H
#define E_H


#include <iostream>
#include <vector>
#include <string>
#include <stack>
//~ #include <stdexcept>




using namespace std;

class E {
    
    private:
    
        // private members
        std::vector<E *> vE;
        
    public:

        //public members:
        
        std::string name;
        std::string data;
        
        
        //static member:
        static std::vector<E**> search_result;
        
        //Public member functions:
        //Constructors:
         /**
         * 
         * ##name: E()
         * 
         * @param No arguments.
         * @return Nothing is returned (void).
         * @exception No exception handling.
         * @brief This is the basic constructor. Do nothing,
         * apart from creating an empty instance of class E.
         */
        E();
        
         /**
         * 
         * ##name: E(std::string init_name, std::string init_data,
         * std::vector<E*> init_vE)
         * 
         * @param init_name is a standard string
         * @param init_data is a standard string
         * @param init_vE is a standard vector of pointers to instances 
         * of object of class E.
         * @return Nothing is returned (void).
         * @see E().
         * @see ~E().
         * @exception No exception handling.
         * @brief Create an instance of class E and sets its public
         * member 'name' to standard string init_name, its public member
         * 'data' to standard string init_data and its private member 
         * vE, vector of pointers to instance of E, to the vector
         * init_vE, standard vector of pointer to instance of E.
         */
        E(std::string init_name, std::string init_data, std::vector<E*> init_vE);
        
        //Destructors;

         /**
         * 
         * ##name: ~E()
         * 
         * @param No arguments.
         * @return Nothing is returned (void).
         * @see E().
         * @exception No exception handling.
         * @brief This is the destructor member function simply deleteing the pointer.
         */
        ~E();
        
        // member functione to create new Entity:

         /**
         * 
         * ##name: new_vE_element()
         *  
         * @param No arguments.
         * @return Nothing is returned (void).
         * @see clear_all_vE().
         * @exception No exception handling.
         * @brief This member function is to create a new pointer to E with a push_back on private member vE.
         */
        void new_vE_element();

         /**
         * 
         * ##name: set_name(std::string eName)
         *  
         * @param eName is a standard string.
         * @return Nothing is returned (void).
         * @see clear_name().
         * @see set_data().
         * @exception No exception handling.
         * @brief This member function is to set a standard string
         * eName into the public member 'name' of an instance 
         * of an object of class E.
         */
        void set_name(std::string eName);
        
         /**
         * 
         * ##name: set_data(std::string data)
         *  
         * @param data is a standard string.
         * @return Nothing is returned (void).
         * @see clear_data().
         * @see set_name().
         * @exception No exception handling.
         * @brief This member function is to set a standard string
         * data into the public member 'data' of an instance 
         * of an object of class E.
         */
        void set_data(std::string data);

         /**
         * 
         * ##name: set_name_vE_back(std::string vName)
         *  
         * @param vName is a standard string.
         * @return Nothing is returned (void).
         * @see set_data_vE_back().
         * @see set_name().
         * @exception No exception handling.
         * @brief This member function is to set a standard string
         * vName into the public member 'name' of a pointer to an 
         * instance of an object of class E, at the back of the private 
         * member vE, vector of pointers to instances of class E.
         */
        void set_name_vE_back(std::string vName);
        
         /**
         * 
         * ##name: set_data_vE_back(std::string vData)
         *  
         * @param vData is a standard string.
         * @return Nothing is returned (void).
         * @see set_name_vE_back().
         * @see set_name().
         * @exception No exception handling.
         * @brief This member function is to set a standard string
         * vData into the public member 'data' of a pointer to an 
         * instance of an object of class E, at the back of the private 
         * member vE, vector of pointers to instances of class E.
         */
        void set_data_vE_back(std::string vData);
        
         /**
         * 
         * ##name: set_name_vE_index(int index_vEe,string vName)
         *  
         * @param index_vEe is an integer.
         * @param vName is a standard string.
         * @return Nothing is returned (void).
         * @see set_name_vE_back().
         * @see set_data_vE_index().
         * @exception No exception handling.
         * @brief This member function is to set a standard string
         * vName into the public member 'name' of a pointer to an 
         * instance of an object of class E, at the integer index number
         * index_vEe of the private member vE, vector of pointers
         * to instances of class E.
         */
        void set_name_vE_index(int index_vEe,string vName);
        
        std::string get_name_vE_by_index(int index_vEe);

         /**
         * 
         * ##name: set_data_vE_index(int index_vEe,string vData)
         *  
         * @param index_vEe is an integer.
         * @param vData is a standard string.
         * @return Nothing is returned (void).
         * @see set_data_vE_back().
         * @see set_name_vE_index().
         * @exception No exception handling.
         * @brief This member function is to set a standard string
         * vData into the public member 'data' of a pointer to an 
         * instance of an object of class E, at the integer index number
         * index_vEe of the private member vE, vector of pointers
         * to instances of class E.
         */
        void set_data_vE_index(int index_vEe,string vData);

        
        // Member functions to clear member variables;

         /**
         * 
         * ##name: clear_name()
         *  
         * @param No arguments.
         * @return Nothing is returned (void).
         * @see clear_all_vE().
         * @see clear_data().
         * @exception No exception handling.
         * @brief This member function is to clear
         * the public member 'name' of an instance of a Class E object
         * by setting 'name' as an empty string.
         */
        void clear_name();

         /**
         * 
         * ##name: clear_data()
         *  
         * @param No arguments.
         * @return Nothing is returned (void).
         * @see clear_all_vE().
         * @see clear_name().
         * @exception No exception handling.
         * @brief This member function is to clear the 
         * public member 'data' of an instance of a Class E object
         * by setting 'data' as an empty string.
         */
        void clear_data();

         /**
         * 
         * ##name: clear_all_vE()
         *  
         * @param No arguments.
         * @return Nothing is returned (void).
         * @see clear_name().
         * @see clear_data().
         * @exception No exception handling.
         * @brief This member function is to clear all entries 
         * for the private member vector vE elements of an instance of 
         * a class E object recursively.
         */
        void clear_all_vE();
        
        // Member functions to display member variables;
        
         /**
         * 
         * ##name: display_name()
         *  
         * @param No arguments.
         * @return Nothing is returned (void).
         * @see display_data().
         * @see display_vE_names().
         * @see display_vE_data().
         * @exception No exception handling.
         * @brief This member function is to display
         * the public member 'name' of an instance of a class E object,
         * using a standard cout.
         */
        void display_name();
        
         /**
         * 
         * ##name: display_data()
         *  
         * @param No arguments.
         * @return Nothing is returned (void).
         * @see display_name().
         * @see display_vE_data().
         * @see display_vE_names().
         * @exception No exception handling.
         * @brief This member function is to display
         * the public member 'data' of an instance of a class E object,
         * using a standard cout. 
         */
        void display_data();
                        
         /**
         * 
         * ##name: display_vE_names()
         *  
         * @param No arguments.
         * @return Nothing is returned (void).
         * @see display_name().
         * @see display_vE_data().
         * @see vdisplay_all().
         * @exception No exception handling.
         * @brief This member function is to display public member
         * 'name' of all elements of the private member vE,
         * vector of pointers to instances of object of class E
         * using a standard cout recursively.
         */
        void display_vE_names();
        
         /**
         * 
         * ##name: display_vE_data()
         *  
         * @param No arguments.
         * @return Nothing is returned (void).
         * @see display_name().
         * @see display_vE_names().
         * @see vdisplay_all().
         * @exception No exception handling.
         * @brief This member function is to display public member
         * 'data' of all elements of the private member vE,
         * vector of pointers to instances of object of class E
         * using a standard cout recursively.
         */
        void display_vE_data();

         /**
         * 
         * ##name: display_all(int n_indent, const int & n_space)
         *  
         * @param n_indent is an integer passed to initialised the
         * position of the first line to display, then to be incremented
         * locally recursively.
         * @param n_space is an address of a constant integer used
         * to indent each lines to separates elements of intances
         * of object of class E from their own sub-elements recursively.
         * @return Nothing is returned (void).
         * @see display_name().
         * @see display_data().
         * @see display_vE_names().
         * @see display_vE_data().
         * @exception No exception handling.
         * @brief This member function is to display all public members
         * 'name' and 'data' of all private members vE vectors
         * of pointers to instances of object of class E, using 
         * a standard cout recursively, and with indentation.
         */
        void display_all(int n_indent, const int & n_space);
        
         /**
         * 
         * ##name: format_display(int n_indent, const int & n_space, std::string start_opening_tag,
            std::string end_opening_tag, std::string start_closing_tag,
            std::string end_closing_tag)
         * @param n_indent is an integer passed to initialised the
         * position of the first line to display, then to be incremented
         * locally recursively.
         * @param n_space is an address of a constant integer used
         * to indent each lines to separates elements of intances
         * of object of class E from their own sub-elements recursively.
         * @param start_opening_tag is a standard string corresponding 
         * to the start of the opening tag ("<" for xml).
         * @param end_opening_tag is a standard string corresponding
         *  to the end of the opening tag (">" for xml).
         * @param start_closing_tag is a standard string corresponding
         *  to the start of the closing tag ("<\" for xml).
         * @param end_closing_tag is a standard string corresponding
         *  to the end of the closing tag (">" for xml).
         * @param &file (address of a file) is an ostream corresponding
         *  to the name of the file .
         *  
         * @return Nothing is returned (void)
         * @see E_save_to_file()
         * @exception
         * @brief This member function is to save the instance of a class into an XML file.
         */        
        
        void format_display(int n_indent, const int & n_space, std::string start_opening_tag,
            std::string end_opening_tag, std::string start_closing_tag,
            std::string end_closing_tag);
        
        //TODO improve user_input_V() by using vector iterator instead of index
         /**
         * 
         * ##name: user_input_V()
         * 
         * @param No arguments.
         * @return Nothing is returned (void).
         * @see set_name_vE_index().
         * @see set_data_vE_index().
         * @exception No exception handling.
         * @brief This function member is for user to enter manually
         * 'name' and 'data' of the main instance of E and recursively
         * all sub-entities until 'end' is entered.
         * The V is for Vertically because user enters higher level of
         * instances of E first, then sub-entities for each instances.
         */
        void user_input_V();
        
         /**
         * 
         * ##name: E_write_to_file()
         * @param start_opening_tag is a standard string corresponding to the start of the opening tag ("<" for xml).
         * @param end_opening_tag is a standard string corresponding to the end of the opening tag (">" for xml).
         * @param start_closing_tag is a standard string corresponding to the start of the closing tag ("<\" for xml).
         * @param end_closing_tag is a standard string corresponding to the end of the closing tag (">" for xml).
         * @param &file (address of a file) is an ostream corresponding to the name of the file .
         * @return Nothing is returned (void)
         * @see E_save_to_file()
         * @exception No exception handling.
         * @brief This member function is to save the instance of a class into an XML file.
         * 
         */
         
        void E_write_to_file(int n_indent, const int & n_space, std::string start_opening_tag,
            std::string end_opening_tag, std::string start_closing_tag,
            std::string end_closing_tag, ostream &file);
        
        void fetch_search_result(std::vector<E**> &search_result);

        void search_For(int & index, int & level, string searchName);

        void simple_loop_VE(string searchName);
        
        void simple_loop_VE_next_level(string searchName);
        
        
    
        void try_copy(int index);
        
            
         /**
         * 
         * ##name: E_save_to_file()
         * @param No parameter
         *  
         * @return Nothing is returned (void)
         * 
         * @brief This member function is to save the instance of a class into an XML file.
         * 
         * The file's name is chosen by user input.
         *  
         * @see E_write_to_file()
         */
        void E_save_to_file();
        
         /**
         * 
         * ##name: load_XML_File_to_E()
         * @param fullFileName is a standard string
         *  
         * @return Nothing is returned (void)
         * 
         * @brief This member function is to read an XML file and store it into an instance of class  E.
         * 
         * @see str_token_tag()
         */
        void load_XML_File_to_E(const std::string & fullFileName);

        /**
         * 
         * ##name: str_token_tag()
         * @param buffer_Input is is a standard string that holds the line input
         * @param output_Tag is a standard string for the token extracted
         * @param buffer_Output is a standard string for the remainder of the line input minus the token extracted and after the delimiter2
         * @param delimiter1 is an char corresponding to the char used as the front delimiter for the token
         * @param delimiter2 is an char corresponding to the char used as the end delimiter for the token
         * 
         * @return void
         * 
         */
        void str_token_tag(std::string & buffer_Input
        , std::string & output_Tag, std::string & buffer_Output
        , char delimiter1, char delimiter2);

        void trim_leading_space(std::string & io_string);

        void testing();
        
        // saving
        void print_flat_E();
        E * vE_get_by_index(std::vector<int>&, std::vector<int>::const_iterator);
        void display_vector_int(std::vector<int>& vect_index);      //  should not be class E
        
        std::vector<int> set_vector_of_indexes(std::string);      //  should not be class E

        void vE_copy_To_Vector_Float(std::vector<float>& vFloat);
        void extractEColorDataToGL(
                            std::vector<float> &colors,
                            std::vector< vector <float> > &color_faces);
        void extractEVertexToGL(
            std::vector<float> &coordinate,
            std::vector< vector <float> > &vertex,
            std::vector< std::vector< std::vector <float> > > &vCube);
            
        void SearchResultsToVectorFloat(std::vector<float> &v_float, std::vector< vector <float> > &v_v_float);            
        void extract_File_to_Flat_E(const std::string & fullFileName);
        void check_token(const std::string & token, std::string & element);
        void process_rule(E * E_to_process);
        E * flat_E_to_E(E * & vector_to_process, std::vector<E *>::iterator it);
        E * process_flat_E_to_E(std::vector<E*>::iterator & it, E & E_source, std::stack<E**> & stack_E_pt);
        void proc_it(E & E_proc, std::stack<E**> & stack_E_pt);
        void print_stack(std::stack<E**> & stack);
        void delete_all_pt();
        void destructor_E();

};



#endif /*E.h*/
