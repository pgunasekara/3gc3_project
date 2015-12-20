#include <vector>
#include "Mesh3D.h"

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
#include <time.h> // diff colours
#include <string>
#include <fstream>
#include <iostream>
#include "splitter.h"
#include <stdlib.h>
#include "Hitbox.h"

using namespace std;

Mesh3D::Mesh3D(int type){
	vector<vertex3D> verts;
	vector<faceVertNorms3D> faceVertNorms;
	vector<vertNorms3D> vertNorms;
	vector<faces3D> faces;
	vector<colour3D> colours;

	this->type = type;

	hedgeTexture = LoadPPM("src/hedge_ascii.ppm", &widthh, &heightg, &kh);
	textures[0] = *hedgeTexture;
	groundTexture = LoadPPM("src/ground.ppm", &widthg, &heightg, &kg);
	textures[1] = *groundTexture;
	
	glGenTextures(2, textures);
}

Mesh3D::~Mesh3D(){
	for (int i = 0; i < faces.size(); i++){
		if (faces[i].lHit){
			delete faces[i].lHit;
			delete faces[i].rHit;
		}
	}
	faces.clear();
	verts.clear();
	faceVertNorms.clear();
	vertNorms.clear();
}

GLubyte* Mesh3D::LoadPPM(char* file, int* width, int* height, int* max)
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

void Mesh3D::drawMesh(){
	if (verts.size() == 0){
		cout << "vertex size is zero";
		return;
	}

	float red[] = {1,0,0};
	float green[] = {0,1,0};
	float blue[] = {0,0,1};
	float lightBlue[] = {0,1,1};
	float purple[] = {1,0,1};
	float yellow[] = {1,1,0};
	float black[] = {0,0,0};


	int vertexIndex1, vertexNormIndex1;
	int vertexIndex2, vertexNormIndex2;
	int vertexIndex3, vertexNormIndex3;
	int vertexIndex4, vertexNormIndex4;


	int count = 0;


	for (int i= 0; i<faces.size();i++){

		 vertexIndex1 = faces[i].v1 -1;
		 vertexIndex2 = faces[i].v2 -1;
		 vertexIndex3 = faces[i].v3 -1;
		 vertexIndex4 = faces[i].v4 -1;

		 vertexNormIndex1 = faceVertNorms[i].vn1;
		 vertexNormIndex2 = faceVertNorms[i].vn2;
		 vertexNormIndex3 = faceVertNorms[i].vn3;
		 vertexNormIndex4 = faceVertNorms[i].vn4;

		glBegin(GL_QUADS);



			switch (count){
				case 0:
					glColor3fv(red);
					break;
				case 1:
					glColor3fv(green);
					break;
				case 2:
					glColor3fv(blue);
					break;
				case 3:
					glColor3fv(lightBlue);
					break;
				case 4:
					glColor3fv(purple);
					break;
				case 5:
					glColor3fv(yellow);
					break;
				case 6:
					glColor3fv(black);
					break;
			}

		glEnd();


		/*//TEXTURING CODE
		if(type == 0)
			glBindTexture(GL_TEXTURE_2D, textures[0]);
		
		else
			glBindTexture(GL_TEXTURE_2D, textures[1]);


		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		if(type == 0)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthh, heighth, 0, GL_RGB,GL_UNSIGNED_BYTE, hedgeTexture); 
		
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthg, heightg, 0, GL_RGB,GL_UNSIGNED_BYTE, groundTexture);
*/
		glBegin(GL_QUADS);

			glPushMatrix();
<<<<<<< HEAD
			glTexCoord2f(0, 0);
			glNormal3f(vertNorms[vertexNormIndex1].x,vertNorms[vertexNormIndex1].y, vertNorms[vertexNormIndex1].z);
			glVertex3f(verts[vertexIndex1].x,verts[vertexIndex1].y,verts[vertexIndex1].z);
			glTexCoord2f(0, 1);
			glNormal3f(vertNorms[vertexNormIndex2].x,vertNorms[vertexNormIndex2].y, vertNorms[vertexNormIndex2].z);
			glVertex3f(verts[vertexIndex2].x,verts[vertexIndex2].y,verts[vertexIndex2].z);
			glTexCoord2f(1, 1);
			glNormal3f(vertNorms[vertexNormIndex3].x,vertNorms[vertexNormIndex3].y, vertNorms[vertexNormIndex3].z);
			glVertex3f(verts[vertexIndex3].x,verts[vertexIndex3].y,verts[vertexIndex3].z);
			glTexCoord2f(1, 0);
=======
			//glTexCoord2f(vertexTexture.at(faceTexCoords.at(i).v1- 1).a, vertexTexture.at(faceTexCoords.at(i).v1- 1).b);
			//glTexCoord2f(0, 0);
			glNormal3f(vertNorms[vertexNormIndex1].x,vertNorms[vertexNormIndex1].y, vertNorms[vertexNormIndex1].z);
			glVertex3f(verts[vertexIndex1].x,verts[vertexIndex1].y,verts[vertexIndex1].z);

			//glTexCoord2f(vertexTexture.at(faceTexCoords.at(i).v2- 1).a, vertexTexture.at(faceTexCoords.at(i).v2- 1).b);
			//glTexCoord2f(0, 1);
			glNormal3f(vertNorms[vertexNormIndex2].x,vertNorms[vertexNormIndex2].y, vertNorms[vertexNormIndex2].z);
			glVertex3f(verts[vertexIndex2].x,verts[vertexIndex2].y,verts[vertexIndex2].z);
			
			//glTexCoord2f(vertexTexture.at(faceTexCoords.at(i).v3- 1).a, vertexTexture.at(faceTexCoords.at(i).v3- 1).b);
			//glTexCoord2f(1, 1);
			glNormal3f(vertNorms[vertexNormIndex3].x,vertNorms[vertexNormIndex3].y, vertNorms[vertexNormIndex3].z);
			glVertex3f(verts[vertexIndex3].x,verts[vertexIndex3].y,verts[vertexIndex3].z);

			//glTexCoord2f(vertexTexture.at(faceTexCoords.at(i).v4- 1).a, vertexTexture.at(faceTexCoords.at(i).v4- 1).b);
			//glTexCoord2f(1, 0);
>>>>>>> 1abac92569fe85c574049b3c6da30e5548d4d24e
			glNormal3f(vertNorms[vertexNormIndex4].x,vertNorms[vertexNormIndex4].y, vertNorms[vertexNormIndex4].z);
			glVertex3f(verts[vertexIndex4].x,verts[vertexIndex4].y,verts[vertexIndex4].z);

			glPopMatrix();

		glEnd();
	}

}

void Mesh3D::loadObj(char* filename){

	srand(time(NULL));

	string fileString;
	string character;
	vector<string> lines;
	vector<string> vertLine;
	vector<string> faceLine;
	vector<string> vertNormsLine;
	vector<string> vertexElements1;
	vector<string> vertexElements2;
	vector<string> vertexElements3;
	vector<string> vertexElements4;

	vector<string> texElements1;


	ifstream infile;
	infile.open(filename);

	if (infile.is_open()){
		while (!infile.eof()){

			character = infile.get();
			fileString += character;
		}
	}

	infile.close();

	lines = split(fileString, '\n');

	for (int i =0; i < lines.size(); i++){
		if (lines[i].substr(0,1) == "v"){
			vertLine = split(lines[i],' ');
			vertex3D v;
			v.x = (float) atof(vertLine[1].c_str());  //atof is converting string to float
			v.y = (float) atof(vertLine[2].c_str());
			v.z = (float) atof(vertLine[3].c_str());

			verts.push_back(v);

		}

		if (lines[i].substr(0,2) == "vn"){
			vertNormsLine = split(lines[i],' ');
			vertNorms3D vn;
			vn.x = (float) atof(vertNormsLine[1].c_str());  //atof is converting string to float
			vn.y = (float) atof(vertNormsLine[2].c_str());
			vn.z = (float) atof(vertNormsLine[3].c_str());

			vertNorms.push_back(vn);

		}
		if (lines[i].substr(0,1) == "f"){
			faceLine =  split(lines[i],' ');

			if (faceLine.size() == 5){
				faces3D f;
				faceVertNorms3D fvn;
				faces3D ft;
				vertexElements1 = split(faceLine[1],'/');
				vertexElements2 = split(faceLine[2],'/');
				vertexElements3 = split(faceLine[3],'/');
				vertexElements4 = split(faceLine[4],'/');

				f.v1 = atoi(vertexElements1[0].c_str());		//atoi is string to int
				f.v2 = atoi(vertexElements2[0].c_str());
				f.v3 = atoi(vertexElements3[0].c_str());
				f.v4 = atoi(vertexElements4[0].c_str());
				// remember to clear memory
				if (verts[f.v1-1].x == verts[f.v2-1].x && verts[f.v1-1].z != verts[f.v2-1].z && verts[f.v1-1].y != verts[f.v3-1].y){
					f.lHit = new Plane(verts[f.v1-1],verts[f.v2-1],verts[f.v3-1],verts[f.v4-1],false,true,true,true);
					f.rHit = new Plane(verts[f.v1-1],verts[f.v2-1],verts[f.v3-1],verts[f.v4-1],false,true,true,false);
				}else if (verts[f.v1-1].x != verts[f.v2-1].x && verts[f.v1-1].z == verts[f.v2-1].z && verts[f.v1-1].y != verts[f.v3-1].y){
					f.lHit = new Plane(verts[f.v1-1],verts[f.v2-1],verts[f.v3-1],verts[f.v4-1],true,true,false,true);
					f.rHit = new Plane(verts[f.v1-1],verts[f.v2-1],verts[f.v3-1],verts[f.v4-1],true,true,false,false);
				}else if (verts[f.v1-1].x != verts[f.v2-1].x && verts[f.v1-1].z != verts[f.v3-1].z && verts[f.v1-1].y == verts[f.v3-1].y){
					f.lHit = new Plane(verts[f.v1-1],verts[f.v2-1],verts[f.v3-1],verts[f.v4-1],true,false,true,true);
					f.rHit = new Plane(verts[f.v1-1],verts[f.v2-1],verts[f.v3-1],verts[f.v4-1],true,false,true,false);
				}else{
					f.lHit = new Plane(verts[f.v1-1],verts[f.v2-1],verts[f.v3-1],verts[f.v4-1],true,false,true,true);
					f.rHit = new Plane(verts[f.v1-1],verts[f.v2-1],verts[f.v3-1],verts[f.v4-1],true,false,true,false);
				}

				fvn.vn1 = atoi(vertexElements1[2].c_str());		//atoi is string to int
				fvn.vn2 = atoi(vertexElements2[2].c_str());
				fvn.vn3 = atoi(vertexElements3[2].c_str());
				fvn.vn4 = atoi(vertexElements4[2].c_str());

				ft.v1 = atoi(vertexElements1[1].c_str());		//atoi is string to int
				ft.v2 = atoi(vertexElements2[1].c_str());
				ft.v3 = atoi(vertexElements3[1].c_str());
				ft.v4 = atoi(vertexElements4[1].c_str());

				faces.push_back(f);
				faceVertNorms.push_back(fvn);
				faceTexCoords.push_back(ft);
			}

		}

		if(lines[i].substr(0,2) == "vt")
		{
			texElements1 = split(lines[i], ' ');
			textureCoord v;
			v.a = (float) atof(texElements1[1].c_str());  //atof is converting string to float
			v.b = (float) atof(texElements1[2].c_str());

			vertexTexture.push_back(v);
		}

		else {}
	}
}
