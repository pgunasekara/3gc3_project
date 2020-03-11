## Vanish: 3D Maze Exploration Game

Group: Pasindu Gunasekara, Roberto Temelkovski, Colin Gillespie, Peter Kalnoki

Maze exploration game created in C++ and OpenGL. The objective of the game is to leave the maze before your flashlight runs out. As you go through the maze, there are FIVE small keys(boxes, these keys will increase the battery life on your flashlight. Most of the maze will be dark as you walk through it, with only a flashlight to light the way, which eventually runs out.

Tested on Windows 10, macOS, and Linux

### Compilation Instructions
OpenGL, FreeGLUT 3, g++, and the make utility are required to compile.

Build the project using ```make```

Make will automatically execute the compiled binary.

Clean up object files using ```make clean```


### Implemented Bonus Features:
  * All the basic parts of a game, object loading, first person camera, surface normals and texture coordinates.
  * Texturing (on the walls, the floor, and the keys)
  * Lighting (The flashlight moves with the player and points in the direction the player faces)
  * Particle Systems(There is a particle system for rain, which spawns 2000 particles max)
  * Picking (ray picking was implemented to pick up items, RIGHT CLICK must be used to ray pick)
  * Advanced Camera Control (A quaternion based camera was created to navigate around the maze using the mouse and keyboard)


### Known Issues:
* Camera sometimes DOES NOT start at the right position
  * To fix this, all you have to do is LEFT CLICK inside the window to fix it.
  * On OSX this problem can be fixed by rotating the camera until you are viewing the right spot_direction by clicking and re-adjusting your current position
