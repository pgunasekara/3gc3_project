#ifndef __MESH3D_H__
#define __MESH3D_H__
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h> 

using namespace std;

	
struct vertex3D{
	float x, y, z;
};

struct faceVertNorms3D{
	float vn1,vn2,vn3,vn4;
};

struct vertNorms3D{
	float x, y, z;
};

struct faces3D{
	int v1, v2, v3, v4;
};

struct colour3D{
	// float red[] = {1,0,0};
	// float green[] = {0,1,0};
	// float blue[] = {0,0,1};
};

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
