#define GL_GLEXT_PROTOTYPES
#define GLM_ENABLE_EXPERIMENTAL
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <GL/gl.h>
#include <GL/glx.h> 
#include <X11/X.h> 
#include <X11/keysym.h>
#include <vector>
#include <SDL2/SDL.h>
#include <AL/al.h>
#include <AL/alc.h>

#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"
#include "assimp/cimport.h"

#include "LoadHelper.h"
#include "BmpLoader.h"
#include "StandardObject.h"
#include "BumpMapGLRenderer.h"
#include "SkyBoxGLRenderer.h"
#include "StandardMesh.h"
#include "Planet.h"
#include "SkyBox.h"
#include "SkyBoxMesh.h"
#include "Camera.h"
#include "Ship.h"
#include "Timer.h"
#include "SdlJoystickDevice.h"

static int dblBuf[]  = {GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None};

Display   *dpy;
Window     win;

int main(int argc, char **argv)
{
	XVisualInfo         *vi;
    Colormap             cmap;
    XSetWindowAttributes swa;
    GLXContext           cx;
    XEvent               event;

    dpy = XOpenDisplay(NULL);
    vi = glXChooseVisual(dpy, DefaultScreen(dpy), dblBuf);
    cx = glXCreateContext(dpy, vi, None, GL_TRUE);
	cmap = XCreateColormap(dpy, RootWindow(dpy, vi->screen), vi->visual, AllocNone);
    swa.colormap = cmap;
    swa.border_pixel = 0;
    swa.event_mask = KeyPressMask | ExposureMask | ButtonPressMask | StructureNotifyMask;
    
    win = XCreateWindow(dpy, RootWindow(dpy, vi->screen), 0, 0, 1920, 1080, 0, vi->depth, InputOutput, vi->visual, CWBorderPixel | CWColormap | CWEventMask, &swa);
    XSetStandardProperties(dpy, win, "OpenGl Test", "OpenGl Test", None, argv, argc, NULL);
    glXMakeCurrent(dpy, win, cx);
    XMapWindow(dpy, win);
    
    if(SDL_Init(SDL_INIT_JOYSTICK) < 0)
    {
		std::cout << "SDL Joystick Error" << std::endl;
	}
    
    std::cout << "Number of Joysticks found: " << SDL_NumJoysticks() << std::endl;
    SDL_Joystick* sdlJoystick;
    SDL_JoystickEventState(SDL_ENABLE);
    sdlJoystick = SDL_JoystickOpen(0);
    SDL_Event sdlEvent;
    
    SdlJoystickDevice* joystick = new SdlJoystickDevice();
    
    
    
    /*ALCdevice* device;
    device = alcOpenDevice(NULL);
    ALCcontext *context;
    context = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);
    ALuint alBuffer;
    alGenBuffers((ALuint)1, &alBuffer);
    
    ALsizei size, freq;
    ALenum format;
    ALvoid* data;
    ALboolean loop = AL_FALSE;
    
    alutLoadWAVFile("test.wav", &format, &data, &size, &freq, &loop);*/
    
    
    
    
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
    GLuint shader_program = loadShaders("Shaders/NormalShader.vert","Shaders/NormalShader.frag");
    GLuint skyBoxShader = loadShaders("Shaders/SkyBoxShader.vert", "Shaders/SkyBoxShader.frag");
    
    
	Assimp::Importer importer;

	const aiScene* planetScene = importer.ReadFile("meshes/planet.dae",aiProcess_CalcTangentSpace | aiProcess_Triangulate);
	if(!planetScene)
	{
		std::cout << "BAD!";
	}

	Camera* activeCamera = new Camera(0.1f, 1000.0f, 67.0f, 1920, 1080);

	BumpMapGLRenderer* moonRenderer = new BumpMapGLRenderer();
	loadNormalMesh(planetScene, *moonRenderer, 0, "Textures/moonDefuse.bmp", "Textures/moonNormal.bmp", "Textures/moonSpecular.bmp", "Textures/moonAmbient.bmp");
    moonRenderer->addShader(shader_program);
	moonRenderer->updateProjectionMatrix(activeCamera->getProjectionMatrix());
    moonRenderer->updateViewMatrix(activeCamera->getViewMatrix());
    moonRenderer->updateCameraLocation(activeCamera->getLocationMatrix());
    StandardMesh* moonMesh = new StandardMesh();
    moonRenderer->updateModelMatrix(moonMesh->getModelMatrix());
    moonMesh->addRenderer(moonRenderer);
    
	Planet* moon = new Planet();
    moon->addComponent(moonMesh);
    moon->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    moon->setRotation(glm::angleAxis(0.0f, glm::vec3(0.0f, 0.0f, -1.0f)));
    
	const aiScene* tieBomberScene = importer.ReadFile("meshes/tiebomber.dae",aiProcess_CalcTangentSpace | aiProcess_Triangulate);
	if(!tieBomberScene)
	{
		std::cout << "BAD!";
	}
    BumpMapGLRenderer** tieBomberRenderer = new BumpMapGLRenderer*[13];
	StandardMesh* tieBomberMesh = new StandardMesh();
    for(int i = 0; i < 9; i++)
    {
		std::cout << "Part: " << i << std::endl;
		tieBomberRenderer[i] = new BumpMapGLRenderer();
		loadNormalMesh(tieBomberScene, *tieBomberRenderer[i], i, "Textures/Ship/ShipDef.bmp", "Textures/Ship/ShipNormal.bmp", "Textures/Ship/ShipSpec.bmp", "Textures/Ship/ShipAmb.bmp");
		tieBomberRenderer[i]->addShader(shader_program);
		tieBomberRenderer[i]->updateProjectionMatrix(activeCamera->getProjectionMatrix());
		tieBomberRenderer[i]->updateViewMatrix(activeCamera->getViewMatrix());
		tieBomberRenderer[i]->updateCameraLocation(activeCamera->getLocationMatrix());
		tieBomberRenderer[i]->updateModelMatrix(tieBomberMesh->getModelMatrix());
		tieBomberMesh->addRenderer(tieBomberRenderer[i]);
	}
	
	
	const aiScene* tieFighterScene = importer.ReadFile("meshes/tiefighter.dae",aiProcess_CalcTangentSpace | aiProcess_Triangulate);
	if(!tieFighterScene)
	{
		std::cout << "BAD!";
	}
	BumpMapGLRenderer** tieFighterRenderer = new BumpMapGLRenderer*[20];
	StandardMesh* tieFighterMesh = new StandardMesh();
	std::cout << "Loading TieFighter" << std::endl;
	for(int i = 0; i < 8; i++)
	{
		std::cout << "Part: " << i << std::endl;
		tieFighterRenderer[i] = new BumpMapGLRenderer();
		loadNormalMesh(tieFighterScene, *tieFighterRenderer[i], i, "Textures/Ship/ShipDef.bmp", "Textures/Ship/ShipNormal.bmp", "Textures/Ship/ShipSpec.bmp", "Textures/Ship/ShipAmb.bmp");
		tieFighterRenderer[i]->addShader(shader_program);
		tieFighterRenderer[i]->updateProjectionMatrix(activeCamera->getProjectionMatrix());
		tieFighterRenderer[i]->updateViewMatrix(activeCamera->getViewMatrix());
		tieFighterRenderer[i]->updateCameraLocation(activeCamera->getLocationMatrix());
		tieFighterRenderer[i]->updateModelMatrix(tieFighterMesh->getModelMatrix());
		tieFighterMesh->addRenderer(tieFighterRenderer[i]);
	}
	
	const aiScene* starDestroyerScene = importer.ReadFile("meshes/stardestroyer.dae",aiProcess_CalcTangentSpace | aiProcess_Triangulate);
	if(!starDestroyerScene)
	{
		std::cout << "BAD!";
	}
	BumpMapGLRenderer** starDestroyerRenderer = new BumpMapGLRenderer*[100];
	StandardMesh* starDestroyerMesh = new StandardMesh();
	std::cout << "Loading StarDestroyer" << std::endl;
	for(int i = 0; i < 56; i++)
	{
		std::cout << "Part: " << i << std::endl;
		starDestroyerRenderer[i] = new BumpMapGLRenderer();
		loadNormalMesh(starDestroyerScene, *starDestroyerRenderer[i], i, "Textures/Ship/ShipDef.bmp", "Textures/Ship/ShipNormal.bmp", "Textures/Ship/ShipSpec.bmp", "Textures/Ship/ShipAmb.bmp");
		starDestroyerRenderer[i]->addShader(shader_program);
		starDestroyerRenderer[i]->updateProjectionMatrix(activeCamera->getProjectionMatrix());
		starDestroyerRenderer[i]->updateViewMatrix(activeCamera->getViewMatrix());
		starDestroyerRenderer[i]->updateCameraLocation(activeCamera->getLocationMatrix());
		starDestroyerRenderer[i]->updateModelMatrix(starDestroyerMesh->getModelMatrix());
		starDestroyerMesh->addRenderer(starDestroyerRenderer[i]);
		
	}
	
	Camera* anotherShipCamera = new Camera(0.1f, 1000.0f, 67.0f, 1920, 1080);

    Ship* anotherShip = new Ship();
    anotherShip->addMesh(tieBomberMesh);
    anotherShip->setPosition(glm::vec3(10.0f, 0.0f, -200.0f));
    anotherShip->setRotation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    anotherShip->setYawSettings(0.5f, 0.85f);
    anotherShip->setPitchSettings(1.0f, 0.85f);
    anotherShip->setRollSettings(2.0f, 0.85f);
    anotherShip->setAccelerationSettings(1.0f, 0.3f, 5.0f);
    anotherShip->addCamera(anotherShipCamera, glm::vec3(0.0f, 0.0f, 0.0f), glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

	Camera* spaceHunterCamera = new Camera(0.1f, 1000.0f, 67.0f, 1920, 1080);
	
	Ship* spaceHunter = new Ship();
    spaceHunter->addMesh(starDestroyerMesh);
    spaceHunter->setPosition(glm::vec3(10.0f, -100.0f, -200.0f));
    spaceHunter->setRotation(glm::angleAxis(glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    spaceHunter->setYawSettings(0.01f, 0.75f);
    spaceHunter->setPitchSettings(0.01f, 0.75f);
    spaceHunter->setRollSettings(0.02f, 0.75f);
    spaceHunter->setAccelerationSettings(1.0f, 0.3f, 5.0f);
    spaceHunter->addCamera(spaceHunterCamera, glm::vec3(0.0f, 100.0f, -80.0f), glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    spaceHunter->addJoystick(joystick);
   
	Camera* candicesShipCamera = new Camera(0.1f, 1000.0f, 67.0f, 1920, 1080);
   
    Ship* candicesShip = new Ship();
    candicesShip->addMesh(tieFighterMesh);
    candicesShip->setPosition(glm::vec3(-10.0f, 0.0f, 20.0f));
    candicesShip->setRotation(glm::angleAxis(0.0f, glm::vec3(0.0f, 0.0f, 1.0f)));
	candicesShip->setYawSettings(0.5f, 0.85f);
    candicesShip->setPitchSettings(1.0f, 0.85f);
    candicesShip->setRollSettings(2.0f, 0.85f);
    candicesShip->setAccelerationSettings(1.0f, 0.3f, 5.0f);
    candicesShip->addCamera(candicesShipCamera, glm::vec3(0.0f, 0.0f, 0.0f), glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

	std::vector<Ship*> shipList;
	shipList.push_back(anotherShip);
	shipList.push_back(spaceHunter);
	shipList.push_back(candicesShip);
	
    std::vector<Camera*> cameraList;
    
    cameraList.push_back(anotherShipCamera);
    cameraList.push_back(spaceHunterCamera);
    cameraList.push_back(candicesShipCamera);
    
    activeCamera->useSettings(*spaceHunterCamera);
	
	SkyBox* skyBox = new SkyBox();
	SkyBoxGLRenderer* skyBoxRenderer = new SkyBoxGLRenderer();
	loadSkyBoxMesh(*skyBoxRenderer, "meshes/box.dae", "Textures/SkyBox/skyBoxUp.bmp", "Textures/SkyBox/skyBoxDown.bmp", "Textures/SkyBox/skyBoxLeft.bmp", "Textures/SkyBox/skyBoxRight.bmp", "Textures/SkyBox/skyBoxFront.bmp", "Textures/SkyBox/skyBoxBack.bmp");
	skyBoxRenderer->addShader(skyBoxShader);
	skyBoxRenderer->updateProjectionMatrix(activeCamera->getProjectionMatrix());

	skyBoxRenderer->updateCameraRotation(activeCamera->getRotationMatrix());
	SkyBoxMesh* skyBoxMesh = new SkyBoxMesh();
	skyBoxMesh->addRenderer(skyBoxRenderer);
	skyBox->addComponent(skyBoxMesh);
	
	Timer* gameClock = new Timer();
	
	gameClock->start();
	double timeLapse;
	
	int cameraCount = 1;
	
	while (1)
    {
		timeLapse = gameClock->getTimeLapse();
		//std::cout << 1/timeLapse << std::endl;
		
		joystick->clearButtons();
		while(SDL_PollEvent(&sdlEvent))
		{
			joystick->getEvent(sdlEvent);
			joystick->poll();
		}
		
		moon->update(timeLapse);
		skyBox->update(timeLapse);
		for(Ship* current : shipList)
		{
			current->update(timeLapse);
		}
		activeCamera->update(timeLapse);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skyBox->draw();
		moon->draw();
		for(Ship* current : shipList)
		{
			current->draw();
		}
		glXSwapBuffers(dpy, win);
		
		while(XPending(dpy))
        {
            XNextEvent(dpy, &event);
            switch (event.type)
            {
                case KeyPress:
                {
                    KeySym     keysym;
                    char       buffer[1];
                    if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_Escape))
                    {
                        exit(0);
                    }
                   
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_v))
					{
						activeCamera->useSettings(*(cameraList.at(cameraCount)));
						shipList.at((cameraCount+cameraList.size()-1)%cameraList.size())->removeJoystick();
						shipList.at(cameraCount)->addJoystick(joystick);
						cameraCount += 1;
						cameraCount %= cameraList.size();
					}
                }
					break;
                case ButtonPress:
                    break;
                case ConfigureNotify:
                    glViewport(0, 0, event.xconfigure.width, event.xconfigure.height);
                case Expose:
                    break;
                    
            }
        } 


    }
	
	return 0;
}
