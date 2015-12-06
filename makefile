#please use 'make clean' to clean the directory of intermediate build files and the executable
#simply typing 'make' will compile all source code files to object files .o, and then link all
#object files into an executable
#we are using a lot of makefile macros

#changing platform dependant stuff, do not change this
# Linux (default)
LDFLAGS = -lGL -lGLU -lglut
CFLAGS=-g -Wall -std=c++11
CC=g++
EXEEXT= .x
RM=rm
SGL=lib/SceneGraph/
ML=lib/Math/
CL=lib/Camera/
HBL=lib/Hitbox/
M3D=lib/Mesh3D/

# Windows (cygwin)
ifeq "$(OS)" "Windows_NT"
	EXEEXT=.exe #on windows applications must have .exe extension
	RM=del #rm command for windows powershell
    LDFLAGS = -lfreeglut -lglu32 -lopengl32
else
	# OS X
	OS := $(shell uname)
	ifeq ($(OS), Darwin)
	        LDFLAGS = -framework Carbon -framework OpenGL -framework GLUT
	endif
endif

#change the 't1' name to the name you want to call your application
PROGRAM_NAME= Project.x

#run target to compile and build, and then launch the executable
run: $(PROGRAM_NAME)
	./$(PROGRAM_NAME)

#when adding additional source files, such as boilerplateClass.cpp
#or yourFile.cpp, add the filename with an object extension below
#ie. boilerplateClass.o and yourFile.o
#make will automatically know that the objectfile needs to be compiled
#form a cpp source file and find it itself :)
$(PROGRAM_NAME): src/main.o $(SGL)node.o $(SGL)nodeGroup.o $(SGL)nodeModel.o $(SGL)nodeTransform.o $(SGL)sceneGraph.o $(ML)math3D.o $(CL)camera.o $(HBL)Hitbox.o $(M3D)Mesh3D.o $(M3D)splitter.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	$(RM) *.o $(SGL)*.o $(ML)*.o $(CL)*.o $(HBL)*.o $(M3D)*.o $(PROGRAM_NAME)$(EXEEXT)
