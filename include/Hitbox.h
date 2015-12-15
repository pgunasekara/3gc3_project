#ifndef HITBOX_H
#define HITBOX_H
#include <vector>
#include "math3D.h"
using namespace std;

class Plane;
	
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
	Plane *hit;
};

struct colour3D{
	// float red[] = {1,0,0};
	// float green[] = {0,1,0};
	// float blue[] = {0,0,1};
};

class Plane {
public:
	Plane();
	Plane(vert3D a, vert3D b, vert3D c, vert3D d,bool xPlane, bool yPlane, bool zPlane);
	Plane(vertex3D a, vertex3D b, vertex3D c, vertex3D d,bool xPlane, bool yPlane, bool zPlane);
	void draw();
	bool Intersect(vec3D v0,vec3D vD, float* tNear, float* tFar,vert3D minP, vert3D maxP);
	bool Intersect(vec3D v0,vec3D vD);
	void Scale(vec3D transform);
	void Translate(vec3D transform);
	void Rotate(quaternion transform);

	vert3D a,b,c,d,minP,maxP;
	vec3D norm;
	bool xPlane,yPlane,zPlane,selected;
};

class Hitbox{
public:
	Hitbox();
	Hitbox(vert3D low, vert3D high, int ID);
	~Hitbox();
	void draw();
	int Intersect(vec3D v0,vec3D vD);
	int IntersectSphere(vec3D Ray);
	void applyAxesChanges(vec3D transform);
	
	void Scale(vec3D transform);
	void Translate(vec3D transform);
	void Rotate(quaternion transform);

	vert3D minP,maxP;
	vec3D xaxis,yaxis,zaxis;
	vector<Plane*> Planes;
	int ID;
};

#endif