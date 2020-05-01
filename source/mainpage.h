/*! \mainpage Multidimensional Data Entity
 * 
 *  Version: 0.3.2
 *
 * \section intro_sec MDE in a glance
 *
 * An MDE can be used for multiple purposes.
 * In this first version of MDE, an MDE instance is simply used to
 * display the content of an xml file in text mode or
 * in an OpenGL 3D viewport.
 *
 * \section install_sec Installation
 *
 * \subsection step1 Step 1: Verify dependencies
 * List of libraries you need to have already installed:
 * <ul>
 *  <li> OpenGL
 *  <li> glut
 *  <li> freeglut
 *  <li> glew
 *  <li> freetype2
 *  <li> glm
 *  </ul>
 * 
 * \subsection step2 Step 2: external resources
 * There are currently  external resources used by this program on top
 *  of the dependancies:
 * <ul>
 *  <li> <b>./source/common/</b> has some useful functions for shader
 * programs from the OpenGL Programming wikibook:
 * http://en.wikibooks.org/wiki/OpenGL_Programming.
 *  <li> <b>./fonts/</b> has a few free/opensource font files used
 *  by the freetype2 library.
 *  </ul>
 * \subsection step3 Step 3: make
 * Makefile is in the <b>source</b> folder.
 * The binary file is output in <b>bin</b> folder
 * 
 * \subsection step4 Step 4: run program
 * 
 * From folder <b>bin</b> execute ./mde [OPTION]
 * 
 * Options:
 * 
 *  <ul>
 * 
 *  <li> <b>no option</b><br>
 * if the program is executed with no options,
 * all text mode menus will be displayed consecutively in the console.
 *  <li> <b>-v, -\-version</b><br>
 *     to get the program's version (and quit).
 *  <li> <b>-g, -\-glmode</b><br>
 *     display directly in graphic mode (openGL), skip the Display Mode menu
 *  <li> <b>-f, -\-file</b> *file*<br>
 *     load (xml) file, skip the Data menu
 *  <li> <b>-n, -\-font</b> *font*<br>
 *     set the font (only open source font on the ./fonts folder for now).
 *  <li> <b>-s, -\-font-size</b> *size*<br>
 *     set the font size as an integer, best to use multiple of 8.
 *  <li> <b>-o, -\-font-color</b> *r,g,b,a*<br>
 *     set rgba colours for font with each values in range float[0,1].
 *  <li> <b>-b, -\-box</b><br>
 *     for displaying a frame box for each text.
 *  <li> <b>-c, -\-box-color</b> *r,g,b,a*<br>
 *     set rgba colours/alpha for the box frames with each values in range float[0,1].
 *  <li> <b>-d, -\-background-color</b> *r,g,b,a*<br>
 *     set rgba colours/alpha for the background with each values in range float[0,1] (alpha has no effect).
 *  <li> <b>-p, -\-padding</b> *vertical_padding,horizontal_padding*<br>
 *     add symetrical vertical and horizontal padding, creating a box frame larger than the text displayed.
 *  <li> <b>-a, -\-origin</b> *x,y,z*<br>
 *     set the coordinates of origin to (x,y,z).
 *  <li> <b>-r, -\-offset-rule</b> *name<->index.x,name<->index.y,name<->index.z,index.x,index.y,index.z,level.x,level.y,level.z*<br>
 *     set the offset rule with floats
 *  <li> <b>-m, -\-move</b><br>
 *     for moving mode (makes the MDE instance move on all axis).
 *  </ul>
 *
 * 
 *  \section Other documentation
 * 
 *  <ul>
 *  <li> <a href="https://github.com/Gweno/MDE/blob/master/README.md">README</a>
 *  <li> <a href="https://github.com/Gweno/MDE/blob/master/dependencies.txt">Dependencies</a>
 *  </ul> 
 */
