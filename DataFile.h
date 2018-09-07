//----------- DataFile.h-----------

#ifndef DATAFILE_H
#define DATAFILE_H

#include <string>
#include <string.h>
//~ #include "Entity.h"
#include "E.h"


class dataFile {
      public:
             dataFile();//Declare constructor w/o parameters
             dataFile(int);
             ~dataFile();//Declare Destructor
             //~ void saveFile(unsigned int nbEntity);
             void saveFile(Entity * objEntity);
             //~ void saveFileEntity(unsigned int nbEntity);
             void saveFileEntity(Entity * objEntity);
             //~ static void saveFileFromGL(const char* FileName);
             //~ void menuFile(Entity objEntity);
             void loadFile(char menuChoice, Entity * objEntity);
             //~ void enterData(unsigned int menuChoice);             
             void enterDataMultiLevel();
             void enterLayer(Entity * objEntity);
             void mergeEntity();
             void listFile();
             void searchFile(std::string curr_directory, std::string extension);
             void displayEntity(Entity * objEntity);
             //~ void toEntity (Entity objEntity);
             //~ static void dataFiletoGlEntity();

//             static void toGlEntity (glEntity objEntity);
             void insertEntity(int input);
             void loadDefault(Entity * objEntity);
             //~ void resetDataFile();
             //~ void deleteEntity(int reference, unsigned int option_delete);
             void firstLine(char* FileNameIn); // to be improved with pointers
             void assignIndex();
//             static void storeDataFromFile(char* FileNameIn);
             void storeDataFromFile();
             //~ void data_to_Entity();
             
             void save_data_to_file(E * objE);


      public:

             int file_size;         // parameter for the size of the Dynamic Array            
             char menuKeyChoice;
             std::vector<std::string> storeDataText;
            static bool load_File;

      };

#endif
