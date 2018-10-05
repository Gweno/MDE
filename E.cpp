//-----------E.cpp-------------
#ifndef E_CPP
#define E_CPP

#include <iostream>
#include <fstream>
#include <vector>
#include "E.h"
#include <sstream>
#include <string.h>
#include <string>
#include "glFunctions.h"

//~ E::E()
//~ {
    //~ std::cout << "do nothing" << std::endl;
//~ };

E::E(){};

E::E(std::string init_name, std::string init_data, std::vector<E*> init_vE)
{
    this->name = init_name;
    this->data = init_data;
    for (std::vector<E*>::iterator it=init_vE.begin(); it != init_vE.end(); ++it)
    {
        this->vE.push_back(*it);
    }
}

// Destructors:
// Destructor at one level:
E::~E()
{
    // delete all memory pointed to in vE;
    for (std::vector<E*>::iterator it=this->vE.begin();it!=this->vE.end();++it)
    {
        delete *it;
    }
};
// :

void E::delete_all_recursive()
{
    if (!this->vE.empty())
    {
        for (std::vector<E*>::iterator it_all=this->vE.begin();it_all!=this->vE.end();++it_all)
        {
            (*it_all)->delete_all_recursive();
        }
        this->vE.clear();
    }
    else delete this;
    
}

void E::delete_all_pt()
{
    for (std::vector<E*>::iterator it_all=this->vE.begin();it_all!=this->vE.end();++it_all)
    {
        delete (*it_all);
    }
}

void E::destructor_E()
{
    this->delete_all_pt();
    this->vE.clear();
    this->name = "";
    this->data = "";
}

void E::vE_clear_all()
{
    for (std::vector<E*>::iterator it_all=this->vE.begin();it_all!=this->vE.end()-1;++it_all)
    {
        if (!this->vE.empty())
        {
            E * pt_to_E = (*it_all);
            delete pt_to_E;
            (*it_all)->vE_clear_all();
            
        }
        else 
        {
            this->vE.clear();
            break;
        }
    }
    this->vE.clear();
}

void E::E_clear_name()
    {
        this->E_set_name("");
    }

void E::E_clear_data()
    {
    this->E_set_data("");
    }
        
// public methods


// E methods

// Methode to create new Entity

void E::new_vE_element()
// Entity creates a new (pointers to) Entity in the vector vE
// with a push_back.        
{
    this->vE.push_back(new E);
};

// set a name from a string into member 'name' of an instance of
// an object of class E
void E::E_set_name(string eName)
{
    this->name=eName;
}

// set a single data from a string into member 'data' of 
//an instance of an object of class E
void E::E_set_data(string data)
{
    this->data=data;
}

// Entity set name for its last Entity in the vector vE

void E::set_last_vEe_name(string vName)
{
    this->vE.back()->name = vName;
};
// Entity set data for its last Entity in the vector vE

void E::set_last_vEe_data(string vData)
{
    this->vE.back()->data = vData;
};

// Entity set name for an Entity in its vector of pointers
// of Entities at a given index (no exception handling)

void E::set_vEe_name(int index_vEe,string vName)
{
    this->vE.at(index_vEe)->name = vName;
};

// Entity set data for an Entity in its vector of pointers
// of Entities at a given index (no exception handling)

void E::set_vEe_data(int index_vEe,string vData)
{
    this->vE.at(index_vEe)->data = vData;
};


// get methods

void E::E_display_name()
// Entity gets its own name and display
{
    std::cout << this->name << std::endl;
}

void E::E_display_data()
// Entity gets its own data and display
{
    std::cout << this->data << std::endl;
}

void E::vE_display_all_names()
{
    for (std::vector<E*>::iterator it=this->vE.begin();it!=this->vE.end();++it){
        (*it)->E_display_name();
    }
};

void E::vE_display_all_data()
{
    for (std::vector<E*>::iterator it=this->vE.begin();it!=this->vE.end();++it){
        (*it)->E_display_data();
    }
};


void E::E_display_all(int n_space)
{
    std::string N_space(n_space*4,' ');
    std::cout << N_space ;
    std::cout << this->name << " : " << this->data << std::endl;
    if (this->vE.size()!=0) n_space++;
    for (std::vector<E*>::iterator it_vE=this->vE.begin();it_vE!=this->vE.end();++it_vE)
        {
            (*it_vE)->E_display_all(n_space);
        }
    return;
};

void E::print_formatted_E(int n_space, std::string start_opening_tag,
            std::string end_opening_tag, std::string start_closing_tag,
            std::string end_closing_tag)
{
    
    std::string N_space(n_space*4,' ');
    std::cout << N_space ;
    std::cout<< start_opening_tag;
    std::cout << this->name;
    std::cout<< end_opening_tag;
    std::cout << endl;
    std::cout << N_space;
    std::cout << this->data;
    std::cout << endl;

    if (this->vE.size()!=0) n_space++;
    for (std::vector<E*>::iterator it=this->vE.begin();it!=this->vE.end();++it)
        {
            (*it)->print_formatted_E(n_space, start_opening_tag,
            end_opening_tag, start_closing_tag, end_closing_tag);
            std::cout << endl;
        }
    std::cout << N_space ;
    std::cout<< start_closing_tag;
    std::cout << this->name;
    std::cout<< end_closing_tag;
    return;
};
/*
void E::E_write_to_file(std::string start_opening_tag,
            std::string end_opening_tag, std::string start_closing_tag,
            std::string end_closing_tag, ostream &file)
{
    file << start_opening_tag;
    file << this->name;
    file << end_opening_tag;
    file << this->data;
    for (std::vector<E*>::iterator it=this->vE.begin();it!=this->vE.end();++it)
        {
            (*it)->E_write_to_file(start_opening_tag,
            end_opening_tag, start_closing_tag, end_closing_tag, file);
        }
    file << start_closing_tag;
    file << this->name;
    file << end_closing_tag;
    return;
};
*/
void E::E_write_to_file(int &n_space, std::string start_opening_tag,
            std::string end_opening_tag, std::string start_closing_tag,
            std::string end_closing_tag, ostream &file)
{
    
    std::string N_space(n_space*4,' ');
    file << N_space;
    file << start_opening_tag;
    file << this->name;
    file << end_opening_tag;
    file << std::endl;
    file << N_space;
    file << this->data;
    file << std::endl;
    
    if (this->vE.size()!=0) n_space++;
    for (std::vector<E*>::iterator it=this->vE.begin();it!=this->vE.end();++it)
        {
            (*it)->E_write_to_file(n_space, start_opening_tag,
            end_opening_tag, start_closing_tag, end_closing_tag, file);
            if (it == this->vE.end()-1) n_space--;
            
        }
    
    file << N_space;
    file << start_closing_tag;
    file << this->name;
    file << end_closing_tag;
    file << std::endl;
    return;
};
/*
void E::E_write_to_E()
{
    
    std::string N_space(n_space*4,' ');
    file << N_space;
    file << start_opening_tag;
    file << this->name;
    file << end_opening_tag;
    file << std::endl;
    file << N_space;
    file << this->data;
    file << std::endl;
    
    if (this->vE.size()!=0) n_space++;
    for (std::vector<E*>::iterator it=this->vE.begin();it!=this->vE.end();++it)
        {
            (*it)->E_write_to_file(n_space, start_opening_tag,
            end_opening_tag, start_closing_tag, end_closing_tag, file);
            if (it == this->vE.end()-1) n_space--;
            
        }
    
    file << N_space;
    file << start_closing_tag;
    file << this->name;
    file << end_closing_tag;
    file << std::endl;
    return;
};
*/

void E::print_flat_E()
{
    std::cout << this->name << " : " << this->data << std::endl;
    if (this->vE.size()!=0)
    {
        for (std::vector<E*>::iterator it=this->vE.begin();it!=this->vE.end();++it)
        {
            (*it)->print_flat_E();
        }
    }
    return;
};

void E::E_save_to_file(){

    char *FileName = new char[20];
    char *FileNameOut = new char[75];
    char *ExtensionXML = new char[4];
    char *Path = new char[50];
    int nb_space = 0;
    FileNameOut[0]=0;                   // to clear the array of Char
    strcpy(ExtensionXML,".xml");
    strcpy(Path,"../datafiles/");
    cout << "\nEnter the name of the file you want to create: " << std::endl;
    cin >> FileName;
    strcat(FileNameOut, Path);
    strcat(FileNameOut,FileName);
    strcat(FileNameOut, ExtensionXML);
    std::ofstream file(FileNameOut);
    //~ this->E_write_to_file("<",">","</",">", file);
    
    //~ this->E_write_to_file(0,"<",">","</",">", file);
    this->E_write_to_file(nb_space,"<",">","</",">", file);
}



void E::fetch_search_result(std::vector<E**> &search_result)
{
    if (search_result.size()!=0)
    {
        for (std::vector<E**>::iterator it=search_result.begin();it!=search_result.end();++it)
        {
            //~ std::cout << (*(*it))->data << std::endl;
            (*(*it))->print_formatted_E(0,"<",">","</",">");
            std::cout << endl;
        }
    }
    else std::cout << "Not found" <<std::endl;
}

void E::search_For(int & index, int & level, string searchName)
{
    if (this->name==searchName)
    {
        std::cout << "Found " << searchName << " at (level,index): " << "(" << level  << "," << index << ")" << std::endl;
        std::cout << std::endl;
        E** ptr_to_ptr_to_E = new E*(this);
        E::search_result.push_back(ptr_to_ptr_to_E);
    }
    else
    {
        level++;
        index=0;
        (this->simple_loop_VE(searchName));
    }
    if (E::search_result.size()==0)
    {
        std::cout << "No results" << std::endl; //not ideal...
    }
    std::cout << "Found " << E::search_result.size() << " Entities with name " << searchName << std::endl;
}

void E::simple_loop_VE(string searchName)
{
    for(std::vector<E*>::const_iterator it=this->vE.begin();it!=this->vE.end();++it)
    {
        if ((*it)->name==searchName)
        {
            E** ptr_to_ptr_to_E = new E*(*it);
            E::search_result.push_back(ptr_to_ptr_to_E);
        }
    }
    this->simple_loop_VE_next_level(searchName);
}

void E::simple_loop_VE_next_level(string searchName)
{
    for(std::vector<E*>::const_iterator it=this->vE.begin();it!=this->vE.end();++it)
    {
        (*it)->simple_loop_VE(searchName);
    }
}

void E::str_token_tag(std::string & buffer_Input,
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

void E::trim_leading_space(std::string & io_string)
{
    size_t pos = 0;
    pos = io_string.find_first_not_of(' ');
    if (pos)
    {
        io_string = io_string.substr(pos);
    }
}

void E::load_XML_File_to_E(const std::string & fullFileName)
{
    E xmlFile;
    std::stack<E**> stack_E_pt;
    
    xmlFile.extract_File_to_Flat_E(fullFileName);
    std::vector<E*>::iterator it_start = xmlFile.vE.begin();
    this->process_flat_E_to_E(it_start, xmlFile, stack_E_pt);
    xmlFile.destructor_E();
}



void E::extract_File_to_Flat_E(const std::string & fullFileName)
{
    std::string bufferString;
    std::string bufferLine;
    std::string buffer_Tag;
    std::string element;
    std::string buffer_End_of_Line;
    
    // Processing file
    ifstream fileEntity;
    fileEntity.open(fullFileName.c_str(), ios::in);

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
            
            //now store into vE
            this->new_vE_element();
            this->vE.back()->name = element;
            this->vE.back()->data = buffer_Tag;
            
            
        }
    
    }
}

void E::check_token(const std::string & token, std::string & element)
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

void E::input_E()
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
            this->new_vE_element();
            this->set_vEe_name(index,key_input);
            std::cout << "enter data" << std::endl;
            cin.ignore(); 
            std:: getline(std:: cin, key_input);                
            if (key_input!="quit")
            {
                this->set_vEe_data(index,key_input);
                index++;
            }
        }
        else if (key_input=="end")
        {
            for (std::vector<E*>::iterator it=this->vE.begin();it!=this->vE.end();++it)
            {
                (*it)->input_E();                           //Where recursivity happen...
            }
        }
    }
    return;
    
}

void E::proc_it(E & E_proc, std::stack<E**> & stack_E_pt)
{
    std::vector<E*>::iterator it_start = E_proc.vE.begin();
    this->process_flat_E_to_E(it_start, E_proc, stack_E_pt);
}

E * E::process_flat_E_to_E(std::vector<E*>::iterator & it, E & E_source, std::stack<E**> & stack_E_pt)
{

    while (it!=E_source.vE.end())
    {
        if ((*it)->name=="opening_tag")
        {
            this->name=(*it)->data;
            ++it;
            E** ptr_to_ptr_to_E = new E*(this);
            stack_E_pt.push(ptr_to_ptr_to_E);

            if ((it!=E_source.vE.end()) && ((*it)->name=="data"))
            {
                this->data=(*it)->data;
                ++it;
                
                if ((it!=E_source.vE.end()) && ((*it)->name=="closing_tag"))
                {
                    return this->process_flat_E_to_E(it, E_source, stack_E_pt);
                }
                else
                {
                    this->new_vE_element();
                    return this->vE.back()->process_flat_E_to_E(it, E_source, stack_E_pt);
                }
                
            }
            else if ((it!=E_source.vE.end()) && ((*it)->name=="closing_tag"))
            {
                return this->process_flat_E_to_E(it, E_source, stack_E_pt);
            }
            else                // else  if next is 'data'
            {
                this->data="";
                this->new_vE_element();
                return this->vE.back()->process_flat_E_to_E(it, E_source, stack_E_pt);
            }
        }
        else if(((*it)->name=="closing_tag") && it!=E_source.vE.begin());
        {
            ++it;
            E** ptr_to_ptr_to_E = stack_E_pt.top();
            delete ptr_to_ptr_to_E;
            stack_E_pt.pop();
            // tricky: if next element is opening tag we create
            // a new vE element in the element at teh top of the stack 
            // (which should be the parent element of this current one)
            if ((it!=E_source.vE.end()) && ((*it)->name=="opening_tag"))
            {
                (*(stack_E_pt.top()))->new_vE_element();
                return (*(stack_E_pt.top()))->vE.back()->process_flat_E_to_E(it, E_source, stack_E_pt);
            }
            else
            {
                if (!stack_E_pt.empty()) return (*(stack_E_pt.top()))->process_flat_E_to_E(it, E_source, stack_E_pt);
                else return this;
            }
        }
    }
    return this;
}

        
void E::try_copy(int index)
{
    this->vE.back()->vE.push_back(this->vE.at(index));
}

void E::testing()
{
    this->new_vE_element();
    this->new_vE_element();
    this->vE.at(0)->new_vE_element();
    this->vE.at(0)->new_vE_element();
    this->vE.at(1)->new_vE_element();
    this->vE.at(1)->new_vE_element();
    this->E_set_name("master");
    this->E_set_data("123");
    this->set_vEe_name(0,"tata");
    this->set_vEe_data(0,"1");
    this->set_vEe_name(1,"titi");
    this->set_vEe_data(1,"2");
    //~ this->vE.at(0)->set_vEe_name(0,"lala");
    this->vE.at(0)->set_vEe_name(0,"toto");
    this->vE.at(0)->set_vEe_data(0,"11");
    this->vE.at(0)->set_vEe_name(1,"lili");
    this->vE.at(0)->set_vEe_data(1,"12");
    this->vE.at(1)->set_vEe_name(0,"toto");
    this->vE.at(1)->set_vEe_data(0,"21");
    this->vE.at(1)->set_vEe_name(1,"lolo");
    this->vE.at(1)->set_vEe_data(1,"22");
    
    this->vE.at(0)->vE.at(0)->new_vE_element();
    this->vE.at(0)->vE.at(0)->set_vEe_name(0,"truc");
    this->vE.at(0)->vE.at(0)->set_vEe_data(0,"999");
}

void E::display_vector_int(std::vector<int>& vect_index) {
    
    std::cout << "(" ;
    for(std::vector<int>::const_iterator it=vect_index.begin();it!=vect_index.end();++it)
    {
        std::cout << *it;
        if (it!=vect_index.end()-1) std::cout << ", " ;
    }
    std::cout << ")" ;
}


E * E::vE_get_by_index(std::vector<int>& vect_index, std::vector<int>::const_iterator it)
{
    int store_it=0;
    if (vect_index.size()==0)
    {
        return this;
    }
    else if (vect_index.size()==1)
    {
        return (this->vE).at(vect_index.front());
    }
    else if ((vect_index.size()>1) && (it!=vect_index.end()-1))
    {
        store_it=*it;
        return ((this->vE).at(store_it))->vE_get_by_index(vect_index,++it);
    }
    else
    {
        return this->vE.at(vect_index.back());
    }
}

void E::vE_copy_To_Vector_Float(std::vector<float>& vFloat)
{
    float num;
    for (std::vector<E*>::iterator it = this->vE.begin(); it != this->vE.end(); ++it)
    {
        
        if(!(istringstream((*it)->data)>>num)) num=0;
        vFloat.push_back(num);
    }
}

//  Index management

std::vector<int> E::set_vector_of_indexes(std::string list_indexes)
{
    std::vector<int> vect_index;
    std::string delimiter = ",";
    
    size_t pos = 0;
    std::string token;
    int num;
    while ((pos = list_indexes.find(delimiter)) != std::string::npos) {
        token = list_indexes.substr(0, pos);
        std::cout << token;
        //~ int num = std::stoi(token);         // convert string to int
        if(!(istringstream(token)>>num)) num=0;
        vect_index.push_back(num);
        list_indexes.erase(0, pos + delimiter.length());
    }
    std::cout << list_indexes << std::endl;
    if(!(istringstream(list_indexes)>>num)) num=0;
    vect_index.push_back(num);    
    return vect_index;
}

void E::extractEColorDataToGL(
                            std::vector<float> &colors,
                            std::vector< vector <float> > &color_faces)
{
    std::vector<int> index;
    for (int f=0; f < 6 ; ++f)
    {
        index = {f,0};
        (this->vE_get_by_index(index, index.begin()))->vE_copy_To_Vector_Float(colors);
        color_faces.push_back(colors);
        colors.clear();
    }
    testVector_Display_2d(color_faces);
}


void E::SearchResultsToVectorFloat(std::vector<float> &v_float, std::vector< vector <float> > &v_v_float)
{
    for (std::vector<E**>::iterator it=E::search_result.begin(); it != E::search_result.end(); ++it)
    {
        (*(*it))->vE_copy_To_Vector_Float(v_float);
        v_v_float.push_back(v_float);
        v_float.clear();
    }
    
}
void E::extractEVertexToGL(
            std::vector<float> &coordinate,
            std::vector< vector <float> > &vertex,
            std::vector< std::vector< std::vector <float> > > &vCube)
{
    std::vector<std::vector<float> > vTriangle_face;
    std::vector<int> index;
    for (int f=0; f < 6 ; ++f)
    {
        for (int v=1; v < 5; ++v)
        {
            index = {f,v};
            (this->vE_get_by_index(index, index.begin()))->vE_copy_To_Vector_Float(coordinate);
            vertex.push_back(coordinate);
            coordinate.clear();
        }
        quads_to_triangles(vertex, vTriangle_face);
        vCube.push_back(vTriangle_face);
        vTriangle_face.clear();
        vertex.clear();
    }
    testVector_Display(vCube);
}

std::vector<E**> E::search_result;

#endif /*E.cpp*/
