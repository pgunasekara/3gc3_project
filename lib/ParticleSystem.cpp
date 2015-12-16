#include "ParticleSystem.h"
#include <stdio.h>

ParticleSystem::ParticleSystem()
{
	width = 0;
	height = 0;
	ySpeed = 0.0f;
	slowdown = 2.0f;
	//image = this->LoadPPM("texture.ppm", &width, &height, &k);
	this->initialize();
}

ParticleSystem::~ParticleSystem(){}

ParticleSystem::initialize()
{
	//Initialize all the particles
	for(int i = 0; i < MAX_PARTICLES; i++)
	{
		system[i].life = true;//set all initial particles to be alive
		system[i].lifeSpan = 1.0f;//all particles will have a lifespan between 0 and 1
		system[i].decay = float(rand()%100)/1000f+0.005f;//Random decay for every drop
		
		system[i].x = float((rand()%(12+12))-12);///Pick any point between 0 and 50 for the x axis
		system[i].y = 7.0f;//Every particle will start at a constant height
		system[i].z = float((rand()%(12+12))-12);///pick any point on the z axis
		system[i].gravity = -9.8f//the drops will fall at random speeds
	}

}

void ParticleSystem::drawParticles()
{
	for(int i = 0; i < MAX_PARTICLES; i++)
	{
		//Draw if current particle is alive
		if(system[i].life)
		{
			glColor3f(0.0f, 0.0f, 1.0f);//Sample colour, until texturing is worked out
			glBegin();
				//
			glEnd();
		}

	}
}

//LoadPPM function taken from avenue, written Thomas Gwosdz
GLubyte Particle::*LoadPPM(char *filename, int *width, int *height, int*max)
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
	img = malloc(3*sizeof(GLuint)*nm);
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
