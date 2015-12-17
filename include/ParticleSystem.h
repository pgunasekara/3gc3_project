//ParticleSystem.h
//This Partcle System was created with the help of a tutorial from NeHe
//http://nehe.gamedev.net/tutorial/particle_engine_using_triangle_strips/21001/
#ifndef __PARTICLE_H_//Guard against cyclical dependancies
#define __PARTICLE_H_

//GLUT and OpenGL includes
#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#define MAX_PARTICLES 2000//the maximum amount of particles being rendered

typedef struct
{
	bool life;//is the particle currently active
	float lifeSpan;//lifespan of the particle
	float decay;//speed at which the particle will decay

	float r,g,b;//Colour, replace with texture information
	float x,y,z;//position of the particle

	float speed;//speed of the particle about each axis
	float gravity;//gravity value of the particle
}particle;

class ParticleSystem
{
public:
	float ySpeed, slowdown;
	particle system[MAX_PARTICLES];//max number of particles is 1000
	
	int width;//width of texture
	int height;//height of texture
	int k;//max value for loadPPM
	GLubyte *image;//holds the texture for each particle

	ParticleSystem();//default constructor
	~ParticleSystem();//destructor
	//GLubyte *LoadPPM(char *filename, int *width, int *height, int*max);//loading textures
	void drawParticles();//draw particles
	void initialize(int i);//initialize a particle and set it's position
	void drawRainParticles();//Draw lines for rain particles
};

#endif
