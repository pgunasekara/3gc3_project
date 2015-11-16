#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

void display();
void reshape(int h, int w);
void glutCallbacks();
void keyboard(char key, int x, int y);
void mouse(int btn, int state, int x, int y);
void motion(int x, int y);
void passive(int x, int y);
void special(int key, int x, int y);

/****************************GLUT CALLBACK FUNCTIONS****************************/
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutSwapBuffers();
}

void reshape(int h, int w)
{

}

void keyboard(char key, int x, int y)
{

}

void mouse(int btn, int state, int x, int y)
{

}

void motion(int x, int y)
{

}

void passive(int x, int y)
{

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
	glutPassiveFunc(passive);
	glutPassiveMotioNFunc(motion);


}
/****************************GLUT CALLBACK FUNCTIONS****************************/

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Project");
	glutCallbacks();
	glutMainLoop();
}