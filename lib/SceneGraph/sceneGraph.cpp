#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include "sceneGraph.h"
#include "node.h"
#include "math3D.h"
#include "Hitbox.h"
#include <stdio.h>
#include <math.h>

SceneGraph::SceneGraph(){
	rootNode = new Node();
	currentNode = rootNode;

	currentNode->current = true;
	printf("scene graph init done\n");
	// R0
	vec3D near = vec3D();
	// R1
	vec3D far = vec3D(1,1,1);
	vec3D distance = (far - near).normalize();
}

//Scene Graph Navigation
//resets the current node to the root of the graph
void SceneGraph::goToRoot(){
	currentNode = rootNode;
}

//moves to a child node i
void SceneGraph::goToChild(int i)
{
	if (i < currentNode->children->size() && i >= 0)
		currentNode = currentNode->children->at(i);
	else
		printf("child out of range");
}

void SceneGraph::goToParent(){
	if (currentNode->parent != 0)
		currentNode = currentNode->parent;
}

void SceneGraph::transformNode(Node *node)
{

	if(node->nodeType == transformation)
	{
		//currentNode.
	}
}

//inserts a child node into the current node
void SceneGraph::insertChildNodeHere(Node *node){
	//add our parent to our child node
	node->parent = currentNode;
	//now lets add it to our children!
	currentNode->children->push_back(node);
	//Switch to the new current node
	if(node->nodeType == model)
	{	
		hitBoxNodes.push_back(node);
		for(int i = 0; i < hitBoxNodes.size(); i++)
		{
			hitBoxNodes.at(i)->current = false;
		}
		hitBoxNodes.at(hitBoxNodes.size()-1)->current = true;
	}
	currentNode = node;
}


//deletes the current node, relinking the children as necessary
void SceneGraph::deleteThisNode()
{
	//Remove the current node, check if it has subnodes, and relink to the node above this one
	//Delete Any dangling nodes

	SceneGraph::goToParent();
	currentNode->children->clear();

}

//draw the scenegraph
void SceneGraph::draw(){
	rootNode->draw();
}


/*THIS NEEDS TO BE FIXED SO THAT IT DOESN't JUST SEARCH TOP NODES*/


void SceneGraph::searchByID(int ID)
{
	//Search for the ID that the program gives, and set the currentNode to be the node from that ID
	Node *tmpnode = currentNode;

	//recursively call our children
	if(currentNode->ID != ID)
	{
		//Go to the top Node, and search all nodes
		currentNode = rootNode;

		const int numberOfChildren = currentNode->children->size();
		if (numberOfChildren > 0)
		{
			for (int i = 0; i < numberOfChildren; i++)
			{
				if(currentNode->children->at(i)->ID == ID)
				{
					currentNode = currentNode->children->at(i);
					break;
				}
			}
		}
	}
}


bool SceneGraph::Intersect(int x, int y){

	Node *currentNodeIntersect = rootNode;

	//grab the matricies
	glGetDoublev(GL_MODELVIEW_MATRIX, matModelView); 
	glGetDoublev(GL_PROJECTION_MATRIX, matProjection); 
	glGetIntegerv(GL_VIEWPORT, viewport); 

	start = near.returnDoubleArray();
	finish = far.returnDoubleArray();

	//unproject the values
	double winX = (double)x; 
	double winY = viewport[3] - (double)y; 

	// get point on the 'near' plane (third param is set to 0.0)
	gluUnProject(winX, winY, 0.0, matModelView, matProjection, 
		 viewport, &start[0], &start[1], &start[2]); 

	// get point on the 'far' plane (third param is set to 1.0)
	gluUnProject(winX, winY, 1.0, matModelView, matProjection, 
		 viewport, &finish[0], &finish[1], &finish[2]); 

	near.update(start);
	far.update(finish);
	distance = (far - near).normalize();

	int ID_tmp;
	for(int i = 0; i < hitBoxNodes.size(); i++)
	{
		ID_tmp = hitBoxNodes.at(i)->hit.Intersect(near,distance);
		if(ID_tmp != -1)
		{
			for(int j = 0; j < hitBoxNodes.size(); j++)
			{
				hitBoxNodes.at(j)->current = false;
			}
			printf("ID FOUND\n");
			hitBoxNodes.at(i)->current = true;
			currentNode = hitBoxNodes.at(i);
		}else {
			printf("miss\n");
		}
	}

	/* check if sphere or box hitbox
	if (hit->Intersect(near,distance)){
		printf("hit\n");
	}else {
		printf("miss\n");
	}*/

	/*vec3D Ray = vec3D(distance.dot(distance),near.dot(distance)*2.0,near.dot(near) - 1);
	
	if (hit->IntersectSphere(Ray)){
		printf("hit\n");

	}else {
		printf("miss\n");
	}*/

	return false;

}
