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
#include "camera.h"
#include "Hitbox.h"
//#include "../lib/SceneGraph/structs.h"
#include "math3D.h"

//sceneGraph
//#include "../lib/SceneGraph/sceneGraph.h"
//#include "../lib/SceneGraph/nodeGroup.h"
//#include "../lib/SceneGraph/nodeModel.h"
//#include "../lib/SceneGraph/nodeTransform.h"
#include "Mesh3D.h"
#include "ParticleSystem.h"
#include <vector>

using namespace std;
// global variables for scene graph and camera
float mouseX,mouseY,globalW,globalH;
bool buttonDown = false;
float pos[] = {23,1.0,-20};
float lookAt[] = {23,1.0,0};
float angle = 0.005f;
bool PlaneExist = false;
Camera camera;
bool moveable = false;

//node ids
int masterID = 0;

//Vector3D ip;
//Vector3D translation;
//Vector4D rotation;
//Vector3D scale;

int getID()
{
	return masterID++;
}
// for the unprojection
double* start = new double[3];
double* finish = new double[3];

ParticleSystem rain;

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


//SceneGraph *SG;

float light_pos[] = {0.0, 5.0, 0.0, 1.0};
float amb0[4] = {1, 1, 1, 1};
float diff0[4] = {1, 0, 0, 1};
float spec0[4] = {1, 1, 1, 1};

float m_amb[] = {0.33, 0.22, 0.03, 1.0};
float m_diff[] = {0.78, 0.57, 0.11, 1.0};
float m_spec[] = {0.99, 0.91, 0.81, 1.0};
float shiny = 27.8;
Mesh3D* test;


void display();
void reshape(int h, int w);
void glutCallbacks();
void keyboard(char key, int x, int y);
void mouse(int btn, int state, int x, int y);
void motion(int x, int y);
void passive(int x, int y);
void special(int key, int x, int y);

/*********************************INIT FUNCTION*********************************/
void initLighting()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);
}

//function which will populate a sample graph
/*
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
*/

void init(void)
{
	//GLuint id = 1;
	camera = Camera();
	rain = ParticleSystem();

	//init our scenegraph
	//SG = new SceneGraph();

	//add various nodes
	//initializing our world
	//initGraph();
}

/*********************************INIT FUNCTION*********************************/


/****************************GLUT CALLBACK FUNCTIONS****************************/
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//clear the screen
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);

	//optionally draw the axis
	glPushMatrix();
	glScalef(2.0,2.0,2.0);
	test->drawMesh();
	glPopMatrix();
	rain.drawRainParticles();
	//for (int i =0; i < test->faces.size();i++){
	//	test->faces[i].lHit->draw();
	//	test->faces[i].rHit->draw();
	//}

	//swap buffers - rendering is done to the back buffer, bring it forward to display
	glutSwapBuffers();

	//force a redisplay, to keep the animation running
	glutPostRedisplay();
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
	gluLookAt(pos[0], pos[1], pos[2], lookAt[0], lookAt[1], lookAt[2], 0, 1, 0);
	//gluLookAt(50, 50, 50, 0, 0, 0, 0, 1, 0);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'W':
			camera.Move(FORWARD,test);
			glutPostRedisplay();
			break;
		case 'A':
			camera.Move(LEFT,test);
			glutPostRedisplay();
			break;
		case 'S':
			camera.Move(BACK,test);
			glutPostRedisplay();
			break;
		case 'D':
			camera.Move(RIGHT,test);
			glutPostRedisplay();
			break;
		case 'q':
		case 27:
			delete start;
			delete finish;
			delete test;
			exit (0);
			break;
		}
}

void mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !moveable){
		mouseX = x;
		mouseY = y;
		moveable = true;
	}else if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN && moveable){
		moveable = false;
	}
}

void motion(int x, int y)
{

}

void passive(int x,int y){
	if (moveable){
		if ((x - mouseX) > 0){
			camera.Spin(SRIGHT,4.0*(x-mouseX)/globalW);
			mouseX = x;
		}
		else if ((x - mouseX) < 0){
			camera.Spin(SLEFT,-4.0*(x-mouseX)/globalW);
			mouseX = x;
		}
		if ((y - mouseY) > 0){
			camera.Spin(SDOWN,4.0*(y-mouseY)/globalH);
			mouseY = y;
		}else if ((y - mouseY) < 0){
			camera.Spin(SUP,-4.0*(y-mouseY)/globalH);
			mouseY = y;
		}
		glutPostRedisplay();
	}
}

void special(int key, int x, int y)
{

}

void glutCallbacks()
{
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(passive);
}
/****************************GLUT CALLBACK FUNCTIONS****************************/

int main(int argc, char **argv)
{
	//glut initialization stuff:
	// set the window size, display mode, and create the window
	glutInit(&argc, argv);
	glutInitWindowSize(1200, 1200);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	init();
	glutCreateWindow("Spinning Cube");

	//enable Z buffer test, otherwise things appear in the order they're drawn
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	//Enable Blending for water particles
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );


	//register glut callbacks
	glutCallbacks();
	initLighting();

	test = new Mesh3D();
	test->loadObj("src/map.obj");
	//start the program!
	glutMainLoop();

	return 0;
}
