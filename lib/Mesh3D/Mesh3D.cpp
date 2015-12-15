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

Mesh3D::Mesh3D(){
	vector<vertex3D> verts;
	vector<faceVertNorms3D> faceVertNorms;
	vector<vertNorms3D> vertNorms;
	vector<faces3D> faces;
	vector<colour3D> colours;
}

Mesh3D::~Mesh3D(){
	for (int i = 0; i < faces.size(); i++){
		delete faces[i].hit;
	}
	faces.clear();
	verts.clear();
	faceVertNorms.clear();
	vertNorms.clear();
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

		glBegin(GL_QUADS);

			glPushMatrix();
			glNormal3f(vertNorms[vertexNormIndex1].x,vertNorms[vertexNormIndex1].y, vertNorms[vertexNormIndex1].z);
			glVertex3f(verts[vertexIndex1].x,verts[vertexIndex1].y,verts[vertexIndex1].z);
			glNormal3f(vertNorms[vertexNormIndex2].x,vertNorms[vertexNormIndex2].y, vertNorms[vertexNormIndex2].z);
			glVertex3f(verts[vertexIndex2].x,verts[vertexIndex2].y,verts[vertexIndex2].z);
			glNormal3f(vertNorms[vertexNormIndex3].x,vertNorms[vertexNormIndex3].y, vertNorms[vertexNormIndex3].z);
			glVertex3f(verts[vertexIndex3].x,verts[vertexIndex3].y,verts[vertexIndex3].z);
			glNormal3f(vertNorms[vertexNormIndex4].x,vertNorms[vertexNormIndex4].y, vertNorms[vertexNormIndex4].z);
			glVertex3f(verts[vertexIndex4].x,verts[vertexIndex4].y,verts[vertexIndex4].z);

			glPopMatrix();

		glEnd();

	}

}

void Mesh3D::load(){

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

				vertexElements1 = split(faceLine[1],'/');
				vertexElements2 = split(faceLine[2],'/');
				vertexElements3 = split(faceLine[3],'/');
				vertexElements4 = split(faceLine[4],'/');

				f.v1 = atoi(vertexElements1[0].c_str());		//atoi is string to int
				f.v2 = atoi(vertexElements2[0].c_str());
				f.v3 = atoi(vertexElements3[0].c_str());
				f.v4 = atoi(vertexElements4[0].c_str());
				// remember to clear memory
				//printf("v1: %f %f %f\n",verts[f.v1-1].x,verts[f.v1-1].y,verts[f.v1-1].z);
				//printf("v2: %f %f %f\n",verts[f.v2-1].x,verts[f.v2-1].y,verts[f.v2-1].z);
				//printf("v3: %f %f %f\n",verts[f.v3-1].x,verts[f.v3-1].y,verts[f.v3-1].z);
				//printf("v4: %f %f %f\n",verts[f.v4-1].x,verts[f.v4-1].y,verts[f.v4-1].z);
				if (verts[f.v1-1].x == verts[f.v2-1].x && verts[f.v1-1].z != verts[f.v2-1].z){
					f.hit = new Plane(verts[f.v1-1],verts[f.v2-1],verts[f.v3-1],verts[f.v4-1],false,true,true);
				}else if (verts[f.v1-1].x != verts[f.v2-1].x && verts[f.v1-1].z == verts[f.v2-1].z){
					f.hit = new Plane(verts[f.v1-1],verts[f.v2-1],verts[f.v3-1],verts[f.v4-1],true,true,false);
				}else{
					printf("It's not a xy or a yz plane?\n");
				}

				fvn.vn1 = atoi(vertexElements1[2].c_str());		//atoi is string to int
				fvn.vn2 = atoi(vertexElements2[2].c_str());
				fvn.vn3 = atoi(vertexElements3[2].c_str());
				fvn.vn4 = atoi(vertexElements4[2].c_str());
				
				faces.push_back(f);
				faceVertNorms.push_back(fvn);
			}	

		}
		else {}
	}
	infile.close();
}