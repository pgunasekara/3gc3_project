/* ----------------README-----------------------------------------------------
This camera was originally developed for the final project along 
with Pasindu Gunasekara(1412115). 

The code for the camera might look similar because we developed it together.

We both informed Dr. Gwosdz and recieved his approval in using this code
-----------------------------------------------------------------------------*/
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
	camera_position = vec3D(11.5,0.7,-10);
	camera_look_at = vec3D(11.5,0.7,-5);
	cameraYMove = 0.7f;
	camera_up = vec3D(0, 1, 0);
	camera_position_delta = vec3D();

	//light_position = vec3D(0,0,0);
	//spot_direction = vec3D(camera_look_at - camera_position);

	camera_scale = 0.1f;
	camera_pitch = 0.0;
	camera_heading = 0.0;
	rotate_camera = false;
}
// initializes all your Vectors, your camera scaling and angles for rotation
Camera::Camera(int tSize) {
	camera_position = vec3D(tSize + 5,tSize/5,tSize + 5);
	camera_look_at = vec3D(0,0,0);
	camera_up = vec3D(0, 1, 0);
	camera_position_delta = vec3D();

	camera_scale = 1.2f;
	camera_pitch = 0.0;
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

	// below you just update the look at
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float *cp = camera_position.returnArray();
	float *cl = camera_look_at.returnArray();
	float *cu = camera_up.returnArray();

	//printf("%f %f %f %f %f %f %f %f %f\n",cp[0],cp[1],cp[2],cl[0],cl[1],cl[2],cu[0],cu[1],cu[2]);

	gluLookAt(cp[0],cp[1],cp[2],cl[0],cl[1],cl[2],cu[0],cu[1],cu[2]);
}

// Move the camera forward depending on your scale
void Camera::Move(CameraDirection dir, Mesh3D* mesh) {
	bool FDir = true;
	switch (dir) {
		/*case UP:
			camera_position_delta = camera_position_delta + camera_up.vectorMultiplyr(camera_scale);
			break;
		case DOWN:
			camera_position_delta = camera_position_delta - camera_up.vectorMultiplyr(camera_scale);
			break;
			*/
		case LEFT:
				camera_position_delta = camera_position_delta - camera_direction.cross(camera_up).vectorMultiplyr(camera_scale);
				FDir = false;
			break;
		case RIGHT:
				camera_position_delta = camera_position_delta + camera_direction.cross(camera_up).vectorMultiplyr(camera_scale);
				FDir = false;
			break;
		case FORWARD:
			camera_position_delta = camera_position_delta + camera_direction.vectorMultiplyr(camera_scale);
			break;
		case BACK:
				camera_position_delta = camera_position_delta - camera_direction.vectorMultiplyr(camera_scale);
			break;
	}
	if (camera_position_delta.y != cameraYMove){
		camera_position_delta.y = 0.0;
	}
	if (checkInvalidMove(mesh,FDir)){
		camera_position_delta.reset();
	}
	// update camera
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

bool Camera::checkInvalidMove(Mesh3D* m,bool forwardBack){
	bool lowerBounds,upperBounds;
	vec3D cP = camera_position + camera_direction;
	//printf("pos:%f %f %f\n",cP.x,cP.y,cP.z);
	//printf("forward:%f %f %f\n",camera_direction.x,camera_direction.y,camera_direction.z);
	//printf("side:%f %f %f\n",camera_direction.cross(camera_up).x,camera_direction.cross(camera_up).y,camera_direction.cross(camera_up).z);
	for (int i = 0; i < m->faces.size();i++){
		if (m->faces[i].hit->xPlane && m->faces[i].hit->yPlane && forwardBack){
			lowerBounds = (min(m->faces[i].hit->minP.x,m->faces[i].hit->maxP.x) < cP.x && min(m->faces[i].hit->minP.y,m->faces[i].hit->maxP.y) < cP.y);
			upperBounds = (max(m->faces[i].hit->minP.x,m->faces[i].hit->maxP.x) > cP.x && max(m->faces[i].hit->minP.y,m->faces[i].hit->maxP.y) > cP.y);
			//printf("min:%f %f %f\nmax:%f %f %f\n",m->faces[i].hit->minP.x,m->faces[i].hit->minP.y,m->faces[i].hit->minP.z,m->faces[i].hit->maxP.x,m->faces[i].hit->maxP.y,m->faces[i].hit->maxP.z);
			// check distance between current position and hitbox's z val
			if((abs(m->faces[i].hit->minP.z - cP.z) <= 2*camera_scale) && lowerBounds && upperBounds){
				return true;
			}
		}else if (m->faces[i].hit->zPlane && m->faces[i].hit->yPlane && forwardBack){
			lowerBounds = (min(m->faces[i].hit->minP.z,m->faces[i].hit->maxP.z) < cP.z && min(m->faces[i].hit->minP.y,m->faces[i].hit->maxP.y) < cP.y);
			upperBounds = (max(m->faces[i].hit->minP.z,m->faces[i].hit->maxP.z) > cP.z && max(m->faces[i].hit->minP.y,m->faces[i].hit->maxP.y) > cP.y);
			//printf("min:%f %f %f\nmax:%f %f %f\n",m->faces[i].hit->minP.x,m->faces[i].hit->minP.y,m->faces[i].hit->minP.z,m->faces[i].hit->maxP.x,m->faces[i].hit->maxP.y,m->faces[i].hit->maxP.z);
			// check distance between current position and hitbox's z val
			if((abs(m->faces[i].hit->minP.x - cP.x) <= 2*camera_scale) && lowerBounds && upperBounds){
				return true;
			}
		}
		else if (m->faces[i].hit->xPlane && m->faces[i].hit->yPlane && !forwardBack){
			cP = camera_position + camera_direction.cross(camera_up);
			lowerBounds = (min(m->faces[i].hit->minP.x,m->faces[i].hit->maxP.x) < cP.x && min(m->faces[i].hit->minP.y,m->faces[i].hit->maxP.y) < cP.y);
			upperBounds = (max(m->faces[i].hit->minP.x,m->faces[i].hit->maxP.x) > cP.x && max(m->faces[i].hit->minP.y,m->faces[i].hit->maxP.y) > cP.y);
			//printf("min:%f %f %f\nmax:%f %f %f\n",m->faces[i].hit->minP.x,m->faces[i].hit->minP.y,m->faces[i].hit->minP.z,m->faces[i].hit->maxP.x,m->faces[i].hit->maxP.y,m->faces[i].hit->maxP.z);
			// check distance between current position and hitbox's z val
			if((abs(m->faces[i].hit->minP.z - cP.z) <= 4*camera_scale) && lowerBounds && upperBounds){
				return true;
			}
		}else if (m->faces[i].hit->zPlane && m->faces[i].hit->yPlane && !forwardBack){
			cP = camera_position + camera_direction.cross(camera_up);
			lowerBounds = (min(m->faces[i].hit->minP.z,m->faces[i].hit->maxP.z) < cP.z && min(m->faces[i].hit->minP.y,m->faces[i].hit->maxP.y) < cP.y);
			upperBounds = (max(m->faces[i].hit->minP.z,m->faces[i].hit->maxP.z) > cP.z && max(m->faces[i].hit->minP.y,m->faces[i].hit->maxP.y) > cP.y);
			//printf("min:%f %f %f\nmax:%f %f %f\n",m->faces[i].hit->minP.x,m->faces[i].hit->minP.y,m->faces[i].hit->minP.z,m->faces[i].hit->maxP.x,m->faces[i].hit->maxP.y,m->faces[i].hit->maxP.z);
			// check distance between current position and hitbox's z val
			if((abs(m->faces[i].hit->minP.x - cP.x) <= 4*camera_scale) && lowerBounds && upperBounds){
				return true;
			}
		}
	}
	return false;
}