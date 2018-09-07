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
void E::vE_clear_all()
{
    for (std::vector<E*>::iterator it_all=this->vE.begin();it_all!=this->vE.end();++it_all)
    {
        (*it_all)->vE_clear_all();
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
    std::cout << "before" << std::endl;
    this->vE.at(index_vEe)->name = vName;
    std::cout << "after" << std::endl;
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
    std::cout << "    ";
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
    FileNameOut[0]=0;                   // to clear the array of Char
    strcpy(ExtensionXML,".xml");
    strcpy(Path,"../datafiles/");
    cout << "\nEnter the name of the file you want to create: " << std::endl;
    cin >> FileName;
    strcat(FileNameOut, Path);
    strcat(FileNameOut,FileName);
    strcat(FileNameOut, ExtensionXML);
    std::ofstream file(FileNameOut);
    this->E_write_to_file("<",">","</",">", file);
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

void E::load_File()

{
    char *FileName = new char[32];
    char *FileNameIn = new char[128];
    char *PathToFile = new char[128];
    char *FullNameFileIn = new char[256];
    char *ExtensionXML = new char[8];
    char *bufferChar = new char[32];
    string bufferString;

    strcpy(FileName, "perma");
    strcpy(PathToFile, "../datafiles/");
    strcpy(ExtensionXML,".xml");
    strcpy(FullNameFileIn,PathToFile);
    strcpy(FileNameIn, FileName);
    strcat(FileNameIn, ExtensionXML);
    strcat(FullNameFileIn, FileNameIn);
    std::cout << " FullNameFileIn " << FullNameFileIn << std::endl;
    
    ifstream fileEntity;

    fileEntity.open(FullNameFileIn, ios::in);

    fileEntity.clear();                   // absolutly needed otherwise the file is flagged at eof, and good=0   
    fileEntity.seekg(0, ios::beg);        // set cursor at 0 from start of file

    while ((fileEntity.good()) && (!fileEntity.eof()))     // loop while extraction from file is possible
    {
        std::getline(fileEntity, bufferString);
        char* bufferLineChar = new char[bufferString.length()+1];
        strcpy(bufferLineChar,bufferString.c_str());
        bufferChar = strtok (bufferLineChar,"</>");
        
            while (bufferChar != NULL)
            {
                printf ("%s\n",bufferChar);
                bufferChar = strtok (NULL, "</>");
            }
        delete bufferLineChar;
          }
    fileEntity.close();           // close file
    delete FileName;
    delete FileNameIn;
    delete PathToFile;
    delete FullNameFileIn;
    delete ExtensionXML;
    delete bufferChar;
    }
    

void E::load_File3()

{
    char *FileName = new char[32];
    char *FileNameIn = new char[128];
    char *PathToFile = new char[128];
    char *FullNameFileIn = new char[256];
    char *ExtensionXML = new char[8];
    char *bufferChar = new char[32];
    
    char *pch;
    char *pch2;
    char *pch3;
    
    long int * pn;
    
    string bufferString;

    strcpy(FileName, "perma");
    strcpy(PathToFile, "../datafiles/");
    strcpy(ExtensionXML,".xml");
    strcpy(FullNameFileIn,PathToFile);
    strcpy(FileNameIn, FileName);
    strcat(FileNameIn, ExtensionXML);
    strcat(FullNameFileIn, FileNameIn);
    
    ifstream fileEntity;

    fileEntity.open(FullNameFileIn, ios::in);

    fileEntity.clear();                   // absolutly needed otherwise the file is flagged at eof, and good=0   
    fileEntity.seekg(0, ios::beg);        // set cursor at 0 from start of file

    this->E_set_name("File");
    this->E_set_data(FullNameFileIn);

    pn = 0;

    while ((fileEntity.good()) && (!fileEntity.eof()))     // loop while extraction from file is possible
    {
        std::cout << "Start!" << std::endl;
        std::getline(fileEntity, bufferString);
        char* bufferLineChar = new char[bufferString.length()+1];
        strcpy(bufferLineChar,bufferString.c_str());
        char *bufferChar2 = new char[32];
        char *bufferChar3 = new char[32];
        
        while (strlen(bufferLineChar)>0)
        {
            pch = strchr(bufferLineChar,'<');
            pch2 = strchr(bufferLineChar,'>');
            memcpy(bufferChar,pch+1,pch2-pch - 1);
            bufferChar[pch2-pch - 1]='\0';
            memcpy(bufferChar2,pch2+1,bufferString.length()-(pch2-pch));
            bufferChar2[bufferString.length()-(pch2-pch)+1]='\0';

            if (bufferChar[0]=='/') 
            {
                printf("(closing tag)%s\n",bufferChar);
            }
            else
            { 
            printf("(opening tag)%s\n",bufferChar);
            }
            
            if (bufferChar2[0]!='<') 
                {
                if (strlen(bufferLineChar)>0 && strlen(bufferChar2)>0) strcpy(bufferLineChar,bufferChar2);
                else break;
                pch3 = strchr(bufferLineChar,'<');
                memcpy(bufferChar3,bufferLineChar, pch3-bufferLineChar);
                bufferChar3[pch3-bufferLineChar]='\0';
                std::cout << "(data)" << bufferChar3 << std::endl;
                
                memcpy(bufferChar2,pch3,bufferString.length()-(pch3-pch));
                bufferChar2[bufferString.length()-(pch3-pch)+1]='\0';
                }
                
            if (strlen(bufferLineChar)>0 && strlen(bufferChar2)>0) strcpy(bufferLineChar,bufferChar2);
            else break;
           }
          }
    fileEntity.close();           // close file
    delete FileName;
    delete FileNameIn;
    delete PathToFile;
    delete FullNameFileIn;
    delete ExtensionXML;
    delete bufferChar;
    }
    

void E::load_File5()
{
    char *FileName = new char[32];
    char *FileNameIn = new char[128];
    char *PathToFile = new char[128];
    char *FullNameFileIn = new char[256];
    char *ExtensionXML = new char[8];
    char *bufferChar = new char[32];
    char *bufferChar2 = new char[300];
    char *bufferChar3 = new char[32];
    
    int n=0;
    int m=0;
    
    string bufferString;
    string bufferString2;

    strcpy(FileName, "perma");
    strcpy(PathToFile, "../datafiles/");
    strcpy(ExtensionXML,".xml");
    strcpy(FullNameFileIn,PathToFile);
    strcpy(FileNameIn, FileName);
    strcat(FileNameIn, ExtensionXML);
    strcat(FullNameFileIn, FileNameIn);
    
    ifstream fileEntity;

    fileEntity.open(FullNameFileIn, ios::in);

    fileEntity.clear();                   // absolutly needed otherwise the file is flagged at eof, and good=0   
    fileEntity.seekg(0, ios::beg);        // set cursor at 0 from start of file

    std::vector<int> vect_index;
    
    while ((fileEntity.good()) && (!fileEntity.eof()))     // loop while extraction from file is possible
    {
        std::getline(fileEntity, bufferString);
        char* bufferLineChar = new char[bufferString.length()+1];
        strcpy(bufferLineChar,bufferString.c_str());
        
        while (strlen(bufferLineChar)>0)
        {
            this->str_token_tag(bufferLineChar,bufferChar,bufferChar2,'<','>');

            // closing tag
            if (bufferChar[0]=='/') 
            {
                if (vect_index.size()>1)
                {
                    vect_index.pop_back();
                    n=vect_index.back()+1;
                    vect_index.back()=n;
                }
            }
            // opening tag
            else
            { 
                n=0;
                if (vect_index.size()!=0)
                {
                    m=vect_index.back();
                    vect_index.pop_back();
                    (this->vE_get_by_index(vect_index,vect_index.begin()))->new_vE_element();
                    vect_index.push_back(m);
                }
                // store name
                this->vE_get_by_index(vect_index,vect_index.begin())->name=bufferChar;
                if (bufferChar2[0]!='<') 
                {
                    if (strlen(bufferLineChar)>0 && strlen(bufferChar2)>0) strcpy(bufferLineChar,bufferChar2);
                    else break;
                    this->str_token_tag(bufferLineChar,bufferChar3,bufferChar2,0,'<');
                    this->vE_get_by_index(vect_index,vect_index.begin())->data=bufferChar3;
                }
                vect_index.push_back(0);
            }
            if (strlen(bufferLineChar)>0 && strlen(bufferChar2)>0)
            {
                strcpy(bufferLineChar,bufferChar2);
            }
            else break;
        }
            // delete[] the line buffer
            delete[] bufferLineChar;
    }
    fileEntity.close();           // close file

    delete[] FileName;
    delete[] FileNameIn;
    delete[] PathToFile;
    delete[] FullNameFileIn;
    delete[] ExtensionXML;
    delete[] bufferChar;
    delete[] bufferChar2;
    delete[] bufferChar3;
}

void E::str_token_tag(char* buffer_Input, char* output_Tag, char* buffer_Output, int delimiter1, int delimiter2)
{
    char *pch=NULL;
    char *pch2=NULL;
    if (delimiter1!=0)
    {
        pch = strchr(buffer_Input,delimiter1);
        pch2 = strchr(buffer_Input,delimiter2);
        memcpy(output_Tag,pch+1,pch2-pch - 1);
        output_Tag[pch2-pch - 1]='\0';
        memcpy(buffer_Output,pch2+1,strlen(buffer_Input)-strlen(output_Tag)); // Including nullcharacter at the end
    } 
    else
    {
        pch = buffer_Input;
        pch2 = strchr(buffer_Input,delimiter2);
        memcpy(output_Tag,pch,pch2-pch);
        output_Tag[pch2-pch]='\0';
        memcpy(buffer_Output,pch2,strlen(buffer_Input)-strlen(output_Tag)+1); // Including nullcharacter at the end
    }

}





void E::load_XML_File_to_E(std::string fullFileName)
{
    // Declaring Array of char to store the tags
    char *buffer_Opening_Tag = new char[32];
    char *buffer_Closing_Tag = new char[32];

    // Declaring integers for holding indexes temporarily
    int n=0;
    int m=0;
    
    // the string buffers to hold the tokens
    std::string bufferString;

    // the vector of indexes
    std::vector<int> vect_index;
    
    // Declaring an array of char for the path+name of the file
    char* FullNameFileIn = new char[fullFileName.length()+1];
    strcpy(FullNameFileIn,fullFileName.c_str());
    
    // Processing file
    ifstream fileEntity;
    fileEntity.open(FullNameFileIn, ios::in);

    fileEntity.clear();                   // absolutly needed otherwise the file is flagged at eof, and good=0   
    fileEntity.seekg(0, ios::beg);        // set cursor at 0 from start of file

    
    while ((fileEntity.good()) && (!fileEntity.eof()))     // loop while extraction from file is possible
    {
        // get a line form the file
        std::getline(fileEntity, bufferString);
        
        // this is needed to transfer the string content in the char*
        char* bufferLineChar = new char[bufferString.length()+1];
        strcpy(bufferLineChar,bufferString.c_str());
        char *buffer_End_of_Line = new char[bufferString.length()+1];
        
        // loop on the line in the buffer
        while (strlen(bufferLineChar)>0)
        {
            // from the line in the buffer, get a token between char < and > 
            // copy what's after the data token into the buffer for processing the rest of the line
            this->str_token_tag(bufferLineChar,buffer_Opening_Tag,buffer_End_of_Line,'<','>');
            
            // token is a closing tag
            if (buffer_Opening_Tag[0]=='/') 
            {
                // take off the last index and increment the value of the last element (which was second from last)
                if (vect_index.size()>1)
                {
                    vect_index.pop_back();
                    n=vect_index.back()+1;
                    vect_index.back()=n;
                }
            }
            // token is an opening tag
            else
            { 
                n=0;
                
                // if the vector of indexes is not empty create a new vE element at the wanted index.
                // the process used is to get the instance E that holds vE by reducing the vector of index
                // using pop_back(), storing first its last element (.back()) then restoring it.
                if (vect_index.size()!=0)
                {
                    m=vect_index.back();
                    vect_index.pop_back();
                    (this->vE_get_by_index(vect_index,vect_index.begin()))->new_vE_element();
                    vect_index.push_back(m);
            
                }
                // store name
                this->vE_get_by_index(vect_index,vect_index.begin())->name=buffer_Opening_Tag;
                // check if the token holds data, by testing if no '<' at the start of the token
                if (buffer_End_of_Line[0]!='<') 
                {
                    // copy the array of char after the token and delimiters to continue
                    // the process and extract data
                    if (strlen(bufferLineChar)>0 && strlen(buffer_End_of_Line)>0) strcpy(bufferLineChar,buffer_End_of_Line);
                    else break;
                    // from the line in the buffer, get a token between the start and char < 
                    // copy what's after the data token into the buffer for processing the rest
                    
                    this->str_token_tag(bufferLineChar,buffer_Closing_Tag,buffer_End_of_Line,0,'<');
                    
                    // write data in
                    this->vE_get_by_index(vect_index,vect_index.begin())->data=buffer_Closing_Tag;
                    
                }
                vect_index.push_back(0);
            }
            if (strlen(bufferLineChar)>0 && strlen(buffer_End_of_Line)>0)
            {
                // copying the buffer holding the rest of the file for processing ina new loop
                strcpy(bufferLineChar,buffer_End_of_Line);
            }
            else break;
        }
            // delete[] the line buffer char[]*
            delete[] bufferLineChar;
            delete[] buffer_End_of_Line;
    }
    fileEntity.close();           // close file
    
    // delete[] all the other char*[]
    delete[] FullNameFileIn;
    delete[] buffer_Opening_Tag;
    delete[] buffer_Closing_Tag;
}


void E::load_File_token()

{
    char *FileName = new char[32];
    char *FileNameIn = new char[128];
    char *PathToFile = new char[128];
    char *FullNameFileIn = new char[256];
    char *ExtensionXML = new char[8];
    char *bufferChar = new char[32];
    string bufferString;
    string bufferTag;

    strcpy(FileName, "perma");
    strcpy(PathToFile, "../datafiles/");
    strcpy(ExtensionXML,".xml");
    strcpy(FullNameFileIn,PathToFile);
    strcpy(FileNameIn, FileName);
    strcat(FileNameIn, ExtensionXML);
    strcat(FullNameFileIn, FileNameIn);
    std::cout << " FullNameFileIn " << FullNameFileIn << std::endl;
    
    ifstream fileEntity;

    fileEntity.open(FullNameFileIn, ios::in);

    fileEntity.clear();                   // absolutly needed otherwise the file is flagged at eof, and good=0   
    fileEntity.seekg(0, ios::beg);        // set cursor at 0 from start of file

    while ((fileEntity.good()) && (!fileEntity.eof()))     // loop while extraction from file is possible
    {
        std::getline(fileEntity, bufferString);
        char* bufferLineChar = new char[bufferString.length()+1];
        
        char* pch1;
        char* pch2;
        
        
        strcpy(bufferLineChar,bufferString.c_str());
        pch1=strchr(bufferLineChar,'<');
        pch2=strchr(bufferLineChar,'>');
        char * tag = new char[pch2-pch1-1];
        strncpy(tag,pch1+1,pch2-pch1-1);
        
        std::cout << tag << std::endl;
        std::cout << tag[2]<< std::endl;
        
        delete [] bufferLineChar;
        delete [] tag;
        
        strcpy(bufferLineChar,pch2);
        
        std::cout << bufferLineChar << std::endl;
        
        delete bufferLineChar;
          }
    fileEntity.close();           // close file
    delete FileName;
    delete FileNameIn;
    delete PathToFile;
    delete FullNameFileIn;
    delete ExtensionXML;
    delete bufferChar;
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


E * E::test(int level, std::vector<int> index) {
    std::cout << "Check level " << level << std::endl;
    for (int i = 0; i < level; ++i) {
        std::cout << index.at(i) << std::endl;
    }
        
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
