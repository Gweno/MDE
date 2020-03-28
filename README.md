## <p align="center">Multidimensional Data Entity - GL display</p>
### <p align="center">Version 0.2.1</p>

A Multidimensional Data Entity is an instance of an object of class **MDE**.
The base class **MDE** has 3 member variables:
#### public: std::string data
#### private: std::string name
#### private: std::vector<*MDE> vMDE

#### The public member *data* holds all data assigned to the private member *name*, can store strings of any size and can include the space character.
#### The private member *name* holds a string of any size but no space character should be included, it acts as a name for the MDE internally.
#### The private member *vMDE* is a standard vector of pointers to instances of MDE, that is what gives the multidimension capability of an MDE.

##### Main documentation and detailed API can be found <a href="http://lesgwenos.freehostia.com/MDE_v0.1/html/index.html">here</a>
---
For now the **MDE** program offers two display modes. The text mode and the openGL (g) mode.
The text mode displays menus in the terminal, it is used for testing and development.
The openGL mode displays the instance of an *MDE* object in an openGL context, it later will be used as a GUI and a as rendered viewport in 3D.

---
## Menu flow

### Data menu:

##### --------- Data ----------

##### f)    Load Data File (xml)
##### k)    or keep current file?
##### s)    Load Data testing Set
##### u)    User Data Input
##### quit) Quit
##### -------------------------

#### If you choose to load a file with option <big>*f*</big> (Load Data File(xml)) then you get the *Load File* menu.

#### Option <big>*k*</big> is not displayed at the first session. You can choose it once a file has been loaded and displayed in text mode already. The name of the file should be displayed at the bottom of this *Data* menu.

#### If you choose option <big>*s*</big> at the *Data* menu, a testing function part of MDE class creates a new MDE instance with sub-entities. This is mainly for quick testing purposes.
#### The option <big>*u*</big> at the *Data* menu is for the user to create an instance of a MDE object.
#### Starting from the master Entity, you then enter sub-entities for each level, starting with the one just after the master Entity and going top to bottom, until user type 'end' in the name of each branches created.
#### You can save the data set as an xml file after selecting <big>*s*</big> or <big>*u*</big>.
### Load File menu:
### <small>(when you choose option <big>*f*</big> at the *Data Menu)</small>
####
##### -------------------- Load File ---------------------
##### Enter name of xml file (including extension)
##### Or choose between examples:
##### 1: perma.xml (this file is on a single line)
##### 2: bookstore.xml
##### 3: activities.xml
##### ----------------------------------------------------

#### You can either enter a file name including its extension, typically an xml file, to load it, or use one of the 3 files examples.
#### All files are in a directory named ./datafiles
#### Once a file or set of data is loaded you get the *DisplayMode* menu
---
### Display Mode menu:
####
##### -------------------- Display Mode ---------------------
##### t)    Display in Text mode
##### g)    Display in Graphic Mode
##### quit) Quit
##### ----------------------------------------------------

#### Enter <big>*t*</big> to display the content loaded on the terminal in text mode. The *Data* menu is then display.
#### Enter <big>*g*</big> to display the content loaded in an openGL viewport.
##### For now you cannot come back to the text-menu once you are in the ogpenGL mode. So you need to close or type the ESCape key to close the progam.

---
### Mouse control in Graphic Mode menu:
#### Use Mouse <big>*Left click*<big> to move camera right,left, up or down.
#### Use Mouse <big>*Right click*<big> to rotate camera on x or y axis.
#### Use Mouse <big>*Wheel*<big> to zoom in or out.
### Keyboard control in Graphic Mode menu:
#### Use key <big>*ESC*<big> to exit the program.
