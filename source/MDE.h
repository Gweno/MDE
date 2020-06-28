//-----------MDE.h-------------
#ifndef MDE_H
#define MDE_H


#include <iostream>
#include <vector>
#include <string>
#include <stack>

using namespace std;
enum MDE_member {name,data};


/**
 * 
 * ##name: GLdata
 * @param level is an integer. It holds the depth level of an instance
 * of MDE within another instance of MDE. 
 * @param index is an integer. It holds the index of the element within
 * the private member vMDE standard vector of pointer to MDE.
 * @param mde_member is an enum MDE_member{name,data}. It holds the
 * about the nature of the data, name for private member 'name', 'data
 * for public member 'data'.
 * @param MDE_data is  standard string. it holds the data itself.
 * @brief This is public structure 'GLdata'. It is used for transferring
 * information to GL functions.
 */
struct GLdata{
    
    int level;
    int index;
    MDE_member mde_member;
    std::string MDE_data;
};

/**
 * This is class MDE.
 * 
 */
class MDE {
    
    private:
    
        // private members
    
        // static member:
        static std::vector<MDE**> search_result;
        
        // non-static members
        std::vector<MDE *> vMDE;
        std::string name;

        
    public:
        //~ static std::vector<MDE**> search_result;

        //public members:
        
         /**
         * 
         * ##name: data
         * @brief This is the public member 'data'. It is holding the
         * information that can be displayed for users.
         */
        std::string data;
        
        //Public member functions:
        //Constructors:
         /**
         * 
         * ##name: MDE()
         * 
         * @param No arguments.
         * @return Nothing is returned (void).
         * @exception No exception handling.
         * @brief This is the basic constructor. Do nothing,
         * apart from creating an empty instance of class MDE.
         */
        MDE();
        
         /**
         * 
         * ##name: MDE(std::string init_name, std::string init_data,
         * std::vector<MDE*> init_vMDE)
         * @param init_name is a standard string
         * @param init_data is a standard string
         * @param init_vMDE is a standard vector of pointers to instances 
         * of object of class MDE.
         * @return Nothing is returned (void).
         * @see MDE().
         * @see ~MDE().
         * @exception No exception handling.
         * @brief Create an instance of class MDE and sets its private
         * member 'name' to standard string init_name, its public member
         * 'data' to standard string init_data and its private member 
         * vMDE, vector of pointers to instance of MDE, to the vector
         * init_vMDE, standard vector of pointer to instance of MDE.
         */
        MDE(std::string init_name, std::string init_data,
        std::vector<MDE*> init_vMDE);
        
        // Destructors and Member functions to clear member variables;

        /** @defgroup group_clear Clear methods
         * Methods to clear the strings 'name' and 'data' in instance of
         * Objects of Class MDE.
         *  @{
         */

         /**
         * 
         * ##name: ~MDE()
         * @param No arguments.
         * @return Nothing is returned (void).
         * @see MDE().
         * @exception No exception handling.
         * @brief This is the destructor member function simply
         * deleteing each pointers from private member vMDE, vector of
         * pointers to MDE.
         */
        ~MDE();

         /**
         * 
         * ##name: destructor_MDE()
         * @param No arguments.
         * @return Nothing is returned (void).
         * @see delete_all_pt().
         * @exception No exception handling.
         * @brief This is function member doing is calling
         * delete_all_pt() to delete all pointers in private member vMDE
         * and clearing members of the instance of object class MDE.
         */        
        public: void destructor_MDE();

         /**
         * 
         * ##name: delete_all_pt()
         * @param No arguments.
         * @return Nothing is returned (void).
         * @see destructor_MDE().
         * @exception No exception handling.
         * @brief This is another destructor member doing exactely the
         * same as~MDE()
         */
        private: void delete_all_pt();
//TODO make sure this delete_all_pt() is necessary        
        
        
        
         /**
         * 
         * ##name: clear_name()
         *  
         * @param No arguments.
         * @return Nothing is returned (void).
         * @see clear_all_vMDE().
         * @see clear_data().
         * @exception No exception handling.
         * @brief This member function is to clear
         * the private member 'name' of an instance of a Class MDE object
         * by setting 'name' as an empty string.
         */
        private: void clear_name();

         /**
         * 
         * ##name: clear_data()
         *  
         * @param No arguments.
         * @return Nothing is returned (void).
         * @see clear_all_vMDE().
         * @see clear_name().
         * @exception No exception handling.
         * @brief This member function is to clear the 
         * public member 'data' of an instance of a Class MDE object
         * by setting 'data' as an empty string.
         */
        private: void clear_data();

         /**
         * 
         * ##name: clear_all_vMDE()
         *  
         * @param No arguments.
         * @return Nothing is returned (void).
         * @see clear_name().
         * @see clear_data().
         * @exception No exception handling.
         * @brief This member function is to clear all entries 
         * for the private member vector vMDE elements of an instance of 
         * a class MDE object recursively.
         */
        private: void clear_all_vMDE();
        
         /**
         * 
         * ##name: clear_all_vMDE_by_index(std::vector<int>& vect_index)
         *  
         * @param *& vect_index* is a reference to a standard vector of integers
         * that holds the sequence of indexes to point to an element of vMDE. 
         * @return Nothing is returned (void).
         * @see clear_all_vMDE().
         * @exception No exception handling.
         * @brief This member function is to clear all entries of the element
         * of the private member vector vMDE of an instance of 
         * a class MDE object recursively using a vector of index pointing
         * to the parent element.
         */
        private: void clear_all_vMDE_by_index(std::vector<int>& vect_index);
        
        
        /** @} */ // end of group_clear

        // member function to create new instance of Class MDE:

         /**
         * 
         * ##name: new_vMDE_element()
         *  
         * @param No arguments.
         * @return Nothing is returned (void).
         * @see clear_all_vMDE().
         * @exception No exception handling.
         * @brief This member function is to create a new pointer to
         * MDE with a push_back on private member vMDE.
         */
        //~ public: void new_vMDE_element();
        public: void new_vMDE_element(std::string name="",std::string data="");

        // SET methods
        /** @defgroup group_set Set methods
         * Methods to set members or instances of MDE and vMDE.
         *  @{
         */
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
         * eName into the private member 'name' of an instance 
         * of an object of class MDE.
         */
        public: void set_name(std::string eName);
        
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
         * of an object of class MDE.
         */
        public: void set_data(std::string data);

         /**
         * 
         * ##name: set_name_vMDE_back(std::string vName)
         *  
         * @param vName is a standard string.
         * @return Nothing is returned (void).
         * @see set_data_vMDE_back().
         * @see set_name().
         * @exception No exception handling.
         * @brief This member function is to set a standard string
         * vName into the private member 'name' of a pointer to an 
         * instance of an object of class MDE, at the back of the private 
         * member vMDE, vector of pointers to instances of class MDE.
         */
        public: void set_name_vMDE_back(std::string vName);
        
         /**
         * 
         * ##name: set_data_vMDE_back(std::string vData)
         * @param vData is a standard string.
         * @return Nothing is returned (void).
         * @see set_name_vMDE_back().
         * @see set_name().
         * @exception No exception handling.
         * @brief This member function is to set a standard string
         * vData into the public member 'data' of a pointer to an 
         * instance of an object of class MDE, at the back of the private 
         * member vMDE, vector of pointers to instances of class MDE.
         */
        public: void set_data_vMDE_back(std::string vData);
        
         /**
         * 
         * ##name: set_name_vMDE_index(int index_vMDEe,string vName)
         *  
         * @param index_vMDEe is an integer.
         * @param vName is a standard string.
         * @return Nothing is returned (void).
         * @see set_name_vMDE_back().
         * @see set_data_vMDE_index().
         * @exception No exception handling.
         * @brief This member function is to set a standard string
         * vName into the private member 'name' of a pointer to an 
         * instance of an object of class MDE, at the integer index number
         * index_vMDEe of the private member vMDE, vector of pointers
         * to instances of class MDE.
         */
        public: void set_name_vMDE_index(int index_vMDEe,std::string vName);

         /**
         * 
         * ##name: set_data_vMDE_index(int index_vMDEe,string vData)
         *  
         * @param index_vMDEe is an integer.
         * @param vData is a standard string.
         * @return Nothing is returned (void).
         * @see set_data_vMDE_back().
         * @see set_name_vMDE_index().
         * @exception No exception handling.
         * @brief This member function is to set a standard string
         * vData into the public member 'data' of a pointer to an 
         * instance of an object of class MDE, at the integer index number
         * index_vMDEe of the private member vMDE, vector of pointers
         * to instances of class MDE.
         */
        public: void set_data_vMDE_index(int index_vMDEe,std::string vData);
        
         /**
         * 
         * ##name: set_vector_of_indexes(std::string list_indexes)
         * @param list_indexes is a standard string.
         * @return A standard vector of integer.
         * @see istringstream().
         * @exception No exception handling.
         * @brief This member function is to convert a string hodling
         * a set of indexes to a standard vector of integer
         * corresponding to the set of indexes to be able to use it.
         * The methods returns the vector of indexes.
         */
        public: std::vector<int> set_vector_of_indexes(
        std::string list_indexes);      //  should not be class MDE

        public: void char_array_to_vMDE_data(int n_element, char **array_char, bool create=true );
        
        
        /** @} */ // end of group_set             

         // GET methods
        /** @defgroup group_get Get methods
         * Methods to get members or instances of MDE and vMDE.
         *  @{
         */

        public: std::string get_name();
         
         /**
         * 
         * ##name: string get_name_vMDE_by_index(int index_vMDEe);
         * 
         * @param index_vMDEe is an integer.
         * @return A standard string is returned.
         * @see set_name_vMDE_index().
         * @see get_data_vMDE_by_index().
         * @see set_data_vMDE_back().
         * @exception No exception handling.
         * @brief This member function is to get the standard string
         * from the private member 'name' of a pointer to an 
         * instance of an object of class MDE, at the integer index number
         * index_vMDEe of the private member vMDE, vector of pointers
         * to instances of class MDE.
         */
         
        public: std::string get_name_vMDE_by_index(int index_vMDEe);
        
         /**
         * 
         * ##name: string get_data_vMDE_by_index(int index_vMDEe);
         * 
         * @param index_vMDEe is an integer.
         * @return A standard string is returned.
         * @see set_data_vMDE_back().
         * @see get_name_vMDE_by_index().
         * @see set_name_vMDE_index().
         * @exception No exception handling.
         * @brief This member function is to get the standard string
         * from the public member 'data' of a pointer to an 
         * instance of an object of class MDE, at the integer index number
         * index_vMDEe of the private member vMDE, vector of pointers
         * to instances of class MDE.
         */
        public: std::string get_data_vMDE_by_index(int index_vMDEe);
        
         /**
         * 
         * ##name: vMDE_get_by_index(std::vector<int>& vect_index,
         * std::vector<int>::const_iterator it)
         * @param N & vect_index is a reference to a vector of integer
         * holding the indexes of the instance of MDE to return.
         * @param const_iterator i is a cosntant iterator of a vector of
         * integer, used for the recursive iteration while browsing
         * through the called instance of MDE.
         * @return MDE * : a pointer to an existing instance of MDE.
         * @exception No exception handling.
         * @brief This member function is to return the pointer to
         * the instance of MDE that is at a given set of indexes.
         */
        public: MDE * vMDE_get_by_index(std::vector<int>& vect_index,
        std::vector<int>::const_iterator it);

        // simplified version of vMDE_get_by_index
        public: MDE * get_vMDE(std::vector<int> vect_index);
 
        //~ public: std::vector<MDE *> get_vMDE_test(std::vector<int> vect_index, std::vector<int>::const_iterator it);
        //public: std::vector<MDE *> get_vMDE_test(std::vector<int> vect_index, uint it);
        //~ public: std::vector<MDE *> get_vMDE_test(std::vector<int> vect_index);
        
        /** @} */ // end of group_get
        
        
        
        // Member functions to display member variables;

        /** @defgroup group_display Display methods
         * Methods to display Instances, part of instances or
         * anything else, in the terminal. Mainly for debugging purposes
         *  @{
         */

         /**
         * 
         * ##name: display_name()
         *  
         * @param No arguments.
         * @return Nothing is returned (void).
         * @see display_data().
         * @see display_vMDE_names().
         * @see display_vMDE_data().
         * @exception No exception handling.
         * @brief This member function is to display
         * the private member 'name' of an instance of a class MDE object,
         * using a standard cout.
         */
        public: void display_name();
        
         /**
         * 
         * ##name: display_data()
         *  
         * @param No arguments.
         * @return Nothing is returned (void).
         * @see display_name().
         * @see display_vMDE_data().
         * @see display_vMDE_names().
         * @exception No exception handling.
         * @brief This member function is to display
         * the public member 'data' of an instance of a class MDE object,
         * using a standard cout. 
         */
        public: void display_data();
                        
         /**
         * 
         * ##name: display_vMDE_names()
         *  
         * @param No arguments.
         * @return Nothing is returned (void).
         * @see display_name().
         * @see display_vMDE_data().
         * @see vdisplay_all().
         * @exception No exception handling.
         * @brief This member function is to display private member
         * 'name' of all elements of the private member vMDE,
         * vector of pointers to instances of object of class MDE
         * using a standard cout recursively.
         */
        public: void display_vMDE_names();
        
         /**
         * 
         * ##name: display_vMDE_data()
         *  
         * @param No arguments.
         * @return Nothing is returned (void).
         * @see display_name().
         * @see display_vMDE_names().
         * @see vdisplay_all().
         * @exception No exception handling.
         * @brief This member function is to display public member
         * 'data' of all elements of the private member vMDE,
         * vector of pointers to instances of object of class MDE
         * using a standard cout recursively.
         */
        public: void display_vMDE_data();

         /**
         * 
         * ##name: display_all(int n_indent, const int & n_space)
         *  
         * @param n_indent is an integer passed to initialised the
         * position of the first line to display, then to be incremented
         * locally recursively.
         * @param n_space is an address of a constant integer used
         * to indent each lines to separates elements of intances
         * of object of class MDE from their own sub-elements recursively.
         * @return Nothing is returned (void).
         * @see display_name().
         * @see display_data().
         * @see display_vMDE_names().
         * @see display_vMDE_data().
         * @exception No exception handling.
         * @brief This member function is to display all members
         * 'name'(private) and 'data'(public) of all private members vMDE vectors
         * of pointers to instances of object of class MDE, using 
         * a standard cout recursively, and with indentation.
         */
        public: void display_all(int n_indent, const int & n_space);
        
         /**
         * 
         * ##name: format_display(int n_indent, const int & n_space,
            std::string start_opening_tag,
            std::string end_opening_tag, std::string start_closing_tag,
            std::string end_closing_tag)
         * @param n_indent is an integer passed to initialised the
         * position of the first line to display, then to be incremented
         * locally recursively.
         * @param n_space is an address of a constant integer used
         * to indent each lines to separates elements of intances
         * of object of class MDE from their own sub-elements recursively.
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
         * @return Nothing is returned (void).
         * @see user_save_to_XML_file()
         * @exception No exception handling.
         * @brief This member function is to display the instance
         * of a class object MDE with a defined format using opening and 
         * closing tags (for example like an XML file).
         */        
        public: void format_display(int n_indent, const int & n_space,
            std::string start_opening_tag,
            std::string end_opening_tag, std::string start_closing_tag,
            std::string end_closing_tag);
        
         /**
         * 
         * ##name: display_vector_int(std::vector<int>& vect_index)
         *  
         * @param & vect_index is a reference to a vector of integer
         * holding the set of indexes for an Entity. The display is
         * in the format (i0,i1,..,in) with each i being the 
         * consecutive indexes of instance member vMDE.
         * @return Nothing is returned (void).
         * @exception No exception handling.
         * @brief This member function is to display all members
         * 'name'(private) and 'data'(public) of all private members vMDE vectors
         * of pointers to instances of object of class MDE, using 
         * a standard cout recursively, and with indentation.
         */
        public: void display_vector_int(std::vector<int>& vect_index);      //  should not be class MDE

         /**
         * 
         * ##name: print_flat_MDE()
         * @param No arguments.
         * @return Nothing is returned (void).
         * @see set_name_vMDE_index().
         * @see set_data_vMDE_index().
         * @exception No exception handling.
         * @brief This function member is to display the Entity and
         * all its sub-entities at the same level.
         * Currently not used.
         */
        private: void print_flat_MDE();
        
         /**
         * 
         * ##name: display_v_MDE_data(std::vector<GLdata> & v_MDE_data)
         * @param & v_MDE_data reference to a standard vector of type
         * struct GLdata.
         * @return Nothing is returned (void).
         * @see set_name_vMDE_index().
         * @see set_data_vMDE_index().
         * @exception No exception handling.
         * @brief This function member is to display all elements of a
         * standard vector of struct GLdata. For example it can displays
         * a vector of offset values for coordinates.
         */
        public: void display_v_MDE_data(std::vector<GLdata> & v_MDE_data);
        
        /** @} */ // end of group_display
        
        // user interface methods
        //TODO improve user_input_vMDE() by using vector iterator instead of index
        
         /**
         * 
         * ##name: user_input_vMDE()
         * 
         * @param No arguments.
         * @return Nothing is returned (void).
         * @see set_name_vMDE_index().
         * @see set_data_vMDE_index().
         * @exception No exception handling.
         * @brief This function member is for user to enter manually
         * 'name' and 'data' recursively for all sub-entities
         * of the instance MDE until 'end' is entered.
         */
        public: void user_input_vMDE();
         
                
        // I/O methods
        
        /** @defgroup group_io I/O methods
         * Input/Output methods including and other useful methods part
         * of their process.
         *  @{
         */
                 
         /**
         * 
         * ##name: save_to_file()
         * @param start_opening_tag is a standard string corresponding
         * to the start of the opening tag ("<" for xml).
         * @param end_opening_tag is a standard string corresponding
         * to the end of the opening tag (">" for xml).
         * @param start_closing_tag is a standard string corresponding
         * to the start of the closing tag ("<\" for xml).
         * @param end_closing_tag is a standard string corresponding
         * to the end of the closing tag (">" for xml).
         * @param &file (reference to a file) is an ostream
         * corresponding to the name of the file .
         * @return Nothing is returned (void).
         * @see user_save_to_XML_file().
         * @exception No exception handling.
         * @brief This member function is to save the instance of
         * a class into an output file with opening and closing tags
         * for example like an xml file.
         * 
         */
        private: void save_to_file(int n_indent, const int & n_space,
            std::string start_opening_tag,
            std::string end_opening_tag, std::string start_closing_tag,
            std::string end_closing_tag, ostream &file);
            
         /**
         * 
         * ##name: user_save_to_XML_file()
         * @param No parameter.
         * @return Nothing is returned (void).
         * @see save_to_file()
         * @exception No exception handling.
         * @brief This member function is to save the instance
         * of a class into an XML file.
         * The file's name is chosen by user input.
         */
        public: void user_save_to_XML_file();
        
         /**
         * 
         * ##name: load_XML_File_to_MDE()
         * @param full_filename is a standard string.
         * @return A boolean value true if file exists and is open,
         * or false if file does not exist or failed to open.
         * @exception No exception handling.
         * @brief This member function is to read an XML file using the 
         * reference to the string full_filename and
         * to store the data from the file into an instance of class MDE.
         * 
         * @note The process is using a standard stack of pointers to
         * pointers of Instances of Objects Class MDE (std::stack<MDE**>).
         */
        public: bool load_XML_File_to_MDE(const std::string & full_filename);

        /**
         * 
         * ##name: load_XML_File_to_vMDE(int & index,
         * ## const std::string & full_filename)
         * @param  & index is a reference to an integer that holds the
         * vector index of the private member vMDE where to load the XML
         * file.
         * struct GLdata.
         * @param  & full_filename is a reference to the path and name
         * of the file to load.
         * @return a bool is returned as false if fail to load or
         * true if the file is loaded successfully.
         * @see load_XML_File_to_MDE()
         * @see replace_vMDE_with_XML()
         * @exception No exception handling.
         * @brief This member function is to read an XML file using the 
         * reference to the string full_filename and
         * to store the data from the file into an element 
         * at a given index of the  private member vMDE 
         * of the instance of a class of MDE.
         */
        public: bool load_XML_File_to_vMDE(int & index, const std::string & full_filename);
        public: bool load_XML_File_to_vMDE_old(int & index, const std::string & full_filename);

        /**
         * 
         * ##name: replace_vMDE_with_XML(
         * std::vector<int> & vect_index_file,
         * const std::string & full_filename)
         * @param  & index is a reference to an integer that holds the
         * vector index of the private member vMDE where to load the XML
         * file.
         * @param  & full_filename is a reference to the path and name
         * of the file to load.
         * @return a bool is returned as false if fail to load or
         * true if the file is loaded successfully.
         * @see load_XML_File_to_vMDE()
         * @see load_XML_File_to_MDE()
         * @exception No exception handling.
         * @brief This member function is to read an XML file using the 
         * reference to the string full_filename and
         * to store the data from the file into an element 
         * at a given index of the  private member vMDE 
         * of the instance of a class of MDE by replacing completely
         * the existing element.
         */
        public: bool replace_vMDE_with_XML(std::vector<int> & vect_index_file, const std::string & full_filename);

        /**
         * 
         * ##name: str_token_tag(std::string & buffer_Input
        , std::string & output_Tag, std::string & buffer_Output
        , char delimiter1, char delimiter2)
         * @param & buffer_Input is a reference to a standard string
         * that holds the line input
         * @param output_Tag is a standard string for
         * the token extracted
         * @param buffer_Output is a standard string for
         * the remainder of the line input minus the token extracted
         * and after the delimiter2
         * @param delimiter1 is an char corresponding to the char used
         * as the front delimiter for the token
         * @param delimiter2 is an char corresponding to the char used
         * as the end delimiter for the token
         * @return Nothing is returned (void)
         * @brief This member function is to read an XML file and
         * store it into an instance of class  MDE. 
         */
        private: void str_token_tag(std::string & buffer_Input
        , std::string & output_Tag, std::string & buffer_Output
        , char delimiter1, char delimiter2);


        /**
         * 
         * ##name: trim_leading_space(std::string & io_string)
         * @param  & io_string is a reference to a standard string
         * that holds the i/o string to be trimed
         * @return Nothing is returned (void)
         * @see str_token_tag()
         * @exception No exception handling.
         * @brief This member function is to trim the leading space
         * (space charaters at the start) of the io_string parameter
         * passed as argument for this member function.
         */
        private: void trim_leading_space(std::string & io_string);


        /**
         * 
         * ##name: extract_File_to_Flat_MDE(
         * const std::string & full_filename)
         * @param  & full_filename is a reference to a standard string
         * that holds the the name of a file to extract.
         * @return A boolean is returned.
         * @see load_XML_File_to_MDE()
         * @see str_token_tag().
         * @exception Standard function ifstream::is_open() is used to
         * return value true if file exists and is open, or false
         * if file does not exist or failed to open.
         * @brief This member function is to extract data from an xml
         * file and store it in an Entity with all sub-entities at the
         * same level and the 'name' member sets to 'name' or 'data'
         * accordingly to wether it's a 'name' or 'data' member.
         * (space charaters at the start) of the io_string parameter
         * @note This function can be optimised.
         */
        private: bool extract_File_to_Flat_MDE(const std::string & full_filename);

        /**
         * 
         * ##name: process_flat_MDE_to_MDE(std::vector<MDE*>::iterator & it,
         * MDE & MDE_source, std::stack<MDE**> & stack_MDE_pt)
         * @param  & it is a reference to an iterator of a standard
         * vector of pointers to instances of MDE.
         * @param  & MDE_source is a reference to an instance of MDE.
         * @param  & stack_MDE_pt is a reference to an standard stack of 
         * pointers to instances of MDE.
         * @return MDE *, a pointer to an new instance of MDE.
         * @see load_XML_File_to_MDE()
         * @see proc_it()
         * @exception No exception handling.
         * @brief This member function is to process an instnace of a
         * 'flat' Entity and return a pointer to a new instance of MDE
         * that has nested sub-entities.
         * The algorhythm checks what is the data, if it is a structural
         * data like an opening tag or closing tag it creates or ends
         * the sub-entities accordingly.
         * @note This function could be optimised. This function is
         * complex and does contain a key algorhythm for building an
         * Entity MDE from a flat source.
         */
        private: MDE * process_flat_MDE_to_MDE(std::vector<MDE*>::iterator & it,
        MDE & MDE_source, std::stack<MDE**> & stack_MDE_pt);
        
        /**
         * 
         * ##name: check_token(onst std::string & token,
         *  std::string & element)
         * @param  & token is a reference to a standard string.
         * @param  & element is a reference to a standard string.
         * @return Nothing is returned (void)
         * @see load_XML_File_to_MDE()
         * @see extract_File_to_Flat_MDE()
         * @exception No exception handling.
         * @brief This member function is to process a token extracted
         * from a file as a tag and checks if it is an 'opening_tag' or
         * a 'closing_tag'.
         * It changes the parameter element accordingly.
         */
        private: void check_token(const std::string & token,
        std::string & element);
        
        private: void split_string(const std::string & tag, std::vector<std::string> & tokens);
        
        /**
         * 
         * ##name: search_results_to_vector_float(
         * std::vector<float> &v_float,
         * std::vector< vector <float> > &v_v_float)
         * @param  &v_float is a reference to a standard vector of
         * float.
         * @param   &v_v_float is a reference to a standard vector of
         * vector of float.
         * @return Nothing is returned (void)
         * @see vMDE_copy_To_Vector_Float()
         * @see search_for()
         * @exception No exception handling.
         * @brief This member function is to process the static(class)
         * member search_result and to store the corresponding public
         * 'data' member for each pointer to instances of MDE that are in
         * the search_result. It uses vMDE_copy_To_Vector_Float() method
         * that cast number strings to float.
         * @note Process could possibly be simplified.
         */
        private: void search_results_to_vector_float(std::vector<float> &v_float,
        std::vector< vector <float> > &v_v_float);            

        /**
         * 
         * ##name: vMDE_copy_To_Vector_Float(std::vector<float>& vFloat)
         * @param  &v_float is a reference to a standard vector of
         * float.
         * @return Nothing is returned (void)
         * @see search_results_to_vector_float()
         * @see search_for()
         * @exception No exception handling.
         * @brief This member function is to process the private member
         * vMDE, hodling vector of pointers to MDE, and cast the public
         * 'data' members of each elements of vMDE from string to float.
         */
        private: void vMDE_copy_To_Vector_Float(std::vector<float>& vFloat);

        /**
         * 
         * ##name: extract_MDE_data_for_v_offset(
         * std::vector<GLdata> & v_MDE_data, int & level, int index = 0)
         * @param  & v_MDE_data is a reference to a standard vector of
         * struct GLdata.
         * @param  & level is a reference to an int.
         * @param  & index is a reference to an int.
         * @return Nothing is returned (void)
         * @see search_results_to_vector_float()
         * @see search_for()
         * @exception No exception handling.
         * @brief This member function is to fill-in a standard vector
         * of struct GLdata by extracting data from private member
         * 'name' and public member 'data', and setting the level
         * and index of each Entities and sub-entities of an instance of
         * object class MDE. The vector of GLdata is used in the standard
         * vector of GLdata to set offset coordinates.
         * @note This method can be optimised and may be able to use
         * other existing methods.
         */
        public: void extract_MDE_data_for_v_offset(std::vector<GLdata> & v_MDE_data,
        int & level, int index = 0);

        /** @} */ // end of group_io
        

        
        
        // Testing
        /** @defgroup group_testing Testing methods
         * Testing methods currently in test or to help with testing.
         * For example methods to create testing sets.
         * Some methods are listed but not documented.
         *  @{
         */
        
        /**
         * 
         * ##name: testing()
         * @return Nothing is returned (void)
         * @exception No exception handling.
         * @brief This member function is to create at runtime
         * a new Entity with sub-entities for testing purposes.
         */
        public: void testing();
         
        /** @} */ // end of group_testing
        
        // X
        /** @defgroup group_search Searching methods
         * Searching methods and associated tools that are used to
         * search for members or instances using different process.
         *  @{
         */
         
        /**
         * 
         * ##name: search_For(int & index, int & level,
         * std::string searchName)
         * @param  & index is a reference to an integer.
         * @param  & level is a reference to an integer.
         * @param  searchName is a standard string.
         * @return Nothing is returned (void)
         * @see simple_loop_VMDE()
         * @see simple_loop_VMDE_next_level()
         * @see fetch_search_result()
         * @exception No exception handling.
         * @brief This member function is to search an entire Entity
         * recursively all instances that has their name equal to the
         * standard string searchName in parameter. It stores all
         * the results in the static (class) member search_result which
         * is a standard vector of pointers to pointers of MDE.
         * The results of the search can be displayed with method
         * fetch_search_result.
         * @note The process can be optimised and simplified.
         */
        public: void search_For(int & index, int & level, std::string searchName, std::vector<MDE**> &search_result = MDE::search_result);

        /**
         * 
         * ##name: simple_loop_VMDE(std::string searchName)
         * @param  searchName is a standard string.
         * @return Nothing is returned (void)
         * @see search_For()
         * @see simple_loop_VMDE_next_level()
         * @exception No exception handling.
         * @brief This member function is to part of the recursive
         * algorhythm in the search_for method. It recursively stores
         * pointers to pointer to MDE in the static (class) member
         * search_result when the search is successful. It calls a
         * similar methods called simple_loop_VMDE_next_level in its
         * recursive process to separate scopes of the different
         * sub-entities of an entity.
         */
        private: void simple_loop_VMDE(std::string searchName, std::vector<MDE**> &search_result = MDE::search_result);
        
        /**
         * 
         * ##name: simple_loop_VMDE_next_level(std::string searchName)
         * @param  searchName is a standard string.
         * @return Nothing is returned (void)
         * @see search_For()
         * @see simple_loop_VMDE()
         * @exception No exception handling.
         * @brief This member function is to part of the recursive
         * algorhythm in the search_for method. It is used to 
         * separate scopes of the different sub-entities of an entity
         * to perform the search.
         */
        private: void simple_loop_VMDE_next_level(std::string searchName, std::vector<MDE**> &search_result = MDE::search_result);

        /**
         * 
         * ##name: fetch_search_result(std::vector<MDE**> &search_result)
         * @param  &search_result is a reference to a standard vector
         * of pointers to pointer of MDE.
         * @return Nothing is returned (void)
         * @see search_For()
         * @exception No exception handling.
         * @brief This member function is to fetch the searching results
         * store a vector of pointers to pointer of MDE, typically in
         * the static (class) member search_result after running method
         * search_for() to search for all Entities with a given member
         * 'name'.
         */
        public: void fetch_search_result(std::vector<MDE**> &search_result = MDE::search_result);

        public: void get_vMDE_test2(int index, std::vector<MDE**> &pt_vMDE = MDE::search_result);

        public: void loop_test2(std::vector<int> & vect_index, int & index, std::vector<MDE**> & pt_vMDE = MDE::search_result);

        public: std::vector<MDE*> loop_test3(std::vector<int> & vect_index, int & index);
        /** @} */ // end of group_search
        
        
        // X
        /** @defgroup group_X X methods
         * X methods ...
         *  @{
         */
        
        /** @} */ // end of group_X

        
};



#endif /*MDE.h*/


//~ /*! \mainpage Multidimensional Data Entity - Display
 //~ *
 //~ * \section intro_sec MDE in a glance
 //~ *
 //~ * An MDE can be used for multiple purposes.
 //~ * In this first version of MDE, an MDE instance is simply used to
 //~ * display the content of an xml file in text mode or
 //~ * in an OpenGL 3D viewport.
 //~ *
 //~ * \section install_sec Installation
 //~ *
 //~ * \subsection step1 Step 1: Verify dependencies
 //~ * List of libraries you need to have already installed:
 //~ * <ul>
 //~ *  <li> OpenGL
 //~ *  <li> glut
 //~ *  <li> freeglut
 //~ *  <li> glew
 //~ *  <li> freetype2
 //~ *  <li> glm
 //~ *  </ul>
 //~ * 
 //~ * \subsection step2 Step 2: external resources
 //~ * There are currently  external resources used by this program on top
 //~ *  of the dependancies:
 //~ * <ul>
 //~ *  <li> <b>./source/common/</b> has some useful functions for shader
 //~ * programs from the OpenGL Programming wikibook:
 //~ * http://en.wikibooks.org/wiki/OpenGL_Programming.
 //~ *  <li> <b>./fonts/</b> has a few free/opensource font files used
 //~ *  by the freetype2 library.
 //~ *  </ul>
 //~ * \subsection step3 Step 3: make
 //~ * Makefile is in the <b>source</b> folder.
 //~ * The binary file is output in <b>bin</b> folder
 //~ * 
 //~ * \subsection step4 Step 4: run program
 //~ * 
 //~ * From folder <b>bin</b> execute ./mde [OPTION]
 //~ * 
 //~ * Options:
 //~ * 
 //~ *  <ul>
 //~ * 
 //~ *  <li> <b>no option</b> : if the program is executed with no options,
 //~ * all text mode menus will be displayed consecutively in the console.
 //~ *  <li> <b>-g</b>
 //~ *     display directly in openGL, skip the Display Mode menu
 //~ *  <li> <b>-f [FILE]</b>
 //~ *     load (xml) file, skip the Data menu
 //~ *  </ul>
 //~ * 
 //~ */
