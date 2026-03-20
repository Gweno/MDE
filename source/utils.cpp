//-----------Utils.cpp-------------
#ifndef UTILS_CPP
#define UTILS_CPP

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>       // iteror istream
#include <vector>

#include "MDE.h"
#include "utils.h"
GLdata current_GLdata;

// test log
int utils_test_log=1;
uint utils_log_level = 0;

// declare and define some booleans
bool container = false;
bool subtags = false;
bool displayTag = false;
bool display = false;
bool attributes = false;
bool hasStyle = false; 
bool hasColor =false;



// declare strings
std::string text;
std::string fontcolor;
std::string fontname;
std::string fontsize;
// declare and define pointers to booleans for recursive loops
//~ bool *pt_container=&container;
//~ bool *pt_subtags=&subtags;
//~ bool *pt_displayTag=&displayTag;
//~ bool *pt_display=&display;
//~ bool *pt_attributes=&attributes;
//~ bool *pt_hasStyle=&hasStyle;
//~ bool *pt_hasColor=&hasColor;

void checkToken(const std::string & token, std::string & element)
{
    if(token.size()>0)
    {
        if(element=="tag")
        {
            if(token[0]=='/') element = "closing_tag";
            else if (token[0]=='?') element = "xml_declaration";
            else if (token[0]=='!') element = "comment_tag";
            else element = "opening_tag";
        }
    }
    else
    {
        element = "null_" + element;
    }
}

void splitString(const std::string & tag, std::vector<std::string> & list_elements)
{
    std::stringstream iss(tag);
    std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},std::istream_iterator<std::string> {}};
    list_elements = tokens;
}

void trimLeadingSpace(std::string & io_string)
{
    size_t pos = 0;
    pos = io_string.find_first_not_of(' ');
    if (pos)
    {
        io_string = io_string.substr(pos);
    }
}

void strTokenTag(std::string & buffer_Input,
 std::string & output_Tag, std::string & buffer_Output,
  char delimiter1, char delimiter2)
{
    size_t pos1=0;
    size_t pos2=0;
    if (delimiter1!=0)
    {
        pos1 = buffer_Input.find_first_of(delimiter1);
        if (pos1!=std::string::npos)
        {
            pos2 = buffer_Input.find_first_of(delimiter2,pos1);
            if (pos2!=std::string::npos)
            {
                output_Tag = buffer_Input.substr(pos1+1,pos2-pos1-1);
                buffer_Output = buffer_Input.substr(pos2+1);
            }
            else
            {
                output_Tag = buffer_Input.substr(pos1+1,pos2-pos1-1)+"__part__";
                buffer_Output = buffer_Input.substr(pos2+1);
            }
        }
        else
        {
            //~ output_Tag="_part";
            buffer_Output = buffer_Input;
        }
    }
    // tag/data with one delimiter at the end
    else
    {
        pos1 = buffer_Input.find_first_of(delimiter2);
        if (pos1!=std::string::npos)
        {
        output_Tag = buffer_Input.substr(0,pos1);
        buffer_Output = buffer_Input.substr(pos1);
        //~ if (output_Tag.size()>0) oMDE->trim_leading_space(output_Tag);       // trimming leading spaces
        if (output_Tag.size()>0) trimLeadingSpace(output_Tag);       // trimming leading spaces
        }
        else
        {
            output_Tag="";
            buffer_Output = buffer_Input;
        }
    }
}

void XcssStyleAttribute(std::string & buffer_Input,
 std::string & output_Tag, std::string & buffer_Output){
    std::string buffer_End_of_Line;
    std::string output2;
    std::string buffer_End_of_Line2;
    std::string sep = ";";
    
    
    strTokenTag(buffer_Input, output_Tag, buffer_End_of_Line,0,'=');
    std::size_t found = buffer_End_of_Line.find(sep);
    if(found!=std::string::npos){
    }
    strTokenTag(buffer_End_of_Line, output2, buffer_End_of_Line2,'"','"');
    
}


void splitCssKeyValuePairs(std::string &bufferLine, std::vector<std::string> &vKeyValuePairs){
    std::size_t strToFind0 = 0;
    //~ std::size_t strToFind1 = 0;
    //~ std::size_t strToFind2 = 0;
    //~ std::string key;
    //~ std::string value;
    std::string kvp;
    //~ std::string pair;
    std::string temp;
    std::string next = bufferLine;
    strToFind0 = next.find(";");
    while(!next.empty()){
        if(strToFind0!=std::string::npos){
            kvp = next.substr(0,strToFind0);
            vKeyValuePairs.push_back(kvp);
            next = next.substr(strToFind0+1,std::string::npos);
            strToFind0 = next.find(";");
        }
        else{
            vKeyValuePairs.push_back(next);
            next.clear();
        }
    }
}

void splitStringSep(std::string &strSource, std::vector<std::string> &vTarget, std::string sep){
    std::size_t strToFind0 = 0;
    std::string element;
    std::string temp;
    std::string next = strSource;
    strToFind0 = next.find(sep);
    while(!next.empty()){
        if(strToFind0!=std::string::npos){
            element = next.substr(0,strToFind0);
            vTarget.push_back(element);
            next = next.substr(strToFind0+1,std::string::npos);
            strToFind0 = next.find(sep);
        }
        else{
            vTarget.push_back(next);
            next.clear();
        }
    }
}

void extractKeyValuePairs(std::string &bufferLine, std::vector<std::string> &vKeyValuePairs){
    std::size_t strToFind0 = 0;
    std::size_t strToFind1 = 0;
    std::size_t strToFind2 = 0;
    std::string key;
    std::string value;
    std::string pair;
    std::string temp;
    std::string next = bufferLine;
    strToFind0 = next.find("=");
    while (!next.empty() && next!="?" && next!=" /"){
        strToFind1 = next.find("\"");
        key = next.substr(0,strToFind1+1);
        temp = next.substr(strToFind1+1,std::string::npos);
        strToFind2 = temp.find("\"");
        value = temp.substr(0,strToFind2+1);
        pair = key + value;
        vKeyValuePairs.push_back(pair);
        next = temp.substr(strToFind2+1,std::string::npos);        
    }
}

void extractXmlElement(std::string & bufferLine, std::string & buffer_Tag, std::string & element, std::vector<std::string> & list_elements)
{
    std::string buffer_End_of_Line;
    std::vector<std::string> vKVP;
    trimLeadingSpace(bufferLine);
    if (bufferLine[0]=='<')
    {
        strTokenTag(bufferLine,buffer_Tag,buffer_End_of_Line,'<','>');
        // To check if "__part__" is at the end of the buffer_Tag
        size_t s_buffer_Tag_len = buffer_Tag.length();
        if (s_buffer_Tag_len>8 && buffer_Tag.substr(s_buffer_Tag_len-8,8) == "__part__")
        {
            // in which case flag element ad only a tag part
            element ="__tag_part__";
            // and take off the marker from buffer_Tag as element is flagged now;
            buffer_Tag = buffer_Tag.substr(0,s_buffer_Tag_len-8);
        }
        else
        {
            element ="tag";
            checkToken(buffer_Tag, element);
            int firstSpace = buffer_Tag.find(' ');
            if (firstSpace!=std::string::npos && element!="comment_tag")
            {
                std::string tag_name = buffer_Tag.substr(0,firstSpace);
                vKVP.push_back(tag_name);
                std::string buffend = buffer_Tag.substr(firstSpace,std::string::npos);
                extractKeyValuePairs(buffend,vKVP);
            }
            
            list_elements=vKVP;

            for(std::vector<std::string>::iterator it=list_elements.begin();it!=list_elements.end();++it){
            }
            std::string xbuffer_Tag;
            std::string xbuffer_End_of_Line;
            XcssStyleAttribute(buffer_Tag,xbuffer_Tag,xbuffer_End_of_Line);
        }
    }
    else
    {
        strTokenTag(bufferLine,buffer_Tag,buffer_End_of_Line,0,'<');
        element = "data";
        if(buffer_Tag.size()==0)
        {
            if ( bufferLine.size() == buffer_End_of_Line.size())
            {
                buffer_Tag = buffer_End_of_Line;
                buffer_End_of_Line = "";                        
            }
            else
            {
                buffer_Tag = "";
            }
        }

    }
    
    bufferLine = buffer_End_of_Line;

}

void XprocessXmlElement(MDE & MDE_source, std::string & buffer_Tag, std::string & element, std::vector<std::string> & list_elements)
{
// check if the element is a tag and if the tag has many elements with it
    if ((element == "opening_tag" || element == "closing_tag") && list_elements.size()>1)
    {
        // first element of the list of element is always the tag's name
        MDE_source.new_vMDE_element();
        // check if last element is a "/" then it's a self-closing tag
        if (list_elements.back()=="/")
        {
            MDE_source.set_name_vMDE_back("self_closing_tag");
            // pop_back that last "/"
            list_elements.pop_back();
        }
        else
        {
            MDE_source.set_name_vMDE_back(element);
        }
        MDE_source.set_data_vMDE_back(list_elements[0]);
        
        // then stores all other elements as 'attribute'
        for (vector<string>::iterator it=list_elements.begin()+1; it!=list_elements.end(); ++it) 
        {
            MDE_source.new_vMDE_element("attribute",*it);
        }
    }
    else if ((element == "xml_declaration")  && list_elements.size()>1)
    {
        // rough management of attributes here... need to take away last '?' of the tag
        MDE_source.new_vMDE_element(element,list_elements[0]);
        
        for (vector<string>::iterator it=list_elements.begin()+1; it!=list_elements.end(); ++it) 
        {
            MDE_source.new_vMDE_element("attribute",*it);
        }
    }
    else
    {
        MDE_source.new_vMDE_element(element,buffer_Tag);
    }
}

bool extractFileToFlatMDE(MDE & MDE_source, const std::string & full_filename)
//~ bool extractFileToFlatMDE(MDE & MDE_source, std::string & full_filename)
{
    std::string bufferString;
    std::string bufferLine;
    std::string buffer_Tag;
    std::string element;
    std::string buffer_End_of_Line;
    std::vector<std::string> list_elements;
    const std::string filename = full_filename;
    
    // Processing file
    ifstream fileEntity;
    fileEntity.open(full_filename.c_str(), ios::in);
    
    if(!fileEntity.is_open())
    {
        printf("File not found\n");
        return false;
    }

    fileEntity.clear();                   // absolutly needed otherwise the file is flagged at eof, and good=0   
    fileEntity.seekg(0, ios::beg);        // set cursor at 0 from start of file

    MDE_source.name = "File";
    MDE_source.data = full_filename;
    
    while ((fileEntity.good()) && (!fileEntity.eof()))     // loop while extraction from file is possible
    {
        // get a line from the file
        std::getline(fileEntity, bufferString);

        // check if element has the flag, it is a partial tag (a tag that has a '\n' in the source file)
        if (element=="__tag_part__")
        {
            // in which case we add the new extracted line at the end of previous line
            // but triming new loaded line first
            trimLeadingSpace(bufferString);
            bufferLine += " " + bufferString;
            // and delete the flag
            element = "";
        }
        else bufferLine = bufferString;
        
        while (bufferLine.size()>0 && element!="__tag_part__")
        {
            
            extractXmlElement(bufferLine, buffer_Tag, element, list_elements);
            
            //now store into vMDE
            // only if element is not a partial tag
            if (element!="__tag_part__")
            {
                XprocessXmlElement(MDE_source, buffer_Tag, element, list_elements);
            }
        }
    
    }
    return true;
}

void keyValuePairParser(std::string expression, std::string &key, std::string &value, const char op, bool hasDleQuotes)
{
    std::string temp;
    std::string trail;
    std::string trail2;
    
    if(hasDleQuotes){
        strTokenTag(expression,key,trail,0,op);
        strTokenTag(trail,temp,trail2,'"','"');
        value = trail2.substr(0, trail2.size()-1);
    }
    else{
        strTokenTag(expression,key,value,0,op);
    }
}

MDE * processXMLAttributes(MDE & MDE_source,std::vector<MDE*>::iterator & it)
{
    // create an new MDE
    // will need to delete?
    MDE * MDE_target = new MDE();
    //add an element at back of vMDE as ("attributes",num of attributes)
    MDE_target->name= "attributes";
    //~ MDE_target->name ("attributes","");
    int n=0;
    while ((it!=MDE_source.vMDE.end()) && ((*it)->name=="attribute"))
    {
        // add each attribute as new element of the vMDE 'attributes'
        // for now name and data are name='values' it will be name:name / data:value
        //~ ((MDE_target)->vMDE.back())->new_vMDE_element((*it)->data,(*it)->data);
        //~ std::string buffer_Tag;
        //~ std::string buffer_Tag2;
        //~ std::string buffer_End_of_Line;
        //~ std::string buffer_End_of_Line2;
        //~ strTokenTag((*it)->data,buffer_Tag,buffer_End_of_Line,0,'=');
        //~ strTokenTag(buffer_End_of_Line,buffer_Tag2,buffer_End_of_Line2,'"','"');
        //~ std::string buffer_End_of_Line3 = buffer_End_of_Line2.substr(0, buffer_End_of_Line2.size()-1);
        //~ MDE_target->new_vMDE_element(buffer_Tag,buffer_End_of_Line3);
        std::string key;
        std::string value;
        // check if (*it)->data is a (CSS) style


        keyValuePairParser((*it)->data, key,value);
        if(key =="style"){
            MDE_target->new_vMDE_element("style","text/css");
            std::vector<std::string> vKvp;
            splitCssKeyValuePairs(value,vKvp);
            for(std::vector<std::string>::iterator it=vKvp.begin(); it!=vKvp.end(); ++it){
                std::string vkey;
                std::string vvalue;
                keyValuePairParser((*it),vkey,vvalue,':', false);
                vvalue = vvalue.substr(1);
                trimLeadingSpace(vkey);
                trimLeadingSpace(vvalue);
                MDE_target->vMDE.back()->new_vMDE_element(vkey,vvalue);
            }
            
        }
        else{
            MDE_target->new_vMDE_element(key,value);
        }
        ++it;
        n++;
    }
    // update number of attributes into the data od vMDE ' attributes'
    (MDE_target)->data = std::to_string(n);
    return MDE_target;
    //~ return std::make_shared<MDE>(MDE_target);
}

MDE * processFlatMDEToMDE(MDE & MDE_source,MDE & MDE_target,std::vector<MDE*>::iterator & it, std::stack<MDE**> & stack_MDE_pt)
{
    while (it!=MDE_source.vMDE.end())
    {
        if ((*it)->name=="xml_declaration")
        {
            // name of this element is name of the flat MDE element (e.g. "xml_declaration")
            MDE_target.name = (*it)->name;
            // set empty data in case writing over existing MDE
            MDE_target.data="";
            
            MDE_target.new_vMDE_element((*it)->data,"");
            
            it++;

            if ((it!=MDE_source.vMDE.end()) && ((*it)->name=="attribute"))
            {
                // create an new MDE element at back of vMDE as ("attributes",num of attributes)
                (MDE_target.vMDE.back()->vMDE).push_back(processXMLAttributes(MDE_source,it));
            }
            (*(stack_MDE_pt.top()))->new_vMDE_element();
            return processFlatMDEToMDE(MDE_source,*((*(stack_MDE_pt.top()))->vMDE.back()),it, stack_MDE_pt);
        }
        else if ((*it)->name=="opening_tag")
        {
            // name of this element is in data of the flat MDE
            MDE_target.name=(*it)->data;
            // set empty data in case writing over existing MDE
            MDE_target.data="";
            if (stack_MDE_pt.size()>0){
            }
            
            // store tag in stack only if it's an opening tag 
            // and not if it's a self closing tag
            MDE** ptr_to_ptr_to_MDE = new MDE*(&MDE_target);
            stack_MDE_pt.push(ptr_to_ptr_to_MDE);
            // go to next element
            ++it;
            
            if ((it!=MDE_source.vMDE.end()) && ((*it)->name=="attribute"))
            {
                (MDE_target.vMDE).push_back(processXMLAttributes(MDE_source,it));

                if (it!=MDE_source.vMDE.end() && (*it)->name=="self_closing_tag")
                {
                    (*(stack_MDE_pt.top()))->new_vMDE_element();
                    return processFlatMDEToMDE(MDE_source,*((*(stack_MDE_pt.top()))->vMDE.back()),it, stack_MDE_pt);
                }
                else if (it!=MDE_source.vMDE.end() && (*it)->name=="data")
                {
                    return processFlatMDEToMDE(MDE_source,*(*(stack_MDE_pt.top())),it, stack_MDE_pt);
                }
                else if (it!=MDE_source.vMDE.end() && (*it)->name!="closing_tag") MDE_target.new_vMDE_element();
                return processFlatMDEToMDE(MDE_source,*(MDE_target.vMDE.back()),it, stack_MDE_pt);
            }
            else if ((it!=MDE_source.vMDE.end()) && ((*it)->name=="data"))
            {
                MDE_target.data=(*it)->data;
                ++it;
                
                if ((it!=MDE_source.vMDE.end()) && ((*it)->name=="closing_tag"))
                {
                    return processFlatMDEToMDE(MDE_source,MDE_target,it, stack_MDE_pt);
                }
                else
                {
                    MDE_target.new_vMDE_element();
                    return processFlatMDEToMDE(MDE_source,*(MDE_target.vMDE.back()),it, stack_MDE_pt);
                }
                
            }
            else if ((it!=MDE_source.vMDE.end()) && ((*it)->name=="closing_tag"))
            {
                return processFlatMDEToMDE(MDE_source,MDE_target,it, stack_MDE_pt);
            }
            else
            {
                MDE_target.new_vMDE_element();
                return processFlatMDEToMDE(MDE_source,*(MDE_target.vMDE.back()),it, stack_MDE_pt);
            }
        }
        else if ((it!=MDE_source.vMDE.end()) && ((*it)->name=="attribute"))
        {
                (MDE_target.vMDE).push_back(processXMLAttributes(MDE_source,it));
                
                if (it!=MDE_source.vMDE.end() && (*it)->name=="self_closing_tag")
                {
                    (*(stack_MDE_pt.top()))->new_vMDE_element();
                    return processFlatMDEToMDE(MDE_source,*((*(stack_MDE_pt.top()))->vMDE.back()),it, stack_MDE_pt);
                    
                }
                else if (it!=MDE_source.vMDE.end() && (*it)->name!="closing_tag") MDE_target.new_vMDE_element();
                return processFlatMDEToMDE(MDE_source,*(MDE_target.vMDE.back()),it, stack_MDE_pt);
    
        }
        else if(((*it)->name=="closing_tag") && it!=MDE_source.vMDE.begin())
        {
            ++it;
            MDE **ptr_to_ptr_to_MDE = stack_MDE_pt.top();       // not sure why doing this
            delete ptr_to_ptr_to_MDE;
            stack_MDE_pt.pop();
            // tricky: if next element is opening tag we create
            // a new vMDE element in the element at the top of the stack 
            // (which should be the parent element of this current one)

            if (it!=MDE_source.vMDE.end() && ((*it)->name=="opening_tag" || (*it)->name=="self_closing_tag" || (*it)->name=="comment_tag"))
            {
                (*(stack_MDE_pt.top()))->new_vMDE_element();
                return processFlatMDEToMDE(MDE_source,*((*(stack_MDE_pt.top()))->vMDE.back()),it, stack_MDE_pt);
            }
            else
            {
                if (!stack_MDE_pt.empty()) return processFlatMDEToMDE(MDE_source,*(*(stack_MDE_pt.top())),it, stack_MDE_pt);
                else return &MDE_target;
            }
        }
        else if ((it!=MDE_source.vMDE.end()) && ((*it)->name=="data"))
        {
            MDE_target.data=(*it)->data;
            ++it;
            
            if ((it!=MDE_source.vMDE.end()) && ((*it)->name=="closing_tag"))
            {
                return processFlatMDEToMDE(MDE_source,MDE_target,it, stack_MDE_pt);
            }
            else
            {
                MDE_target.new_vMDE_element();
                return processFlatMDEToMDE(MDE_source,*(MDE_target.vMDE.back()),it, stack_MDE_pt);
            }
            
        }
        else if (it!=MDE_source.vMDE.end() && ((*it)->name=="self_closing_tag" || (*it)->name=="comment_tag"))
        {
            // name of this element is in data of the flat MDE
            MDE_target.name=(*it)->data;
            // set empty data in case writing over existing MDE
            MDE_target.data="";
            
            if (it!=MDE_source.vMDE.begin())
            {
                ++it;
                if (it!=MDE_source.vMDE.end() && (*it)->name=="attribute")
                {
                    return processFlatMDEToMDE(MDE_source,*((*(stack_MDE_pt.top()))->vMDE.back()),it, stack_MDE_pt);
                    
                }
                else if (it!=MDE_source.vMDE.end() && (*it)->name!="closing_tag") (*(stack_MDE_pt.top()))->new_vMDE_element();
                return processFlatMDEToMDE(MDE_source,*((*(stack_MDE_pt.top()))->vMDE.back()),it, stack_MDE_pt);
            }
            else
            {
                it++;
                MDE_target.new_vMDE_element();
                return processFlatMDEToMDE(MDE_source,*(MDE_target.vMDE.back()),it, stack_MDE_pt);
            }
        }
        else return (&MDE_target);
    }
    return  (&MDE_target);
}

bool loadXMLFileTovMDE(MDE & MDE_source, int & index, const std::string & full_filename)
{

    MDE xmlFile;
    std::stack<MDE**> stack_MDE_pt;
    fprintf(stderr,"Oh yeah!\n");
    if (extractFileToFlatMDE(xmlFile, full_filename)){
        fprintf(stderr,"hola 0\n");
        fprintf(stderr,"hola 1\n");
        std::vector<MDE*>::iterator it_start = xmlFile.vMDE.begin();
        fprintf(stderr,"hola 2\n");
        // store 'this' in the stack of pointers to be able to come back
        // to this element if there is an xml declaration
        // Need to create a new pointer as using 'this' directly is not possible (temp scope object)
        MDE ** pt_to_pt_MDE = new MDE*(&MDE_source);
        stack_MDE_pt.push(pt_to_pt_MDE);
        std::vector<int> vect_index = {index};
        processFlatMDEToMDE(xmlFile,*(MDE_source.get_vMDE(vect_index)),it_start, stack_MDE_pt);

        delete pt_to_pt_MDE;
        return true;
    }
    else return false;
        
}

bool loadXMLFileToMDE(MDE & MDE_source, const std::string & full_filename)
{

    MDE xmlFile;
    std::stack<MDE**> stack_MDE_pt;
    
    if (extractFileToFlatMDE(xmlFile, full_filename)){
        std::vector<MDE*>::iterator it_start =  xmlFile.vMDE.begin();
        processFlatMDEToMDE(xmlFile, MDE_source, it_start,  stack_MDE_pt);
        xmlFile.destructor_MDE();
        return true;
    }
    else return false;
        
}

void loadXML(MDE & sourceMDE,  MDE & xmlMDE, std::vector<int> &vectIndex,const std::string & filename)
{
    std::vector<int> vectIndexTop = vectIndex;
    bool destructAll=false;
    vectIndexTop.pop_back();
    xmlMDE.destructor_MDE();
    sourceMDE.get_vMDE(vectIndexTop)->destructor_MDE();
    sourceMDE.get_vMDE(vectIndexTop)->name = "file";
    sourceMDE.get_vMDE(vectIndexTop)->data = filename;
    sourceMDE.get_vMDE(vectIndexTop)->new_vMDE_element(loadXMLFileToPtMDE(xmlMDE, filename));                                           
}

MDE * loadXMLFileToPtMDE(MDE & xmlMDE,const std::string & full_filename, bool destructAll)
{

    MDE xmlFile;
    MDE * ptXmlMDE;
    ptXmlMDE = &xmlMDE;
    std::stack<MDE**> stack_MDE_pt;
    
    if (extractFileToFlatMDE(xmlFile, full_filename)){
        std::vector<MDE*>::iterator it_start =  xmlFile.vMDE.begin();
        processFlatMDEToMDE(xmlFile, xmlMDE, it_start,  stack_MDE_pt);
        if(destructAll) xmlFile.destructor_MDE();
        return ptXmlMDE;
    }
    else return nullptr;
        
}

MDE * replaceVMDEwithXML(MDE & MDE_source,std::vector<int> & vect_index_file, std::string & full_filename)
{
    std::vector<int>::const_iterator it_file;
    bool input_file = false;
    int index_file;
    MDE_source.display_vector_int(vect_index_file);
    
    MDE * ptMDE = MDE_source.get_vMDE(vect_index_file);
    ptMDE->deleteAllPt();
    
    MDE_source.clear_all_vMDE_by_index(vect_index_file);
    std::vector<int> tmp_vect_index= vect_index_file;
    tmp_vect_index.pop_back();
    MDE_source.display_vector_int(tmp_vect_index);
    index_file = vect_index_file.back();
    fprintf(stderr,"index file:%i\n", index_file);

    // get vDME pointer at index just before last of vector of indexes, then load at that last index.
    input_file = loadXMLFileTovMDE(*(MDE_source.get_vMDE(tmp_vect_index)), index_file, full_filename);
    
    // set the name of the file to the mde pointer at the given index, one level above what has been loaded
    MDE_source.get_vMDE(tmp_vect_index)->set_data(full_filename);
    
    return ptMDE;
}

bool replaceVMDEwithXML_old(MDE & MDE_source,std::vector<int> & vect_index_file, std::string & full_filename)
{
    std::vector<int>::const_iterator it_file;
    bool input_file = false;
    int index_file;
    MDE_source.display_vector_int(vect_index_file);
    
    MDE * ptMDE = MDE_source.get_vMDE(vect_index_file);
    ptMDE->deleteAllPt();
    
    MDE_source.clear_all_vMDE_by_index(vect_index_file);
    std::vector<int> tmp_vect_index= vect_index_file;
    tmp_vect_index.pop_back();
    MDE_source.display_vector_int(tmp_vect_index);
    index_file = vect_index_file.back();

    // get vDME pointer at index just before last of vector of indexes, then load at that last index.
    input_file = loadXMLFileTovMDE(*(MDE_source.get_vMDE(tmp_vect_index)), index_file, full_filename);
    
    // set the name of the file to the mde pointer at the given index, one level above what has been loaded
    MDE_source.get_vMDE(tmp_vect_index)->set_data(full_filename);

    return input_file;
}

void extractMDEdataForVoffset(MDE & MDE_source, std::vector<GLdata> & v_MDE_data,
                             int & level, int & index, bool _displayDataOnly, bool _hideParent)
{
    PRINT_LOG("########## extractMDEdataForVoffset ##########");
    
    current_GLdata.level= level;
    current_GLdata.index= index;

    if (!_hideParent){
        if (!_displayDataOnly){
            current_GLdata.mde_member=MDE_member::name;
            current_GLdata.MDE_data=MDE_source.name;
            v_MDE_data.push_back(current_GLdata);
            }
        current_GLdata.mde_member=MDE_member::data;
        current_GLdata.MDE_data=MDE_source.data;
        v_MDE_data.push_back(current_GLdata);
        }

   if (MDE_source.vMDE.size()!=0){
       level++;
        for (int it=0; it<MDE_source.vMDE.size();++it)
            {
                index = it;
                extractMDEdataForVoffset(*MDE_source.vMDE[it], v_MDE_data, level, index, _displayDataOnly);
            }
        level--;
        index=0;
   }
    PRINT_LOG("########## End extractMDEdataForVoffset ##########");
    return;
}

void displayVMDEdata(std::vector<GLdata> & v_offset, uint target_log_level)
{
    if(utils_log_level == target_log_level)
    {
        for (std::vector<GLdata>::iterator it=v_offset.begin(); it!=v_offset.end(); ++it){
            PRINT_LOG("member, index, level,data: " + std::to_string((*it).mde_member) + ", " + std::to_string((*it).index) + ", "  + std::to_string((*it).level)  + ", " + (*it).MDE_data);
        }
    }
}


void formatDisplay(MDE & MDE_source, int n_indent, const int & n_space, std::string start_opening_tag,
            std::string end_opening_tag, std::string start_closing_tag,
            std::string end_closing_tag)
{
    
    std::string N_space(n_indent*n_space,' ');
    std::cout << N_space ;
    std::cout<< start_opening_tag;
    std::cout << MDE_source.name;
    std::cout<< end_opening_tag;
    std::cout << endl;
    std::cout << N_space;
    std::cout << MDE_source.data;
    std::cout << endl;

    if (MDE_source.vMDE.size()!=0) n_indent++;
    for (auto it:MDE_source.vMDE)
        {
            formatDisplay(*(it),n_indent, n_space, start_opening_tag,
            end_opening_tag, start_closing_tag, end_closing_tag);
            std::cout << endl;
        }
    std::cout << N_space ;
    std::cout<< start_closing_tag;
    std::cout << MDE_source.name;
    std::cout<< end_closing_tag;
    return;
};

void fetchSearchResult(std::vector<MDE**> &search_result)
{
    const int n_space = 2;
    if (search_result.size()!=0)
    {
        //~ for (std::vector<MDE**>::iterator it=search_result.begin();it!=search_result.end();++it)
        for (auto it:search_result)
        {
            //~ formatDisplay(**(*it),0,n_space,"<",">","</",">");
            formatDisplay(**(it),0,n_space,"<",">","</",">");
            std::cout << endl;
        }
    }
    else std::cout << "Not found" <<std::endl;
}

void searchFor(MDE & MDE_source, int & index, int & level, std::string searchName, std::vector<MDE**> &search_result )
{
    if (MDE_source.name==searchName)
    {
        std::cout << "Found " << searchName << " at (level,index): " << "(" << level  << "," << index << ")" << std::endl;
        std::cout << std::endl;
        //~ MDE** ptr_to_ptr_to_MDE = new MDE*(this);
        MDE** ptr_to_ptr_to_MDE = new MDE*(&MDE_source);
        //~ MDE::search_result.push_back(ptr_to_ptr_to_MDE);
        search_result.push_back(ptr_to_ptr_to_MDE);
        delete ptr_to_ptr_to_MDE;
    }
    else
    {
        level++;
        index=0;
        std::cout << "(level,index): " << "(" << level  << "," << index << ")" << std::endl;
        //~ (this->simple_loop_VMDE(searchName));
        simpleLoopVMDE(MDE_source,searchName, search_result);
    }
    //~ if (MDE::search_result.size()==0)
    if (search_result.size()==0)
    {
        std::cout << "No results" << std::endl; //not ideal...
    }
    //~ std::cout << "Found " << MDE::search_result.size() << " Entities with name " << searchName << std::endl;
    std::cout << "Found " << search_result.size() << " Entities with name " << searchName << std::endl;
}

void simpleLoopVMDE(MDE & MDE_source, std::string searchName, std::vector<MDE**> &search_result)
{
    for(std::vector<MDE*>::const_iterator it=MDE_source.getVmdeBegin();it!=MDE_source.getVmdeEnd();++it)
    {
        if ((*it)->name==searchName)
        {
            MDE** ptr_to_ptr_to_MDE = new MDE*(*it);
            //~ MDE::search_result.push_back(ptr_to_ptr_to_MDE);
            search_result.push_back(ptr_to_ptr_to_MDE);
            //~ delete ptr_to_ptr_to_MDE;
        }
    }
    //~ this->simple_loop_VMDE_next_level(searchName);
    simpleLoopVMDEnextLevel(MDE_source,searchName, search_result);
}

void simpleLoopVMDEnextLevel(MDE & MDE_source, std::string searchName,  std::vector<MDE**> &search_result)
{
    //~ for(std::vector<MDE*>::const_iterator it=MDE_source.vMDE.begin();it!=MDE_source.vMDE.end();++it)
    for(auto it:MDE_source.vMDE)
    {
        //~ simpleLoopVMDE(*(*it),searchName, search_result);
        simpleLoopVMDE(*(it),searchName, search_result);
    }
}



void checkHtmlTag2(MDE & MDE_source){

    if(utils_test_log>1) std::clog<<"(in checkHtmlTag2) MDE_source.name: "<<MDE_source.name<< "  MDE_source.data: "<<MDE_source.data<<std::endl;
/*
                                    if (checkHtmlTag(*it2,"attributes")){
                                        attributes = true;
                                        for(auto it3:it2->vMDE){
                                            checkHtmlTag2(*it3);
                                        }
                                        if (display)break;
                                    }


    if (MDE_source.name=="p" || checkHtmlTag(*it1,"li")){
        text = MDE_source.data;
        if(MDE_source.vMDE.size()>0){
            subtags = true;
            for(auto it:MDE_source.vMDE){
                checkHtmlTag2(*it);
            }            
        }

        
    }

*/
    //~ else if (MDE_source.name=="li")){
    
    //~ }
    // by default display should be on
    display = true;
    hasColor = false;
    hasStyle = false;
    displayTag = false;
    if(MDE_source.name=="attributes"){
        if(utils_test_log>1) std::clog<<"Found atributes" << std::endl;
        attributes = true;
        for(auto it:MDE_source.vMDE){
            checkHtmlTag2(*it);
        }
        if (display) return;
    }
    else if(MDE_source.name=="style"){
        if(utils_test_log>1) std::clog<<"Found style" << std::endl;
        hasStyle=true;
        for(auto it:MDE_source.vMDE){
            checkHtmlTag2(*it);
        }
    }
    else if (MDE_source.name == "display"){
        if(utils_test_log>1) std::clog<<"Found display" << std::endl;
        displayTag = true;
        display = rawDisplayContent(MDE_source);
        std::clog<<"display is set to "<< (display?"true":"false") << std::endl;
    }
    else if (MDE_source.name == "color"){
            if(utils_test_log>1) std::clog<<"Found color" << std::endl;
            hasColor = true;
            fontcolor = MDE_source.data;
    }
};

/*
if (checkHtmlTag(*it3,"style")){
    hasStyle = true;
    for(auto it4:it3->vMDE){
        //~ std::clog << "And further Checking:" << it4->name << std::endl;
        if (checkHtmlTag(*it4,"display")){
            displayTag = true;
            display = rawDisplayContent(*it4);
            //~ break;
        }
        else if (checkHtmlTag(*it4,"color")){
            //~ std::clog << "Found some font color style! " << std::endl;
            hasColor = true;
            color = it4->data;
            //~ std::clog << "color: " << color << std::endl;

        }
    }
}

*/
 MDE * htmlParser(MDE & MDE_source, std::vector<std::vector<float>> &vect_color, std::vector<int> &vect_size, std::vector<float> &vect_line_height){
    PRINT_LOG("In html Parser",5);
    //~ fprintf(stderr,"MDE_source name %s, data %s: \n", (MDE_source.name).c_str(),MDE_source.data.c_str());
    PRINT_LOG("MDE_source name: "+MDE_source.name+ ", data: " + MDE_source.data, 5);
    if (MDE_source.vMDE.size()==0){
        //~ std::clog<<"No tag to parse" <<std::endl;
        return nullptr;
    }
    
    // set default value for (global) booleans for recursive algorithm 
    container = false;
    subtags = false;
    displayTag = false;
    display = true;
    attributes = false;
    hasStyle = false; 
    hasColor =false;

    
    MDE * newMDE = new MDE();
    newMDE->name = MDE_source.name;
    newMDE->data = MDE_source.data;
    
    // a vector for the colors
    std::vector<float> defaultColor = {0.0,0.0,0.0,1.0};
    
    // font size
    uint defaultSize = 16;
    
    
    // Checking if MDE name is a Div -> later will add further MDE above , like <HTML>, <body>...
    container = false;
    if (checkHtmlTag(MDE_source,"div")){
        container = true;
        for(auto it:MDE_source.vMDE){
            PRINT_LOG("Name/Data: " + (*it).name + " / " + (*it).data, 5);
            if (checkHtmlTag(*it,"h3")){
                PRINT_LOG("It's a h3", 5);
                newMDE->new_vMDE_element(it->name,it->data);
                vect_color.push_back(defaultColor);
                //~ vect_size.push_back(96);
                vect_size.push_back(19); // it's actually 18.72px
                vect_line_height.push_back(19*1.2);
            }
            else if (checkHtmlTag(*it,"ul"))
            {
                container = true;
                // check all tags for a 'p' or ul or li 
                    for(auto it1:it->vMDE){
                        PRINT_LOG("Name/Data: " + (*it1).name + " / " + (*it1).data, 5);
                        if (checkHtmlTag(*it1,"p") || checkHtmlTag(*it1,"li")){
                            container = false;
                            //~ subtags = false;
                            //~ displayTag = false;
                            //~ display = false;
                            //~ attributes = false;
                            //~ hasStyle = false; 
                            //~ hasColor =false;
                            
                            if (it1->vMDE.size()>0){
                                subtags = true;
                                for(auto it2:it1->vMDE){
                                    PRINT_LOG("Name/Data: " + (*it2).name + " / " + (*it2).data, 5);;
                                    checkHtmlTag2(*it2);
                                    //~ if(checkHtmlTag(*it2,"display")){
                                        //~ std::clog<<"Display Tag"<<std::endl;
                                    //~ }
                                }
                            }
                            if(utils_test_log==5) std::clog<<"(In htmplParser) Checking hasStyle, displayTag, display , hasColor, fontcolor: " << 
                            hasStyle << ", "  << displayTag << ", " << display << ", " << hasColor << ", " << fontcolor << std::endl;
                            if (!subtags || !attributes || !displayTag || display){
                                newMDE->new_vMDE_element(it1->name,it1->data);
                                // testing font size
                                vect_size.push_back(defaultSize);
                                vect_line_height.push_back(defaultSize*1.2);

                                
                                if(hasColor){
                                    processCssColor(fontcolor, vect_color);
                                }
                                else{
                                    // use default color for font
                                    vect_color.push_back(defaultColor);
                                }
                            }
                        }
                        
                        
                        //
                                                
                    }
                    }
            else
            {
                        //~ std::clog << "Not a ul\n"<<std::endl;
                    }
                }
            }
    else{
        //~ std::clog << "Not a div\n"<<std::endl;
    }
    return newMDE;
}

 MDE * htmlParser_previous(MDE & MDE_source, std::vector<std::vector<float>> &vect_color, std::vector<int> &vect_size){
    PRINT_LOG("In html Parser",5);
    //~ fprintf(stderr,"MDE_source name %s, data %s: \n", (MDE_source.name).c_str(),MDE_source.data.c_str());
    PRINT_LOG("MDE_source name: "+MDE_source.name+ ", data: " + MDE_source.data, 5);
    if (MDE_source.vMDE.size()==0){
        //~ std::clog<<"No tag to parse" <<std::endl;
        return nullptr;
    }
    
    // set default value for (global) booleans for recursive algorithm 
    container = false;
    subtags = false;
    displayTag = false;
    display = true;
    attributes = false;
    hasStyle = false; 
    hasColor =false;

    
    MDE * newMDE = new MDE();
    newMDE->name = MDE_source.name;
    newMDE->data = MDE_source.data;
    
    // a vector for the colors
    std::vector<float> defaultColor = {0.0,0.0,0.0,1.0};
    
    // font size
    uint defaultSize = 16;
    
    
    // Checking if MDE name is a Div -> later will add further MDE above , like <HTML>, <body>...
    container = false;
    if (checkHtmlTag(MDE_source,"div")){
        container = true;
        for(auto it:MDE_source.vMDE){
            PRINT_LOG("Name/Data: " + (*it).name + " / " + (*it).data, 5);
            if (checkHtmlTag(*it,"h3")){
                PRINT_LOG("It's a h3", 5);
                newMDE->new_vMDE_element(it->name,it->data);
                vect_color.push_back(defaultColor);
                //~ vect_size.push_back(96);
                vect_size.push_back(19); // it's actually 18.72px
            }
            else if (checkHtmlTag(*it,"ul"))
            {
                container = true;
                // check all tags for a 'p' or ul or li 
                    for(auto it1:it->vMDE){
                        PRINT_LOG("Name/Data: " + (*it1).name + " / " + (*it1).data, 5);
                        if (checkHtmlTag(*it1,"p") || checkHtmlTag(*it1,"li")){
                            container = false;
                            //~ subtags = false;
                            //~ displayTag = false;
                            //~ display = false;
                            //~ attributes = false;
                            //~ hasStyle = false; 
                            //~ hasColor =false;
                            

                                        
                            if (it1->vMDE.size()>0){
                                subtags = true;
                                for(auto it2:it1->vMDE){
                                    PRINT_LOG("Name/Data: " + (*it2).name + " / " + (*it2).data, 5);;
                                    checkHtmlTag2(*it2);
                                    //~ if(checkHtmlTag(*it2,"display")){
                                        //~ std::clog<<"Display Tag"<<std::endl;
                                    //~ }
                                }
                            }
                            if(utils_test_log==5) std::clog<<"(In htmplParser) Checking hasStyle, displayTag, display , hasColor, fontcolor: " << 
                            hasStyle << ", "  << displayTag << ", " << display << ", " << hasColor << ", " << fontcolor << std::endl;
                            if (!subtags || !attributes || !displayTag || display){
                                newMDE->new_vMDE_element(it1->name,it1->data);
                                // testing font size
                                vect_size.push_back(defaultSize);
                                
                                if(hasColor){
                                    processCssColor(fontcolor, vect_color);
                                }
                                else{
                                    // use default color for font
                                    vect_color.push_back(defaultColor);
                                }
                            }
                        }
                        
                        
                        //
                                                
                    }
                    }
            else
            {
                        //~ std::clog << "Not a ul\n"<<std::endl;
                    }
                }
            }
    else{
        //~ std::clog << "Not a div\n"<<std::endl;
    }
    return newMDE;
}

 MDE * htmlParser_old(MDE & MDE_source, std::vector<std::vector<float>> &vect_color){
    std::clog<<"In html Parser"<<std::endl;
    fprintf(stderr,"MDE_source name %s, data %s: \n", (MDE_source.name).c_str(),MDE_source.data.c_str());
    if (MDE_source.vMDE.size()==0){
        //~ std::clog<<"No tag to parse" <<std::endl;
        return nullptr;
    }
    
    // set default value for (global) booleans for recursive algorithm 
    container = false;
    subtags = false;
    displayTag = false;
    display = true;
    attributes = false;
    hasStyle = false; 
    hasColor =false;

    
    MDE * newMDE = new MDE();
    newMDE->name = MDE_source.name;
    newMDE->data = MDE_source.data;
    
    //~ std::string color;

    // a vector for the colors
    //~ std::vector<std::vector<float>> vect_color;
    // default (font) color
    std::vector<float> defaultColor = {0.0,0.0,0.0,1.0};
    
    
    // Checking if MDE name is a Div -> later will add further MDE above , like <HTML>, <body>...
    container = false;
    if (checkHtmlTag(MDE_source,"div")){
        // check if it's a div...
        //~ std::clog<<"It's a Div!\n"<<std::endl;
        container = true;
        for(auto it:MDE_source.vMDE){
            std::clog<<"Name/Data: "<< (*it).name <<" / " << (*it).data <<std::endl;
            //~ std::clog <<"Checking if it's a h3 or ul\n"<<std::endl;
            if (checkHtmlTag(*it,"h3")){
                std::clog <<"It's a h3\n"<<std::endl;
                newMDE->new_vMDE_element(it->name,it->data);
                vect_color.push_back(defaultColor);
            }
            else if (checkHtmlTag(*it,"ul"))
            {
                container = true;
                //~ std::clog <<"It's a ul\n"<<std::endl;
                // check all tags for a 'p' or ul or li 
                    for(auto it1:it->vMDE){
                        //
                        std::clog<<"Name/Data: "<< (*it1).name <<" / " << (*it1).data <<std::endl;
                        if (checkHtmlTag(*it1,"p") || checkHtmlTag(*it1,"li")){
                            container = false;
                            //~ subtags = false;
                            //~ displayTag = false;
                            //~ display = false;
                            //~ attributes = false;
                            //~ hasStyle = false; 
                            //~ hasColor =false;            
                            if (it1->vMDE.size()>0){
                                subtags = true;
                                for(auto it2:it1->vMDE){
                                    std::clog<<"Name/Data: "<< (*it2).name <<" / " << (*it2).data <<std::endl;
                                    checkHtmlTag2(*it2);
                                    //~ if(checkHtmlTag(*it2,"display")){
                                        //~ std::clog<<"Display Tag"<<std::endl;
                                    //~ }
                                }
                            }
                            if(utils_test_log>1) std::clog<<"(In htmplParser) Checking hasStyle, displayTag, display , hasColor, fontcolor: " << 
                            hasStyle << ", "  << displayTag << ", " << display << ", " << hasColor << ", " << fontcolor << std::endl;
                            if (!subtags || !attributes || !displayTag || display){
                                //~ newMDE->new_vMDE_element(it1->name,it1->data);
                                newMDE->new_vMDE_element(it1->name,it1->data);
                                //~ std::clog << "Tag is to be displayed: " << it->name << std::endl;
                                if(hasColor){
                                    //~ std::clog << "has color: " << color << std::endl;
                                    //~ processCssColor(color, vect_color);
                                    processCssColor(fontcolor, vect_color);
                                }
                                else{
                                    // use default color for font
                                    vect_color.push_back(defaultColor);
                                }
                            }
                        }
                        
                        
                        //
                                                
                    }
                    }
            else
            {
                        //~ std::clog << "Not a ul\n"<<std::endl;
                    }
                }
            }
    else{
        //~ std::clog << "Not a div\n"<<std::endl;
    }
    return newMDE;
}

void processCssColor(std::string &cssColor, std::vector<std::vector<float>> &vect_color){
    int checkHead = 4;
    std::string head = cssColor.substr(0,checkHead);
    //~ std::clog << "head: " << head << std::endl;
    if (head=="rgba"){
        vect_color.push_back(processCssColor_RGBA(cssColor));
        fprintf(stderr,"RGBA\n");
    }
    else if (head=="rgb("){
        //~ processCssColor_RGB();
        fprintf(stderr,"RGB\n");
    }
    else if (head[0]=='#'){
        //~ processCssColor_hex();
        fprintf(stderr,"Hex\n");
    }
    else{
        fprintf(stderr,"Not supported yet\n");
    }
}

std::vector<float> processCssColor_RGBA(std::string &cssColor){
    std::string output;
    std::string tail;
    std::vector<std::string> strResult;
    std::vector<float> result;
    float n=0.0;
    float color=0.0;
    strTokenTag(cssColor,output,tail,'(',')');
    //~ std::clog << "output: " << output << " tail: " << tail << std::endl;
    splitStringSep(output,strResult,",");
    int index=0; 
    for(auto it:strResult){
        // will nee a 'try/catch' here
        try{
            n = std::stof(it);
        }
        catch(const std::invalid_argument& e){
            n = (index==3)?1.0:0.0;
            std::cerr << e.what() << "\nvalue set to 0.0 for colors and 1.0 for transparency"<< std::endl;
        }
        if(index<3){
            color = n / 255.0;
        }
        else{
            color = n;
        }
        result.push_back(color);
        index++;
    }
    return result;
}

//~ MDE * htmlParser_old(MDE & MDE_source){
    //~ printf("MDE_source name %s, data %s: \n", (MDE_source.name).c_str(),MDE_source.data.c_str());
    //~ if (MDE_source.vMDE.size()==0){
        //~ std::clog<<"No tag to parse" <<std::endl;
        //~ return NULL;
    //~ }
    //~ bool subtags;
    //~ bool displayTag;
    //~ bool display;
    //~ bool attributes;
    
    //~ MDE * newMDE = new MDE();
    //~ newMDE->name=MDE_source.name;
    //~ newMDE->data = MDE_source.data;

    
    //~ for(std::vector<MDE*>::iterator it=MDE_source.vMDE.begin(); it!=MDE_source.vMDE.end(); ++it){
        //~ // check all tags for a 'p'
        //~ if (checkHtmlTag(*(*it),"p")){
            //~ subtags = false;
            //~ displayTag = false;
            //~ display = false;
            //~ attributes = false;              
            //std::clog << "Checking:" << (*it)->name << std::endl;
            //std::clog << "Does it have a style?: " << hasCssStyle(*(*it),true)<< std::endl ;
            //~ if (getCssStyle(*(*it),1)!=NULL){
                //~ std::clog << "The style: " <<getCssStyle(*(*it),1)->data << std::endl;
                //XcssStyleAttribute(getCssStyle(*(*it),1)->data);
                
            //~ }
            
            //~ if ((*it)->vMDE.size()>0){
                //~ subtags = true;
                //~ for(std::vector<MDE*>::iterator it2=(*it)->vMDE.begin(); it2!=(*it)->vMDE.end(); ++it2){
                    //std::clog << "Now Checking:" << (*it2)->name << std::endl;
                    //~ if (checkHtmlTag(*(*it2),"attributes")){
                        //~ attributes = true;
                        //~ for(std::vector<MDE*>::iterator it3=(*it2)->vMDE.begin(); it3!=(*it2)->vMDE.end(); ++it3){
                            //~ if (checkHtmlTag(*(*it3),"display")){
                                //~ displayTag = true;
                                //~ display = rawDisplayContent(*(*it3));
                                //~ break;
                            //~ }
                        //~ }
                        //~ if (display)break;
                    //~ }
                //~ }
            //~ }
            //~ if (!subtags || !attributes || !displayTag || display){
                //~ newMDE->new_vMDE_element((*it)->name,(*it)->data);              
            //~ }
        //~ }
    //~ }
    //~ return newMDE;
//~ }




bool checkHtmlTag(MDE & MDE_source, std::string tag){
    //~ std::clog << "Checking if " << MDE_source.name << " is " << tag << std::endl;
    
    if (MDE_source.name == tag)return true;
    else return false; 
}


bool  htmlTagHasAttributes(MDE & MDE_source){
    //~ std::clog << "Are there attributes in " << MDE_source.name << std::endl;
    if (MDE_source.vMDE.size()==0){
        //~ std::clog<<"No attribute!" <<std::endl;
        return false;        
    }
    else{
        //~ for(std::vector<MDE*>::iterator it=MDE_source.vMDE.begin(); it!=MDE_source.vMDE.end(); ++it){
        for(auto it:MDE_source.vMDE){
            //~ if ((*it)->name=="attributes"){
            if (it->name=="attributes"){
                //~ std::clog<<"Attributes found!" <<std::endl;
                return true;
            }
        }
        return false;
    }
}

bool rawDisplayContent(MDE & MDE_source){
    if (MDE_source.data == "block" || MDE_source.data == "inline"){
        //~ fprintf(stderr,"It's a block!\n");
        return true;
    }
    else if (MDE_source.data == "none"){
        //~ fprintf(stderr,"It's a none!\n");
        return false;
    }
    else {
        fprintf(stderr,"value not recognized\n");
        return false;
    }    
}

bool checkHtmlAttribute(MDE & MDE_source, std::string attribute){
    if (MDE_source.vMDE.size()==0){
        //~ std::clog<<"No attribute name to parse" <<std::endl;
        fprintf(stderr,"No arguments, so default to display='block'\n");
        return false;        
    }
    else{
        //~ for(std::vector<MDE*>::iterator it=MDE_source.vMDE.begin(); it!=MDE_source.vMDE.end(); ++it){
        for(auto it:MDE_source.vMDE){
            //~ if ((*it)->name==attribute){
            if (it->name==attribute){
                //~ std::clog<<"Found " << attribute << std::endl;
                return true;
            }
        }
    }
    return false;
}

int hexToInt(int x)
{
    std::stringstream stream;
    int y;
    stream << x;
    stream >> std::hex >> y;
    return y;
}

bool hasCssStyle(MDE & MDE_source, bool isInline){
    bool hasAttributes = false;

    if (isInline){
        //~ std::clog << "looking for 'Style' inline" << std::endl;
        if(MDE_source.vMDE.size()==0){
            //~ std::clog << "Found no attributes, so no CSS style inline" << std::endl;
            return false;
        }
        //~ for(std::vector<MDE*>::iterator it=MDE_source.vMDE.begin(); it!=MDE_source.vMDE.end(); ++it){
        for(auto it:MDE_source.vMDE){
            //~ if(checkHtmlTag(*(*it),"attributes")){
            if(checkHtmlTag(*it,"attributes")){
                hasAttributes = true;
                //~ if((*it)->vMDE.size()==0){
                if(it->vMDE.size()==0){
                    //~ std::clog << "Attributes has no content, so no CSS style inline" << std::endl;
                    return false;                   
                }
                //~ for(std::vector<MDE*>::iterator it2=(*it)->vMDE.begin(); it2!=(*it)->vMDE.end(); ++it2){
                for(auto it2:it->vMDE){
                    //~ if(checkHtmlTag(*(*it2),"style")){
                    if(checkHtmlTag(*(it2),"style")){
                        //~ std::clog << "Found a style attribute!" << std::endl;
                        return true;
                    }
                }
            }
        }
    }
    //~ else{
        //~ std::clog << "looking for 'Style' element" << std::endl;
    //~ }
    
}

MDE * getCssStyle(MDE & MDE_source, bool isInline){
    //~ bool hasAttributes = false;

    if (isInline){
        //~ std::clog << "looking for 'Style' inline" << std::endl;
        if(MDE_source.vMDE.size()==0){
            //~ std::clog << "Found no attributes, so no CSS style inline" << std::endl;
            return NULL;
        }
        //~ for(std::vector<MDE*>::iterator it=MDE_source.vMDE.begin(); it!=MDE_source.vMDE.end(); ++it){
        for(auto it:MDE_source.vMDE){
            //~ if(checkHtmlTag(*(*it),"attributes")){
            if(checkHtmlTag(*it,"attributes")){
                //~ hasAttributes = true;
                //~ if((*it)->vMDE.size()==0){
                if(it->vMDE.size()==0){
                    //~ std::clog << "Attributes has no content, so no CSS style inline" << std::endl;
                    return NULL;                   
                }
                //~ for(std::vector<MDE*>::iterator it2=(*it)->vMDE.begin(); it2!=(*it)->vMDE.end(); ++it2){
                for(auto it2:it->vMDE){
                    //~ if(checkHtmlTag(*(*it2),"style")){
                    if(checkHtmlTag(*it2,"style")){
                        //~ std::clog << "Found a style attribute!" << std::endl;
                        return it2;
                    }
                }
            }
        }
    }
    //~ else{
        //~ std::clog << "looking for 'Style' element" << std::endl;
    //~ }
    
}

void PRINT_LOG(const std::string text, uint target_level){
    if(utils_log_level == target_level)std::clog<<text<<std::endl;
}

void set_utils_log_level(uint &_log_level)
{
    utils_log_level = _log_level;
};

//~ cssParser
        //~ std::string key;
        //~ std::string value;        
        //~ keyValuePairParser((*it)->data, key,value);
        //~ MDE_target->new_vMDE_element(key,value);

template <typename T>
void PRINT_VECTOR(const std::vector<T> &vec, uint target_level){
    if(utils_log_level == target_level)
    {
        std::clog<<"| ";
        for(auto it = vec.begin();it != vec.end();++it)
        {
            std::clog<<*it;
            if(std::next(it)!=vec.end()) std::clog<<", ";
        }
        std::clog << " |";
        std::clog << std::endl;
    }
}

// Template instantiation
template void PRINT_VECTOR<uint>(const std::vector<uint> &vec, uint target_level);



#endif /*UTILS_CPP*/
