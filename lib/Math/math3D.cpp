#include <cmath>
#include <sstream>
#include <fstream>
#include "math3D.h"
#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

/* VECTORS */

// general case
vec3D::vec3D(){
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

// specify components
vec3D::vec3D(float x, float y, float z){
	this->x = x;
	this->y = y;
	this->z = z;
}

vec3D::vec3D(vert3D p1, vert3D p2){
	x = p2.x - p1.x;
	y = p2.y - p1.y;
	z = p2.z - p1.z;
}

vec3D::~vec3D(){

}

vec3D vec3D::vectorMultiplyr(float s){
	return vec3D(x*s,y*s,z*s);
}

vec3D vec3D::normalize() {
	float magnitude = sqrt(pow(x,2) + pow(y,2) + pow(z,2));

	return vec3D(x/magnitude,y/magnitude,z/magnitude);
};

vec3D vec3D::cross(vec3D other) {
	// cross product of vectors
	float cx = y*other.z - z*other.y;
	float cy = z*other.x - x*other.z;
	float cz = x*other.y - y*other.x;

	return vec3D(cx,cy,cz);
};

float vec3D::dot(vec3D other) {
	// dot product of vectors
	return x*other.x + y*other.y + z*other.z;
};

void vec3D::flipVector(){
	x *= -1;
	y *- -1;
	z *= -1;
}

vec3D vec3D::flipVectorR(){
// make note of -0
	return vec3D(-1*x,-1*y,-1*z);
}

void vec3D::reset(){
	x=0;
	y=0;
	z=0;
}

float* vec3D::returnArray(){
	// deallocate pls
	float *vert = new float[3];

	vert[0] = static_cast<float>(x);
	vert[1] = static_cast<float>(y);
	vert[2] = static_cast<float>(z);

	return vert;
}

double* vec3D::returnDoubleArray(){
	double *vert = new double[3];

	vert[0] = static_cast<double>(x);
	vert[1] = static_cast<double>(y);
	vert[2] = static_cast<double>(z);

	return vert;
}

void vec3D::applyMatrix(float* matrix){
	x = matrix[0]*x + matrix[1]*y + matrix[2]*z + matrix[3];
	y = matrix[4]*x + matrix[5]*y + matrix[6]*z + matrix[7];
	z = matrix[8]*x + matrix[9]*y + matrix[10]*z + matrix[11];
}

void vec3D::update(double v[]){
	x = v[0];
	y = v[1];
	z = v[2];
}

bool vec3D::isOrthogonal(vec3D v){
	return (dot(v) == 0.0);
}

/* POINTS */

vert3D::vert3D(){
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

vert3D::vert3D(float x, float y,float z){
	this->x = x;
	this->y = y;
	this->z = z;
}

vert3D::~vert3D(){

}

// if difficulties switch to pointer
float vert3D::distanceFrom(vert3D p){
	return sqrt(pow(p.x - x,2) + pow(p.y - y,2) + pow(p.z - z,2));
}

// used for comparisons
float vert3D::fastDistanceFrom(vert3D p){
	return pow(p.x - x,2) + pow(p.y - y,2) + pow(p.z - z,2);
}

// moves point along vector
void vert3D::movePoint(vec3D v){
	x += v.x;
	y += v.y;
	z += v.z;
}

void vert3D::scalePoint(vec3D v){
	x *= v.x;
	y *= v.y;
	z *= v.z;
}

vert3D vert3D::scalePointR(vec3D v){
	return vert3D(this->x*v.x,this->y*v.y,this->z*v.z);
}

void vert3D::rotatePoint(float* matrix){
	x = matrix[0]*x + matrix[1]*y + matrix[2]*z + matrix[3];
	y = matrix[4]*x + matrix[5]*y + matrix[6]*z + matrix[7];
	z = matrix[8]*x + matrix[9]*y + matrix[10]*z + matrix[11];
}

// returns pointer to array with components of vertex
float* vert3D::returnArray(){
	float *vert = new float[3];

	vert[0] = static_cast<float>(x);
	vert[1] = static_cast<float>(y);
	vert[2] = static_cast<float>(z);
	return vert;
}

double* vert3D::returnDoubleArray(){
	double *vert = new double[3];

	vert[0] = static_cast<double>(x);
	vert[1] = static_cast<double>(y);
	vert[2] = static_cast<double>(z);

	return vert;
}



/* QUATERNIONS */
// general case
quaternion::quaternion(){
	w = 1.0;
	x = 0.0;
	y = 0.0;
	z = 0.0;

	updateRotationMatrix();
}

// specify components
quaternion::quaternion(float w,float x, float y, float z){
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;
	updateRotationMatrix();
}

quaternion::~quaternion(){

}

// updates rotation matrix that quaternion associates with
void quaternion::updateRotationMatrix(){
	rotationMatrix = new float[16];

	rotationMatrix[0] = pow(w,2)+pow(x,2)-pow(y,2)-pow(z,2);
	rotationMatrix[1] = 2*x*y - 2*w*z;
	rotationMatrix[2] = 2*x*z + 2*w*y;
	rotationMatrix[3] = 0.0;
	rotationMatrix[4] = 2*x*y + 2*w*z;
	rotationMatrix[5] = pow(w,2)-pow(x,2)+pow(y,2)-pow(z,2);
	rotationMatrix[6] = 2*y*z + 2*w*x;
	rotationMatrix[7] = 0.0;
	rotationMatrix[8] = 2*x*z - 2*w*y;
	rotationMatrix[9] = 2*y*z - 2*w*x;
	rotationMatrix[10] = pow(w,2)-pow(x,2)-pow(y,2)+pow(z,2);
	rotationMatrix[11] = 0.0;
	rotationMatrix[12] = 0.0;
	rotationMatrix[13] = 0.0;
	rotationMatrix[14] = 0.0;
	rotationMatrix[15] = 1.0;
}

// normalizes quaternion
quaternion quaternion::normalize() {
	float magnitude = sqrt(pow(w,2) + pow(x,2) + pow(y,2) + pow(z,2));

	return quaternion(w/magnitude,x/magnitude,y/magnitude,z/magnitude);
};


// cross two quaternions
quaternion quaternion::cross(quaternion other) {

	float cw = w*other.w - x*other.x - y*other.y - z*other.z;
	float cx = w*other.x + x*other.w + y*other.z - z*other.y;
	float cy = w*other.y + x*other.z - y*other.w + z*other.x;
	float cz = w*other.z + x*other.y + y*other.x - z*other.w;

	return quaternion(cw,cx,cy,cz);
};

/* STATIC FUNCTIONS */

quaternion angleAxis(float angle, vec3D axis){
	// axis has to be unit vector
	quaternion newQuat = quaternion();

	newQuat.w  = cosf(angle/2);
	newQuat.x = axis.x * sinf(angle/2);
	newQuat.y = axis.y * sinf(angle/2);
	newQuat.z = axis.z * sinf(angle/2);

	return newQuat;
}

vec3D operator+ (vec3D v1, vec3D v2)
{
    return vec3D(v1.x + v2.x, v1.y + v2.y,v1.z + v2.z);
}

vec3D operator- (vec3D v1, vec3D v2)
{
    return vec3D(v1.x - v2.x, v1.y - v2.y,v1.z - v2.z);
}


vec3D operator* (vec3D v1, vec3D v2)
{
    return vec3D(v1.x*v2.x, v1.y*v2.y,v1.z*v2.z);
}
