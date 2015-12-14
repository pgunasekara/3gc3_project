#ifndef __NODEMODEL_H__	//guard against cyclic dependancy
#define __NODEMODEL_H__

#include "node.h"

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>	
#endif
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

#include "Hitbox.h"
#include "math3D.h"

enum ModelType{
	Sphere,
	Cube,
	Cone,
	Cylinder,
	Torus,
	Teapot,
	Tetrahedron,
	Octahedron,
	Dodecahedron,
	Icosahedron,
	Custom
};

enum Materials
{
	mat1, mat2, mat3, mat4, mat5
};

class NodeModel:public Node{
public:
	NodeModel(ModelType whatType);	//constructor
	ModelType modelType;

	Vector3D position, rotation, scale;
	Materials currentMat;

	bool current;
	Hitbox hit;

	vec3D tr;
	vec3D sc;
	quaternion rot;

	//Bounding box information is already specified in the math library

	virtual void nodeSpecificCodeDown();
};

#endif