/*
THIS IS NOT THE ACTUAL MAIN, MAKE ALL CHANGES IN SOURCE MAIN


 */

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
#include <math.h>
#include "lib/Camera/camera.h"
#include "lib/Hitbox/Hitbox.h"
#include "lib/SceneGraph/structs.h"
#include "lib/Math/math3D.h"

//sceneGraph
#include "lib/SceneGraph/sceneGraph.h"
#include "lib/SceneGraph/nodeGroup.h"
#include "lib/SceneGraph/nodeModel.h"
#include "lib/SceneGraph/nodeTransform.h"
#include <vector>

float mouseX,mouseY,globalW,globalH;
bool buttonDown = false;
float pos[] = {0,1,0};
float camPos[] = {2.5, 2.5, 5};
float angle = 0.005f;
bool PlaneExist = false;
Hitbox *hit;
Camera camera;

//node ids
int masterID = 0;

Vector3D ip;
Vector3D translation;
Vector4D rotation; 
Vector3D scale;

int getID()
{
	return masterID++;
}
// for the unprojection
double* start = new double[3];
double* finish = new double[3];

SceneGraph *SG;


/*LIGHTING*/
float light_pos[] = {0.0f, 10.0f, 0.0f, 1.0f};
float amb0[4] = {1, 1, 1, 1};
float diff0[4] = {1, 1, 1, 1};
float spec0[4] = {1, 1, 1, 1};

float m_amb[] = {0.0215, 0.1745, 0.0215, 1.0};
float m_diff[] = {0.07568, 0.61424, 0.07568, 1.0};
float m_spec[] = {0.633, 0.727811, 0.633, 1.0};
float shiny = 0.6;
//LIGHTING



void mouse(int button, int state, int x, int y){
	if(button ==  GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		SG->Intersect(x,y);
	}
}

void initLighting()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);
}

/* drawAxis() -- draws an axis at the origin of the coordinate system
 *   red = +X axis, green = +Y axis, blue = +Z axis
 */
void drawAxis()
{
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0,0,0);
	glVertex3f(50,0,0);
	glColor3f(0,1,0);
	glVertex3f(0,0,0);
	glVertex3f(0,50,0);
	glColor3f(0,0,1);
	glVertex3f(0,0,0);
	glVertex3f(0,0,50);
	glEnd();
}


//function which will populate a sample graph 
void initGraph()
{
	//Initial Transformation node
	//This is node 1, every time a node is created the masterId will incremenet by 2
	//NODE 0 is Root
		
	//DELETE THE ENTIRE GROUP NODE, WHEN CLEARING THE SCENE
	//The masterID for this first node will always be 1
	//When clearing, reset masterID to 0
	SG->insertChildNodeHere(new NodeGroup());
	printf("\nFIRST NODE %i\n", SG->currentNode->nodeType);
}

//callbacks
void keyboard(unsigned char key, int x, int y)
{
	
	switch (key)
	{
		case 'W':
			camera.Move(FORWARD);
			glutPostRedisplay();
			break;
		case 'A':
			camera.Move(LEFT);
			glutPostRedisplay();
			break;
		case 'S':
			camera.Move(BACK);
			glutPostRedisplay();
			break;
		case 'D':
			camera.Move(RIGHT);
			glutPostRedisplay();
			break;
		case 'Q':
			camera.Move(DOWN);
			glutPostRedisplay();
			break;
		case 'E':
			camera.Move(UP);
			glutPostRedisplay();
			break;
		case 'q':
		case 27:
			exit (0);
			break;
		case 'z':
			//CUBE
			//Go to the top group node
			while(SG->currentNode->ID > 1)
			{
				SG->goToParent();
			}

			//Create new transformation Node
			ip.x = 0;
			ip.y = 0;
			ip.z = 0;
			//SG->insertChildNodeHere(new NodeGroup());
			SG->insertChildNodeHere(new NodeTransform(Translate, ip));
			
			SG->insertChildNodeHere(new NodeModel(Cube));
			hit = new Hitbox();
			break;
		case 'x':
			//SPHERE
			//Go to the top group node
			while(SG->currentNode->ID > 1)
			{
				SG->goToParent();
			}
			
			//Create new transformation Node
			ip.x = 0;
			ip.y = 0;
			ip.z = 0;
			SG->insertChildNodeHere(new NodeTransform(Translate, ip));
			
			SG->insertChildNodeHere(new NodeModel(Sphere));
			hit = new Hitbox();
			break;

			//hit = new Hitbox();
			//PlaneExist = true;
		case 'c':
			//CONE
			//Go to the top group node
			while(SG->currentNode->ID > 1)
			{
				SG->goToParent();
			}
			
			//Create new transformation Node
			ip.x = 0;
			ip.y = 0;
			ip.z = 0;
			SG->insertChildNodeHere(new NodeTransform(Translate, ip));
			
			SG->insertChildNodeHere(new NodeModel(Cone));
			hit = new Hitbox();
			break;
		case 'v':
			//CYLINDER
			//Go to the top group node
			while(SG->currentNode->ID > 1)
			{
				SG->goToParent();
			}
			
			//Create new transformation Node
			ip.x = 0;
			ip.y = 0;
			ip.z = 0;
			SG->insertChildNodeHere(new NodeTransform(Translate, ip));
			
			SG->insertChildNodeHere(new NodeModel(Cylinder));
			hit = new Hitbox();
			break;
		case 'b':
			//TORUS
			//Go to the top group node
			while(SG->currentNode->ID > 1)
			{
				SG->goToParent();
			}
			
			//Create new transformation Node
			ip.x = 0;
			ip.y = 0;
			ip.z = 0;
			SG->insertChildNodeHere(new NodeTransform(Translate, ip));
			
			SG->insertChildNodeHere(new NodeModel(Torus));
			hit = new Hitbox();
			break;
		case 'n':
			//TEAPOT
			//Go to the top group node
			while(SG->currentNode->ID > 1)
			{
				SG->goToParent();
			}
			
			//Create new transformation Node
			ip.x = 0;
			ip.y = 0;
			ip.z = 0;
			SG->insertChildNodeHere(new NodeTransform(Translate, ip));
			
			SG->insertChildNodeHere(new NodeModel(Teapot));
			hit = new Hitbox();
			break;
		case 'm':
			//CUBE
			//Go to the top group node
			while(SG->currentNode->ID > 1)
			{
				SG->goToParent();
			}
			
			//Create new transformation Node
			ip.x = 0;
			ip.y = 0;
			ip.z = 0;
			SG->insertChildNodeHere(new NodeTransform(Translate, ip));
			
			SG->insertChildNodeHere(new NodeModel(Tetrahedron));
			hit = new Hitbox();
			break;


		//---------------TRANSLATION-------------------//
		case 'y':
		{
			//Modify Transformation Node
			//Node *tempCurrentNode = SG->currentNode;
			
			while(SG->currentNode->nodeType == model)
				SG->goToParent();

			translation.x = 0;

			int mod = glutGetModifiers();
			
			if(mod == GLUT_ACTIVE_ALT)
				translation.y = -0.05;
			else
				translation.y = 0.05;
			
			translation.z = 0;

			NodeTransform *tempNode = new NodeTransform(Translate, translation);

			int count;
			for(count = 0; count < SG->hitBoxNodes.size(); count++)
			{
				if(SG->hitBoxNodes.at(count)->current == true)
				{
					break;
				}
			}

			SG->hitBoxNodes.at(count)->hit.Translate(vec3D(translation.x, translation.y, translation.z));


			//Now we need to link nodes below it to the new node
			for(int i = 0; i < SG->currentNode->children->size(); i++)
			{
				tempNode->children->push_back(SG->currentNode->children->at(i));
				//The current node also needs to have it's old nodes removed
			}

			//The only node the currentNode should have is the the new node
			
			SG->currentNode->children->clear();

			SG->insertChildNodeHere(new NodeGroup());
			SG->insertChildNodeHere(tempNode);
			//SG->currentNode = tempCurrentNode;

			SG->draw();
			break;
		}

		case 't':
		{
			//Modify Transformation Node
			while(SG->currentNode->nodeType == model)
				SG->goToParent();

			int mod = glutGetModifiers();

			if(mod == GLUT_ACTIVE_ALT)
				translation.x = -0.05;
			else
				translation.x = 0.05;

			translation.y = 0;
			translation.z = 0;

			NodeTransform *tempNode = new NodeTransform(Translate, translation);
			
			int count;
			for(count = 0; count < SG->hitBoxNodes.size(); count++)
			{
				if(SG->hitBoxNodes.at(count)->current == true)
				{
					break;
				}
			}

			SG->hitBoxNodes.at(count)->hit.Translate(vec3D(translation.x, translation.y, translation.z));



			//Now we need to link nodes below it to the new node
			for(int i = 0; i < SG->currentNode->children->size(); i++)
			{
				tempNode->children->push_back(SG->currentNode->children->at(i));
				//The current node also needs to have it's old nodes removed
			}

			//The only node the currentNode should have is the the new node
			
			SG->currentNode->children->clear();

			SG->insertChildNodeHere(new NodeGroup());
			SG->insertChildNodeHere(tempNode);
			SG->draw();
			break;
		}

		case 'u':
		{
			Node *tempCurrentNode;
			//Modify Transformation Node
			while(SG->currentNode->nodeType == model)
				SG->goToParent();

			int mod = glutGetModifiers();

			translation.x = 0;
			translation.y = 0;
			if(mod == GLUT_ACTIVE_ALT)
			{
				translation.z = -0.05;
			}

			else
				translation.z = 0.05;

			NodeTransform *tempNode = new NodeTransform(Translate, translation);
			
			
			int count;
			for(count = 0; count < SG->hitBoxNodes.size(); count++)
			{
				if(SG->hitBoxNodes.at(count)->current == true)
				{
					break;
				}
			}

			SG->hitBoxNodes.at(count)->hit.Translate(vec3D(translation.x, translation.y, translation.z));

			//Now we need to link nodes below it to the new node
			for(int i = 0; i < SG->currentNode->children->size(); i++)
			{
				tempNode->children->push_back(SG->currentNode->children->at(i));
				//The current node also needs to have it's old nodes removed
			}

			//The only node the currentNode should have is the the new node
			
			SG->currentNode->children->clear();

			SG->insertChildNodeHere(new NodeGroup());
			SG->insertChildNodeHere(tempNode);
			SG->draw();
			break;
		}
		//---------------TRANSLATION-------------------//
		//------------------SCALE---------------------//
		case 'g':
		{
			//Modify Transformation Node
			//Node *tempCurrentNode = SG->currentNode;
			
			while(SG->currentNode->nodeType == model)
				SG->goToParent();

			scale.x = 1.0;

			int mod = glutGetModifiers();
			
			if(mod == GLUT_ACTIVE_ALT)
				scale.y = 0.5;
			else
				scale.y = 2.0;
			
			scale.z = 1.0;

			NodeTransform *tempNode = new NodeTransform(Scale, scale);

			int count;
			for(count = 0; count < SG->hitBoxNodes.size(); count++)
			{
				if(SG->hitBoxNodes.at(count)->current == true)
				{
					break;
				}
			}

			SG->hitBoxNodes.at(count)->hit.Scale(vec3D(scale.x, scale.y, scale.z));


			//Now we need to link nodes below it to the new node
			for(int i = 0; i < SG->currentNode->children->size(); i++)
			{
				tempNode->children->push_back(SG->currentNode->children->at(i));
				//The current node also needs to have it's old nodes removed
			}

			//The only node the currentNode should have is the the new node
			
			SG->currentNode->children->clear();

			SG->insertChildNodeHere(new NodeGroup());
			SG->insertChildNodeHere(tempNode);
			SG->draw();
			//SG->currentNode = tempCurrentNode;

			break;
		}

		case 'h':
		{
			//Modify Transformation Node
			while(SG->currentNode->nodeType == model)
				SG->goToParent();

			int mod = glutGetModifiers();

			if(mod == GLUT_ACTIVE_ALT)
				scale.x = 0.5;
			else
				scale.x = 2.0;

			scale.y = 1.0;
			scale.z = 1.0;

			NodeTransform *tempNode = new NodeTransform(Scale, scale);
			
			int count;
			for(count = 0; count < SG->hitBoxNodes.size(); count++)
			{
				if(SG->hitBoxNodes.at(count)->current == true)
				{
					break;
				}
			}

			SG->hitBoxNodes.at(count)->hit.Scale(vec3D(scale.x, scale.y, scale.z));



			//Now we need to link nodes below it to the new node
			for(int i = 0; i < SG->currentNode->children->size(); i++)
			{
				tempNode->children->push_back(SG->currentNode->children->at(i));
				//The current node also needs to have it's old nodes removed
			}

			//The only node the currentNode should have is the the new node
			
			SG->currentNode->children->clear();

			SG->insertChildNodeHere(new NodeGroup());
			SG->insertChildNodeHere(tempNode);
			SG->draw();
			break;
		}

		case 'k':
		{
			Node *tempCurrentNode;
			//Modify Transformation Node
			while(SG->currentNode->nodeType == model)
				SG->goToParent();

			int mod = glutGetModifiers();

			scale.x = 1.0;
			scale.y = 1.0;
			if(mod == GLUT_ACTIVE_ALT)
				scale.z = 0.5;
			else
				scale.z = 2.0;

			NodeTransform *tempNode = new NodeTransform(Scale, scale);
			
			int count;
			for(count = 0; count < SG->hitBoxNodes.size(); count++)
			{
				if(SG->hitBoxNodes.at(count)->current == true)
				{
					break;
				}
			}

			SG->hitBoxNodes.at(count)->hit.Scale(vec3D(scale.x, scale.y, scale.z));

			//Now we need to link nodes below it to the new node
			for(int i = 0; i < SG->currentNode->children->size(); i++)
			{
				tempNode->children->push_back(SG->currentNode->children->at(i));
				//The current node also needs to have it's old nodes removed
			}

			//The only node the currentNode should have is the the new node
			
			SG->currentNode->children->clear();

			SG->insertChildNodeHere(new NodeGroup());
			SG->insertChildNodeHere(tempNode);
			SG->draw();
			break;
		}
		//------------------SCALE---------------------//
		//-----------------ROTATE---------------------//
		case 'i':
		{
			//Modify Transformation Node
			//Node *tempCurrentNode = SG->currentNode;
			
			while(SG->currentNode->nodeType == model)
				SG->goToParent();

			int mod = glutGetModifiers();
			
			if(mod == GLUT_ACTIVE_ALT)
				rotation.w = 1.0;
			else
				rotation.w = -1.0;
			
			rotation.x = 1.0;
			rotation.y = 0.0;
			rotation.z = 0.0;

			NodeTransform *tempNode = new NodeTransform(Rotate, rotation);

			int count;
			for(count = 0; count < SG->hitBoxNodes.size(); count++)
			{
				if(SG->hitBoxNodes.at(count)->current == true)
				{
					break;
				}
			}

			SG->hitBoxNodes.at(count)->hit.Rotate(quaternion(rotation.w,rotation.x, rotation.y, rotation.z));


			//Now we need to link nodes below it to the new node
			for(int i = 0; i < SG->currentNode->children->size(); i++)
			{
				tempNode->children->push_back(SG->currentNode->children->at(i));
				//The current node also needs to have it's old nodes removed
			}

			//The only node the currentNode should have is the the new node
			
			SG->currentNode->children->clear();

			SG->insertChildNodeHere(new NodeGroup());
			SG->insertChildNodeHere(tempNode);
			SG->draw();
			//SG->currentNode = tempCurrentNode;

			break;
		}

		case 'o':
		{
			//Modify Transformation Node
			while(SG->currentNode->nodeType == model)
				SG->goToParent();

			int mod = glutGetModifiers();

			if(mod == GLUT_ACTIVE_ALT)
				rotation.w = 1.0;
			else
				rotation.w = -1.0;
			
			rotation.x = 0.0;
			rotation.y = 1.0;
			rotation.z = 0.0;

			NodeTransform *tempNode = new NodeTransform(Rotate, rotation);
			
			int count;
			for(count = 0; count < SG->hitBoxNodes.size(); count++)
			{
				if(SG->hitBoxNodes.at(count)->current == true)
				{
					break;
				}
			}

			SG->hitBoxNodes.at(count)->hit.Rotate(quaternion(rotation.w,rotation.x, rotation.y, rotation.z));



			//Now we need to link nodes below it to the new node
			for(int i = 0; i < SG->currentNode->children->size(); i++)
			{
				tempNode->children->push_back(SG->currentNode->children->at(i));
				//The current node also needs to have it's old nodes removed
			}

			//The only node the currentNode should have is the the new node
			
			SG->currentNode->children->clear();

			SG->insertChildNodeHere(new NodeGroup());
			SG->insertChildNodeHere(tempNode);
			SG->draw();
			break;
		}

		case 'p':
		{
			Node *tempCurrentNode;
			//Modify Transformation Node
			while(SG->currentNode->nodeType == model)
				SG->goToParent();

			int mod = glutGetModifiers();

			if(mod == GLUT_ACTIVE_ALT)
				rotation.w = 1.0;
			else
				rotation.w = -1.0;
			
			rotation.x = 0.0;
			rotation.y = 0.0;
			rotation.z = 1.0;

			NodeTransform *tempNode = new NodeTransform(Rotate, rotation);
			
			int count;
			for(count = 0; count < SG->hitBoxNodes.size(); count++)
			{
				if(SG->hitBoxNodes.at(count)->current == true)
				{
					break;
				}
			}

			SG->hitBoxNodes.at(count)->hit.Rotate(quaternion(rotation.w,rotation.x, rotation.y, rotation.z));

			//Now we need to link nodes below it to the new node
			for(int i = 0; i < SG->currentNode->children->size(); i++)
			{
				tempNode->children->push_back(SG->currentNode->children->at(i));
				//The current node also needs to have it's old nodes removed
			}

			//The only node the currentNode should have is the the new node
			
			SG->currentNode->children->clear();

			SG->insertChildNodeHere(new NodeGroup());
			SG->insertChildNodeHere(tempNode);
			SG->draw();
			break;
		}
		//------------------SCALE---------------------//


		case 'j':
			//delete the current Node
			//TODO, FIRST select the node to delete!

			//Make sure that the node being deleted is a NodeModel
			if(SG->currentNode->nodeType == model)
			{
				SG->deleteThisNode();
			}
			break;
	}
	glutPostRedisplay();
}

/*void special(int key, int x, int y)
{
	/* arrow key presses move the camera *0/
	switch(key)
	{
		case GLUT_KEY_LEFT:
			camPos[0]-=0.1;
			break;

		case GLUT_KEY_RIGHT:
			camPos[0]+=0.1;
			break;

		case GLUT_KEY_UP:
			camPos[2] -= 0.1;
			break;

		case GLUT_KEY_DOWN:
			camPos[2] += 0.1;
			break;
		
		case GLUT_KEY_HOME:
			camPos[1] += 0.1;
			break;

		case GLUT_KEY_END:
			camPos[1] -= 0.1;
			break;

	}
	glutPostRedisplay();
}*/

void init(void)
{	
	GLuint id = 1;
	camera = Camera();

	//init our scenegraph
	SG = new SceneGraph();

	//add various nodes
	//initializing our world
	initGraph();
}

void reshape(int w, int h)
{
	glEnable(GLUT_DEPTH);

	glClearColor(0, 0, 0, 0);
	glColor3f(1, 1, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 100);

	glMatrixMode(GL_MODELVIEW);
	glViewport(0,0,w,h);

	globalW = w;
	globalH = h;
	mouseX = w/2;
	mouseY = h/2;
	gluLookAt(2.5, 2.5, 2.5, 0, 0, 0, 0, 1, 0);
}



void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//draw the sceneGraph
	drawAxis();
	SG->draw();
	if (PlaneExist){
		hit->draw();
	}

	glutSwapBuffers();
}

void passive(int x,int y){
	if ((x - mouseX) > 0){
		camera.Spin(SRIGHT,angle);
	}
	else if ((x - mouseX) < 0){ 
		camera.Spin(SLEFT,angle);
	}
	if ((y - mouseY) > 0){
		camera.Spin(SDOWN,angle);
	}else if ((y - mouseY) < 0){
		camera.Spin(SUP,angle);
	} 
	glutPostRedisplay();
}

/* main function - program entry point */
int main(int argc, char** argv)
{
	glutInit(&argc, argv);		//starts up GLUT
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	init();

	glutInitWindowSize(600, 600);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("SimpleSceneGraph");	//creates the window

	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutKeyboardFunc(keyboard);
	//glutSpecialFunc(special);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(passive);

	initLighting();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glutMainLoop();				//starts the event loop
	return(0);					//return may not be necessary on all compilers
}