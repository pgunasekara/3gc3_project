#ifndef __MESH3D_H__
#define __MESH3D_H__
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "Hitbox.h"

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

using namespace std;

class Mesh3D{
public:
	Mesh3D();
	Mesh3D(int type);
	~Mesh3D();
	void drawMesh();
	void load();
	void loadObj(char* filename);
	GLubyte* LoadPPM(char* file, int* width, int* height, int* max);
//private:
	vector<vertex3D> verts;
	vector<faces3D> faces;
	vector<faceVertNorms3D> faceVertNorms;
	vector<vertNorms3D> vertNorms;
	vector<colour3D> colours;
	vector<textureCoord> vertexTexture;
	vector<faces3D> faceTexCoords;
	int type;

};

#endif
