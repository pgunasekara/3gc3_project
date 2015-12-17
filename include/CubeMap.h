//CubeMap.h
#ifndef __CUBEMAP_H_
#define __CUBEMAP_H_

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

class CubeMap
{
public:
    int width, height;
    unsigned char *left, *right, *up, *down, *foward, *backward;
    GLunit texture;
    void loadTexture(char *filename);
    void drawMap();
};
#endif
