/* ----------------README-----------------------------------------------------
This camera was originally developed for the final project along
with Pasindu Gunasekara(1412115).

The code for the camera used for this assignment might look similar because
we developed it together.

We both informed Dr. Gwosdz and recieved his approval in using this code
-----------------------------------------------------------------------------*/

#ifndef CAMERA_H
#define CAMERA_H

// classes uses 3D math class
#include "math3D.h"
#include "Mesh3D.h"
#include "Hitbox.h"

// camera direction for which direction you are moving
enum CameraDirection {
	UP, DOWN, LEFT, RIGHT, FORWARD, BACK
};

// camera direction for which way you are spinning
enum CameraSpinDirection {
	SUP, SDOWN, SLEFT, SRIGHT
};

struct compass3D{
	vec3D n,e,s,w,ne,nw,se,sw;
};

// class for the camera
class Camera {
	public:
		// constructor
		Camera();
		Camera(int tSize);
		// destructor
		~Camera();

		// update the camera after any transformation
		void Update();

		// translate the camera in a set direction
		// add float for amount
		void Move(CameraDirection dir, Mesh3D* mesh);
		// rotate the camera in a set direction by a certain amount of degrees
		void Spin(CameraSpinDirection dir,float degrees);

		// temporarily changes angle around y
		void ChangeHeading(float degrees);
		// temporarily changes angle around x
		void ChangePitch(float degrees);
		void checkCompassDirection();
		bool checkInvalidMove(Mesh3D* m,CameraDirection dir);
		bool checkFBHit(faces3D face, vec3D cP, CameraDirection dir);
		bool checkLRHit(faces3D face, vec3D cP, CameraDirection dir);

		// amount that you scale
		float camera_scaleX,camera_scaleZ;
		// angle around x and y axis
		float camera_heading,camera_pitch;
		// boolean whether you just rotated the camera or not
		bool rotate_camera;

		// vectors holding on to your current camera position, change in camera position, point you are looking at, camera direction and up vector
		vec3D camera_position;
		vec3D camera_position_delta;
		vec3D camera_look_at;
		vec3D camera_direction;
		vec3D camera_up;
		vec3D light_position;
		vec3D spot_direction;
		compass3D compass;
		bool* availableDirections;
		float cameraYMove;
};
#endif
