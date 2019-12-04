/*! \mainpage Multidimensional Data Entity
 * 
 *  Version: 0.1.0
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
 *  <li> <b>no option</b> : if the program is executed with no options,
 * all text mode menus will be displayed consecutively in the console.
 *  <li> <b>-g</b>
 *     display directly in openGL, skip the Display Mode menu
 *  <li> <b>-f [FILE]</b>
 *     load (xml) file, skip the Data menu
 *  </ul>
 * 
 */
