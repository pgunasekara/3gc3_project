//CubeMap.cpp
#include "CubeMap.h"

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

CubeMap::CubeMap()
{
    /*int w, h;
    unsigned char *image;
    GLunit texture;*/
    //load the map
    this->texture = SOIL_load_image(filename, this->&width, this->&height, 0, SOIL_LOAD_RGB);
    float cubeVerts[][] = {{},{},{},{},{},{}};

    {{-0.5f, 0.0f, 0.5f, 0.5f}, {0.0f, 0.5f,   0.5f, 1.0f}, {0.5f,  -0.5f, 1.0f, 0.5f}, {-0.5f, 1.0f, -0.5f,  0.5f}, {1.0f, -0.5f,  0.5f, 0.0f}, {-0.5f, -0.5f, 0.0f, -0.5f}, {0.5f, 0.0f, 0.5f,   0.5f}, {0.0f, -0.5f,  0.5f, 1.0f}, {-0.5f,  0.5f, 1.0f, 0.5f}, {-0.5f, 0.0f, -0.5f,  -0.5f}, {0.0f, 0.5f, -0.5f, 1.0f}, {0.5f, -0.5f, 1.0f, -0.5f}};

    float cubeTextureCoords[] = { 0.0,0.0, 1.0,0.0, 1.0,1.0, 0.0,1.0, 0.0,0.0, 1.0,0.0, 1.0,1.0, 0.0,1.0, 0.0,0.0, 1.0,0.0, 1.0,1.0, 0.0,1.0, 0.0,0.0, 1.0,0.0, 1.0,1.0, 0.0,1.0};
}

CubeMap::~CubeMap()
{

}

void CubeMap::drawMap()
{

}
