#ifndef __NODE_H__	//guard against cyclic dependancy
#define __NODE_H__

//Node Class.
//should act as a template class for other sub-types of
//Nodes

#include <vector>
#include "structs.h"
#include "../Hitbox/Hitbox.h"
#include "../Math/math3D.h"
using namespace std;

extern int getID();

//if you add more derived classes
//add the types here
enum NodeType{
	root,
	group,
	transformation,
	model
};

class Node{
public:
	Node();	//constructor

	NodeType nodeType;
	bool isDrawable;
	int ID;
	vector<Node*> *children;
	Node* parent;
	int currentChild;
	bool current;
	Hitbox hit;

	vec3D tr;
	vec3D sc;
	quaternion rot;
	vec3D xaxis,yaxis,zaxis;

	void draw();
	virtual void nodeSpecificCodeDown();
	virtual void nodeSpecificCodeUp();
	//virtual void applyChangesAxes(vec3D transform);
};

#endif