//Particle.h
#ifndef __PARTICLE_H_
#define __PARTICLE_H_

#define MAX_PARTICLES 1000;

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
	float ySpeed, slowdown;
	particle system[MAX_PARTICLES];//max number of particles is 1000
	
	int width;//width of texture
	int height;//height of texture
	int k;//max value for loadPPM
	GLubyte *image;//holds the texture for each particle

	ParticleSystem();
	~ParticleSystem();
	GLubyte *LoadPPM(char *filename, int *width, int *height, int*max);
	void drawParticles();
};

#endif
