//ParticleSystem.cpp
//This Partcle System was created with the help of a tutorial from NeHe
//http://nehe.gamedev.net/tutorial/particle_engine_using_triangle_strips/21001/
//GLUT and openGL includes
#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include "ParticleSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctime>

ParticleSystem::ParticleSystem()
{
	width = 0;
	height = 0;
	ySpeed = 0.0f;
	slowdown = 2.0f;
	srand((unsigned)(time(0)));
	//image = this->LoadPPM("texture.ppm", &width, &height, &k);
	//Initialize all the particles
	for(int i = 0; i < MAX_PARTICLES; i++)
	{
		this->initialize(i);
	}
}

ParticleSystem::~ParticleSystem(){}

void ParticleSystem::initialize(int i)
{
	system[i].life = true;//set all initial particles to be alive
	system[i].lifeSpan = 1.0f;//all particles will have a lifespan between 0 and 1
	system[i].decay = float(rand()%100)/1000.0f+0.005f;//Random decay for every drop

	system[i].x =  (30+30) * (float)rand()/(float)RAND_MAX - 30;///Pick any point between 0 and 50 for the x axis
	system[i].y = 7.0f;//Every particle will start at a constant height
	system[i].z =  (30+30) * (float)rand()/(float)RAND_MAX - 30;///pick any point on the z axis
	system[i].gravity = -1.8f;//the drops will fall at random speeds
	system[i].speed = ySpeed;
}

void ParticleSystem::drawRainParticles()
{
	float amb[4] = {0.1f, 0.18725f, 0.1745f, 1.0f};
	float diff[4] = {0.396f, 0.74151f, 0.69102f, 1.0f};
	float spec[4] = {0.297254f, 0.30829f, 0.306678f, 1.0f};
	float shine = 0.1f;
	//Make slightly thicker lines
	glLineWidth(1.5f);
	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	glMaterialf(GL_FRONT, GL_SHININESS, shine * 128.0);

	for(int i = 0; i < MAX_PARTICLES; i++)
	{
		//Draw and update if current particle is alive
		if(system[i].life)
		{
			glColor3f(0.0f, 0.0f, 1.0f);//Sample colour, until texturing is worked out
			glBegin(GL_LINES);
				glVertex3f(system[i].x, system[i].y, system[i].z);
				glVertex3f(system[i].x, system[i].y+0.7f, system[i].z);
			glEnd();

			//Move the particles after drawing them
			system[i].y += system[i].speed/(slowdown*500);
			system[i].speed += system[i].gravity;
			system[i].lifeSpan -= system[i].decay;

			//remove particles that are offscreen
			/*if(system[i].y < -5.0f)
			{
				system[i].lifeSpan = -0.5f;
			}*/

			if(system[i].lifeSpan < 0.0f)
			{
				//restore the particles that were removed
				this->initialize(i);
			}
		}
	}
	glPopMatrix();
}
