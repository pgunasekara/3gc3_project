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
	camera_position = vec3D(34.5,1.5,-30);
	camera_look_at = vec3D(34.5,1.5,0);
	cameraYMove = 1.0f;
	camera_up = vec3D(0, 1, 0);
	camera_position_delta = vec3D();
	availableDirections = new bool[4];
	availableDirections[0] = true;
	availableDirections[1] = false;
	availableDirections[2] = false;
	availableDirections[3] = false;

	spot_direction = (camera_position-camera_look_at).normalize();

	camera_scaleX = 0.1f;
	camera_scaleZ = 0.5f;
	camera_heading = 0.0;
	rotate_camera = false;
}

// destructor
Camera::~Camera() {
	delete availableDirections;
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
	//spot_direction = (camera_position-camera_look_at).normalize();

	// below you just update the look
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float *cp = camera_position.returnArray();
	float *cl = camera_look_at.returnArray();
	float *cu = camera_up.returnArray();

	gluLookAt(cp[0],cp[1],cp[2],cl[0],cl[1],cl[2],cu[0],cu[1],cu[2]);
	delete cp;
	delete cl;
	delete cu;
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
	bool lowerBounds,upperBounds,hitCheck,hitCheckSec;
	vec3D cP;
	if (dir == FORWARD){
		cP = camera_position + camera_direction.vectorMultiplyr(camera_scaleZ);
	}else if (dir == BACK){
		cP = camera_position - camera_direction.vectorMultiplyr(camera_scaleZ);
	}else if (dir == LEFT){
		cP = camera_position - camera_direction.cross(camera_up).vectorMultiplyr(camera_scaleX);
	}else if (dir == RIGHT){
		cP = camera_position + camera_direction.cross(camera_up).vectorMultiplyr(camera_scaleX);
	}
	// N
	if (availableDirections[0]){
		for (int i = 0; i < m->faces.size();i++){
			if (checkFBHit(m->faces[i],cP,dir)) return true;
			if (checkLRHit(m->faces[i],cP,dir)) return true;
		}
		// If you angle it causes problems
	}else if (availableDirections[1]){
		for (int i = 0; i < m->faces.size();i++){
			if (checkFBHit(m->faces[i],cP,dir)) return true;
			if (checkLRHit(m->faces[i],cP,dir)) return true;
			}
		// S
	}else if (availableDirections[2]){
			for (int i = 0; i < m->faces.size();i++){
				if (checkFBHit(m->faces[i],cP,dir)) return true;
				if (checkLRHit(m->faces[i],cP,dir)) return true;
			}
		// W
	}else if (availableDirections[3]){
		for (int i = 0; i < m->faces.size();i++){
			if (checkFBHit(m->faces[i],cP,dir)) return true;
			if (checkLRHit(m->faces[i],cP,dir)) return true;
		}
	}
	return false;
}

bool Camera::checkLRHit(faces3D face, vec3D cP, CameraDirection dir){
	bool lowerBounds,upperBounds,leftBounds,rightBounds;
	bool hitCheck,hitCheckSec;
	// check hit on z-y plane
	if (face.lHit->xPlane && face.lHit->yPlane){
		// if you are checking for an intersection with a xy Plane check whether its within the x and y bounds plane so that
		// you aren't missing the plane
		lowerBounds = (min(face.lHit->minP.x,face.lHit->maxP.x) < cP.x);
		upperBounds = (max(face.lHit->minP.x,face.lHit->maxP.x) > cP.x);
		if (availableDirections[0] || availableDirections[2]){
			// if you are going in the North or South directions you will see whether a move can be made
			// based on the distance between a wall and the camera and how much the camera moves each time
			// these two checks below compare the distance the camera travels from one move's x value and the distance
			// between the current position and the wall's x-value
			// hitCheck checks for collision with left walls, hitCheckSec checks for collision with right walls
			hitCheck = (abs(face.lHit->minP.z - cP.z) <= abs(camera_direction.cross(camera_up).vectorMultiplyr(camera_scaleX).z));
			hitCheckSec = (abs(face.rHit->minP.z - cP.z) <= abs(camera_direction.cross(camera_up).vectorMultiplyr(camera_scaleX).z));
		}else{
			// if you are going in the East or West directions you will see whether a move can be made
			// based on the distance between a wall and the camera and how much the camera moves each time
			// these two checks below compare the distance the camera travels from one move's z value and the distance
			// between the current position and the wall's z-value
			// between the current position and the wall's x-value
			// hitCheck checks for collision with left walls, hitCheckSec checks for collision with right walls
			hitCheck = (abs(face.lHit->minP.z - cP.z) <= abs(camera_direction.cross(camera_up).vectorMultiplyr(camera_scaleX).z));
			hitCheckSec = (abs(face.rHit->minP.z - cP.z) <= abs(camera_direction.cross(camera_up).vectorMultiplyr(camera_scaleX).z));
		}

		if (availableDirections[0]){
			// true represents unavailable move
			if (lowerBounds && upperBounds && hitCheck && dir == RIGHT) return true;
			else if (lowerBounds && upperBounds && hitCheckSec && dir == LEFT) return true;
		}else if (availableDirections[2]) {
			if (lowerBounds && upperBounds && hitCheck && dir == LEFT) return true;
			else if (lowerBounds && upperBounds && hitCheckSec && dir == RIGHT) return true;
		}else if (availableDirections[1] || availableDirections[3]) {
			// check both behave in same way
			// handles checks when moving in diagonal direction
			if (lowerBounds && upperBounds && hitCheck && dir == RIGHT) return true;
			else if (lowerBounds && upperBounds && hitCheckSec && dir == RIGHT) return true;
			if (lowerBounds && upperBounds && hitCheck && dir == LEFT) return true;
			else if (lowerBounds && upperBounds && hitCheckSec && dir == LEFT) return true;
		}
		// check hit on x y plane
	}else if (face.lHit->zPlane && face.lHit->yPlane){
		// if you are checking for an intersection with a xy Plane check whether its within the x and y bounds plane so that
		// you aren't missing the plane
		lowerBounds = (min(face.lHit->minP.z,face.lHit->maxP.z) < cP.z);
		upperBounds = (max(face.lHit->minP.z,face.lHit->maxP.z) > cP.z);
		if (availableDirections[0] || availableDirections[2]){
			// if you are going in the North or South directions you will see whether a move can be made
			// based on the distance between a wall and the camera and how much the camera moves each time
			// these two checks below compare the distance the camera travels from one move's x value and the distance
			// between the current position and the wall's x-value
			// hitCheck checks for collision with left walls, hitCheckSec checks for collision with right walls
			hitCheck = abs(face.lHit->minP.x - cP.x) <= abs(camera_direction.cross(camera_up).vectorMultiplyr(camera_scaleX).x);
			hitCheckSec = abs(face.rHit->minP.x - cP.x) <= abs(camera_direction.cross(camera_up).vectorMultiplyr(camera_scaleX).x);
		}else{
			// if you are going in the East or West directions you will see whether a move can be made
			// based on the distance between a wall and the camera and how much the camera moves each time
			// these two checks below compare the distance the camera travels from one move's z value and the distance
			// between the current position and the wall's z-value
			// between the current position and the wall's x-value
			// hitCheck checks for collision with left walls, hitCheckSec checks for collision with right walls
			hitCheck = abs(face.lHit->minP.x - cP.x) <= abs(camera_direction.cross(camera_up).vectorMultiplyr(camera_scaleX).x);
			hitCheckSec =abs(face.rHit->minP.x - cP.x) <= abs(camera_direction.cross(camera_up).vectorMultiplyr(camera_scaleX).x);
		}
		if (availableDirections[0] || availableDirections[2]){
			// true represents unavailable move
			if (lowerBounds && upperBounds && hitCheck && dir == RIGHT) return true;
			else if (lowerBounds && upperBounds && hitCheckSec && dir == RIGHT) return true;
			else if (lowerBounds && upperBounds && hitCheck && dir == LEFT) return true;
			else if (lowerBounds && upperBounds && hitCheckSec && dir == LEFT) return true;
		}else if (availableDirections[1]) {
			if (lowerBounds && upperBounds && hitCheck && dir == RIGHT) return true;
			else if (lowerBounds && upperBounds && hitCheckSec && dir == LEFT) return true;
		}else if (availableDirections[3]) {
			if (lowerBounds && upperBounds && hitCheck && dir == LEFT) return true;
			else if (lowerBounds && upperBounds && hitCheckSec && dir == RIGHT) return true;
		}
	}
	return false;
}

bool Camera::checkFBHit(faces3D face, vec3D cP, CameraDirection dir){
	bool lowerBounds,upperBounds,leftBounds,rightBounds;
	bool hitCheck,hitCheckSec;
	// check hit on z-y plane
	if (face.lHit->xPlane && face.lHit->yPlane){
		// if you are checking for an intersection with a xy Plane check whether its within the x and y bounds plane so that
		// you aren't missing the plane
		lowerBounds = (min(face.lHit->minP.x,face.lHit->maxP.x) < cP.x);
		upperBounds = (max(face.lHit->minP.x,face.lHit->maxP.x) > cP.x);
		if (availableDirections[0] || availableDirections[2]){
			// if you are going in the North or South directions you will see whether a move can be made
			// based on the distance between a wall and the camera and how much the camera moves each time
			// these two checks below compare the distance the camera travels from one move's x value and the distance
			// between the current position and the wall's x-value
			// hitCheck checks for collision with left walls, hitCheckSec checks for collision with right walls
			hitCheck = (abs(face.lHit->minP.z - cP.z) <= abs(camera_direction.vectorMultiplyr(camera_scaleZ).z));
			hitCheckSec = (abs(face.rHit->minP.z - cP.z) <= abs(camera_direction.vectorMultiplyr(camera_scaleZ).z));
		}else{
			// if you are going in the East or West directions you will see whether a move can be made
			// based on the distance between a wall and the camera and how much the camera moves each time
			// these two checks below compare the distance the camera travels from one move's z value and the distance
			// between the current position and the wall's z-value
			// between the current position and the wall's x-value
			// hitCheck checks for collision with left walls, hitCheckSec checks for collision with right walls
			hitCheck = (abs(face.lHit->minP.z - cP.z) <= abs(camera_direction.vectorMultiplyr(camera_scaleZ).z));
			hitCheckSec = (abs(face.rHit->minP.z - cP.z) <= abs(camera_direction.vectorMultiplyr(camera_scaleZ).z));
		}

		if (availableDirections[0]){
			// true represents unavailable move
			if (lowerBounds && upperBounds && hitCheck && dir == BACK) return true;
			else if (lowerBounds && upperBounds && hitCheckSec && dir == FORWARD) return true;
		}else if (availableDirections[2]) {
			if (lowerBounds && upperBounds && hitCheck && dir == FORWARD) return true;
			else if (lowerBounds && upperBounds && hitCheckSec && dir == BACK) return true;
		}else if (availableDirections[1] || availableDirections[3]) {
			if (lowerBounds && upperBounds && hitCheck && dir == FORWARD) return true;
			else if (lowerBounds && upperBounds && hitCheckSec && dir == FORWARD) return true;
			if (lowerBounds && upperBounds && hitCheck && dir == BACK) return true;
			else if (lowerBounds && upperBounds && hitCheckSec && dir == BACK) return true;
		}
		// check hit on x y plane
	}else if (face.lHit->zPlane && face.lHit->yPlane){
		// if you are checking for an intersection with a xy Plane check whether its within the x and y bounds plane so that
		// you aren't missing the plane
		lowerBounds = (min(face.lHit->minP.z,face.lHit->maxP.z) < cP.z);
		upperBounds = (max(face.lHit->minP.z,face.lHit->maxP.z) > cP.z);
		if (availableDirections[0] || availableDirections[2]){
			// if you are going in the North or South directions you will see whether a move can be made
			// based on the distance between a wall and the camera and how much the camera moves each time
			// these two checks below compare the distance the camera travels from one move's x value and the distance
			// between the current position and the wall's x-value
			// hitCheck checks for collision with left walls, hitCheckSec checks for collision with right walls
			hitCheck = abs(face.lHit->minP.x - cP.x) <= abs(camera_direction.vectorMultiplyr(camera_scaleZ).x);
			hitCheckSec = abs(face.rHit->minP.x - cP.x) <= abs(camera_direction.vectorMultiplyr(camera_scaleZ).x);
		}else{
			// if you are going in the East or West directions you will see whether a move can be made
			// based on the distance between a wall and the camera and how much the camera moves each time
			// these two checks below compare the distance the camera travels from one move's z value and the distance
			// between the current position and the wall's z-value
			// between the current position and the wall's x-value
			// hitCheck checks for collision with left walls, hitCheckSec checks for collision with right walls
			hitCheck = abs(face.lHit->minP.x - cP.x) <= abs(camera_direction.vectorMultiplyr(camera_scaleZ).x);
			hitCheckSec =abs(face.rHit->minP.x - cP.x) <= abs(camera_direction.vectorMultiplyr(camera_scaleZ).x);
		}
		if (availableDirections[0] || availableDirections[2]){
			// true represents unavailable move
			if (lowerBounds && upperBounds && hitCheck && dir == FORWARD) return true;
			else if (lowerBounds && upperBounds && hitCheckSec && dir == FORWARD) return true;
			else if (lowerBounds && upperBounds && hitCheck && dir == BACK) return true;
			else if (lowerBounds && upperBounds && hitCheckSec && dir == BACK) return true;
		}else if (availableDirections[1]) {
			if (lowerBounds && upperBounds && hitCheck && dir == FORWARD) return true;
			else if (lowerBounds && upperBounds && hitCheckSec && dir == BACK) return true;
		}else if (availableDirections[3]) {
			if (lowerBounds && upperBounds && hitCheck && dir == BACK) return true;
			else if (lowerBounds && upperBounds && hitCheckSec && dir == FORWARD) return true;
		}
	}
	return false;
}

void Camera::checkCompassDirection(){
// checks to see which way you are looking
// the initial direction (north)
	if (abs(camera_direction.x) < abs(camera_direction.z) && camera_direction.z > 0){
		availableDirections[0] = true;
		availableDirections[1] = false;
		availableDirections[2] = false;
		availableDirections[3] = false;
// you are facing east
	}else if (abs(camera_direction.x) > abs(camera_direction.z) && camera_direction.x < 0) {
		availableDirections[0] = false;
		availableDirections[1] = true;
		availableDirections[2] = false;
		availableDirections[3] = false;
// you are facing south
	}else if (abs(camera_direction.x) < abs(camera_direction.z) && camera_direction.z < 0) {
		availableDirections[0] = false;
		availableDirections[1] = false;
		availableDirections[2] = true;
		availableDirections[3] = false;
// you are facing west
	}else if (abs(camera_direction.x) > abs(camera_direction.z) && camera_direction.x > 0) {
		availableDirections[0] = false;
		availableDirections[1] = false;
		availableDirections[2] = false;
		availableDirections[3] = true;
	}
}
