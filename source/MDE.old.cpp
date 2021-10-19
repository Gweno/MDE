//-----------MDE.cpp-------------
#ifndef MDE_CPP
#define MDE_CPP

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string.h>
#include <string>
#include <stdexcept>      // std::out_of_range
#include <iterator>       // iteror istream
#include <memory>

#include "MDE.h"

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

//~ void MDE::new_vMDE_element()
//~ {
    //~ this->vMDE.push_back(new MDE);
//~ };
void MDE::new_vMDE_element(std::string name,std::string data)
{
    this->vMDE.push_back(new MDE);
    if (name !="")
    {
        this->set_name_vMDE_back(name);
    }
    if (data !="")
    {
        this->set_data_vMDE_back(data);
    }
};
//~ void MDE::new_vMDE_element(std::string &name,std::string &data)
//~ void MDE::new_vMDE_element(MDE *mde)
//~ {
    //~ this->vMDE.push_back(new MDE);
    //~ if (name !="")
    //~ {
        //~ this->set_name_vMDE_back(mde->name);
    //~ }
    //~ if (data !="")
    //~ {
        //~ this->set_data_vMDE_back(mde->data);
    //~ }
//~ };
void MDE::new_vMDE_element(MDE *mde)
{
    if (mde != NULL){
        this->vMDE.push_back(new MDE);
        if (name !="")
        {
            this->set_name_vMDE_back(mde->name);
        }
        if (data !="")
        {
            this->set_data_vMDE_back(mde->data);
        }
    }
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

void MDE::clear_all_vMDE_by_index(std::vector<int>& vect_index)
{
    std::vector<int>::const_iterator it=vect_index.begin();
    (this->get_MDE_by_index(vect_index,it))->clear_all_vMDE();
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

//~ GLdata current_GLdata;

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

void MDE::char_array_to_vMDE_data(int n_element, char **array_char, bool create )
{
    for (int i = 0; i < n_element; ++i)
    {
        std::cout << i << ", " << array_char[i] << std::endl;
        if (create){
            this->new_vMDE_element(std::to_string(i),std::string(array_char[i]));
        }
        else
        {
            try
            {
                this->set_data_vMDE_index(i,std::string(array_char[i]));
            }
            catch (const std::out_of_range& oor)
            {
                std::cerr << "Out of Range error: " << oor.what() << '\n';
            }
            
        }
    
    } 
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

// test loop vMDE to get vector vMDE
void MDE::get_vMDE_test2(int index, std::vector<MDE**> & pt_vMDE){
    
    try
            {
                MDE** ptr_to_ptr_to_MDE = new MDE*(this->vMDE.at(index));
                pt_vMDE.push_back(ptr_to_ptr_to_MDE);
            }
            catch (const std::out_of_range& oor)
            {
                std::cerr << "Out of Range error: " << oor.what() << '\n';
            }
}


void MDE::loop_test2(std::vector<int> & vect_index, int & index, std::vector<MDE**> & pt_vMDE){
    
    //~ printf("0. %i\n",index);
    //~ printf("0.1 %i\n",vect_index[index]);
    //~ std::cout << "0.2 " <<  this->data << std::endl;
    if (index<vect_index.size()-1)
    {
        //~ printf("1. %i\n",index);
        //~ printf("1.1 %i\n",vect_index[index]);
        //~ std::cout << "1.2 " <<  this->data << std::endl;
        //~ std::cout << "1.3 " <<  (this->vMDE)[(vect_index[index])]->data << std::endl;
        
        try
        {
            //~ ((this->vMDE)[(vect_index[index])])->loop_test2(vect_index,++index,pt_vMDE);
            //~ std::cout << "1.3.1 " <<  (this->vMDE)[(vect_index[index])]->data << std::endl;
            MDE * ptr_to_MDE = ((this->vMDE)[(vect_index[index])]);
            //~ return ((this->vMDE)[(vect_index[index])])->loop_test2(vect_index,++index,pt_vMDE);
            //~ (ptr_to_ptr_to_MDE)->loop_test2(vect_index,++index,pt_vMDE);
            (ptr_to_MDE)->loop_test2(vect_index,++index,pt_vMDE);
            //~ return ((this->vMDE)[(vect_index[index])]);
        }
        catch(const std::out_of_range& oor)
        {
            std::cerr << "Out of Range error: " << oor.what() << '\n';
        }
    }
    
    else
    {
        //~ printf("2. %i\n",index);
        //~ printf("2.1 %i\n",vect_index[index]);
        std::cout << "2.2 " <<  this->data << std::endl;
        try
        {
            MDE** ptr_to_ptr_to_MDE = new MDE*((this->vMDE)[(vect_index[index])]);
            pt_vMDE.push_back(ptr_to_ptr_to_MDE);

        }
        catch (const std::out_of_range& oor)
        {
            std::cerr << "Out of Range error: " << oor.what() << '\n';
        }
    }
    
}

//~ std::vector<MDE*> MDE::get_vMDE(std::vector<int> & vect_index, int & index){
std::vector<MDE*> MDE::get_vMDE(std::vector<int> & vect_index, int & index){
    
    if (index<vect_index.size()-1)
    {
        try
        {
            MDE * in_loop_MDE = (this->vMDE)[(vect_index[index])];
            return (in_loop_MDE)->get_vMDE(vect_index,++index);
        }
        catch(const std::out_of_range& oor)
        {
            std::cerr << "Out of Range error: " << oor.what() << '\n';
        }
    }
    
    else
    {
        try
        {
            MDE * in_loop_MDE = (this->vMDE)[(vect_index[index])];
            return in_loop_MDE->vMDE;
        }
        catch (const std::out_of_range& oor)
        {
            std::cerr << "Out of Range error: " << oor.what() << '\n';
        }
    }
    
}

// simplified version of get_MDE_by_index
MDE * MDE::get_MDE(std::vector<int> vect_index)
{
    printf("in get_MDE");
    std::vector<int>::const_iterator it;
    this->get_MDE_by_index(vect_index, it);
}

MDE * MDE::get_MDE_by_index(std::vector<int>& vect_index, std::vector<int>::const_iterator it)
{
    int store_it=0;
    //~ printf("0. %i", *it);
    if (vect_index.size()==0)
    {
        //~ printf("1. %i", *it);
        return this;
    }
    else if (vect_index.size()==1)
    {
        //~ printf("2. %i", *it);
        return (this->vMDE).at(vect_index.front());
    }
    else if ((vect_index.size()>1) && (it!=vect_index.end()-1))
    {
        //~ printf("3. %i", *it);
        store_it=*it;
        return ((this->vMDE).at(store_it))->get_MDE_by_index(vect_index,++it);
    }
    else
    {
        //~ printf("4. %i", *it);
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
