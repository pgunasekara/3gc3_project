#ifndef HITBOX_H
#define HITBOX_H
#include <vector>
#include "../Math/math3D.h"
using namespace std;


class Plane {
public:
	Plane();
	Plane(vert3D a, vert3D b, vert3D c, vert3D d,bool xPlane, bool yPlane, bool zPlane);
	void draw();
	bool Intersect(vec3D v0,vec3D vD, float* tNear, float* tFar,vert3D minP, vert3D maxP);
	void Scale(vec3D transform);
	void Translate(vec3D transform);
	void Rotate(quaternion transform);

	vert3D a,b,c,d;
	vec3D norm;
	bool xPlane,yPlane,zPlane;
};

class Hitbox{
public:
	Hitbox();
	Hitbox(vert3D low, vert3D high, int ID);
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