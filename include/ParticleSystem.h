//Particle.h
#ifndef __PARTICLE_H_
#define __PARTICLE_H_

typedef struct
{
	bool life;//is the particle currently active
	float lifeSpan;//lifespan of the particle
	float decay;//speed at which the particle will decay

	float r,g,b;//Colour, replace with texture information
	float x,y,z;//position of the particle

	float speedx, speedy, speedz;//speed of the particle about each axis
	float gravityx, gravityy, gravityz;//gravity values in each direction
}particle;

class ParticleSystem
{
	float xspeed, yspeed, slowdown;
	particle system[1000];//max number of particles is 1000
	
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