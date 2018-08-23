//----------------DataFile.cpp--------
//01/08/2012 replace Entity by glEntit7

#ifndef DATAFILE_CPP
#define DATAFILE_CPP

#include <fstream>
#include <iostream>
#include <string.h>
#include <string>
#include <vector>
#include <cstdlib>
#include "Entity.h"
#include "glDisplay.h"
#include "DataFile.h"
#include<dirent.h>

#include <errno.h>


using namespace std;

// init static variable(s) for this class
bool dataFile::load_File = false;

std::vector<std::string> results;// holds search results
int input;

char storeName[10][24];
unsigned int* indexMembers = new unsigned int[6];

char storeText[10][10];

//DIR* dir_point;
//dirent* entry;

std::vector<string> vector_swap_dataTextEntity;

void dataFile::saveFile(Entity * objEntity){

    char *FileName = new char[20];
    char *FileNameOut = new char[24];    
    char *ExtensionCSV = new char[4];
    strcpy(ExtensionCSV,".csv");
    cout << "\nEnter the name of the file you want to create: ";
    cin >> FileName;
    FileNameOut = strcat(FileName, ExtensionCSV);
    ofstream fileEntity(FileNameOut, ios::out);    
    fileEntity << "Entity number,Xpos,Ypos,Zpos,Xmov,Ymov,Zmov,Xacc,Yacc,Zacc\n";
}

void dataFile::save_data_to_file(E * objE)
{
    objE->E_save_to_file();
};

void dataFile::saveFileEntity(Entity * objEntity){

    char *FileName = new char[20];
    char *FileNameOut = new char[75];
    char *ExtensionCSV = new char[4];
    char *Path = new char[50];
    FileNameOut[0]=0;                   // to clear the array of Char
    strcpy(ExtensionCSV,".csv");
    strcpy(Path,"./datafiles");
    cout << "\nEnter the name of the file you want to create: " << std::endl;
    cin >> FileName;

    strcat(FileNameOut, Path);
    strcat(FileNameOut,FileName);
    strcat(FileNameOut, ExtensionCSV);
    ofstream fileEntity(FileNameOut, ios::out);

    std:: cout << FileNameOut << std::endl;
    fileEntity << "Entity number,typeEntity,textEntity\n";
        for (unsigned int i=0; i<objEntity->valueEntity.size();i++){

        	            fileEntity << i << "," << objEntity->valueEntity.at(i) << "," << objEntity->textEntity.at(i) <<"\n";
        }
}


void dataFile::firstLine(char* FileNameIn){			 // to be improved with pointers
    char *bufferString1 = new char[20];
    char *bufferChar;
    ifstream fileEntity;
    fileEntity.open(FileNameIn, ios::in);			//file should be open already
    int j=0;
    fileEntity.clear();                   // absolutly needed otherwise the file is flagged at eof, and good=0
    fileEntity.seekg(0, ios::beg);        // set cursor at 0 from start of file

//  definition of 1D array and declaration des tableaux 1D elements de notre 1er tableau

    j=0;
    fileEntity.getline(bufferString1, 100, '\n');           // store character strings between each ',' (comma) in bufferString (size max=100)
    bufferChar= strtok (bufferString1,",");
    while (bufferChar!=NULL){
        strcpy(storeName[j],bufferChar);
        bufferChar= strtok (NULL,",");
        std::cout<<"1stline"<<storeName[j]<<std::endl;
        j++;
      }
    fileEntity.close();           // close file
}

void dataFile::assignIndex(){
    for(int j=0;j<10;j++){
        std::cout << "assignIndex " << j << std::endl;
        if(strcmp(storeName[j],"typeEntity")==0) indexMembers[0]=j;
        else if(strcmp(storeName[j],"nbChildren")==0) indexMembers[1]=j;
        else if(strcmp(storeName[j],"masterEntity")==0) indexMembers[2]=j;
        else if(strcmp(storeName[j],"layerEntity")==0) indexMembers[3]=j;
        else if(strcmp(storeName[j],"valueEntity")==0) indexMembers[4]=j;
        else if(strcmp(storeName[j],"textEntity")==0) indexMembers[5]=j;
        std::cout << "storeName[" << j << "]: " << storeName[j] << std::endl;
    }
}


void dataFile::loadFile(char menuChoice, Entity *objEntity)

{
    //~ unsigned int FileNum;
    char *FileName = new char[32];
    char *FileNameIn = new char[128];
    char *bufferString = new char[32];
    char *PathToFile = new char[128];
    char *FullNameFileIn = new char[256];
    char *ExtensionCSV = new char[8];
    char *bufferChar;
    int nb_column, nb_line;

    strcpy(PathToFile, "/home/gweno/Documents/Project/Cpp/git-repos-ESV7/ESV7/datafiles/");
    strcpy(ExtensionCSV,".csv");
    strcpy(FullNameFileIn,PathToFile);

    switch (menuChoice)
    {
           case '1':
                strcpy(FileName, "default");
                std::cout << " FullNameFileIn " << FullNameFileIn << std::endl;
                nb_column=7;
                break;           
           case 't':
                strcpy(FileName,"test01");
                nb_column=7;
                break;
           case '2':
                strcpy(FileName,"joe");
                nb_column=7;
                break;
           default:
                break;
    }
    strcpy(FileNameIn, FileName);
    strcat(FileNameIn, ExtensionCSV);
    strcat(FullNameFileIn, FileNameIn);
    std::cout << " FullNameFileIn " << FullNameFileIn << std::endl;
    
    ifstream fileEntity;

    fileEntity.open(FullNameFileIn, ios::in);
                 std::cout << "control 7" << std::endl;
    int i=0, j=0;

    while (fileEntity.good())     // loop while extraction from file is possible
        {
         fileEntity.ignore(100,'\n');
         std::cout << "control loop" << std::endl;
         i++;    
         }
    fileEntity.clear();                   // absolutly needed otherwise the file is flagged at eof, and good=0   
    fileEntity.seekg(0, ios::beg);        // set cursor at 0 from start of file
    nb_line=i-2;                          // substract 2 lines 1 for the header, 1 for the eof
    dataFile::file_size=nb_line;

//  declaration of the array of DOUBLE one dimension, with pointer of pointer
    double** storeData= new double*[nb_line];
    char storeText[nb_line][10];

//  definition of 1D array and declaration des tableaux 1D elements de notre 1er tableau
    for (int i=0;i<nb_line;i++)
    {
        storeData[i]=new double[nb_column];
    }
    
    if (fileEntity.good()){                         // test if extraction from file is possible
        fileEntity.ignore(100,'\n');
    } 

    i=0;
    j=0;
    while ((fileEntity.good()) && (!fileEntity.eof()))     // loop while extraction from file is possible
          {
          fileEntity.getline(bufferString, 100, '\n');           // store character strings between each ',' (comma) in bufferString (size max=100)
          bufferChar= strtok (bufferString,",");
          while (bufferChar!=NULL){
              if (j<6){
                  storeData[i][j] = atof(bufferChar);
                  bufferChar= strtok (NULL,",");
               }
              else{
                  strcpy(storeText[i],bufferChar);
                  bufferChar= strtok (NULL,",");
              }

              j++;
          }
          i++;
          j=0;
          }

  
    fileEntity.close();           // close file
  
//  load data into entity array
    switch(menuChoice)  {
    case 't':
    case '1':
    case '2':
        for (int i=0;i<nb_line; i++){
            objEntity->typeEntity.push_back(storeData[i][1]);
            objEntity->nbChildren.push_back(storeData[i][2]);
            objEntity->masterEntity.push_back(storeData[i][3]);
            objEntity->layerEntity.push_back(storeData[i][4]);
            objEntity->valueEntity.push_back(storeData[i][5]);
            objEntity->textEntity.push_back(storeText[i]);
            }
            break;
    case '9'   :
        objEntity->nbChildren.at(input)++;
        dataFile::file_size=objEntity->masterEntity.size();
        for (int i=0;i<nb_line; i++){
            objEntity->typeEntity.push_back(storeData[i][1]);
            objEntity->nbChildren.push_back(storeData[i][2]);
            objEntity->masterEntity.push_back(storeData[i][3]+dataFile::file_size);
            objEntity->layerEntity.push_back(storeData[i][4]+objEntity->layerEntity.at(input));
            objEntity->valueEntity.push_back(storeData[i][5]);
            }
        objEntity->masterEntity.at(dataFile::file_size)=input;
        dataFile::file_size=objEntity->masterEntity.size()-1;
            break;
     default:
            break;

    }
//      Destroy the array ...

 for(int i = 0 ; i < nb_line ; i++)
 {
 delete[] storeData[i];
 }
 delete[] storeData;

// delete char pointers

delete FileName;
delete FileNameIn;
delete bufferString;
delete PathToFile;
delete FullNameFileIn;
delete ExtensionCSV;
delete bufferChar;

}

                                                          
void dataFile::enterLayer(Entity * objEntity) {
    unsigned int indexEntity, indexChildrenEnd, indexChildrenStart;
    indexEntity=0;
    indexChildrenStart=0;
    indexChildrenEnd=0;
    unsigned int input,input2;
    char *input_char = new char[10];

    while (indexEntity<objEntity->layerEntity.size() and indexEntity<=indexChildrenEnd) {
          std::cout<<"Vector's size: " << objEntity->layerEntity.size() << std::endl;
          std::cout<<"Parent is Entity #" << objEntity->masterEntity.at(indexEntity)<< std::endl;
          objEntity->typeEntity.push_back(1);
          std::cout << "Value Entity #" << indexEntity << " ?" << std::endl;
          std::cin>>input;
          objEntity->valueEntity.push_back(input);
          std::cout << " Input: " << input << std::endl;
          std::cout << "Text Entity #" << indexEntity << " ?" << std::endl;
          std::cin>>input_char;
          objEntity->textEntity.push_back(input_char);
          std::cout << " textEntity.at(indexEntity) " << objEntity->textEntity.at(indexEntity) << std::endl;
          std::cout << " textEntity.at(0) " << objEntity->textEntity.at(0) << std::endl;
          std::cout << "Nb Children of Entity #" << indexEntity << " ?" << std::endl;
          std::cin>>input2;
          objEntity->nbChildren.push_back(input2);

          if (objEntity->nbChildren.at(indexEntity)>0){
             indexChildrenStart=indexChildrenEnd+1;
             indexChildrenEnd=indexChildrenStart+objEntity->nbChildren.at(indexEntity)-1;
             for (unsigned int i=indexChildrenStart; i<=indexChildrenEnd; i++){
                 objEntity->masterEntity.push_back(indexEntity);
                 objEntity->layerEntity.push_back(objEntity->layerEntity.at(indexEntity)+1);
                 std::cout << i <<std::endl ;
                 }
             }
             std::cout<<"Layer is " << objEntity->layerEntity.at(indexEntity) << std::endl;
             std::cout<<"First Child: " << indexChildrenStart << std::endl;
             std::cout<<"Last Child: " << indexChildrenEnd << std::endl<< std::endl ;
             indexEntity++;
          }

            displayEntity(objEntity);
}




dataFile::dataFile(int file_size){                     //init the constructor with the array size in parameter
    //~ dataFile::structureEntity.push_back(0);
    //~ dataFile::layerEntity.push_back(0);
                       }


dataFile::~dataFile(){
    results.clear();
    dataFile::storeDataText.clear();
    std::cout << "size destructor dataFile storeDataText:" << storeDataText.size() << std::endl;

    }


void dataFile::listFile(){


//    DIR *dir;
//    struct dirent *ent;
//    dir = opendir (".");
//    if (dir != NULL) {

//      /* print all the files and directories within directory */
//      while ((ent = readdir (dir)) != NULL) {
//          std::string fname = ent->d_name;
//          if (fname!="." and fname!=".."){
////          std::cout <<fname.substr(fname.length()-3,3)<< std::endl;
//              if (fname.substr(fname.length()-3,3)=="csv" ){
//          std::cout <<fname<< std::endl;
//          }
//          }
//      }
//      closedir (dir);
//    } else {
//      /* could not open directory */
//      perror ("");
////      return EXIT_FAILURE;

//    }
    std::string extension = "csv";

    // setup search parameters
//    std::string curr_directory = get_current_dir_name();
//    std::string curr_directory=".";
    std::string curr_directory="/home/gweno/workspace/ESV5-Linux";

    std::cout << curr_directory << std::endl;


    dataFile::searchFile(curr_directory, extension);

    std::cout << "Control 1 List file"<< std::endl;

    // output results
    if (results.size()){
    std::cout << results.size() << " files were found:" << std::endl;
    for (unsigned int i = 0; i < results.size(); ++i) // used unsigned to appease compiler warnings
    std::cout << "- \t" << i << " : " << results[i] << std::endl;
    }
    else{
    std::cout << "No files ending in '" << extension << "' were found." << std::endl;
    }
}




// recursive search algorithm
void dataFile::searchFile(std::string curr_directory, std::string extension){
	std::cout << "Control 1 searchfile, curr_directory: "<< curr_directory << " , extension:" << extension << std::endl;
DIR* dir_point = opendir(curr_directory.c_str());
	dir_point = opendir(curr_directory.c_str());
std::cout << "Control 2 searchfile " << dir_point << std::endl;
std::cout << "Control Error " << strerror(errno) << std::endl;
dirent* entry = readdir(dir_point);
entry = readdir(dir_point);
std::cout << "Control 3 searchfile, entry: " << entry->d_name << std::endl;

while (entry){ // if !entry then end of directory
if (entry->d_type == DT_DIR){ // if entry is a directory
	std::cout << "Control 4 searchfile"<< std::endl;
	std::string fname = entry->d_name;
	std::cout << "Control 5 searchfile "<< fname << std::endl;
if (fname != "." && fname != "..")
		{std::cout << "Control 6 searchfile"<< std::endl;
	if (opendir(fname.c_str())!=NULL){
	dataFile::searchFile(entry->d_name, extension); // search through it
	std::cout << "Control 7 searchfile"<< std::endl;}
		}
}
else if (entry->d_type == DT_REG){ // if entry is a regular file
	std::cout << "Control 8 searchfile"<< std::endl;
std::string fname = entry->d_name; // filename
// if filename's last characters are extension
	std::cout << "Control 9 searchfile "<< fname<< std::endl;
if (fname.find(extension, (fname.length() - extension.length())) != std::string::npos)
	{std::cout << "Control 10 searchfile "<< fname << std::endl;
	std::string fname_fullpath;
	fname_fullpath=curr_directory;
	fname_fullpath=fname_fullpath+'/'+fname;
std::cout << "Control 101 fnamefullpath"<< fname_fullpath << std::endl;

//std::string string_name_fullpath(fname_fullpath);
//results.push_back(string_name_fullpath); // add filename to results vector
	results.push_back(fname_fullpath); // add filename to results vector
	std::cout << "Control 11 searchfile"<< std::endl;
	std::cout << "Control 111 fnamefullpath"<< fname_fullpath << std::endl;

//	delete [] fname_fullpath;
//	delete [] slash;
	}
}
std::cout << "Control 112 searchfile " << std::endl;
entry = readdir(dir_point);
std::cout << "Control 12 searchfile " << dir_point << std::endl;
}

return;
}

// displayEntityLayer is used to display Entities using tags

void dataFile::displayEntity(Entity * objEntity){

	dataFile::file_size=objEntity->masterEntity.size();
    std::cout << "Displaying Entity" << std::endl;
        for (int i=0; i<file_size;i++){
        std::cout << "&" << i;
        std::cout << ":" << objEntity->valueEntity.at(i);
        std::cout << "#" << objEntity->textEntity.at(i);
        std::cout << "<" << objEntity->masterEntity.at(i);
        std::cout << ">" << objEntity->nbChildren.at(i) << std::endl;
    }
}


void dataFile::loadDefault(Entity *objEntity){
    dataFile::loadFile('1',objEntity);
    }


#endif /* DataFile.cpp */
