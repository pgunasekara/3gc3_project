LDFLAGS = -lGL -lGLU -lglut
CFLAGS=-g -Wall -std=c++11
CC=g++
EXEEXT= .x
RM=rm
ML=lib/Math
CL=lib/Camera
HBL=lib/Hitbox
M3D=lib/Mesh3D
LIB=lib
LIB_PATH=-l SOIL
INCLUDE=-I include

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

PROGRAM_NAME=Project.x

#run target to compile and build, and then launch the executable
run: $(PROGRAM_NAME)
	./$(PROGRAM_NAME)

main.o: src/main.cpp
	g++ $(INCLUDE) -c -o src/main.o src/main.cpp

math3D.o: $(ML)/math3D.cpp
	g++ $(INCLUDE) -c -o $(ML)/math3D.o $(ML)/math3D.cpp

camera.o: $(CL)/camera.cpp
	g++ $(INCLUDE) -c -o $(CL)/camera.o $(CL)/camera.cpp

Hitbox.o: $(HBL)/Hitbox.cpp
	g++ $(INCLUDE) -c -o $(HBL)/Hitbox.o $(HBL)/Hitbox.cpp

Mesh3D.o: $(M3D)/Mesh3D.cpp
	g++ $(INCLUDE) -c -o $(M3D)/Mesh3D.o $(M3D)/Mesh3D.cpp

splitter.o: $(M3D)/splitter.cpp
	g++ $(INCLUDE) -c -o $(M3D)/splitter.o $(M3D)/splitter.cpp

ParticleSystem.o: $(LIB)/ParticleSystem.cpp
	g++ $(INCLUDE) -c -o $(LIB)/ParticleSystem.o $(LIB)/ParticleSystem.cpp

$(PROGRAM_NAME): main.o ParticleSystem.o math3D.o camera.o Hitbox.o Mesh3D.o splitter.o compile

#$(CC) $(CFLAGS) $(INCLUDE) $(LDFLAGS) -static -L soil/ $(LIB_PATH) $^ -o $(PROGRAM_NAME)


compile: src/main.o $(ML)/math3D.o $(CL)/camera.o $(HBL)/Hitbox.o $(M3D)/Mesh3D.o $(M3D)/splitter.o $(LIB)/ParticleSystem.o
		$(CC) -o $(PROGRAM_NAME) $^ $(CFLAGS) $(LDFLAGS) $(INCLUDE)
clean:
	$(RM) $(ML)/*.o $(CL)/*.o $(HBL)/*.o $(M3D)/*.o $(PROGRAM_NAME)
