//-----------Utils.h-------------
#ifndef UTILS_H
#define UTILS_H

#include<string>
#include "MDE.h"

struct GLdata{
    
    int level;
    int index;
    MDE_member mde_member;
    std::string MDE_data;
};

void checkToken(const std::string & token, std::string & element);

void splitString(const std::string & tag, std::vector<std::string> & list_elements);

void strTokenTag(std::string & buffer_Input,
 std::string & output_Tag, std::string & buffer_Output,
  char delimiter1, char delimiter2);
  
void trimLeadingSpace(std::string & io_string);

void extractXmlElement(std::string & bufferLine, std::string & buffer_Tag, std::string & element, std::vector<std::string> & list_elements);

void XprocessXmlElement(MDE & MDE_source, std::string & buffer_Tag, std::string & element, std::vector<std::string> & list_elements);

bool extractFileToFlatMDE(MDE & MDE_source, const std::string & full_filename);

MDE * processFlatMDEToMDE(MDE & MDE_source,MDE & MDE_target,std::vector<MDE*>::iterator & it, std::stack<MDE**> & stack_MDE_pt);

bool loadXMLFileTovMDE(MDE & MDE_source, int & index, const std::string & full_filename);
bool loadXMLFileToMDE(MDE & MDE_source, const std::string & full_filename);

bool replaceVMDEwithXML(MDE & MDE_source,std::vector<int> & vect_index_file, const std::string & full_filename);

void extractMDEdataForVoffset(MDE & MDE_source, std::vector<GLdata> & v_MDE_data, int & level, int index = 0);
void displayVMDEdata(std::vector<GLdata> & v_offset);
void formatDisplay(MDE & MDE_source, int n_indent, const int & n_space, std::string start_opening_tag,
            std::string end_opening_tag, std::string start_closing_tag,
            std::string end_closing_tag);
void fetchSearchResult(std::vector<MDE**> &search_result);
void searchFor(MDE & MDE_source, int & index, int & level, std::string searchName, std::vector<MDE**> &search_result );
void simpleLoopVMDE(MDE & MDE_source, std::string searchName, std::vector<MDE**> &search_result);
void simpleLoopVMDEnextLevel(MDE & MDE_source, std::string searchName,  std::vector<MDE**> &search_result);
/*
bool extractFileToFlatMDE(const std::string & full_filename, MDE & oMDE);
bool loadXMLFileTovMDE(int & index, const std::string & full_filename, MDE & oMDE);
MDE * processFlatMdeToMde(std::vector<MDE*>::iterator & it, MDE & MDE_source, std::stack<MDE**> & stack_MDE_pt);
*/
#endif /*UTILS_CPP*/
