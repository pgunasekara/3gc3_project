#readme

COMP SCI 3GC3 Final Project

Description:
Maze exploration game created in C++ and OpenGL. The objective of the game is to leave the maze before your flashlight runs out. As you go through the maze, there are FIVE small keys(boxes), of which THREE must be collected by the player to walk through the exit of the maze. Most of the maze will be dark as you walk through it, with only a flashlight to light the way, which eventually runs out.

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
		<< "Group: \n\tPasindu Gunasekara\n\t\tgunasepi\n\t\t1412155"
		<< "\n\tRoberto Temelkovski\n\t\ttemelkr\n\t\t1418731"
		<< "\n\tColin Gillespie\n\t\tgillescj\n\t\t1330655"
		<< "\n\tPeter Kalnoki\n\t\tkalnokp\n\t\t1160986"
		<< "\n\nInstructions:\n"
		<< "Use the mouse to rotate the camera around\n\tLEFT CLICK to stop the camera from moving, \n\tuse this to move the mouse pointer to a more comfortable position."
		<< "\n(UPPERCASE)W A S D: Translate the camera, move forwards, backwards, left, and right."
		<< "\nRIGHT CLICK: Ray picking, for picking up keys.\n"
		<< "(LOWERCASE) q: quit.\n"

Known Issues:
	
Sources:
	Help with the particle system from NeHe - http://nehe.gamedev.net/article/particle_systems/15008/