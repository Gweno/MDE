#include <iostream>
#include <vector>
#include <GL/glut.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/ext.hpp>
#include <stdlib.h>
#include "E.h"

//~ using namespace glm;

//~ GLfloat MatSpec[] = {1.0,1.0,1.0,1.0};
//~ GLfloat MatShininess[] = {128.0};
//~ GLfloat LightPos[] = {-1.0,1.0,0.0,0.0};
float rotx=0.0f;

//Cube primitive
void drawCube() {
    glPushMatrix();
    glTranslatef (0.0,0.0,0.0);
    glEnable (GL_BLEND);
    glEnable(GL_COLOR_MATERIAL);
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
    glutSolidCube (2.0);
    glPopMatrix();
}
 
//Sphere primitive
void drawSphere() {
    glPushMatrix();
    glTranslatef (0.0,0.0,0.0);
    glEnable (GL_BLEND);
    glEnable(GL_COLOR_MATERIAL);
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
    glutSolidSphere (0.3,20,20);    
    glPopMatrix();
}

static const float arr1[] = {1.0f,1.0f,1.0f, 1.0f};
std::vector<float> vec1 (arr1, arr1 + sizeof(arr1) / sizeof(arr1[0]) );

std::vector<std::vector<float> > vCube
{
// top
{1.0f,1.0f,1.0f},
{-1.0f,1.0f,1.0f},
{-1.0f,1.0f,-1.0f},
{1.0f,1.0f,-1.0f},

// bottom
{1.0f,-1.0f,-1.0f},
{-1.0f,-1.0f,-1.0f},
{-1.0f,-1.0f,1.0f},
{1.0f,-1.0f,1.0f},

// front
{1.0f,1.0f,1.0f},
{1.0f,-1.0f,1.0f},
{-1.0f,-1.0f,1.0f},
{-1.0f,1.0f,1.0f},

// back
{1.0f,1.0f,-1.0f},
{-1.0f,1.0f,-1.0f},
{-1.0f,-1.0f,-1.0f},
{1.0f,-1.0f,-1.0f},

// left
{-1.0f,-1.0f,1.0f},
{-1.0f,-1.0f,-1.0f},
{-1.0f,1.0f,-1.0f},
{-1.0f,1.0f,1.0f},

// right
{1.0f,-1.0f,1.0f},
{1.0f,-1.0f,-1.0f},
{1.0f,1.0f,-1.0f},
{1.0f,1.0f,1.0f}
};

std::vector<std::vector<std::vector<float> > > vCube2
{
    // top
    {
        // vertices
        {1.0f,1.0f,1.0f},
        {-1.0f,1.0f,1.0f},
        {-1.0f,1.0f,-1.0f},
        {1.0f,1.0f,-1.0f}
    },
    
    // bottom
    {
        // vertices
        {1.0f,-1.0f,-1.0f},
        {-1.0f,-1.0f,-1.0f},
        {-1.0f,-1.0f,1.0f},
        {1.0f,-1.0f,1.0f},

    },
    
    // front
    {
        // vertices
        {1.0f,1.0f,1.0f},
        {1.0f,-1.0f,1.0f},
        {-1.0f,-1.0f,1.0f},
        {-1.0f,1.0f,1.0f},
        
    },
    
    // back
    {
        // vertices
        {1.0f,1.0f,-1.0f},
        {-1.0f,1.0f,-1.0f},
        {-1.0f,-1.0f,-1.0f},
        {1.0f,-1.0f,-1.0f},
        
    },
    
    // left
    {
        // vertices
        {-1.0f,-1.0f,1.0f},
        {-1.0f,-1.0f,-1.0f},
        {-1.0f,1.0f,-1.0f},
        {-1.0f,1.0f,1.0f},
    },
    
    // right
    {
        // vertices
        {1.0f,-1.0f,1.0f},
        {1.0f,-1.0f,-1.0f},
        {1.0f,1.0f,-1.0f},
        {1.0f,1.0f,1.0f},
        
    }
};

std::vector<std::vector<float> > vColor
{
// top
{0.0f, 1.0f, 0.0f},

// bottom
{0.5f, 0.0f, 0.5f},

// front
{1.0f, 0.0f, 0.0f},

// back
{0.0f, 0.0f, 1.0f},

// left
{0.0f, 0.5f, 0.5f},

// right
{0.5f, 0.5f, 0.0f}
};

//~ std::vector<float> vec;



// Cube with Vertices

void cubeV (){

    glTranslatef (0.0,0.0,0.0);
    glEnable (GL_BLEND);
    glEnable(GL_COLOR_MATERIAL);

        
    glBegin(GL_QUADS);
    // top
    glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
    //~ glVertex4f(1.0f,1.0f,1.0f, 1.0f);
    glVertex4fv(vec1.data());                   // testing with pointer to vector using .data() with glVertex4fv
    glVertex4f(-1.0f,1.0f,1.0f, 1.0f);
    glVertex4f(-1.0f,1.0f,-1.0f, 1.0f);
    glVertex4f(1.0f,1.0f,-1.0f, 1.0f);
    
    // bottom
    glColor4f(0.5f, 0.0f, 0.5f, 1.0f);
    glVertex4f(1.0f,-1.0f,-1.0f, 1.0f);
    glVertex4f(-1.0f,-1.0f,-1.0f, 1.0f);
    glVertex4f(-1.0f,-1.0f,1.0f, 1.0f);
    glVertex4f(1.0f,-1.0f,1.0f, 1.0f);
    
    // front
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);    
    glVertex4f(1.0f,1.0f,1.0f, 1.0f);
    glVertex4f(1.0f,-1.0f,1.0f, 1.0f);
    glVertex4f(-1.0f,-1.0f,1.0f, 1.0f);
    glVertex4f(-1.0f,1.0f,1.0f, 1.0f);

    
    // back
    glColor4f(0.0f, 0.0f, 1.0f, 1.0f);    
    glVertex4f(1.0f,1.0f,-1.0f, 1.0f);
    glVertex4f(-1.0f,1.0f,-1.0f, 1.0f);
    glVertex4f(-1.0f,-1.0f,-1.0f, 1.0f);
    glVertex4f(1.0f,-1.0f,-1.0f, 1.0f);


    
    // left
    glColor4f(0.0f, 0.5f, 0.5f, 1.0f);
    glVertex4f(-1.0f,-1.0f,1.0f, 1.0f);
    glVertex4f(-1.0f,-1.0f,-1.0f, 1.0f);
    glVertex4f(-1.0f,1.0f,-1.0f, 1.0f);
    glVertex4f(-1.0f,1.0f,1.0f, 1.0f);
    
    // right
    glColor4f(0.5f, 0.5f, 0.0f, 1.0f);
    glVertex4f(1.0f,-1.0f,1.0f, 1.0f);
    glVertex4f(1.0f,-1.0f,-1.0f, 1.0f);
    glVertex4f(1.0f,1.0f,-1.0f, 1.0f);
    glVertex4f(1.0f,1.0f,1.0f, 1.0f);
     
    glEnd();

    }

void cubeVec (){

    glTranslatef (0.0,0.0,0.0);
    glEnable (GL_BLEND);
    glEnable(GL_COLOR_MATERIAL);

    int it_color;    
    glBegin(GL_QUADS);
    
    std::vector< std::vector<float> >::iterator it;
    std::vector<float>::iterator itf;
    
    for (it=vCube.begin();it!=vCube.end();++it)
    {
        it_color = int(it-vCube.begin())/4;
        glColor3fv(vColor[it_color].data());
        glVertex3fv((*it).data());
    }

    glEnd();

    }
    
void cubeVec2 (){

    glTranslatef (0.0,0.0,0.0);
    glEnable (GL_BLEND);
    glEnable(GL_COLOR_MATERIAL);

    int it_color;    
    glBegin(GL_QUADS);
    
    std::vector< std::vector< std::vector<float> > >::iterator it;
    std::vector< std::vector<float> >::iterator itf;
    
    for (it=vCube2.begin();it!=vCube2.end();++it)
    {
        it_color = it-vCube2.begin();
        glColor3fv(vColor[it_color].data());

        for (itf=(*it).begin();itf!=(*it).end();++itf)
        {
            glVertex3fv((*itf).data());
        }
    }

    glEnd();
    }
    
    
void cubeVec3 (std::vector< std::vector < std::vector <float> > >& vect){

    glTranslatef (0.0,0.0,0.0);
    glEnable (GL_BLEND);
    glEnable(GL_COLOR_MATERIAL);

    int it_color;    
    glBegin(GL_QUADS);
    
    std::vector< std::vector< std::vector<float> > >::iterator it;
    std::vector< std::vector<float> >::iterator itf;
    
    for (it=vect.begin();it!=vect.end();++it)
    {
        it_color = it-vect.begin();
        glColor3fv(vColor[it_color].data());

        for (itf=(*it).begin();itf!=(*it).end();++itf)
        {
            glVertex3fv((*itf).data());
        }
    }

    glEnd();
    }


void reshape(int w, int h){
    glViewport (0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode (GL_MODELVIEW);
}

void update(){
    glRotatef(rotx,1.0,1.0,0.0);
    if(rotx<360) rotx+=0.025;
    else rotx=0;
    }

//Drawing function
void display(void)
{
    glClearDepth (1);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    //~ glLightfv(GL_LIGHT0, GL_POSITION, LightPos);
    
    //Background color
    //~ glClearColor(0,0,0,1);
    glTranslatef (0,0,-10);
    update();
    //~ drawSphere();
    //~ drawCube();
    //~ cubeV();
    cubeVec2();
    
    
    // Vector Position and Matrix Model (vec4 has 4 elements x,y,z and w.
    // w=1 when the vector is a position in space and w=0 when it is a direction

/*    glm::vec4 Position = glm::vec4(glm::vec3(10.0), 1.0);
    glm::mat4 Model = glm::translate(
                glm::mat4(1.0f), glm::vec3(1.0f));      // this operation is to get a matrix like:
                //  1 0 0 1        x 0 0 w
                //  0 1 0 1 where: 0 y 0 w
                //  0 0 1 1        0 0 z w
                //  0 0 0 1        0 0 0 w
                
    
    std::cout << "Position" << std::endl;
    for (int i=0;i<4;i++){
        std::cout << Position[i] << std::endl;
        }
        std::cout << std::endl;
        
    std::cout << "Model" << std::endl;
    for (int j=0;j<4;j++){        
            for (int i=0;i<4;i++){
                std::cout << Model[i][j] << " " ;
                }
            std::cout << std::endl;
    }                

    std::cout << std::endl;
    std::cout << std::endl;
    
    // Vector Transformed is a multiplication of Matrix Model by Vector Position

    glm::vec4 Transformed = Model * Position;
    
    std::cout << "Transformed" << std::endl;
    for (int i=0;i<4;i++){
        std::cout << Transformed[i] << std::endl;
        }
        std::cout << std::endl;
    
    //Draw order

*/
    //~ glFlush();
    glutSwapBuffers();    
}

void testVector_Display(std::vector< std::vector< std::vector <float> > > &testVector)
{
    // iterators for testVector:
    std::vector< std::vector< std::vector <float> > >::iterator face;
    std::vector< std::vector <float> >::iterator vertex;
    std::vector <float>::iterator coordinate;
    
    // nested loops
    for (face = testVector.begin(); face != testVector.end(); ++face)
    {
        std::cout << "face " << (face - testVector.begin()) + 1 << " : " << std::endl;
        for (vertex = (*face).begin(); vertex != (*face).end();++vertex)
        {
            std::cout << "    vertex " << (vertex - (*face).begin()) + 1 << " : " << std::endl;
            std::cout << "            (";
            for (coordinate = (*vertex).begin(); coordinate != (*vertex).end(); ++coordinate)
            {
                std::cout << (*coordinate);
                if (coordinate != (*vertex).end()-1) std::cout << ", ";
            }
            std:: cout << ")" << std::endl;
        }
    };
}

/*
void setVector(std::vector< std::vector< std::vector <float> > > &testVector)
{
    // iterators for testVector:
    std::vector< std::vector< std::vector <float> > >::iterator face;
    std::vector< std::vector <float> >::iterator vertex;
    std::vector <float>::iterator coordinate;
    
    // nested loops
    for (face = testVector.begin(); face != testVector.end(); ++face)
    {
        for (vertex = (*face).begin(); vertex != (*face).end();++vertex)
        {
            for (coordinate = (*vertex).begin(); coordinate != (*vertex).end(); ++coordinate)
            {
                (*coordinate).push_back(0);
            }
            (*vertex).push_back(*coordinate);
        }
        (*face).push_back(*vertex);
    };
}
*/

//Main program
int main(int argc, char **argv)
{

    E testEntity;
    testEntity.load_XML_File_to_E("../datafiles/testCube.xml");
    
    std::vector< std::vector< std::vector <float> > > testCube;

    //~ testVector_Display(vCube2);

    
    std::vector<float> coordinate;
    
    std::vector<int> index;
    
    //~ index = {0,1};
    
    //~ std::cout << testEntity.vE_get_by_index(index, index.begin())-> name <<  testEntity.vE_get_by_index(index, index.begin())-> data<< std::endl;
    //~ (testEntity.vE_get_by_index(index, index.begin()))->vE_copy_To_Vector_Float(coordinate);
    
    std::vector< vector <float> > vertex;
    
    for (int f=0; f < 6 ; ++f)
    {
        for (int v=0; v < 4; ++v)
        {
            index = {f,v};
            std::cout << testEntity.vE_get_by_index(index, index.begin())-> name <<  testEntity.vE_get_by_index(index, index.begin())-> data<< std::endl;
            (testEntity.vE_get_by_index(index, index.begin()))->vE_copy_To_Vector_Float(coordinate);
            vertex.push_back(coordinate);
            coordinate.clear();
        }
        testCube.push_back(vertex);
        vertex.clear();
    }
    testVector_Display(testCube);
    
    cubeVec3(testCube);

//*    
  glutInit(&argc, argv);
  //Simple buffer
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH );
  glutInitWindowPosition(0,0);
  glutInitWindowSize(250,250);
  glutCreateWindow("OpenGL Window");
  
  //Call to the drawing function
  glutDisplayFunc(display);
  glutIdleFunc (display);
  glutReshapeFunc (reshape);

  //Lighting stuff:      option 2
  //~ glShadeModel(GL_SMOOTH);	//GL_FLAT would look much worse
  glEnable(GL_DEPTH_TEST);
  //~ glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpec);
  //~ glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, MatShininess);  
  //~ glEnable(GL_LIGHTING);
  //~ glEnable(GL_LIGHT0);
  
  glutMainLoop();
  //*/
  return 0;
}
