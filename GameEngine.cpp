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
    
    Camera* shipCamera = new Camera(0.1f, 1000.0f, 67.0f, 1920, 1080);
    shipCamera->setPosition(glm::vec3(0.0f, 0.0f, 90.0f));
    shipCamera->setRotation(glm::angleAxis(0.0f,glm::vec3(0.0f, 0.0f, -1.0f)));
    shipCamera->update(0);
    
	Assimp::Importer importer;

	const aiScene* planetScene = importer.ReadFile("meshes/planet.dae",aiProcess_CalcTangentSpace | aiProcess_Triangulate);
	if(!planetScene)
	{
		std::cout << "BAD!";
	}

	BumpMapGLRenderer* moonRenderer = new BumpMapGLRenderer();
	loadNormalMesh(planetScene, *moonRenderer, 0, "Textures/moonDefuse.bmp", "Textures/moonNormal.bmp", "Textures/moonSpecular.bmp", "Textures/moonAmbient.bmp");
    moonRenderer->addShader(shader_program);
	moonRenderer->updateProjectionMatrix(shipCamera->getProjectionMatrix());
    moonRenderer->updateViewMatrix(shipCamera->getViewMatrix());
    moonRenderer->updateCameraLocation(shipCamera->getLocationMatrix());
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
		tieBomberRenderer[i]->updateProjectionMatrix(shipCamera->getProjectionMatrix());
		tieBomberRenderer[i]->updateViewMatrix(shipCamera->getViewMatrix());
		tieBomberRenderer[i]->updateCameraLocation(shipCamera->getLocationMatrix());
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
		tieFighterRenderer[i]->updateProjectionMatrix(shipCamera->getProjectionMatrix());
		tieFighterRenderer[i]->updateViewMatrix(shipCamera->getViewMatrix());
		tieFighterRenderer[i]->updateCameraLocation(shipCamera->getLocationMatrix());
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
		starDestroyerRenderer[i]->updateProjectionMatrix(shipCamera->getProjectionMatrix());
		starDestroyerRenderer[i]->updateViewMatrix(shipCamera->getViewMatrix());
		starDestroyerRenderer[i]->updateCameraLocation(shipCamera->getLocationMatrix());
		starDestroyerRenderer[i]->updateModelMatrix(starDestroyerMesh->getModelMatrix());
		starDestroyerMesh->addRenderer(starDestroyerRenderer[i]);
		
	}

	
	Ship* spaceHunter = new Ship();
    spaceHunter->addComponent(starDestroyerMesh);
    spaceHunter->setPosition(glm::vec3(10.0f, 0.0f, -200.0f));
    spaceHunter->setRotation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    spaceHunter->setYawSettings(1.0f, 0.75f);
    spaceHunter->setPitchSettings(1.0f, 0.75f);
    spaceHunter->setRollSettings(2.0f, 0.75f);
    spaceHunter->setAccelerationSettings(1.0f, 0.3f, 50.0f);

    
    Ship* anotherShip = new Ship();
    anotherShip->addComponent(tieBomberMesh);
    anotherShip->setPosition(glm::vec3(10.0f, 0.0f, -200.0f));
    anotherShip->setRotation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    
    Ship* candicesShip = new Ship();
    candicesShip->addComponent(tieBomberMesh);
    candicesShip->setPosition(glm::vec3(-10.0f, 0.0f, 20.0f));
    candicesShip->setRotation(glm::angleAxis(0.0f, glm::vec3(0.0f, 0.0f, 1.0f)));
	
	SkyBox* skyBox = new SkyBox();
	SkyBoxGLRenderer* skyBoxRenderer = new SkyBoxGLRenderer();
	loadSkyBoxMesh(*skyBoxRenderer, "meshes/box.dae", "Textures/SkyBox/skyBoxUp.bmp", "Textures/SkyBox/skyBoxDown.bmp", "Textures/SkyBox/skyBoxLeft.bmp", "Textures/SkyBox/skyBoxRight.bmp", "Textures/SkyBox/skyBoxFront.bmp", "Textures/SkyBox/skyBoxBack.bmp");
	skyBoxRenderer->addShader(skyBoxShader);
	skyBoxRenderer->updateProjectionMatrix(shipCamera->getProjectionMatrix());

	skyBoxRenderer->updateCameraRotation(shipCamera->getRotationMatrix());
	SkyBoxMesh* skyBoxMesh = new SkyBoxMesh();
	skyBoxMesh->addRenderer(skyBoxRenderer);
	skyBox->addComponent(skyBoxMesh);
	
	Timer* gameClock = new Timer();
	
	gameClock->start();
	double timeLapse;
	
	while (1)
    {
		timeLapse = gameClock->getTimeLapse();
		shipCamera->update(timeLapse);
		moon->update(timeLapse);
		skyBox->update(timeLapse);
		spaceHunter->update(timeLapse);
		anotherShip->update(timeLapse);
		candicesShip->update(timeLapse);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skyBox->draw();
		moon->draw();
		spaceHunter->draw();
		anotherShip->draw();
		candicesShip->draw();
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
                    if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_w))
                    {
                        glm::mat3 camRotMat = glm::toMat3(shipCamera->getRotation());
                        glm::vec3 movement = glm::vec3(0.0f, 0.0f, -0.3f);
                        glm::vec3 finalMovement = movement * camRotMat;
                        shipCamera->setPosition(shipCamera->getPosition() + finalMovement);
                    }
                   
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_s))
                    {
                        glm::mat3 camRotMat = glm::toMat3(shipCamera->getRotation());
                        glm::vec3 movement = glm::vec3(0.0f, 0.0f, 0.3f);
                        glm::vec3 finalMovement = movement * camRotMat;
                        shipCamera->setPosition(shipCamera->getPosition() + finalMovement);
                    } 
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_a))
                    {
                        glm::mat3 camRotMat = glm::toMat3(shipCamera->getRotation());
                        glm::vec3 movement = glm::vec3(-0.3f, 0.0f, 0.0f);
                        glm::vec3 finalMovement = movement * camRotMat;
                        shipCamera->setPosition(shipCamera->getPosition() + finalMovement);
                    }
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_d))
                    {
                        glm::mat3 camRotMat = glm::toMat3(shipCamera->getRotation());
                        glm::vec3 movement = glm::vec3(0.3f, 0.0f, 0.0f);
                        glm::vec3 finalMovement = movement * camRotMat;
                        shipCamera->setPosition(shipCamera->getPosition() + finalMovement);
                    }
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_q))
                    {
						shipCamera->setRotation(shipCamera->getRotation() * glm::angleAxis(glm::radians(-2.0f),glm::vec3(0.0f, 1.0f, 0.0f)));
                    }
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_e))
                    {
						shipCamera->setRotation(shipCamera->getRotation() * glm::angleAxis(glm::radians(2.0f),glm::vec3(0.0f, 1.0f, 0.0f)));
                    }
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_u))
					{
						spaceHunter->yaw(1);
					}
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_o))
					{
						spaceHunter->yaw(-1);
					}
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_i))
					{
						spaceHunter->pitch(1);
					}
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_k))
					{
						spaceHunter->pitch(-1);
					}
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_j))
					{
						spaceHunter->roll(1);
					}
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_l))
					{
						spaceHunter->roll(-1);
					}
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_y))
					{
						spaceHunter->accelerate(1);
					}
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_n))
					{
						spaceHunter->accelerate(-1);
					}


					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_h))
					{
						spaceHunter->yaw(0);
						spaceHunter->pitch(0);
						spaceHunter->roll(0);
						spaceHunter->accelerate(0);
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
