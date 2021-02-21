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
    std::cout << "In utils!!!" << std::endl;
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

void extractXmlElement(std::string & bufferLine, std::string & buffer_Tag, std::string & element, std::vector<std::string> & list_elements)
{
    std::string buffer_End_of_Line;
    trimLeadingSpace(bufferLine);
    std::cout << "actually Here" << std::endl;
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
            splitString(buffer_Tag,list_elements);
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
            //~ MDE_source->vMDE.back()->name = "self_closing_tag";
            MDE_source.set_name_vMDE_back("self_closing_tag");
            // pop_back that last "/"
            list_elements.pop_back();
        }
        else
        {
            //~ MDE_source->vMDE.back()->name = element;
            MDE_source.set_name_vMDE_back(element);

        }
        //~ MDE_source->vMDE.back()->data = list_elements[0];
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
{
    std::string bufferString;
    std::string bufferLine;
    std::string buffer_Tag;
    std::string element;
    std::string buffer_End_of_Line;
    std::vector<std::string> list_elements;
    
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
        //~ bufferLine = bufferString;
        
        while (bufferLine.size()>0 && element!="__tag_part__")
        {
            
            extractXmlElement(bufferLine, buffer_Tag, element, list_elements);
            
            //now store into vMDE
            // only if element is not a partial tag
            if (element!="__tag_part__")
            {
                //~ MDE_source.processXmlElement(buffer_Tag, element, list_elements);
                XprocessXmlElement(MDE_source, buffer_Tag, element, list_elements);
            }
        }
    
    }
    std::cout << "Done" << std::endl;
    return true;
}

MDE * processFlatMDEToMDE(MDE & MDE_source,MDE & MDE_target,std::vector<MDE*>::iterator & it, std::stack<MDE**> & stack_MDE_pt)
{

    //~ while (it!=MDE_source.vMDE.end())
    while (it!=MDE_source.getVmdeEnd())
    {
        if ((*it)->name=="xml_declaration")
        {
            // name of this element is name of the flat MDE element (e.g. "xml_declaration")
            MDE_target.name = (*it)->name;
            // set empty data in case writing over existing MDE
            MDE_target.data="";
            
            MDE_target.new_vMDE_element((*it)->data,"");
            
            it++;

            if ((it!=MDE_source.getVmdeEnd()) && ((*it)->name=="attribute"))
            {
                while ((it!=MDE_source.getVmdeEnd()) && ((*it)->name=="attribute"))
                {
                    (MDE_target.getVmdeBack())->new_vMDE_element((*it)->name,(*it)->data);
                    ++it;
                }
            }
            
            (*(stack_MDE_pt.top()))->new_vMDE_element();
            std::cout << "stack top: " << (*(stack_MDE_pt.top()))->name << ", " << (*(stack_MDE_pt.top()))->data <<std::endl;
            //~ return (*(stack_MDE_pt.top()))->vMDE.back()->processFlatMDEToMDE(MDE_source,MDE_target,it, stack_MDE_pt);
            return processFlatMDEToMDE(MDE_source,*((*(stack_MDE_pt.top()))->getVmdeBack()),it, stack_MDE_pt);
        }
        
        else if ((*it)->name=="opening_tag")
        //~ if ((*it)->name=="opening_tag")
        {
            // name of this element is in data of the flat MDE
            MDE_target.name=(*it)->data;
            // set empty data in case writing over existing MDE
            MDE_target.data="";
            
            // store tag in stack only if it's an opening tag 
            // and not if it's a self closing tag
            //~ MDE** ptr_to_ptr_to_MDE = new MDE*(this);
            MDE** ptr_to_ptr_to_MDE = new MDE*(&MDE_target);
            stack_MDE_pt.push(ptr_to_ptr_to_MDE);
            // go to next element
            ++it;
            
            if ((it!=MDE_source.getVmdeEnd()) && ((*it)->name=="attribute"))
            {
                while ((it!=MDE_source.getVmdeEnd()) && ((*it)->name=="attribute"))
                {
                    MDE_target.new_vMDE_element((*it)->name,(*it)->data);
                    ++it;
                }

                if (it!=MDE_source.getVmdeEnd() && (*it)->name=="self_closing_tag")
                {
                    (*(stack_MDE_pt.top()))->new_vMDE_element();
                    std::cout << "stack top: " << (*(stack_MDE_pt.top()))->name << ", " << (*(stack_MDE_pt.top()))->data <<std::endl;
                    //~ return (*(stack_MDE_pt.top()))->vMDE.back()->processFlatMDEToMDE(MDE_source,MDE_target,it, stack_MDE_pt);
                    return processFlatMDEToMDE(MDE_source,*((*(stack_MDE_pt.top()))->getVmdeBack()),it, stack_MDE_pt);

                }
                else if (it!=MDE_source.getVmdeEnd() && (*it)->name=="data")
                {
                    std::cout << "stack top: " << (*(stack_MDE_pt.top()))->name << ", " << (*(stack_MDE_pt.top()))->data <<std::endl;
                    //~ return (*(stack_MDE_pt.top()))->processFlatMDEToMDE(MDE_source,MDE_target,it, stack_MDE_pt);
                    return processFlatMDEToMDE(MDE_source,*(*(stack_MDE_pt.top())),it, stack_MDE_pt);

                    
                }
                else if (it!=MDE_source.getVmdeEnd() && (*it)->name!="closing_tag") MDE_target.new_vMDE_element();
                //~ return MDE_target.vMDE.back()->processFlatMDEToMDE(MDE_source,MDE_target,it, stack_MDE_pt);
                return processFlatMDEToMDE(MDE_source,*(MDE_target.getVmdeBack()),it, stack_MDE_pt);

            }
            else if ((it!=MDE_source.getVmdeEnd()) && ((*it)->name=="data"))
            {
                MDE_target.data=(*it)->data;
                ++it;
                
                if ((it!=MDE_source.getVmdeEnd()) && ((*it)->name=="closing_tag"))
                {
                    //~ return MDE_target.processFlatMDEToMDE(MDE_source,MDE_target,it, stack_MDE_ptt);
                    return processFlatMDEToMDE(MDE_source,MDE_target,it, stack_MDE_pt);
                }
                else
                {
                    MDE_target.new_vMDE_element();
                    //~ return MDE_target.getVmdeBack()->processFlatMDEToMDE(MDE_source,MDE_target,it, stack_MDE_pt);
                    return processFlatMDEToMDE(MDE_source,*(MDE_target.getVmdeBack()),it, stack_MDE_pt);
                }
                
            }
            else if ((it!=MDE_source.getVmdeEnd()) && ((*it)->name=="closing_tag"))
            {
                //~ return MDE_target.processFlatMDEToMDE(MDE_source,MDE_target,it, stack_MDE_pt);
                return processFlatMDEToMDE(MDE_source,MDE_target,it, stack_MDE_pt);
            }
            else
            {
                MDE_target.data="";
                MDE_target.new_vMDE_element();
                //~ return MDE_target.vMDE.back()->processFlatMDEToMDE(MDE_source,MDE_target,it, stack_MDE_pt);
                return processFlatMDEToMDE(MDE_source,*(MDE_target.getVmdeBack()),it, stack_MDE_pt);
            }
        }
        else if ((it!=MDE_source.getVmdeEnd()) && ((*it)->name=="attribute"))
        {
            while ((it!=MDE_source.getVmdeEnd()) && ((*it)->name=="attribute"))
                {
                    MDE_target.new_vMDE_element((*it)->name,(*it)->data);
                    ++it;
                }
                
                if (it!=MDE_source.getVmdeEnd() && (*it)->name=="self_closing_tag")
                {
                    (*(stack_MDE_pt.top()))->new_vMDE_element();
                    std::cout << "stack top: " << (*(stack_MDE_pt.top()))->name << ", " << (*(stack_MDE_pt.top()))->data <<std::endl;
                    //~ return (*(stack_MDE_pt.top()))->vMDE.back()->processFlatMDEToMDE(MDE_source,MDE_target,it, stack_MDE_pt);
                    return processFlatMDEToMDE(MDE_source,*((*(stack_MDE_pt.top()))->getVmdeBack()),it, stack_MDE_pt);
                    
                }
                else if (it!=MDE_source.getVmdeEnd() && (*it)->name!="closing_tag") MDE_target.new_vMDE_element();
                std::cout << "End of attributes at " << (MDE_target.getVmdeBack())->data << std::endl;
                //~ return MDE_target.vMDE.back()->processFlatMDEToMDE(MDE_source,MDE_target,it, stack_MDE_pt);
                return processFlatMDEToMDE(MDE_source,*(MDE_target.getVmdeBack()),it, stack_MDE_pt);
    
        }
        else if(((*it)->name=="closing_tag") && it!=MDE_source.getVmdeBegin())
        {
            ++it;
            MDE** ptr_to_ptr_to_MDE = stack_MDE_pt.top();       // not sure why doing this 
            delete ptr_to_ptr_to_MDE;
            stack_MDE_pt.pop();
            // tricky: if next element is opening tag we create
            // a new vMDE element in the element at the top of the stack 
            // (which should be the parent element of this current one)

            if (it!=MDE_source.getVmdeEnd() && ((*it)->name=="opening_tag" || (*it)->name=="self_closing_tag" || (*it)->name=="comment_tag"))
            {
                (*(stack_MDE_pt.top()))->new_vMDE_element();
                //~ return (*(stack_MDE_pt.top()))->vMDE.back()->processFlatMDEToMDE(MDE_source,MDE_target,it, stack_MDE_pt);
                return processFlatMDEToMDE(MDE_source,*((*(stack_MDE_pt.top()))->getVmdeBack()),it, stack_MDE_pt);
            }
            else
            {
                if (!stack_MDE_pt.empty()) return processFlatMDEToMDE(MDE_source,*(*(stack_MDE_pt.top())),it, stack_MDE_pt);
                else return &MDE_target;
            }
        }
        else if ((it!=MDE_source.getVmdeEnd()) && ((*it)->name=="data"))
        {
            MDE_target.data=(*it)->data;
            ++it;
            
            if ((it!=MDE_source.getVmdeEnd()) && ((*it)->name=="closing_tag"))
            {
                //~ return MDE_target.processFlatMDEToMDE(MDE_source,MDE_target,it, stack_MDE_pt);
                return processFlatMDEToMDE(MDE_source,MDE_target,it, stack_MDE_pt);
            }
            else
            {
                MDE_target.new_vMDE_element();
                //~ return MDE_target.vMDE.back()->processFlatMDEToMDE(MDE_source,MDE_target,it, stack_MDE_pt);
                return processFlatMDEToMDE(MDE_source,*(MDE_target.getVmdeBack()),it, stack_MDE_pt);
            }
            
        }
        else if (it!=MDE_source.getVmdeEnd() && ((*it)->name=="self_closing_tag" || (*it)->name=="comment_tag"))
        {
            // name of this element is in data of the flat MDE
            MDE_target.name=(*it)->data;
            // set empty data in case writing over existing MDE
            MDE_target.data="";
            
            if (it!=MDE_source.getVmdeBegin())
            {
                ++it;
                if (it!=MDE_source.getVmdeEnd() && (*it)->name=="attribute")
                {
                    //~ return (*(stack_MDE_pt.top()))->vMDE.back()->processFlatMDEToMDE(MDE_source,MDE_target,it, stack_MDE_pt);
                    return processFlatMDEToMDE(MDE_source,*((*(stack_MDE_pt.top()))->getVmdeBack()),it, stack_MDE_pt);
                    
                }
                else if (it!=MDE_source.getVmdeEnd() && (*it)->name!="closing_tag") (*(stack_MDE_pt.top()))->new_vMDE_element();
                printf("test!");
                //~ return (*(stack_MDE_pt.top()))->vMDE.back()->processFlatMDEToMDE(MDE_source,MDE_target,it, stack_MDE_pt);
                return processFlatMDEToMDE(MDE_source,*((*(stack_MDE_pt.top()))->getVmdeBack()),it, stack_MDE_pt);
            }
            else
            {
                it++;
                MDE_target.new_vMDE_element();
                return processFlatMDEToMDE(MDE_source,*(MDE_target.getVmdeBack()),it, stack_MDE_pt);
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
    printf("Oh yeah!\n");
    //~ if (xmlFile.extract_File_to_Flat_MDE(full_filename)){
    if (extractFileToFlatMDE(xmlFile, full_filename)){
        MDE_source.display_all(0,2);
        printf("hola\n");
        std::vector<MDE*>::iterator it_start = xmlFile.getVmdeBegin();
        printf("hola\n");
        // store 'this' in the stack of pointers to be able to come back
        // to this element if there is an xml declaration
        // Need to create a new pointer as using 'this' directly is not possible (temp scope object)
        //~ MDE ** pt_to_pt_MDE = new MDE*(this);
        MDE ** pt_to_pt_MDE = new MDE*(&MDE_source);
        stack_MDE_pt.push(pt_to_pt_MDE);
        std::cout << "name,data" << MDE_source.name << ", " << MDE_source.data << std::endl;
        //~ this->vMDE[index]->process_flat_MDE_to_MDE(it_start, xmlFile, stack_MDE_pt);
        //~ processFlatMDEToMDE(xmlFile,*this,it_start, stack_MDE_pt);
        //~ processFlatMDEToMDE(xmlFile,*this->vMDE[index],it_start, stack_MDE_pt);
        std::vector<int> vect_index = {index};
        processFlatMDEToMDE(xmlFile,*(MDE_source.get_vMDE(vect_index)),it_start, stack_MDE_pt);

        printf("hola\n");
        
        std::cout << "Here 3" << std::endl;
        
        xmlFile.display_all(0,2);
        
        std::cout << "Here 4" << std::endl;
        
        MDE_source.display_all(0,2);
        xmlFile.destructor_MDE();
        delete pt_to_pt_MDE;
        return true;
    }
    else return false;
        
}

bool loadXMLFileToMDE(MDE & MDE_source, const std::string & full_filename)
{

    MDE xmlFile;
    std::stack<MDE**> stack_MDE_pt;
    
    //~ if (xmlFile.extract_File_to_Flat_MDE(full_filename)){
    if (extractFileToFlatMDE(xmlFile, full_filename)){
        std::vector<MDE*>::iterator it_start =  xmlFile.getVmdeBegin();
        processFlatMDEToMDE(xmlFile, MDE_source, it_start,  stack_MDE_pt);
        xmlFile.destructor_MDE();
        return true;
    }
    else return false;
        
}

bool replaceVMDEwithXML(MDE & MDE_source,std::vector<int> & vect_index_file, const std::string & full_filename)
{
    std::cout << "file to load:" << full_filename << std::endl;
    //~ newMDE.destructor_MDE();
    std::vector<int>::const_iterator it_file;
    bool input_file = false;
    int index_file;
    MDE_source.display_vector_int(vect_index_file);
    //~ vect_index_file.push_back(0);
    //~ newMDE.display_vector_int(vect_index_file);
    MDE_source.clear_all_vMDE_by_index(vect_index_file);
    std::vector<int> tmp_vect_index= vect_index_file;
    tmp_vect_index.pop_back();
    //~ vect_index_file.pop_back();
    MDE_source.display_vector_int(tmp_vect_index);
    index_file = vect_index_file.back();
    printf("index file:%i\n", index_file);


    std::cout << "here 0" << std::endl;


    // get vDME pointer at index just before last of vector of indexes, then load at that last index.
    //~ input_file = (this->vMDE_get_by_index(tmp_vect_index,it_file))->load_XML_File_to_vMDE(index_file, full_filename);
    input_file = loadXMLFileTovMDE(*(MDE_source.get_vMDE(tmp_vect_index)), index_file, full_filename);
    
    std::cout << "here 1" << std::endl;

    
    // set the name of the file to the mde pointer at the given index, one level above what has been loaded
    MDE_source.get_vMDE(tmp_vect_index)->set_data(full_filename);
    
    std::cout << "here 2" << std::endl;

    return input_file;
}

void extractMDEdataForVoffset(MDE & MDE_source, std::vector<GLdata> & v_MDE_data, int & level, int index)
{
    level++;
    current_GLdata.level= level;
    current_GLdata.index= index;
    current_GLdata.mde_member=MDE_member::name;
    //~ current_GLdata.MDE_data=this->name;
    current_GLdata.MDE_data=MDE_source.name;
    v_MDE_data.push_back(current_GLdata);
    current_GLdata.mde_member=MDE_member::data;
    //~ current_GLdata.MDE_data=this->data;
    current_GLdata.MDE_data=MDE_source.data;
    v_MDE_data.push_back(current_GLdata);
    //~ if (this->vMDE.size()!=0) {index++; };
    if (MDE_source.getVmdeSize()!=0) {index++; };
    //~ for (std::vector<MDE*>::iterator it_vMDE=this->vMDE.begin();it_vMDE!=this->vMDE.end();++it_vMDE)
    for (std::vector<MDE*>::iterator it_vMDE=MDE_source.getVmdeBegin();it_vMDE!=MDE_source.getVmdeEnd();++it_vMDE)
        {
            //~ (*it_vMDE)->extract_MDE_data_for_v_offset(v_MDE_data, level, index);
            extractMDEdataForVoffset(*(*it_vMDE), v_MDE_data, level, index);
        }
    return;
}

void displayVMDEdata(std::vector<GLdata> & v_offset){
    for (std::vector<GLdata>::iterator it=v_offset.begin(); it!=v_offset.end(); ++it){
        printf("level, index,member,data: %i,%i,%i,%s\n",
         (*it).level, (*it).index, (*it).mde_member, (*it).MDE_data.c_str());
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

    if (MDE_source.getVmdeSize()!=0) n_indent++;
    for (std::vector<MDE*>::iterator it=MDE_source.getVmdeBegin();it!=MDE_source.getVmdeEnd();++it)
        {
            formatDisplay(*(*it),n_indent, n_space, start_opening_tag,
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
        for (std::vector<MDE**>::iterator it=search_result.begin();it!=search_result.end();++it)
        {
            //~ (*(*it))->format_display(0,n_space,"<",">","</",">");
            formatDisplay(**(*it),0,n_space,"<",">","</",">");
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
        }
    }
    //~ this->simple_loop_VMDE_next_level(searchName);
    simpleLoopVMDEnextLevel(MDE_source,searchName, search_result);
}

void simpleLoopVMDEnextLevel(MDE & MDE_source, std::string searchName,  std::vector<MDE**> &search_result)
{
    for(std::vector<MDE*>::const_iterator it=MDE_source.getVmdeBegin();it!=MDE_source.getVmdeEnd();++it)
    {
        //~ (*it)->simple_loop_VMDE(searchName);
        simpleLoopVMDE(*(*it),searchName, search_result);
    }
}

#endif /*UTILS_CPP*/
