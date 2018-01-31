CC=g++

CFLAGS = -c -Wall -std=c++11

LIBS = -lGL -lX11 -lassimp

all: GameEngine

GameEngine: GameEngine.o Component.o StandardObject.o Mesh.o RenderTarget.o TestRenderer.o BitMapHeader.o BmpLoader.o DibHeader.o
	$(CC) GameEngine.o Component.o StandardObject.o Mesh.o RenderTarget.o TestRenderer.o BitMapHeader.o BmpLoader.o DibHeader.o -o GameEngine $(LIBS)

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
	
TestRenderer.o: TestRenderer.cpp
	$(CC) $(CFLAGS) TestRenderer.cpp
	
BmpLoader.o: BmpLoader.cpp
	$(CC) $(CFLAGS) BmpLoader.cpp
	
BitMapHeader.o: BitMapHeader.cpp
	$(CC) $(CFLAGS) BitMapHeader.cpp
	
DibHeader.o: DibHeader.cpp
	$(CC) $(CFLAGS) DibHeader.cpp
	
clean:
	rm -rf *.o core.* GameEngine


