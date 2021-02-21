#include "mdeUtils.h"

bool mdeUtils::extract_File_to_Flat_MDE(const std::string & full_filename)
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
    
    if(!fileEntity.is_open()){
        
        printf("File not found\n");
        return false;
        
    }

    fileEntity.clear();                   // absolutly needed otherwise the file is flagged at eof, and good=0   
    fileEntity.seekg(0, ios::beg);        // set cursor at 0 from start of file

    this->name = "File";
    this->data = full_filename;
    
    while ((fileEntity.good()) && (!fileEntity.eof()))     // loop while extraction from file is possible
    {
        // get a line from the file
        std::getline(fileEntity, bufferString);

        // check if element has the flag, it is a partial tag (a tag that has a '\n' in the source file)
        if (element=="__tag_part__")
        {
            // in which case we add the new extracted line at the end of previous line
            // but triming new loaded line first
            //~ this->trim_leading_space(bufferString);
            trimLeadingSpace(bufferString);
            bufferLine += " " + bufferString;
            // and delete the flag
            element = "";
        }
        else bufferLine = bufferString;
        //~ bufferLine = bufferString;
        
        while (bufferLine.size()>0 && element!="__tag_part__")
        {
            //~ this->trim_leading_space(bufferLine);
            trimLeadingSpace(bufferLine);
            if (bufferLine[0]=='<')
            {
                //~ this->str_token_tag(bufferLine,buffer_Tag,buffer_End_of_Line,'<','>');
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
                    //~ this->check_token(buffer_Tag, element);
                    checkToken(buffer_Tag, element);
                    //~ this->split_string(buffer_Tag,list_elements);
                    splitString(buffer_Tag,list_elements);
                    
                    {
                        //~ std::cout << "first element: " << list_elements[0] << std::endl;
                        //~ std::cout << "last element: " << list_elements.back() << std::endl;
                        //~ for (vector<string>::iterator it=list_elements.begin(); it!=list_elements.end(); ++it) 
                        //~ {
                            //~ cout<<"before it"<<endl;
                            //~ cout<<*it<<endl;
                            //~ cout<<"after it"<<endl;
                        //~ }
                        //~ cout<<"after loop"<<endl;
                    }
                }
            }
            else
            {
                //~ this->str_token_tag(bufferLine,buffer_Tag,buffer_End_of_Line,0,'<');
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
            
            //now store into vMDE
            // only if element is not a partial tag
            if (element!="__tag_part__")
            {
                // check if the element is a tag and if the tag has many elements with it
                if ((element == "opening_tag" || element == "closing_tag") && list_elements.size()>1)
                {
                    // first element of the list of element is always the tag's name
                    this->new_vMDE_element();
                    // check if last element is a "/" then it's a self-closing tag
                    if (list_elements.back()=="/")
                    {
                        this->vMDE.back()->name = "self_closing_tag";
                        // pop_back that last "/"
                        list_elements.pop_back();
                    }
                    else
                    {
                        this->vMDE.back()->name = element;
                    }
                    this->vMDE.back()->data = list_elements[0];
                    // then stores all other elements as 'attribute'
                    for (vector<string>::iterator it=list_elements.begin()+1; it!=list_elements.end(); ++it) 
                    {
                        this->new_vMDE_element();
                        this->vMDE.back()->name = "attribute";
                        this->vMDE.back()->data = *it;
                    }
                }
                else if ((element == "xml_declaration")  && list_elements.size()>1)
                {
                    this->new_vMDE_element();
                    
                    // rough management of attributes here... need to take away last '?' of the tag
                    this->vMDE.back()->name = element;
                    this->vMDE.back()->data = list_elements[0];
                    
                    for (vector<string>::iterator it=list_elements.begin()+1; it!=list_elements.end(); ++it) 
                    {
                        this->new_vMDE_element();
                        this->vMDE.back()->name = "attribute";
                        this->vMDE.back()->data = *it;
                    }
                }
                else
                {
                    this->new_vMDE_element();
                    this->vMDE.back()->name = element;
                    this->vMDE.back()->data = buffer_Tag;
                }
            }
        }
    
    }
    std::cout << "Done" << std::endl;
    return true;
}
