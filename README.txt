#readme

COMP SCI 3GC3 Final Project

Description:
Maze exploration game created in C++ and OpenGL. The objective of the game is to leave the maze before your flashlight runs out. As you go through the maze, there are FIVE small keys(boxes, these keys will increase the battery life on your flashlight. Most of the maze will be dark as you walk through it, with only a flashlight to light the way, which eventually runs out.

NOTE: main.cpp is in the src/ folder, include files are in include/ and the libraries and classes are in lib/
See makefile for complete paths

Tested on Linux(arch), OSX, and Windows

Group:
	Pasindu Gunasekara
		gunasepi
		1412155
	Roberto Temelkovski
		temelkr
		1418731
	Colin Gillespie
		gillescj
		1330655
	Peter Kalnoki
		kalnokp
		1160986

Implemented Features:
	-All the basic parts of a game, object loading, first person camera, surface normals and texture coordinates.
	-Texturing (on the walls, the floor, and the keys)
	-Lighting (The flashlight moves with the player and points in the direction the player faces)
	-Particle Systems(There is a particle system for rain, which spawns 2000 particles max)
	-Picking (ray picking was implemented to pick up items, RIGHT CLICK must be used to ray pick)
	-Advanced Camera Control (A quaternion based camera was created to navigate around the maze using the mouse and keyboard)

CLI Comments:
cout << "\nCOMP SCI 3GC3 Final Project\n\n"
	<< "\n\nNOTE: On initial startup, click inside the window to properly position the camera.\n\n"
	<< "Group: \n\tPasindu Gunasekara\n\t\tgunasepi\n\t\t1412155"
	<< "\n\tRoberto Temelkovski\n\t\ttemelkr\n\t\t1418731"
	<< "\n\tColin Gillespie\n\t\tgillescj\n\t\t1330655"
	<< "\n\tPeter Kalnoki\n\t\tkalnokp\n\t\t1160986"
	<< "\n\nInstructions:\n"
	<< "Use the mouse to rotate the camera around\n\tLEFT CLICK to stop the camera from moving, \n\tOnce clicked, the camera will stop responding to the mouse, this will allow you to move the cursor into a more comfortable spot\n\tmaking it easier to navigate."
	<< "\nOnce the user is done positioning the mouse, LEFT CLICK again, the capture the mouse."
	<< "\n(UPPERCASE)W A S D: Translate the camera, move forwards, backwards, left, and right."
	<< "\nRIGHT CLICK: Ray picking, for picking up Batteries (These are the boxes found through out the maze).\n"
	<< "You start off with low battery life so you should pick up the battery that is near the entrance\n"
	<< "\nPicking up Batteries will increase the battery life of your flashlight.\n"
	<< "\nAfter taking a certain amount of steps, the battery will start to degrade."
	<< "b or B: See current battery level in command line.\n"
	<< "(LOWERCASE) q: quit.\n";

Known Issues:
	Camera sometimes DOES NOT start at the right position
		-To fix this, all you have to do is LEFT CLICK inside the window to fix it.
		-On OSX this problem can be fixed by rotating the camera until you are viewing the right spot_direction by clicking and readjusting your current position

Sources:
	Help with the particle system from NeHe - http://nehe.gamedev.net/article/particle_systems/15008/
	Help with the quaternion based Camera system - http://www.cprogramming.com/tutorial/3d/quaternions.html
