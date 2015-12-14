#ifndef __MESH3D_H__
#define __MESH3D_H__
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h> 
#include "../HitBox/Hitbox.h"

using namespace std;

class Mesh3D{
public:
	Mesh3D();
	void drawMesh();
	void load();
	void loadObj(char* filename);

private:
	vector<vertex3D> verts;
	vector<faces3D> faces;
	vector<faceVertNorms3D> faceVertNorms;
	vector<vertNorms3D> vertNorms;
	vector<colour3D> colours;

};

#endif
