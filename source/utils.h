//-----------Utils.h-------------
#ifndef UTILS_H
#define UTILS_H

#include<string>
#include "MDE.h"


/**
 * 
 * @file utils.h
 * 
*/

//~ enum MDE_member {name,data};

        // all utilitary functions
        /** @defgroup group_utils Functions Utils
         * Functions Utils is not a class but a collection of utilitary
         * functions to manipulate MDE instances, like loading from an 
         * XML file, or saving to an XML file.
         *  @{
         */
        
struct singleMember{
    
    MDE_member mde_member;
    std::string MDE_data;
    
};

struct pairKeyValue{
    
    singleMember key;
    singleMember value;
};

// Struct for htmlParser state management
struct HtmlParseState {
    bool container = false;
    bool subtags = false;
    bool displayTag = false;
    bool display = true;
    bool attributes = false;
    bool hasStyle = false;
    bool hasColor = false;
    std::string fontcolor = "";
};


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
         * 
         * ##name: checkToken(const std::string & token,
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
void checkToken(const std::string & token, std::string & element);


        /**
         * 
         * ##name: strTokenTag(std::string & buffer_Input
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
void strTokenTag(std::string & buffer_Input,
 std::string & output_Tag, std::string & buffer_Output,
  char delimiter1, char delimiter2);

        /**
         * 
         * ##name: trimLeadingSpace(std::string & io_string)
         * @param  & io_string is a reference to a standard string
         * that holds the i/o string to be trimed
         * @return Nothing is returned (void)
         * @see str_token_tag()
         * @exception No exception handling.
         * @brief This member function is to trim the leading space
         * (space charaters at the start) of the io_string parameter
         * passed as argument for this member function.
         */  
void trimLeadingSpace(std::string & io_string);

       /**
         * 
         * ##name: extractFileToFlatMDE(MDE & MDE_source,
         * const std::string & full_filename)
         * @param & MDE_source is a reference to the MDE instance
         * that holds the the file's data
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
//~ bool extractFileToFlatMDE(MDE & MDE_source, const std::string & full_filename);
bool extractFileToFlatMDE(MDE & MDE_source, std::string & full_filename);

        /**
         * 
         * ##name: processFlatMDEToMDE(MDE & MDE_source,MDE & MDE_target
         * ,std::vector<MDE*>::iterator & it,
         * MDE & MDE_source, std::stack<MDE**> & stack_MDE_pt)
         * @param & MDE_source is a reference to the MDE instance
         * that contains the 'flat' data as one Entity
         * @param & MDE_target is a reference to the MDE instance 
         * to transfer the data to with sub Entities
         * @param  & it is a reference to an iterator of a standard
         * vector of pointers to instances of MDE.
         * @param  & stack_MDE_pt is a reference to an standard stack of 
         * pointers to instances of MDE.
         * @return & MDE_target a reference to the target instance of MDE.
         * @see load_XML_File_to_MDE()
         * @see proc_it()
         * @exception No exception handling.
         * @brief This member function is to process an instance of a
         * 'flat' Entity and return a pointer to a new instance of MDE
         * that has nested sub-entities.
         * The algorithm checks what is the data, if it is a structural
         * data like an opening tag or closing tag it creates or ends
         * the sub-entities accordingly.
         * @note This function could be optimised. This function is
         * complex and does contain a key algorithm for returning an
         * Entity MDE from a flat source.
         */
MDE * processFlatMDEToMDE(MDE & MDE_source,MDE & MDE_target,std::vector<MDE*>::iterator & it, std::stack<MDE**> & stack_MDE_pt);

        /**
         * 
         * ##name: loadXMLFileTovMDE(MDE & MDE_source, int & index,
         * const std::string & full_filename)
         * @param & MDE_source is a reference to the MDE instance
         * that holds the the file's data
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
bool loadXMLFileTovMDE(MDE & MDE_source, int & index, const std::string & full_filename);


         /**
         * 
         * ##name: loadXMLFileToMDE(MDE & MDE_source,
         * const std::string & full_filename)
         * @param & MDE_source is a reference to the MDE instance 
         * that holds the the file's data
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
bool loadXMLFileToMDE(MDE & MDE_source, const std::string & full_filename);

MDE * loadXMLFileToPtMDE(MDE & xmlMDE, const std::string & full_filename, bool destructAll = true);

void loadXML(MDE & sourceMDE,  MDE & xmlMDE, std::vector<int> &vectIndex,const std::string & filename);

         /**
         * 
         * ##name: formatDisplay(MDE & MDE_source,int n_indent, 
            const int & n_space,
            std::string start_opening_tag,
            std::string end_opening_tag, std::string start_closing_tag,
            std::string end_closing_tag)
         * @param & MDE_source is a reference to the MDE instance 
         * to display
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
void formatDisplay(MDE & MDE_source, int n_indent, const int & n_space, std::string start_opening_tag,
            std::string end_opening_tag, std::string start_closing_tag,
            std::string end_closing_tag);
            
        /**
         * 
         * ##name: void extractMDEdataForVoffset(MDE & MDE_source,
         * std::vector<GLdata> & v_MDE_data, int & level,
         * int & index = 0, bool displayDataOnly = false,
         * bool hideParent = false);
         * @param &MDE_source is a reference to a MDE instance 
         * @param &v_MDE_data is a reference to a standard vector of
         * struct GLdata.
         * @param &level is a reference to an int.
         * @param &index is a reference to an int.
         * @param displayDataOnly is a boolean, set to false by default.
         * @param hideParent is a boolean, set to false by default.
         * @return Nothing is returned (void)
         * @see init_data()
         * @exception No exception handling.
         * @brief This member function is to recursively fill-in
         * a standard vector of struct GLdata by extracting
         * data from public member 'name' and public member 'data',
         * and setting the level and index of each Entities and 
         * sub-entities of an instance of object class MDE.
         * The vector of GLdata my_v_MDE_data is used to set offset
         * coordinates for displaying MDE and further up in the pipeline
         * to process other coordinates input like rules and padding.
         * This function has a wrapper init_data(), called from main.cpp
         * without any arguments.
         * The two booleans variables displayDataOnly and hideparents
         * are used as their names clearly state, for an end display
         * of only data, and without the parent MDE. This is typically
         * for HTML rendering.
         * @note This method can be optimised and may be able to use
         * other existing methods.
         */
void extractMDEdataForVoffset(MDE & MDE_source, std::vector<GLdata> & v_MDE_data, int & level,
                             int & index, bool _displayDataOnly = false, bool _hideParent = false);

        /**
         * 
         * ##name: searchFor(MDE & MDE_source,int & index, int & level,
         * std::string searchName)
         * @param & MDE_source is a reference to a MDE instance
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
void searchFor(MDE & MDE_source, int & index, int & level, std::string searchName, std::vector<MDE**> &search_result );

        /**
         * 
         * ##name: simpleLoopVMDE(MDE & MDE_source,
         * std::string searchName)
         * @param & MDE_source is a reference to a MDE instance
         * @param  searchName is a standard string.
         * @return Nothing is returned (void)
         * @see search_For()
         * @see simple_loop_VMDE_next_level()
         * @exception No exception handling.
         * @brief This member function is to part of the recursive
         * algorithm in the search_for method. It recursively stores
         * pointers to pointer to MDE in the static (class) member
         * search_result when the search is successful. It calls a
         * similar methods called simple_loop_VMDE_next_level in its
         * recursive process to separate scopes of the different
         * sub-entities of an entity.
         */
void simpleLoopVMDE(MDE & MDE_source, std::string searchName, std::vector<MDE**> &search_result);

        /**
         * 
         * ##name: simpleLoopVMDEnextLevel(MDE & MDE_source,
         * std::string searchName)
         * @param & MDE_source is a reference to a MDE instance
         * @param  searchName is a standard string.
         * @return Nothing is returned (void)
         * @see search_For()
         * @see simple_loop_VMDE()
         * @exception No exception handling.
         * @brief This member function is to part of the recursive
         * algorithm in the search_for method. It is used to 
         * separate scopes of the different sub-entities of an entity
         * to perform the search.
         */
void simpleLoopVMDEnextLevel(MDE & MDE_source, std::string searchName,  std::vector<MDE**> &search_result);

         /**
         * 
         * ##name: displayVMDEdata(std::vector<GLdata> & v_MDE_data)
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
void displayVMDEdata(std::vector<GLdata> & v_offset, uint target_log_level=1);

        /** @} */ // end of group_X

void splitString(const std::string & tag, std::vector<std::string> & list_elements);
void fetchSearchResult(std::vector<MDE**> &search_result);
void extractXmlElement(std::string & bufferLine, std::string & buffer_Tag, std::string & element, std::vector<std::string> & list_elements);
void XprocessXmlElement(MDE & MDE_source, std::string & buffer_Tag, std::string & element, std::vector<std::string> & list_elements);
//~ bool replaceVMDEwithXML(MDE & MDE_source,std::vector<int> & vect_index_file, const std::string & full_filename);
MDE *  replaceVMDEwithXML(MDE & MDE_source,std::vector<int> & vect_index_file, std::string & full_filename);
bool replaceVMDEwithXML_old(MDE & MDE_source,std::vector<int> & vect_index_file, std::string & full_filename);
MDE * processXMLAttributes(MDE & MDE_source,MDE & MDE_target,std::vector<MDE*>::iterator & it);
void keyValuePairParser(std::string expression, std::string &key, std::string &value, const char op='=', bool hasDleQuotes=true);
//~ MDE * htmlParser_element(MDE & MDE_source);
MDE * htmlParser_attributes(MDE & MDE_source);

bool checkAttribute(MDE & MDE_source, std::string attribute);
MDE * htmlParser(MDE & MDE_source, std::vector<std::vector<float>> &vect_color, std::vector<int> &vect_size, std::vector<float> &vect_line_height, HtmlParseState &state);
MDE * htmlParser_previous(MDE & MDE_source, std::vector<std::vector<float>> &vect_color, std::vector<int> &vect_size);
bool checkHtmlTag(MDE & MDE_source, std::string tag);
bool  htmlTagHasAttributes(MDE & MDE_source);
/*
 * rawDisplayContent checks if display=None or else... and returns false
 * if the tag should not be displayed, or True if it should.
 *  
 * */
bool rawDisplayContent(MDE & MDE_source);
bool checkHtmlAttribute(MDE & MDE_source, std::string attribute);
int hexToInt(int x);
bool hasCssStyle(MDE & MDE_source, bool isInline);
MDE * getCssStyle(MDE & MDE_source, bool isInline);

void XcssStyleAttribute(std::string & buffer_Input,
 std::string & output_Tag, std::string & buffer_Output);
void extractKeyValuePairs(std::string &bufferLine, std::vector<std::string> &vKeyValuePairs);
void splitCssKeyValuePairs(std::string &bufferLine, std::vector<std::string> &vKeyValuePairs);
void processCssColor(std::string &cssColor, std::vector<std::vector<float>> &vect_color);
std::vector<float> processCssColor_RGBA(std::string &cssColor);
void splitStringSep(std::string &strSource, std::vector<std::string> &vTarget, std::string sep);
void PRINT_LOG(const std::string text, uint target_level=1);
/*
bool extractFileToFlatMDE(const std::string & full_filename, MDE & oMDE);
bool loadXMLFileTovMDE(int & index, const std::string & full_filename, MDE & oMDE);
MDE * processFlatMdeToMde(std::vector<MDE*>::iterator & it, MDE & MDE_source, std::stack<MDE**> & stack_MDE_pt);
*/

void set_utils_log_level(uint &_log_level);

// Templates
template <typename T>
void PRINT_VECTOR(const std::vector<T> &vec, uint target_level);
#endif /*UTILS_CPP*/
