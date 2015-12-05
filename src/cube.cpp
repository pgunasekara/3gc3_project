
/* cube.c by R. Teather
 *  Simple "hello world" equivalent 3D graphics program
 *  Draws a spinning cube centered at the origin
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

#include "Mesh3D.h"

int ang = 0;	//angle for rotating cube
int cnt = 0;
float eye[] = {30,30,30};


float light_pos[] = {0.0, 5.0, 0.0, 1.0};


float amb0[4] = {1, 1, 1, 1};
float diff0[4] = {1, 0, 0, 1};
float spec0[4] = {1, 1, 1, 1};

float m_amb[] = {0.33, 0.22, 0.03, 1.0};
float m_diff[] = {0.78, 0.57, 0.11, 1.0};
float m_spec[] = {0.99, 0.91, 0.81, 1.0};
float shiny = 27.8; 
Mesh3D test;
void display()
{
	 
	


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye[0],eye[1],eye[2],0,0,0,0,1,0);

	//increment the rotation angle every 100th display cycle

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
	// drawAxis();

	test.drawMesh();

	


	
	//swap buffers - rendering is done to the back buffer, bring it forward to display
	glutSwapBuffers();

	//force a redisplay, to keep the animation running
	glutPostRedisplay();
}

/* kbd -- the GLUT keyboard function 
 *  key -- the key pressed
 *  x and y - mouse x and y coordinates at the time the function is called
 */
void kbd(unsigned char key, int x, int y)
{
	//if the "q" key is pressed, quit the program
	if(key == 'q' || key == 'Q')
	{
		exit(0);
	}
}

void special(int key, int x, int y){

	switch(key){

		case GLUT_KEY_LEFT:
			eye[2]-=0.5;
			break;
		case GLUT_KEY_RIGHT:
			eye[2]+=0.5;
			break; 
		case GLUT_KEY_UP:
			eye[0]+=0.5; eye[1]+=0.5; eye[2]+=0.5;
			break; 
		case GLUT_KEY_DOWN:
			eye[0]-=0.5; eye[1]-=0.5; eye[2]-=0.5;
			break; 
	}
}

int main(int argc, char** argv)
{


	//glut initialization stuff:
	// set the window size, display mode, and create the window
	glutInit(&argc, argv);
	glutInitWindowSize(600, 600);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Spinning Cube");

	//enable Z buffer test, otherwise things appear in the order they're drawn
	glEnable(GL_DEPTH_TEST);

	//setup the initial view
	// change to projection matrix mode, set the extents of our viewing volume
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-2.5, 2.5, -2.5, 2.5, -2.5, 2.5);
	gluPerspective(45,1,1,200);
	

	//set clear colour to white
	glClearColor(1, 1, 1, 0);

	glMatrixMode(GL_MODELVIEW);
	//look down from a 45 deg. angle
	glRotatef(45, 1, 0, 0);


	//register glut callbacks for keyboard and display function
	glutKeyboardFunc(kbd);
	glutSpecialFunc(special);
	glutDisplayFunc(display);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	test = Mesh3D();
	test.loadObj("map.obj");


	//start the program!
	glutMainLoop();

	return 0;
}
