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

std::string choice;

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

    std::vector< std::vector< std::vector <float> > > vCube1;
    std::vector< std::vector< std::vector <float> > > vCube2;
    std::vector< std::vector< std::vector <float> > > vCube3;

    std::vector<float> coordinate;
    std::vector<float> colors;
    
    std::vector< vector <float> > vertex;
    std::vector< vector <float> > color_faces;




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

std::vector<std::vector<std::vector<float> > > vectCube2
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

std::vector<std::vector<float> > vColor =
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

std::vector< std::vector<float> > GLtriangle_half_cubeFace
{
    {1.0f,1.0f,1.0f},
    {-1.0f,1.0f,1.0f},
    {-1.0f,1.0f,-1.0f}
};



std::vector< std::vector<float> > GLtriangle_cubeFace
{
    {1.0f,1.0f,1.0f},
    {-1.0f,1.0f,1.0f},
    {-1.0f,1.0f,-1.0f},
    {-1.0f,1.0f,-1.0f},
    {1.0f,1.0f,-1.0f},
    {1.0f,1.0f,1.0f}
};

std::vector<std::vector<float> > vTriangle_face;

std::vector<std::vector<float> > vQuaq_face
{
    // vertices
    {1.0f,1.0f,1.0f},
    {-1.0f,1.0f,1.0f},
    {-1.0f,1.0f,-1.0f},
    {1.0f,1.0f,-1.0f}
};

// Cube with Vertices

void half_faceV_0 ()
{

    glTranslatef (0.0,0.0,0.0);
    glEnable (GL_BLEND);
    glEnable(GL_COLOR_MATERIAL);
    
    glBegin(GL_TRIANGLES);
    glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
    //~ glVertex3fv(GLtriangle_half_cubeFace.data());
    glVertex3f(-1.0f,1.0f,1.0f);
    glVertex3f(-1.0f,1.0f,-1.0f);
    glVertex3f(1.0f,1.0f,-1.0f);
    glEnd();
} 

void half_faceV ()
{

    glTranslatef (0.0,0.0,0.0);
    glEnable (GL_BLEND);
    glEnable(GL_COLOR_MATERIAL);
    
    glBegin(GL_TRIANGLES);
    glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
    std::vector< std::vector<float> >::iterator it;
    
    for (it=GLtriangle_half_cubeFace.begin();it!=GLtriangle_half_cubeFace.end();++it)
    {
        glVertex3fv((*it).data());
    }
    glEnd();
}



void faceV ()
{
    glTranslatef (0.0,0.0,0.0);
    glEnable (GL_BLEND);
    glEnable(GL_COLOR_MATERIAL);
    
    glBegin(GL_TRIANGLES);
    glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
    std::vector< std::vector<float> >::iterator it;
    
    //~ for (it=GLtriangle_cubeFace.begin();it!=GLtriangle_cubeFace.end();++it)
    for (it=vTriangle_face.begin();it!=vTriangle_face.end();++it)
    {
        std::cout << "vertex " << (*it).data() << std::endl;
        glVertex3fv((*it).data());
    }
    glEnd();
}

void quads_to_triangles(std::vector< std::vector<float> > & vect_quads, std::vector< std::vector<float> > & vect_triangles)
{
    //~ for (std::vector< std::vector<float> >::iterator qit = vect_quads.begin();qit != vect_quads.end(); ++qit)
    //~ {
        //~ vect_triangles.push_back((*qit));
    //~ }
    vect_triangles.push_back(vect_quads.front());
    vect_triangles.push_back(vect_quads[1]);
    vect_triangles.push_back(vect_quads[2]);
    vect_triangles.push_back(vect_quads[2]);
    vect_triangles.push_back(vect_quads.back());
    vect_triangles.push_back(vect_quads.front());
}

void update(){
    glRotatef(rotx,1.0,1.0,0.0);
    if(rotx<360) rotx+=0.05;
    else rotx=0;
    }
    
void cubeV (){

    glPushMatrix();
    
    glTranslatef (-5.0,5.0,-5.0);
    update();
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

    glPopMatrix();

    }

void cubeVec (){

    glPushMatrix();

    glTranslatef (0.0,5.0,-5.0);
    update();
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
    
    glPopMatrix();
    
    }
    
void cubeVec2 ()
{

    int it_color;    
    std::vector< std::vector< std::vector<float> > >::iterator it;
    std::vector< std::vector<float> >::iterator itf;

    glPushMatrix();

    glTranslatef (5.0,5.0,-5.0);
    update();
    glEnable (GL_BLEND);
    glEnable(GL_COLOR_MATERIAL);

    glBegin(GL_QUADS);
    
    for (it=vectCube2.begin();it!=vectCube2.end();++it)
    {
        it_color = it-vectCube2.begin();
        glColor3fv(vColor[it_color].data());

        for (itf=(*it).begin();itf!=(*it).end();++itf)
        {
            glVertex3fv((*itf).data());
        }
    }

    glEnd();
    
    glPopMatrix();
    
}
    
    
void cubeVec3 (std::vector< std::vector < std::vector <float> > >& vect)
{

    std::vector< std::vector< std::vector<float> > >::iterator it;
    std::vector< std::vector<float> >::iterator itf;

    glPushMatrix();

    glTranslatef (-5.0,0.0,-5.0);
    update();
    glEnable (GL_BLEND);
    glEnable(GL_COLOR_MATERIAL);

    int it_color;    
    glBegin(GL_QUADS);
    
   
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
    
    glPopMatrix();
}

void glTrianlges_cubeVec5 (std::vector< std::vector < std::vector <float> > >& vect)
{

    glTranslatef (0.0,0.0,0.0);
    glEnable (GL_BLEND);
    glEnable(GL_COLOR_MATERIAL);

    int it_color;    
    glBegin(GL_TRIANGLES);
    
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

void cubeVec4 (std::vector<std::vector<float> > & vect_colors, std::vector< std::vector < std::vector <float> > >& vect_coord)
{

    glPushMatrix();

    glTranslatef (0.0,0.0,-5.0);
    update();
    glEnable (GL_BLEND);
    glEnable(GL_COLOR_MATERIAL);

    int it_color;    
    glBegin(GL_QUADS);
    
    std::vector< std::vector< std::vector<float> > >::iterator it;
    std::vector< std::vector<float> >::iterator itf;
    
    glColor3f(0.0f,1.0f,0.0f);
    
    for (it=vect_coord.begin();it!=vect_coord.end();++it)
    {
        it_color = it-vect_coord.begin();
        //~ std::cout << "it_color" << it_color << std::endl;
        //~ glColor3fv(vect_colors[it_color].data());

        for (itf=(*it).begin();itf!=(*it).end();++itf)
        {
            glVertex3fv((*itf).data());
        }
    }

    glEnd();
    
    glPopMatrix();
}

void reshape(int w, int h){
    glViewport (0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode (GL_MODELVIEW);
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
    //~ update();
    
    //~ drawSphere();
    //~ drawCube();
    //~ cubeV();
    if (choice=="t0") half_faceV_0();
    if (choice=="t") half_faceV();
    if (choice=="f" || choice=="q2f") faceV();
    if (choice=="0")
    {
        cubeV();
        cubeVec();
        cubeVec2();
        cubeVec3(vCube1);
        //~ cubeVec4(color_faces, vCube2);
    }
    if (choice=="1") cubeVec();
    if (choice=="2") cubeVec2();
    if (choice=="3") cubeVec3(vCube1);
    if (choice=="4") cubeVec4(color_faces, vCube2);
    if (choice=="5") glTrianlges_cubeVec5(vCube3);
    
    
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

void testVector_Display_2d(std::vector< std::vector <float> > &testVector)
{
    // iterators for testVector:
    std::vector< std::vector <float> >::iterator color_faces;
    std::vector <float>::iterator colors;
    
    // nested loops
        for (color_faces = testVector.begin(); color_faces != testVector.end();++color_faces)
        {
            std::cout << "Color for face " << (color_faces - testVector.begin()) + 1 << " : " ;
            std::cout << " (";
            for (colors = (*color_faces).begin(); colors != (*color_faces).end(); ++colors)
            {
                std::cout << (*colors);
                if (colors != (*color_faces).end()-1) std::cout << ", ";
            }
            std:: cout << ")" << std::endl;
        }
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
    
    std::vector<int> index;

    std::cout << argc << "arguments " << std::endl;
    for (int i = 0; i < argc; ++i) 
    {
        std::cout << "for i: " << i << " : ";
        std::cout << argv[i] << std::endl;
    }
    if (argc>1) choice= std::string(argv[1]);
    else choice = "2";
    
    if (choice=="3" || choice == "0")
    {
        testEntity.load_XML_File_to_E("../datafiles/testCube0.xml");
        testEntity.E_display_all(0);
        for (int f=0; f < 6 ; ++f)
        {
            for (int v=0; v < 4; ++v)
            {
                index = {f,v};
                //~ std::cout << testEntity.vE_get_by_index(index, index.begin())-> name <<  testEntity.vE_get_by_index(index, index.begin())-> data<< std::endl;
                (testEntity.vE_get_by_index(index, index.begin()))->vE_copy_To_Vector_Float(coordinate);
                vertex.push_back(coordinate);
                coordinate.clear();
            }
            vCube1.push_back(vertex);
            testVector_Display_2d(vertex);
            vertex.clear();
        }
        testVector_Display(vCube1);
    }
    //~ if (!strcmp(choice,"4"))
    if (choice=="4")
    {
        std::cout << "testCube" << std::endl;
        testEntity.load_XML_File_to_E("../datafiles/testCube.xml");
        testEntity.E_display_all(0);
        for (int f=0; f < 6 ; ++f)
        {
            //~ for (int v=0; v < 4; ++v)
            
            index = {f,0};
            //~ std::cout << testEntity.vE_get_by_index(index, index.begin())-> name <<  testEntity.vE_get_by_index(index, index.begin())-> data<< std::endl;
            (testEntity.vE_get_by_index(index, index.begin()))->vE_copy_To_Vector_Float(colors);
            color_faces.push_back(colors);
            colors.clear();
            
            
            for (int v=1; v < 5; ++v)
            {
                index = {f,v};
                //~ std::cout << testEntity.vE_get_by_index(index, index.begin())-> name <<  testEntity.vE_get_by_index(index, index.begin())-> data<< std::endl;
                (testEntity.vE_get_by_index(index, index.begin()))->vE_copy_To_Vector_Float(coordinate);
                vertex.push_back(coordinate);
                coordinate.clear();
            }
            vCube2.push_back(vertex);
            vertex.clear();
        }
        testVector_Display(vCube2);
        testVector_Display_2d(color_faces);
    }
    
    if (choice=="f") vTriangle_face = GLtriangle_cubeFace;
    
    if (choice=="q2f")
    {
        quads_to_triangles(vQuaq_face,vTriangle_face);
        std::cout << std::endl;
        std::cout << "vQuaq_face" << std::endl;
        testVector_Display_2d(vQuaq_face);
        std::cout << "vTriangle_face" << std::endl;
        testVector_Display_2d(vTriangle_face);
    }


    if (choice == "5")
    {
        testEntity.load_XML_File_to_E("../datafiles/testCube0.xml");
        testEntity.E_display_all(0);
        for (int f=0; f < 6 ; ++f)
        {
            
            for (int v=0; v < 4; ++v)
            {
                index = {f,v};
                (testEntity.vE_get_by_index(index, index.begin()))->vE_copy_To_Vector_Float(coordinate);
                vertex.push_back(coordinate);
                //~ testVector_Display_2d(vertex);
                coordinate.clear();
            }
            quads_to_triangles(vertex, vTriangle_face);
            testVector_Display_2d(vertex);
            testVector_Display_2d(vTriangle_face);
            vCube3.push_back(vTriangle_face);
            vTriangle_face.clear();
            vertex.clear();
        }
        testVector_Display(vCube3);
    }
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
