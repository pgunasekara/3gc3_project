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
//#include "SOIL.h"

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
float pos[] = {34.5,1.5,-30};
float *light_pos_tmp, *spot_direction;
float lookAt[] = {34.5,1.5,0};
float angle = 0.005f;
bool PlaneExist = false;
Camera* camera;
bool moveable = true;

//Texture information
int widthh=0, heighth=0, widthg=0, heightg=0, kh, kg;
//unsigned char *hedgeTexture;
GLubyte *hedgeTexture;
GLubyte *groundTexture;
//GLuint textureID[1];
GLuint textures[2];

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


//-------------------------------TEXTURE CODE-----------------------------------//
GLubyte* LoadPPM(char* file, int* width, int* height, int* max)
{
	GLubyte* img;
	FILE *fd;
	int n, m;
	int  k, nm;
	char c;
	int i;
	char b[100];
	float s;
	int red, green, blue;
	
	/* first open file and check if it's an ASCII PPM (indicated by P3 at the start) */
	fd = fopen(file, "r");
	fscanf(fd,"%[^\n] ",b);
	if(b[0]!='P'|| b[1] != '3')
	{
		printf("%s is not a PPM file!\n",file); 
		exit(0);
	}
	printf("%s is a PPM file\n", file);
	fscanf(fd, "%c",&c);

	/* next, skip past the comments - any line starting with #*/
	while(c == '#') 
	{
		fscanf(fd, "%[^\n] ", b);
		printf("%s\n",b);
		fscanf(fd, "%c",&c);
	}
	ungetc(c,fd); 

	/* now get the dimensions and max colour value from the image */
	fscanf(fd, "%d %d %d", &n, &m, &k);

	printf("%d rows  %d columns  max value= %d\n",n,m,k);

	/* calculate number of pixels and allocate storage for this */
	nm = n*m;
	img = (GLubyte*)malloc(3*sizeof(GLuint)*nm);
	s=255.0/k;

	/* for every pixel, grab the read green and blue values, storing them in the image data array */
	for(i=0;i<nm;i++) 
	{
		fscanf(fd,"%d %d %d",&red, &green, &blue );
		img[3*nm-3*i-3]=red*s;
		img[3*nm-3*i-2]=green*s;
		img[3*nm-3*i-1]=blue*s;
	}

	/* finally, set the "return parameters" (width, height, max) and return the image array */
	*width = n;
	*height = m;
	*max = k;

	return img;
}

void loadTextures()
{
	GLuint id = 1;
	
	hedgeTexture = LoadPPM("src/hedge_ascii.ppm", &widthh, &heightg, &kh);
	//Setup hedge texture

	groundTexture = LoadPPM("src/ground.ppm", &widthg, &heightg, &kg);
	
	textures[0] = *hedgeTexture;
	textures[1] = *groundTexture;
	glGenTextures(2, textures);
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
Mesh3D* maze;
Mesh3D* groundPlane;


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

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb0);

	glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,50.0f);
	glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,120.0f);

	light_pos_tmp = camera->camera_position.returnArray4L();
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos_tmp);

	spot_direction = camera->spot_direction.returnArray();
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION,spot_direction);

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

void initFog()
{
	glEnable(GL_FOG);
	float FogCol[3]={0.8f,0.8f,0.8f}; // Define a nice light grey
	glFogfv(GL_FOG_COLOR,FogCol);     // Set the fog color
	glFogi(GL_FOG_MODE, GL_LINEAR); // Note the 'i' after glFog - the GL_LINEAR constant is an integer.
	glFogf(GL_FOG_START, 2.f);
	glFogf(GL_FOG_END, 40.f);
}

void init(void)
{
	//GLuint id = 1;
	camera = new Camera();
	rain = ParticleSystem();
	glEnable(GL_TEXTURE_2D);
	loadTextures();

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

	delete light_pos_tmp;
	light_pos_tmp = camera->camera_position.returnArray4L();
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos_tmp);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);

	//optionally draw the axis
	glPushMatrix();
	glScalef(3.0,3.0,3.0);
	//test->drawMesh();
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	//set texture parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//Create texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthh, heighth, 0, GL_RGB, GL_UNSIGNED_BYTE, hedgeTexture);
	
	maze->drawMesh();
		
		glPushMatrix();
		//glTranslatef(-5,0,0);
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		//set texture parameters
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//Create texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthg, heightg, 0, GL_RGB, GL_UNSIGNED_BYTE, groundTexture);
		
		groundPlane->drawMesh();
		glPopMatrix();
	
	glPopMatrix();
	rain.drawRainParticles();
	/*for (int i =0; i < test->faces.size();i++){
		test->faces[i].lHit->draw();
		test->faces[i].rHit->draw();
	}*/

	for (int i =0; i < maze->faces.size();i++){
		maze->faces[i].lHit->draw();
		maze->faces[i].rHit->draw();
	}
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
			camera->Move(FORWARD,maze);
			glutPostRedisplay();
			break;
		case 'A':
			camera->Move(LEFT,maze);
			glutPostRedisplay();
			break;
		case 'S':
			camera->Move(BACK,maze);
			glutPostRedisplay();
			break;
		case 'D':
			camera->Move(RIGHT,maze);
			glutPostRedisplay();
			break;
		case 'q':
		case 27:
			delete start;
			delete finish;
			delete maze;
			delete camera;
			delete light_pos_tmp;
			delete spot_direction;
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
			camera->Spin(SRIGHT,4.0*(x-mouseX)/globalW);
			mouseX = x;
		}
		else if ((x - mouseX) < 0){
			camera->Spin(SLEFT,-4.0*(x-mouseX)/globalW);
			mouseX = x;
		}
		if ((y - mouseY) > 0){
			camera->Spin(SDOWN,4.0*(y-mouseY)/globalH);
			mouseY = y;
		}else if ((y - mouseY) < 0){
			camera->Spin(SUP,-4.0*(y-mouseY)/globalH);
			mouseY = y;
		}
	}
	glutPostRedisplay();
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
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	//Enable Blending for water particles
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );

	//register glut callbacks
	glutCallbacks();
	initLighting();




	//test = new Mesh3D();
	//test->loadObj("src/map.obj");
	maze = new Mesh3D();
	maze->loadObj("src/maze_2.obj");
	groundPlane = new Mesh3D();
	groundPlane->loadObj("src/groundPlane.obj");
	//start the program!



	glutMainLoop();

	return 0;
}
