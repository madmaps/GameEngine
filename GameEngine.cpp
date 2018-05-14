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
#include <AL/alut.h>

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
#include "OpenALSound.h"

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
    
  
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    GLuint shader_program = loadShaders("Shaders/NormalShader.vert","Shaders/NormalShader.frag");
    GLuint skyBoxShader = loadShaders("Shaders/SkyBoxShader.vert", "Shaders/SkyBoxShader.frag");
    
    Camera* activeCamera = new Camera(0.1f, 1000.0f, 67.0f, 1920, 1080);
    
    StandardMesh* moonMesh = new StandardMesh();
    loadNormalMeshLoopAll("meshes/planet.dae", moonMesh, activeCamera, shader_program, 1, "Textures/moonDefuse.bmp", "Textures/moonNormal.bmp", "Textures/moonSpecular.bmp", "Textures/moonAmbient.bmp");
    
    StandardMesh* tieBomberMesh = new StandardMesh();
    loadNormalMeshLoopAll("meshes/tiebomber.dae", tieBomberMesh, activeCamera, shader_program, 9, "Textures/Ship/ShipDef.bmp", "Textures/Ship/ShipNormal.bmp", "Textures/Ship/ShipSpec.bmp", "Textures/Ship/ShipAmb.bmp");

    StandardMesh* tieFighterMesh = new StandardMesh();
    loadNormalMeshLoopAll("meshes/tiefighter.dae", tieFighterMesh, activeCamera, shader_program, 8, "Textures/Ship/ShipDef.bmp", "Textures/Ship/ShipNormal.bmp", "Textures/Ship/ShipSpec.bmp", "Textures/Ship/ShipAmb.bmp");

    StandardMesh* starDestroyerMesh = new StandardMesh();
    loadNormalMeshLoopAll("meshes/stardestroyer.dae", starDestroyerMesh, activeCamera, shader_program, 56, "Textures/Ship/ShipDef.bmp", "Textures/Ship/ShipNormal.bmp", "Textures/Ship/ShipSpec.bmp", "Textures/Ship/ShipAmb.bmp");

	
    Planet* moon = new Planet();
    moon->addComponent(moonMesh);
    moon->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    moon->setRotation(glm::angleAxis(0.0f, glm::vec3(0.0f, 0.0f, -1.0f)));

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
   
    Camera* candicesShipCamera = new Camera(0.1f, 1000.0f, 67.0f, 1920, 1080);
    Ship* candicesShip = new Ship();
    candicesShip->addMesh(tieFighterMesh);
    candicesShip->setPosition(glm::vec3(-10.0f, 0.0f, 20.0f));
    candicesShip->setRotation(glm::angleAxis(0.0f, glm::vec3(0.0f, 0.0f, 1.0f)));
    candicesShip->setYawSettings(0.5f, 0.95f);
    candicesShip->setPitchSettings(2.0f, 0.95f);
    candicesShip->setRollSettings(5.0f, 0.95f);
    candicesShip->setAccelerationSettings(1.0f, 0.3f, 15.0f);
    candicesShip->addCamera(candicesShipCamera, glm::vec3(0.0f, 0.0f, -0.05f), glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

    std::vector<Ship*> shipList;
    shipList.push_back(anotherShip);
    shipList.push_back(spaceHunter);
    shipList.push_back(candicesShip);
	
 
    activeCamera->useSettings(*anotherShipCamera);
	
    SkyBox* skyBox = new SkyBox();
    SkyBoxGLRenderer* skyBoxRenderer = new SkyBoxGLRenderer();
    loadSkyBoxMesh(*skyBoxRenderer, "meshes/box.dae", "Textures/SkyBox/skyBoxUp.bmp", "Textures/SkyBox/skyBoxDown.bmp", "Textures/SkyBox/skyBoxLeft.bmp", "Textures/SkyBox/skyBoxRight.bmp", "Textures/SkyBox/skyBoxFront.bmp", "Textures/SkyBox/skyBoxBack.bmp");
    skyBoxRenderer->addShader(skyBoxShader);
    skyBoxRenderer->updateProjectionMatrix(activeCamera->getProjectionMatrix());

    skyBoxRenderer->updateCameraRotation(activeCamera->getRotationMatrix());
    SkyBoxMesh* skyBoxMesh = new SkyBoxMesh();
    skyBoxMesh->addRenderer(skyBoxRenderer);
    skyBox->addComponent(skyBoxMesh);
    
	alutInit(NULL, NULL);
    alGetError();
    ALuint alBuffer;
    alBuffer = alutCreateBufferFromFile("Sounds/test.wav");
    ALuint source;
    alGenSources((ALuint)1, &source);
    alSourcef(source, AL_PITCH, 1);
    alSourcef(source, AL_GAIN, 1);
    alSource3f(source, AL_POSITION, 0, 0, 0);
    alSource3f(source, AL_VELOCITY, 0, 0, 0);
    alSourcei(source, AL_LOOPING, AL_FALSE);
    alSourcei(source, AL_BUFFER, alBuffer);
    
    ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
    alListener3f(AL_POSITION, 0, 0, 1.0f);
    alListener3f(AL_VELOCITY, 0, 0, 0);
    alListenerfv(AL_ORIENTATION, listenerOri);
	
    ALCenum error;
    error = alGetError();
    if (error != AL_NO_ERROR)
    {
    	std::cout << "ERROR!" << std::endl;
    }
    
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
	anotherShip->addJoystick(joystick);


	
    Timer* gameClock = new Timer();
	
    gameClock->start();
    double timeLapse;
	
    int shipCount = 0;
    
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
						SDL_JoystickClose(sdlJoystick);
						alDeleteSources(1, &source);
						alDeleteBuffers(1, &alBuffer);
						//device = alcGetContextsDevice(context);
						//alcMakeContextCurrent(NULL);
						//alcDestroyContext(context);
						alutExit();
						//alcCloseDevice(device);
									exit(0);
					}
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_p))
					{
						alGetError();
						alSourcePlay(source);
						error = alGetError();

					}
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_v))
					{
						shipList.at(shipCount)->removeJoystick();
						shipCount += 1;
						shipCount %= shipList.size();
						activeCamera->useSettings(*(shipList.at(shipCount))->getCamera());
						shipList.at(shipCount)->addJoystick(joystick);
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



