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
#include "algorithm"
#include <math.h>
#include "camera.h"
#include "Hitbox.h"

#include "math3D.h"

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
bool moveable = false;
GLubyte* img, *groundTex;
int height,width,maxAmount,height2,width2,max2;
GLuint textures[2];
double matModelView[16], matProjection[16];
int viewport[4];
vector<Hitbox*> hitBoxes;
// change these so they work with your camera position
vec3D near,far,distanceRay;
Plane entrance;
int BatteryLife = 70;
int batteryChangeCount = 0;
vector<int> batteriesAcquired;

//node ids
int masterID = 0;

int getID()
{
	return masterID++;
}
// for the unprojection
double* start = new double[3];
double* finish = new double[3];

ParticleSystem rain;

/*************************RAY PICKING***************************/
int Intersect(int x, int y){

	//grab the matricies
	glGetDoublev(GL_MODELVIEW_MATRIX, matModelView);
	glGetDoublev(GL_PROJECTION_MATRIX, matProjection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	start = camera->camera_position.returnDoubleArray();
	finish = camera->camera_look_at.returnDoubleArray();

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
	distanceRay = (far - near).normalize();

	int ID_tmp;
	for(int i = 0; i < hitBoxes.size(); i++)
	{
		ID_tmp = hitBoxes[i]->Intersect(near,distanceRay);
		if(ID_tmp != -1 && !(find(batteriesAcquired.begin(), batteriesAcquired.end(), ID_tmp) != batteriesAcquired.end()))
		{
			printf("You have found Batteries\n");
			return ID_tmp;
		}
	}
	printf("There is nothing here\n");
	return -1;

}



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
	//printf("%s is a PPM file\n", file);
	fscanf(fd, "%c",&c);

	/* next, skip past the comments - any line starting with #*/
	while(c == '#')
	{
		fscanf(fd, "%[^\n] ", b);
		//printf("%s\n",b);
		fscanf(fd, "%c",&c);
	}
	ungetc(c,fd);

	/* now get the dimensions and max colour value from the image */
	fscanf(fd, "%d %d %d", &n, &m, &k);

	//printf("%d rows  %d columns  max value= %d\n",n,m,k);

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

//SceneGraph *SG;

float light_pos[] = {0.0, 5.0, 0.0, 1.0};
float amb0[4] = {1, 1, 1, 1};
float diff0[4] = {1, 0, 0, 1};
float spec0[4] = {1, 1, 1, 1};

float m_amb[] = {0.33, 0.22, 0.03, 1.0};
float m_diff[] = {0.78, 0.57, 0.11, 1.0};
float m_spec[] = {0.99, 0.91, 0.81, 1.0};
float shiny = 27.8;
Mesh3D* test, *groundPlane,*keyObject;


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

	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb0);

	glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,BatteryLife);
	glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,30.0f);

	light_pos_tmp = camera->camera_position.returnArray4L();
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos_tmp);

	spot_direction = camera->spot_direction.returnArray();
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION,spot_direction);
	delete spot_direction;

	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);
}

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
	camera = new Camera();
	rain = ParticleSystem();

	glGenTextures(2,textures);
	img = LoadPPM("src/hedge_ascii.ppm",&width,&height,&maxAmount);

	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);

	groundTex = LoadPPM("src/ground.ppm",&width,&height,&maxAmount);
	glBindTexture(GL_TEXTURE_2D, textures[1]);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, groundTex);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/*********************************INIT FUNCTION*********************************/


/****************************GLUT CALLBACK FUNCTIONS****************************/
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,BatteryLife);

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
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		test->drawMesh();
			glPushMatrix();
				glBindTexture(GL_TEXTURE_2D, textures[1]);
				glTranslatef(0,0.3,0);
				groundPlane->drawMesh();
			glPopMatrix();
	glPopMatrix();

	for (int i =0; i < test->faces.size();i++){
		test->faces[i].lHit->draw();
		test->faces[i].rHit->draw();
	}

	if (!(find(batteriesAcquired.begin(), batteriesAcquired.end(), 1) != batteriesAcquired.end())){
		glPushMatrix();
			glTranslatef(34.5,0,-6);
			glutSolidCube(1);
		glPopMatrix();
	}
	// second key
	if (!(find(batteriesAcquired.begin(), batteriesAcquired.end(), 2) != batteriesAcquired.end())){
		glPushMatrix();
			glTranslatef(-34.5,0,-6);
			glutSolidCube(1);
		glPopMatrix();
	}
	//third key
	if (find(batteriesAcquired.begin(), batteriesAcquired.end(), 3) != batteriesAcquired.end()){
		glPushMatrix();
			glTranslatef(-18,0,24);
			glutSolidCube(1);
		glPopMatrix();
	}
	// fourth key
	if (find(batteriesAcquired.begin(), batteriesAcquired.end(), 4) != batteriesAcquired.end()){
		glPushMatrix();
			glTranslatef(0,0,0);
			glutSolidCube(1);
		glPopMatrix();
	}
	// fifth key
	if (find(batteriesAcquired.begin(), batteriesAcquired.end(), 5) != batteriesAcquired.end()){
		glPushMatrix();
			glTranslatef(-12.0,0,15);
			glutSolidCube(1);
		glPopMatrix();
	}
	rain.drawRainParticles();

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
	//gluLookAt(70, 70, 70, 0, 0, 0, 0, 1, 0);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'W':
			camera->Move(FORWARD,test);
			batteryChangeCount++;
			break;
		case 'A':
			camera->Move(LEFT,test);
			batteryChangeCount++;
			break;
		case 'S':
			camera->Move(BACK,test);
			batteryChangeCount++;
			break;
		case 'D':
			camera->Move(RIGHT,test);
			batteryChangeCount++;
			break;
		case 'k':
		case 'K':
			//printf("You have %i keys\n",keyCount);
			break;
		case 'q':
		case 27:
			delete start;
			delete finish;
			delete test;
			delete camera;
			delete light_pos_tmp;
			exit (0);
			break;
		}
		if (batteryChangeCount > 30){
			BatteryLife -= 5;
			batteryChangeCount = 0;
		}
		if (fabs(camera->camera_position.x) > 30.0 && fabs(camera->camera_position.z) > 35.5f){
			exit(0);
		}
		glutPostRedisplay();
}

void mouse(int btn, int state, int x, int y)
{
	int id;
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !moveable){
		mouseX = x;
		mouseY = y;
		moveable = true;
	}else if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN && moveable){
		moveable = false;
	}else if (btn==GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		id = Intersect(x,y);
		if (id != -1){
			batteriesAcquired.push_back(id);
			BatteryLife = 70;
		}
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
	glutCreateWindow("COMP SCI 3GC3 Final Project");
	cout << "\nCOMP SCI 3GC3 Final Project\n\n"
		<< "Group: \n\tPasindu Gunasekara\n\t\tgunasepi\n\t\t1412155"
		<< "\n\tRoberto Temelkovski\n\t\ttemelkr\n\t\t1418731"
		<< "\n\tColin Gillespie\n\t\tgillescj\n\t\t1330655"
		<< "\n\tPeter Kalnoki\n\t\tkalnokp\n\t\t1160986"
		<< "\n\nInstructions:\n"
		<< "Use the mouse to rotate the camera around\n\tLEFT CLICK to stop the camera from moving, \n\tuse this to move the mouse pointer to a more comfortable position."
		<< "\n(UPPERCASE)W A S D: Translate the camera, move forwards, backwards, left, and right."
		<< "\nRIGHT CLICK: Ray picking, for picking up keys.\n"
		<< "(LOWERCASE) q: quit.\n";

	//enable Z buffer test, otherwise things appear in the order they're drawn
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	init();

	//register glut callbacks
	glutCallbacks();
	initLighting();
	//entrance = Plane();
	hitBoxes.push_back(new Hitbox(vert3D(-0.5,-0.5,-0.5),vert3D(0.5,0.5,0.5),1));
	hitBoxes[0]->Translate(vec3D(34.5,0,-6));
	hitBoxes.push_back(new Hitbox(vert3D(-0.5,-0.5,-0.5),vert3D(0.5,0.5,0.5),2));
	hitBoxes[1]->Translate(vec3D(-34.5,0,-6));
	hitBoxes.push_back(new Hitbox(vert3D(-0.5,-0.5,-0.5),vert3D(0.5,0.5,0.5),3));
	hitBoxes[2]->Translate(vec3D(-18,0,24));
	hitBoxes.push_back(new Hitbox(vert3D(-0.5,-0.5,-0.5),vert3D(0.5,0.5,0.5),4));
	hitBoxes[3]->Translate(vec3D(0,0,0));
	hitBoxes.push_back(new Hitbox(vert3D(-0.5,-0.5,-0.5),vert3D(0.5,0.5,0.5),5));
	hitBoxes[4]->Translate(vec3D(-12.0,0,15));

	test = new Mesh3D(1);
	test->loadObj("src/maze_2.obj");
	groundPlane = new Mesh3D(0);
	groundPlane->loadObj("src/groundPlane_new.obj");

	//start the program!
	glutMainLoop();

	return 0;
}
