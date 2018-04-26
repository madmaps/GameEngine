CC=g++

CFLAGS = -c -Wall -std=c++11

LIBS = -lGL -lX11 -lassimp -lSDL2 -lopenal

all: GameEngine

GameEngine: GameEngine.o Component.o StandardObject.o Mesh.o RenderTarget.o BumpMapGLRenderer.o \
			BitMapHeader.o BmpLoader.o DibHeader.o LoadHelper.o SkyBoxGLRenderer.o ModelMesh.o StandardMesh.o \
			Planet.o SkyBox.o SkyBoxMesh.o ObjectSocket.o Camera.o Ship.o Timer.o InputDevice.o \
			JoystickDevice.o SdlJoystickDevice.o
	$(CC) GameEngine.o Component.o StandardObject.o Mesh.o RenderTarget.o BumpMapGLRenderer.o \
		  BitMapHeader.o BmpLoader.o DibHeader.o SkyBoxGLRenderer.o LoadHelper.o ModelMesh.o StandardMesh.o \
		  Planet.o SkyBox.o SkyBoxMesh.o ObjectSocket.o Camera.o Ship.o Timer.o  InputDevice.o \
		  JoystickDevice.o SdlJoystickDevice.o $(LIBS) -o GameEngine 

GameEngine.o: GameEngine.cpp
	$(CC) $(CFLAGS) GameEngine.cpp 
	
Component.o: Component.cpp
	$(CC) $(CFLAGS) Component.cpp
	
StandardObject.o: StandardObject.cpp
	$(CC) $(CFLAGS) StandardObject.cpp
	
Mesh.o: Mesh.cpp
	$(CC) $(CFLAGS) Mesh.cpp
	
RenderTarget.o: RenderTarget.cpp
	$(CC) $(CFLAGS) RenderTarget.cpp
	
BumpMapRenderer.o: BumpMapGLRenderer.cpp
	$(CC) $(CFLAGS) BumpMapGLRenderer.cpp
	
BmpLoader.o: BmpLoader.cpp
	$(CC) $(CFLAGS) BmpLoader.cpp
	
BitMapHeader.o: BitMapHeader.cpp
	$(CC) $(CFLAGS) BitMapHeader.cpp
	
DibHeader.o: DibHeader.cpp
	$(CC) $(CFLAGS) DibHeader.cpp
	
SkyBoxGLRenderer.o: SkyBoxGLRenderer.cpp
	$(CC) $(CFLAGS) SkyBoxGLRenderer.cpp
	
LoadHelper.o: LoadHelper.cpp
	$(CC) $(CFLAGS) LoadHelper.cpp
	
ModelMesh.o: ModelMesh.cpp
	$(CC) $(CFLAGS) ModelMesh.cpp
	
StandardMesh.o: StandardMesh.cpp
	$(CC) $(CFLAGS) StandardMesh.cpp
	
Planet.o: Planet.cpp
	$(CC) $(CFLAGS) Planet.cpp
	
SkyBox.o: SkyBox.cpp
	$(CC) $(CFLAGS) SkyBox.cpp
	
SkyBoxMesh.o: SkyBoxMesh.cpp
	$(CC) $(CFLAGS) SkyBoxMesh.cpp
	
ObjectSocket.o: ObjectSocket.cpp
	$(CC) $(CFLAGS) ObjectSocket.cpp

Camera.o: Camera.cpp
	$(CC) $(CFLAGS) Camera.cpp

Ship.o: Ship.cpp
	$(CC) $(CFLAGS) Ship.cpp

Timer.o: Timer.cpp
	$(CC) $(CFLAGS) Timer.cpp
	
InputDevice.o: InputDevice.cpp
	$(CC) $(CFLAGS) InputDevice.cpp
	
JoystickDevice.o: JoystickDevice.cpp
	$(CC) $(CFLAGS) JoystickDevice.cpp
	
SdlJoystickDevice.o: SdlJoystickDevice.cpp
	$(CC) $(CFLAGS) SdlJoystickDevice.cpp
	
clean:
	rm -rf *.o core.* GameEngine



