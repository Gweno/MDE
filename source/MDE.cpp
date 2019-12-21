//-----------MDE.cpp-------------
#ifndef MDE_CPP
#define MDE_CPP

#include <iostream>
#include <fstream>
#include <vector>
#include "MDE.h"
#include <sstream>
#include <string.h>
#include <string>

//Constructors:
MDE::MDE(){};

MDE::MDE(std::string init_name, std::string init_data, std::vector<MDE*> init_vMDE)
{
    this->name = init_name;
    this->data = init_data;
    for (std::vector<MDE*>::iterator it=init_vMDE.begin(); it != init_vMDE.end(); ++it)
    {
        this->vMDE.push_back(*it);
    }
}


// Destructors:

MDE::~MDE()
{
    // delete all memory pointed to in vMDE;
    for (std::vector<MDE*>::iterator it=this->vMDE.begin();it!=this->vMDE.end();++it)
    {
        delete *it;
    }
};

// member function to create new Entity

void MDE::new_vMDE_element()
{
    this->vMDE.push_back(new MDE);
};

void MDE::set_name(std::string eName)
{
    this->name=eName;
}

void MDE::set_data(std::string data)
{
    this->data=data;
}

void MDE::set_name_vMDE_back(std::string vName)
{
    this->vMDE.back()->name = vName;
};

void MDE::set_data_vMDE_back(std::string vData)
{
    this->vMDE.back()->data = vData;
};

void MDE::set_name_vMDE_index(int index_vMDEe,std::string vName)
{
    this->vMDE.at(index_vMDEe)->name = vName;
};

void MDE::set_data_vMDE_index(int index_vMDEe,std::string vData)
{
    this->vMDE.at(index_vMDEe)->data = vData;
};

// Get member functions;

std::string MDE::get_name()
{
    return this->name;
};

std::string MDE::get_name_vMDE_by_index(int index_vMDEe)
{
    return this->vMDE.at(index_vMDEe)->name;
};

std::string MDE::get_data_vMDE_by_index(int index_vMDEe)
{
    return this->vMDE.at(index_vMDEe)->data;
};


// Clear member functions;

void MDE::clear_name()
    {
        this->set_name("");
    }

void MDE::clear_data()
    {
    this->set_data("");
    }

void MDE::clear_all_vMDE()
{
    if (!this->vMDE.empty())
    {
        for (std::vector<MDE*>::iterator it_all=this->vMDE.begin();it_all!=this->vMDE.end();++it_all)
        {
            (*it_all)->clear_all_vMDE();
        }
        this->vMDE.clear();
    }
    else delete this;
}

void MDE::delete_all_pt()
{
    for (std::vector<MDE*>::iterator it_all=this->vMDE.begin();it_all!=this->vMDE.end();++it_all)
    {
        delete (*it_all);
    }
}

void MDE::destructor_MDE()
{
    this->delete_all_pt();
    this->vMDE.clear();
    this->name = "";
    this->data = "";
}



        
// public member functions




// get member functions

void MDE::display_name()
// Entity gets its own name and display
{
    std::cout << this->name << std::endl;
}

void MDE::display_data()
// Entity gets its own data and display
{
    std::cout << this->data << std::endl;
}

void MDE::display_vMDE_names()
{
    for (std::vector<MDE*>::iterator it=this->vMDE.begin();it!=this->vMDE.end();++it){
        (*it)->display_name();
    }
};

void MDE::display_vMDE_data()
{
    for (std::vector<MDE*>::iterator it=this->vMDE.begin();it!=this->vMDE.end();++it){
        (*it)->display_data();
    }
};


void MDE::display_all(int n_indent, const int & n_space)
{
    std::string N_space(n_indent*n_space,' ');
    std::cout << N_space ;
    std::cout << this->name << " : " << this->data << std::endl;
    if (this->vMDE.size()!=0) n_indent++;
    for (std::vector<MDE*>::iterator it_vMDE=this->vMDE.begin();it_vMDE!=this->vMDE.end();++it_vMDE)
        {
            (*it_vMDE)->display_all(n_indent, n_space);
        }
    return;
};

GLdata current_GLdata;

void MDE::extract_MDE_data_for_v_offset(std::vector<GLdata> & v_MDE_data, int & level, int index)
{
    level++;
    current_GLdata.level= level;
    current_GLdata.index= index;
    current_GLdata.mde_member=MDE_member::name;
    current_GLdata.MDE_data=this->name;
    v_MDE_data.push_back(current_GLdata);
    current_GLdata.mde_member=MDE_member::data;
    current_GLdata.MDE_data=this->data;
    v_MDE_data.push_back(current_GLdata);
    if (this->vMDE.size()!=0) {index++; };
    for (std::vector<MDE*>::iterator it_vMDE=this->vMDE.begin();it_vMDE!=this->vMDE.end();++it_vMDE)
        {
            (*it_vMDE)->extract_MDE_data_for_v_offset(v_MDE_data, level, index);
        }
    return;
}

void MDE::display_v_MDE_data(std::vector<GLdata> & v_offset){
    for (std::vector<GLdata>::iterator it=v_offset.begin(); it!=v_offset.end(); ++it){
        printf("level, index,member,data: %i,%i,%i,%s\n",
         (*it).level, (*it).index, (*it).mde_member, (*it).MDE_data.c_str());
    }
}
void MDE::format_display(int n_indent, const int & n_space, std::string start_opening_tag,
            std::string end_opening_tag, std::string start_closing_tag,
            std::string end_closing_tag)
{
    
    std::string N_space(n_indent*n_space,' ');
    std::cout << N_space ;
    std::cout<< start_opening_tag;
    std::cout << this->name;
    std::cout<< end_opening_tag;
    std::cout << endl;
    std::cout << N_space;
    std::cout << this->data;
    std::cout << endl;

    if (this->vMDE.size()!=0) n_indent++;
    for (std::vector<MDE*>::iterator it=this->vMDE.begin();it!=this->vMDE.end();++it)
        {
            (*it)->format_display(n_indent, n_space, start_opening_tag,
            end_opening_tag, start_closing_tag, end_closing_tag);
            std::cout << endl;
        }
    std::cout << N_space ;
    std::cout<< start_closing_tag;
    std::cout << this->name;
    std::cout<< end_closing_tag;
    return;
};

void MDE::user_input_vMDE()
{
    string key_input;
    int index=0;
    
    std::cout<< "Does Entity " << this->name << " : " << this->data << " has sub-entities (y/n)?" << std::endl;
    std:: cin >> key_input;
    if (key_input!="y") return;
    
    // loop to enter Entities at index > 0
    while (key_input!="quit" && key_input!="end")
    {
        std::cout << "enter a name" << std::endl;
        std:: cin >> key_input;
        if (key_input!="quit" && key_input!="end")
        {
            this->new_vMDE_element();
            this->set_name_vMDE_index(index,key_input);
            std::cout << "enter data" << std::endl;
            cin.ignore(); 
            std:: getline(std:: cin, key_input);                
            if (key_input!="quit")
            {
                this->set_data_vMDE_index(index,key_input);
                index++;
            }
        }
        else if (key_input=="end")
        {
            for (std::vector<MDE*>::iterator it=this->vMDE.begin();it!=this->vMDE.end();++it)
            {
                (*it)->user_input_vMDE();                           //Where recursivity happen...
            }
        }
    }
    return;
}


void MDE::save_to_file(int n_indent, const int & n_space,
            std::string start_opening_tag, std::string end_opening_tag,
            std::string start_closing_tag, std::string end_closing_tag,
            ostream &file)
{
    
    std::string N_space(n_indent*n_space,' ');
    file << N_space;
    file << start_opening_tag;
    file << this->name;
    file << end_opening_tag;
    file << std::endl;
    file << N_space;
    file << this->data;
    file << std::endl;
    
    if (this->vMDE.size()!=0) n_indent++;
    for (std::vector<MDE*>::iterator it=this->vMDE.begin();it!=this->vMDE.end();++it)
        {
            (*it)->save_to_file(n_indent, n_space, start_opening_tag,
            end_opening_tag, start_closing_tag, end_closing_tag, file);
            if (it == this->vMDE.end()-1) n_indent--;
            
        }
    
    file << N_space;
    file << start_closing_tag;
    file << this->name;
    file << end_closing_tag;
    file << std::endl;
    return;
};

void MDE::print_flat_MDE()
{
    std::cout << this->name << " : " << this->data << std::endl;
    if (this->vMDE.size()!=0)
    {
        for (std::vector<MDE*>::iterator it=this->vMDE.begin();it!=this->vMDE.end();++it)
        {
            (*it)->print_flat_MDE();
        }
    }
    return;
};

void MDE::user_save_to_XML_file(){

    char *FileName = new char[20];
    char *FileNameOut = new char[75];
    char *ExtensionXML = new char[5];
    char *Path = new char[50];
    int n_indent = 0;
    const int n_space = 2;
    FileNameOut[0]=0;                   // to clear the array of Char
    strcpy(ExtensionXML,".xml");
    strcpy(Path,"../datafiles/");
    std::cout << "\nEnter the name of the file you want to create: " << std::endl;
    std::cout << "(Extension '.xml' is added to the file name)" << std::endl;
    std::cin >> FileName;
    strcat(FileNameOut, Path);
    strcat(FileNameOut,FileName);
    strcat(FileNameOut, ExtensionXML);
    std::ofstream file(FileNameOut);
    this->save_to_file(n_indent, n_space,"<",">","</",">", file);
    
    // destroy * char[]
    delete [] FileName;
    delete [] FileNameOut;
    delete [] ExtensionXML;
    delete [] Path;
}


void MDE::fetch_search_result(std::vector<MDE**> &search_result)
{
    const int n_space = 2;
    if (search_result.size()!=0)
    {
        for (std::vector<MDE**>::iterator it=search_result.begin();it!=search_result.end();++it)
        {
            (*(*it))->format_display(0,n_space,"<",">","</",">");
            std::cout << endl;
        }
    }
    else std::cout << "Not found" <<std::endl;
}

void MDE::search_For(int & index, int & level, std::string searchName)
{
    if (this->name==searchName)
    {
        std::cout << "Found " << searchName << " at (level,index): " << "(" << level  << "," << index << ")" << std::endl;
        std::cout << std::endl;
        MDE** ptr_to_ptr_to_MDE = new MDE*(this);
        MDE::search_result.push_back(ptr_to_ptr_to_MDE);
    }
    else
    {
        level++;
        index=0;
        (this->simple_loop_VMDE(searchName));
    }
    if (MDE::search_result.size()==0)
    {
        std::cout << "No results" << std::endl; //not ideal...
    }
    std::cout << "Found " << MDE::search_result.size() << " Entities with name " << searchName << std::endl;
}

void MDE::simple_loop_VMDE(std::string searchName)
{
    for(std::vector<MDE*>::const_iterator it=this->vMDE.begin();it!=this->vMDE.end();++it)
    {
        if ((*it)->name==searchName)
        {
            MDE** ptr_to_ptr_to_MDE = new MDE*(*it);
            MDE::search_result.push_back(ptr_to_ptr_to_MDE);
        }
    }
    this->simple_loop_VMDE_next_level(searchName);
}

void MDE::simple_loop_VMDE_next_level(std::string searchName)
{
    for(std::vector<MDE*>::const_iterator it=this->vMDE.begin();it!=this->vMDE.end();++it)
    {
        (*it)->simple_loop_VMDE(searchName);
    }
}

void MDE::str_token_tag(std::string & buffer_Input,
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
        output_Tag = buffer_Input.substr(pos1+1,pos2-pos1-1);
        buffer_Output = buffer_Input.substr(pos2+1);
        }
        else
        {
            output_Tag="";
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
        if (output_Tag.size()>0) this->trim_leading_space(output_Tag);       // trimming leading spaces
        }
        else
        {
            output_Tag="";
            buffer_Output = buffer_Input;
        }
    }
}

void MDE::trim_leading_space(std::string & io_string)
{
    size_t pos = 0;
    pos = io_string.find_first_not_of(' ');
    if (pos)
    {
        io_string = io_string.substr(pos);
    }
}

bool MDE::load_XML_File_to_MDE(const std::string & fullFileName)
{

    MDE xmlFile;
    std::stack<MDE**> stack_MDE_pt;
    
    if (xmlFile.extract_File_to_Flat_MDE(fullFileName)){
        std::vector<MDE*>::iterator it_start = xmlFile.vMDE.begin();
        this->process_flat_MDE_to_MDE(it_start, xmlFile, stack_MDE_pt);
        xmlFile.destructor_MDE();
        return true;
    }
    else return false;
        
}



bool MDE::extract_File_to_Flat_MDE(const std::string & fullFileName)
{
    std::string bufferString;
    std::string bufferLine;
    std::string buffer_Tag;
    std::string element;
    std::string buffer_End_of_Line;
    
    // Processing file
    ifstream fileEntity;
    fileEntity.open(fullFileName.c_str(), ios::in);
    
    if(!fileEntity.is_open()){
        
        printf("File not found\n");
        return false;
        
    }

    fileEntity.clear();                   // absolutly needed otherwise the file is flagged at eof, and good=0   
    fileEntity.seekg(0, ios::beg);        // set cursor at 0 from start of file

    this->name = "File";
    this->data = fullFileName;
    
    while ((fileEntity.good()) && (!fileEntity.eof()))     // loop while extraction from file is possible
    {
        // get a line from the file
        std::getline(fileEntity, bufferString);

        bufferLine = bufferString;
        
        while (bufferLine.size()>0)
        {
            this->trim_leading_space(bufferLine);
            if (bufferLine[0]=='<')
            {
                this->str_token_tag(bufferLine,buffer_Tag,buffer_End_of_Line,'<','>');
                element ="tag";
                this->check_token(buffer_Tag, element);
            }
            else
            {
                this->str_token_tag(bufferLine,buffer_Tag,buffer_End_of_Line,0,'<');
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
                this->check_token(buffer_Tag, element);                         // buffer_End_of_Line helps to decide if data is empty or not
                
            }
            bufferLine = buffer_End_of_Line;
            
            //now store into vMDE
            this->new_vMDE_element();
            this->vMDE.back()->name = element;
            this->vMDE.back()->data = buffer_Tag;
            
            
        }
    
    }
    return true;
}

void MDE::check_token(const std::string & token, std::string & element)
{
    if(token.size()>0)
    {
        if(element=="tag")
        {
            if(token[0]=='/') element = "closing_tag";
            else element = "opening_tag";
        }
    }
    else
    {
        element = "null_" + element;
    }
}


MDE * MDE::process_flat_MDE_to_MDE(std::vector<MDE*>::iterator & it, MDE & MDE_source, std::stack<MDE**> & stack_MDE_pt)
{

    while (it!=MDE_source.vMDE.end())
    {
        if ((*it)->name=="opening_tag")
        {
            this->name=(*it)->data;
            ++it;
            MDE** ptr_to_ptr_to_MDE = new MDE*(this);
            stack_MDE_pt.push(ptr_to_ptr_to_MDE);

            if ((it!=MDE_source.vMDE.end()) && ((*it)->name=="data"))
            {
                this->data=(*it)->data;
                ++it;
                
                if ((it!=MDE_source.vMDE.end()) && ((*it)->name=="closing_tag"))
                {
                    return this->process_flat_MDE_to_MDE(it, MDE_source, stack_MDE_pt);
                }
                else
                {
                    this->new_vMDE_element();
                    return this->vMDE.back()->process_flat_MDE_to_MDE(it, MDE_source, stack_MDE_pt);
                }
                
            }
            else if ((it!=MDE_source.vMDE.end()) && ((*it)->name=="closing_tag"))
            {
                return this->process_flat_MDE_to_MDE(it, MDE_source, stack_MDE_pt);
            }
            else                // else  if next is 'data'
            {
                this->data="";
                this->new_vMDE_element();
                return this->vMDE.back()->process_flat_MDE_to_MDE(it, MDE_source, stack_MDE_pt);
            }
        }
        else if(((*it)->name=="closing_tag") && it!=MDE_source.vMDE.begin());
        {
            ++it;
            MDE** ptr_to_ptr_to_MDE = stack_MDE_pt.top();
            delete ptr_to_ptr_to_MDE;
            stack_MDE_pt.pop();
            // tricky: if next element is opening tag we create
            // a new vMDE element in the element at teh top of the stack 
            // (which should be the parent element of this current one)
            if ((it!=MDE_source.vMDE.end()) && ((*it)->name=="opening_tag"))
            {
                (*(stack_MDE_pt.top()))->new_vMDE_element();
                return (*(stack_MDE_pt.top()))->vMDE.back()->process_flat_MDE_to_MDE(it, MDE_source, stack_MDE_pt);
            }
            else
            {
                if (!stack_MDE_pt.empty()) return (*(stack_MDE_pt.top()))->process_flat_MDE_to_MDE(it, MDE_source, stack_MDE_pt);
                else return this;
            }
        }
    }
    return this;
}

void MDE::testing()
{
    this->new_vMDE_element();
    this->new_vMDE_element();
    this->vMDE.at(0)->new_vMDE_element();
    this->vMDE.at(0)->new_vMDE_element();
    this->vMDE.at(1)->new_vMDE_element();
    this->vMDE.at(1)->new_vMDE_element();
    this->set_name("master");
    this->set_data("123");
    this->set_name_vMDE_index(0,"tata");
    this->set_data_vMDE_index(0,"1");
    this->set_name_vMDE_index(1,"titi");
    this->set_data_vMDE_index(1,"2");
    //~ this->vMDE.at(0)->set_name_vMDE_index(0,"lala");
    this->vMDE.at(0)->set_name_vMDE_index(0,"toto");
    this->vMDE.at(0)->set_data_vMDE_index(0,"11");
    this->vMDE.at(0)->set_name_vMDE_index(1,"lili");
    this->vMDE.at(0)->set_data_vMDE_index(1,"12");
    this->vMDE.at(1)->set_name_vMDE_index(0,"toto");
    this->vMDE.at(1)->set_data_vMDE_index(0,"21");
    this->vMDE.at(1)->set_name_vMDE_index(1,"lolo");
    this->vMDE.at(1)->set_data_vMDE_index(1,"22");
    
    this->vMDE.at(0)->vMDE.at(0)->new_vMDE_element();
    this->vMDE.at(0)->vMDE.at(0)->set_name_vMDE_index(0,"truc");
    this->vMDE.at(0)->vMDE.at(0)->set_data_vMDE_index(0,"999");
}


void MDE::display_vector_int(std::vector<int>& vect_index) {
    
    std::cout << "(" ;
    for(std::vector<int>::const_iterator it=vect_index.begin();it!=vect_index.end();++it)
    {
        std::cout << *it;
        if (it!=vect_index.end()-1) std::cout << ", " ;
    }
    std::cout << ")" ;
}


MDE * MDE::vMDE_get_by_index(std::vector<int>& vect_index, std::vector<int>::const_iterator it)
{
    int store_it=0;
    if (vect_index.size()==0)
    {
        return this;
    }
    else if (vect_index.size()==1)
    {
        return (this->vMDE).at(vect_index.front());
    }
    else if ((vect_index.size()>1) && (it!=vect_index.end()-1))
    {
        store_it=*it;
        return ((this->vMDE).at(store_it))->vMDE_get_by_index(vect_index,++it);
    }
    else
    {
        return this->vMDE.at(vect_index.back());
    }
}

void MDE::vMDE_copy_To_Vector_Float(std::vector<float>& vFloat)
{
    float num;
    for (std::vector<MDE*>::iterator it = this->vMDE.begin(); it != this->vMDE.end(); ++it)
    {
        
        if(!(istringstream((*it)->data)>>num)) num=0;
        vFloat.push_back(num);
    }
}

//  Index management

std::vector<int> MDE::set_vector_of_indexes(std::string list_indexes)
{
    std::vector<int> vect_index;
    std::string delimiter = ",";
    
    size_t pos = 0;
    std::string token;
    int num;
    while ((pos = list_indexes.find(delimiter)) != std::string::npos) {
        token = list_indexes.substr(0, pos);
        std::cout << token;
        if(!(istringstream(token)>>num)) num=0;
        vect_index.push_back(num);
        list_indexes.erase(0, pos + delimiter.length());
    }
    std::cout << list_indexes << std::endl;
    if(!(istringstream(list_indexes)>>num)) num=0;
    vect_index.push_back(num);    
    return vect_index;
}

void MDE::search_results_to_vector_float(std::vector<float> &v_float, std::vector< vector <float> > &v_v_float)
{
    for (std::vector<MDE**>::iterator it=MDE::search_result.begin(); it != MDE::search_result.end(); ++it)
    {
        (*(*it))->vMDE_copy_To_Vector_Float(v_float);
        v_v_float.push_back(v_float);
        v_float.clear();
    }
    
}

std::vector<MDE**> MDE::search_result;

#endif /*MDE.cpp*/
