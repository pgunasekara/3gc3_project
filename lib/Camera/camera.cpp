#include "camera.h"
#include "Mesh3D.h"
#include "Hitbox.h"
#include "stdio.h"
#include <cmath>
#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif


Camera::Camera(){
	camera_position = vec3D(23,1.0,-20);
	camera_look_at = vec3D(23,1.0,-10);
	cameraYMove = 1.0f;
	camera_up = vec3D(0, 1, 0);
	camera_position_delta = vec3D();
	compass.n = vec3D(0,0,1);
	//compass.ne = vec3D(-0.5,0,0.5);
	compass.e = vec3D(1,0,0);
	//compass.se = vec3D(-0.5,0,-0.5);
	compass.s = compass.n.flipVectorR();
	//compass.sw = compass.ne.flipVectorR();
	compass.w = compass.e.flipVectorR();
	//compass.nw = compass.se.flipVectorR();
	universal_camera_direction = &compass.n;

	//light_position = vec3D(0,0,0);
	//spot_direction = vec3D(camera_look_at - camera_position);

	camera_scaleX = 0.025f;
	camera_scaleZ = 0.1f;
	camera_heading = 0.0;
	rotate_camera = false;
}

// destructor
Camera::~Camera() {

}

// updates camera anytime transformation happens
void Camera::Update() {
	// reset camera direction and normalize it
	camera_direction = camera_look_at - camera_position;
	camera_direction = camera_direction.normalize();
	// set up quaternions for rotations around y and x axis as well as one to hold rotations for both
	quaternion pitch_quat,heading_quat,temp;

	// if you just rotated the camera
	if (rotate_camera){
		// this vector holds the axis which will initially be the x-axis however changes after camera_direction is changed
		vec3D axis = camera_direction.cross(camera_up);

		// restricts how far up and down you can look,
		// makes sure that you can't rotate around x axis too much so that you don't flip upside down
		// maintains realistic view, where you can turn around but can only look up and down so far
		float height = camera_look_at.y - camera_position.y;
		float length = camera_look_at.x - camera_position.x;
		float pitchAngle = tan(height/length);

		// allows or doesnt allow rotation around x and applies both rotations together to new quaternion
		if ((pitchAngle > 1.0  && camera_pitch >= 0) || (pitchAngle < -1.0 && camera_pitch <= 0)){
			temp = angleAxis(camera_heading, camera_up).normalize();
		}else{
			pitch_quat = angleAxis(camera_pitch, axis);
			heading_quat = angleAxis(camera_heading, camera_up);
			temp = pitch_quat.cross(heading_quat).normalize();
		}

		// updates rotation matrix stored by quaternion
		temp.updateRotationMatrix();

		// applies rotation matrix to camera direction
		camera_direction.applyMatrix(temp.rotationMatrix);
		// you are no longer rotating camera and your angles get reset
		rotate_camera = false;
		camera_pitch = 0.0;
		camera_heading = 0.0;
	}

	// add the amount you are moving by and reset the delta vector
	camera_position = camera_position + camera_position_delta;
	camera_position_delta.reset();

	// creates new lookat and rounds it off to nicer numbers (by scalar multiplication)
	camera_look_at = camera_position + camera_direction;
	camera_look_at= camera_look_at.vectorMultiplyr(1.0f);

	// below you just update the look
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float *cp = camera_position.returnArray();
	float *cl = camera_look_at.returnArray();
	float *cu = camera_up.returnArray();
	float *cd = camera_direction.returnArray();

	gluLookAt(cp[0],cp[1],cp[2],cl[0],cl[1],cl[2],cu[0],cu[1],cu[2]);
}

// Move the camera forward depending on your scale
void Camera::Move(CameraDirection dir, Mesh3D* mesh) {
	switch (dir) {
		case LEFT:
			camera_position_delta = camera_position_delta - camera_direction.cross(camera_up).vectorMultiplyr(camera_scaleX);
			break;
		case RIGHT:
			camera_position_delta = camera_position_delta + camera_direction.cross(camera_up).vectorMultiplyr(camera_scaleX);
			break;
		case FORWARD:
			camera_position_delta = camera_position_delta + camera_direction.vectorMultiplyr(camera_scaleZ);
			break;
		case BACK:
				camera_position_delta = camera_position_delta - camera_direction.vectorMultiplyr(camera_scaleZ);
			break;
	}
	if (camera_position_delta.y != cameraYMove){
		camera_position_delta.y = 0.0;
	}
	if (checkInvalidMove(mesh,dir)){
		camera_position_delta.reset();
	}
	checkCompassDirection();
	Update();
}

// change the angle around axis depending on which arrow key is pressed by degrees given
void Camera::Spin(CameraSpinDirection dir,float degrees){
	switch (dir) {
		case SUP:
			ChangePitch(-1*degrees);
			break;
		case SDOWN:
			ChangePitch(degrees);
			break;
		case SLEFT:
			ChangeHeading(degrees);
			break;
		case SRIGHT:
			ChangeHeading(-1*degrees);
			break;
	}
	checkCompassDirection();
	// switch boolean to true and update camera
	rotate_camera = true;
	Update();
}

// temporary change around x axis agnle
void Camera::ChangePitch(float degrees) {
	if (camera_pitch > 90.0f) {
		camera_pitch -= 90.0f;
	} else if (camera_pitch < -90.0f) {
		camera_pitch += 90.0f;
	}else {
		camera_pitch += degrees;
	}
}

// temporary change around y axis angle
void Camera::ChangeHeading(float degrees) {
	if (camera_heading > 360.0f) {
		camera_heading -= 360.0f;
	} else if (camera_heading < -360.0f) {
		camera_heading += 360.0f;
	} else if (camera_pitch > 90 && camera_pitch < 270 || (camera_pitch < -90 && camera_pitch > -270)) {
		camera_heading -= degrees;
	} else {
		camera_heading += degrees;
	}
}

bool Camera::checkInvalidMove(Mesh3D* m,CameraDirection dir){
	bool lowerBounds,upperBounds;
	vec3D cP;
	float scale;
	float distanceFactor;
	switch (dir){
		case LEFT:
			cP = camera_position + universal_camera_direction->cross(camera_up).vectorMultiplyr(camera_scaleX);
			break;
		case RIGHT:
			cP = camera_position - universal_camera_direction->cross(camera_up).vectorMultiplyr(camera_scaleX);
			cP.flipVector();
			break;
		case FORWARD:
			cP = camera_position + universal_camera_direction->vectorMultiplyr(camera_scaleZ);
			break;
		case BACK:
			cP = camera_position - universal_camera_direction->vectorMultiplyr(camera_scaleZ);
			cP.flipVector();
			break;
	}
	if (dir == FORWARD || dir == BACK){
		for (int i = 0; i < m->faces.size();i++){
			if (m->faces[i].lHit->xPlane && m->faces[i].lHit->yPlane){
				lowerBounds = (min(m->faces[i].lHit->minP.x,m->faces[i].lHit->maxP.x) < cP.x && min(m->faces[i].lHit->minP.y,m->faces[i].lHit->maxP.y) < cP.y);
				upperBounds = (max(m->faces[i].lHit->minP.x,m->faces[i].lHit->maxP.x) > cP.x && max(m->faces[i].lHit->minP.y,m->faces[i].lHit->maxP.y) > cP.y);
				if(((abs(m->faces[i].lHit->minP.z - cP.z) <= camera_scaleZ*3.0f) || (abs(m->faces[i].rHit->minP.z - cP.z) <= camera_scaleZ*3.0f)) && lowerBounds && upperBounds) return true;
			}else if (m->faces[i].lHit->zPlane && m->faces[i].lHit->yPlane){
				lowerBounds = (min(m->faces[i].lHit->minP.z,m->faces[i].lHit->maxP.z) < cP.z && min(m->faces[i].lHit->minP.y,m->faces[i].lHit->maxP.y) < cP.y);
				upperBounds = (max(m->faces[i].lHit->minP.z,m->faces[i].lHit->maxP.z) > cP.z && max(m->faces[i].lHit->minP.y,m->faces[i].lHit->maxP.y) > cP.y);
				if(((abs(m->faces[i].lHit->minP.x - cP.x) <= camera_scaleZ*3.0f) || (abs(m->faces[i].rHit->minP.x - cP.x) <= camera_scaleZ*3.0f)) && lowerBounds && upperBounds) return true;
			}
		}
	}else{
		for (int i = 0; i < m->faces.size();i++){
			if (m->faces[i].lHit->xPlane && m->faces[i].lHit->yPlane){
				lowerBounds = (min(m->faces[i].lHit->minP.x,m->faces[i].lHit->maxP.x) < cP.x && min(m->faces[i].lHit->minP.y,m->faces[i].lHit->maxP.y) < cP.y);
				upperBounds = (max(m->faces[i].lHit->minP.x,m->faces[i].lHit->maxP.x) > cP.x && max(m->faces[i].lHit->minP.y,m->faces[i].lHit->maxP.y) > cP.y);
				if(((abs(m->faces[i].lHit->minP.z - cP.z) <= camera_scaleX) || (abs(m->faces[i].rHit->minP.z - cP.z) <= camera_scaleZ)) && lowerBounds && upperBounds) return true;
			}else if (m->faces[i].lHit->zPlane && m->faces[i].lHit->yPlane){
				lowerBounds = (min(m->faces[i].lHit->minP.z,m->faces[i].lHit->maxP.z) < cP.z && min(m->faces[i].lHit->minP.y,m->faces[i].lHit->maxP.y) < cP.y);
				upperBounds = (max(m->faces[i].lHit->minP.z,m->faces[i].lHit->maxP.z) > cP.z && max(m->faces[i].lHit->minP.y,m->faces[i].lHit->maxP.y) > cP.y);
				if(((abs(m->faces[i].lHit->minP.x - cP.x) <= camera_scaleX) || (abs(m->faces[i].rHit->minP.x - cP.x) <= camera_scaleX)) && lowerBounds && upperBounds) return true;
			}
		}
	}
	return false;
}

void Camera::checkCompassDirection(){
	if (abs(camera_direction.x) < abs(camera_direction.z) && camera_direction.z > 0){
		universal_camera_direction = &compass.n;
	//else if (abs(camera_direction.x) > 0.5f && abs(camera_direction.z) > 0.5f && camera_direction.x < 0 && camera_direction.z > 0){
	//	universal_camera_direction = &compass.ne;
	}else if (abs(camera_direction.x) > abs(camera_direction.z) && camera_direction.x < 0) {
		universal_camera_direction = &compass.e;
	}else if (abs(camera_direction.x) < abs(camera_direction.z) && camera_direction.z < 0) {
		universal_camera_direction = &compass.s;
	}else if (abs(camera_direction.x) > abs(camera_direction.z) && camera_direction.x > 0) {
		universal_camera_direction = &compass.w;
	}
}
